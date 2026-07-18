// achievement_bridge_gc.mm - Game Center achievement bridge (iOS + macOS)
//
// Achievement ID convention (must match App Store Connect registration):
//   "ach_NNN"      - zero-padded Xbox achievement ID (001..065)
//   "ach_platinum" - awarded once all 65 base + DLC achievements are complete

#ifdef LIBERTY_RECOMP_GAMECENTER

#include <rex/platform.h>
#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>
#if REX_PLATFORM_IOS
#import <UIKit/UIKit.h>
#else
#import <AppKit/AppKit.h>
#endif
#include "achievement_bridge_gc.h"

static constexpr uint32_t kNonPlatinumCount = 65;
static constexpr uint32_t kFirstAchievementId = 1;
static NSString* const kPlatinumIdentifier = @"ach_platinum";

static BOOL s_gc_authenticated = NO;

static NSMutableSet<NSString*>* PendingAchievementIdentifiers()
{
    static NSMutableSet<NSString*>* pending = [[NSMutableSet alloc] init];
    return pending;
}

static NSString* GCIdentifierForXboxId(uint32_t xbox_id)
{
    return [NSString stringWithFormat:@"ach_%03u", xbox_id];
}

#if REX_PLATFORM_IOS
static UIViewController* TopViewController(UIViewController* controller)
{
    while (controller.presentedViewController) {
        controller = controller.presentedViewController;
    }

    if ([controller isKindOfClass:UINavigationController.class]) {
        return TopViewController(((UINavigationController*)controller).visibleViewController);
    }

    if ([controller isKindOfClass:UITabBarController.class]) {
        return TopViewController(((UITabBarController*)controller).selectedViewController);
    }

    return controller;
}

static UIViewController* ActiveRootViewController()
{
    UIApplication* app = UIApplication.sharedApplication;
    if (@available(iOS 13.0, *)) {
        for (UIScene* scene in app.connectedScenes) {
            if (scene.activationState != UISceneActivationStateForegroundActive ||
                ![scene isKindOfClass:UIWindowScene.class]) {
                continue;
            }

            UIWindowScene* windowScene = (UIWindowScene*)scene;
            for (UIWindow* window in windowScene.windows) {
                if (window.isKeyWindow && window.rootViewController) {
                    return TopViewController(window.rootViewController);
                }
            }

            for (UIWindow* window in windowScene.windows) {
                if (!window.hidden && window.rootViewController) {
                    return TopViewController(window.rootViewController);
                }
            }
        }
    }

    return nil;
}

static BOOL PresentAuthViewControllerNow(UIViewController* viewController)
{
    UIViewController* presenter = ActiveRootViewController();
    if (!presenter) {
        return NO;
    }

    [presenter presentViewController:viewController animated:YES completion:nil];
    return YES;
}

static void PresentAuthViewController(UIViewController* viewController, uint32_t attempt)
{
    static constexpr uint32_t kMaxAttempts = 8;

    if (PresentAuthViewControllerNow(viewController)) {
        return;
    }

    if (attempt >= kMaxAttempts) {
        NSLog(@"[LibertyRecomp] Game Center auth UI could not be presented.");
        return;
    }

    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        PresentAuthViewController(viewController, attempt + 1);
    });
}
#else
static NSViewController* ActiveMacPresenter()
{
    NSApplication* app = NSApplication.sharedApplication;
    NSWindow* window = app.keyWindow ?: app.mainWindow;

    if (!window) {
        for (NSWindow* candidate in app.windows) {
            if (candidate.isVisible) {
                window = candidate;
                break;
            }
        }
    }

    return window.contentViewController;
}

static BOOL PresentAuthViewControllerNow(NSViewController* viewController)
{
    NSViewController* presenter = ActiveMacPresenter();
    if (!presenter) {
        return NO;
    }

    [presenter presentViewControllerAsSheet:viewController];
    return YES;
}

static void PresentAuthViewController(NSViewController* viewController, uint32_t attempt)
{
    static constexpr uint32_t kMaxAttempts = 8;

    if (PresentAuthViewControllerNow(viewController)) {
        return;
    }

    if (attempt >= kMaxAttempts) {
        NSLog(@"[LibertyRecomp] Game Center auth UI could not be presented.");
        return;
    }

    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        PresentAuthViewController(viewController, attempt + 1);
    });
}
#endif

static void CheckAndGrantPlatinumIfComplete();

static void ReportAchievementNow(NSString* identifier)
{
    GKAchievement* achievement = [[GKAchievement alloc] initWithIdentifier:identifier];
    achievement.percentComplete = 100.0;
    achievement.showsCompletionBanner = YES;

    [GKAchievement reportAchievements:@[achievement]
                 withCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[LibertyRecomp] GC achievement report failed (%@): %@",
                  identifier, error.localizedDescription);
            return;
        }

        if (![identifier isEqualToString:kPlatinumIdentifier]) {
            dispatch_async(dispatch_get_main_queue(), ^{
                CheckAndGrantPlatinumIfComplete();
            });
        }
    }];
}

static void SubmitAchievement(NSString* identifier)
{
    if (!s_gc_authenticated) {
        [PendingAchievementIdentifiers() addObject:identifier];
        return;
    }

    ReportAchievementNow(identifier);
}

static void FlushPendingAchievements()
{
    if (!s_gc_authenticated) {
        return;
    }

    NSArray<NSString*>* pending = [PendingAchievementIdentifiers() allObjects];
    [PendingAchievementIdentifiers() removeAllObjects];

    for (NSString* identifier in pending) {
        ReportAchievementNow(identifier);
    }
}

static void CheckAndGrantPlatinumIfComplete()
{
    if (!s_gc_authenticated) {
        return;
    }

    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray<GKAchievement*>* loaded,
                                                            NSError* error) {
        if (error || !loaded) {
            if (error) {
                NSLog(@"[LibertyRecomp] GC achievement load failed: %@",
                      error.localizedDescription);
            }
            return;
        }

        NSMutableSet<NSString*>* complete = [NSMutableSet setWithCapacity:loaded.count];
        for (GKAchievement* ach in loaded) {
            if (ach.percentComplete >= 100.0) {
                [complete addObject:ach.identifier];
            }
        }

        for (uint32_t i = kFirstAchievementId; i <= kNonPlatinumCount; ++i) {
            NSString* ident = GCIdentifierForXboxId(i);
            if (![complete containsObject:ident]) {
                return;
            }
        }

        dispatch_async(dispatch_get_main_queue(), ^{
            ReportAchievementNow(kPlatinumIdentifier);
        });
    }];
}

void LibertyGCInit()
{
    static dispatch_once_t token;
    dispatch_once(&token, ^{
        dispatch_async(dispatch_get_main_queue(), ^{
#if REX_PLATFORM_IOS
            GKLocalPlayer.localPlayer.authenticateHandler =
                ^(UIViewController* viewController, NSError* error) {
                if (viewController) {
                    PresentAuthViewController(viewController, 0);
                } else if (GKLocalPlayer.localPlayer.isAuthenticated) {
                    s_gc_authenticated = YES;
                    NSLog(@"[LibertyRecomp] Game Center authenticated: %@",
                          GKLocalPlayer.localPlayer.displayName);
                    FlushPendingAchievements();
                    CheckAndGrantPlatinumIfComplete();
                } else {
                    s_gc_authenticated = NO;
                    NSLog(@"[LibertyRecomp] Game Center auth failed: %@",
                          error ? error.localizedDescription : @"(unknown)");
                }
            };
#else
            GKLocalPlayer.localPlayer.authenticateHandler =
                ^(NSViewController* viewController, NSError* error) {
                if (viewController) {
                    PresentAuthViewController(viewController, 0);
                } else if (GKLocalPlayer.localPlayer.isAuthenticated) {
                    s_gc_authenticated = YES;
                    NSLog(@"[LibertyRecomp] Game Center authenticated: %@",
                          GKLocalPlayer.localPlayer.displayName);
                    FlushPendingAchievements();
                    CheckAndGrantPlatinumIfComplete();
                } else {
                    s_gc_authenticated = NO;
                    NSLog(@"[LibertyRecomp] Game Center auth failed: %@",
                          error ? error.localizedDescription : @"(unknown)");
                }
            };
#endif
        });
    });
}

void LibertyGCOnXboxAchievementUnlocked(uint32_t xbox_id)
{
    if (xbox_id < kFirstAchievementId || xbox_id > kNonPlatinumCount) {
        return;
    }

    LibertyGCInit();
    NSString* identifier = GCIdentifierForXboxId(xbox_id);
    dispatch_async(dispatch_get_main_queue(), ^{
        SubmitAchievement(identifier);
    });
}

#endif  // LIBERTY_RECOMP_GAMECENTER
