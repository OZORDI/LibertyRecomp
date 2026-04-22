// achievement_bridge_gc.mm — Game Center achievement bridge (iOS + macOS)
//
// Objective-C++ so we can call GameKit APIs from a C++ translation unit.
// The public interface is pure C (extern "C"-compatible) so callers don't
// need to include any ObjC headers.
//
// Achievement ID convention (must match App Store Connect registration):
//   "ach_NNN"     — NNN is the zero-padded Xbox achievement ID (001..065)
//   "ach_platinum" — awarded automatically once all 65 are 100 % complete
//
// Authentication:
//   GKLocalPlayer.localPlayer.authenticateHandler fires a UIViewController
//   on iOS when login UI is required, and a completion block on macOS.
//   We store the result and silently skip reporting if auth failed.
//
// Thread safety:
//   GKAchievement reporting dispatches the SCE call to the main queue via
//   dispatch_async, so it is safe to call LibertyGCOnXboxAchievementUnlocked
//   from any thread (including the XAM worker thread).

#ifdef LIBERTY_RECOMP_GAMECENTER

#include <rex/platform.h>
#import <GameKit/GameKit.h>
#import <Foundation/Foundation.h>
#include "achievement_bridge_gc.h"

// ── constants ─────────────────────────────────────────────────────────────────

static constexpr uint32_t kNonPlatinumCount = 65;
static NSString* const kPlatinumIdentifier  = @"ach_platinum";

// ── state ─────────────────────────────────────────────────────────────────────

// Protected by dispatch_once (init) and only written from main queue.
static BOOL  s_gcAuthenticated  = NO;
static BOOL  s_initCalled       = NO;

// ── helpers ───────────────────────────────────────────────────────────────────

// Build the Game Center identifier string for a given Xbox achievement ID.
static NSString* GCIdentifierForXboxId(uint32_t xbox_id)
{
    return [NSString stringWithFormat:@"ach_%03u", xbox_id];
}

// Submit a single GKAchievement at 100 % completion.
// No-ops if not authenticated.  Must be called from main queue.
static void SubmitAchievement(NSString* identifier)
{
    if (!s_gcAuthenticated) return;

    GKAchievement* ach = [[GKAchievement alloc] initWithIdentifier:identifier];
    ach.percentComplete   = 100.0;
    ach.showsCompletionBanner = YES;  // let GC show its own banner

    [GKAchievement reportAchievements:@[ach]
                withCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[LibertyRecomp] GC achievement report failed (%@): %@",
                  identifier, error.localizedDescription);
        }
    }];
}

// Query all 65 non-platinum achievements; returns YES if every one is complete.
// Calls back on whatever queue GK uses internally, so the completion handler
// does the platinum grant via dispatch_async(main_queue).
static void CheckAndGrantPlatinumIfComplete()
{
    if (!s_gcAuthenticated) return;

    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray<GKAchievement*>* loaded,
                                                            NSError* error) {
        if (error || !loaded) return;

        // Build a set of identifiers that are at 100 %
        NSMutableSet<NSString*>* complete = [NSMutableSet setWithCapacity:loaded.count];
        for (GKAchievement* ach in loaded) {
            if (ach.percentComplete >= 100.0)
                [complete addObject:ach.identifier];
        }

        // Check every non-platinum achievement
        for (uint32_t i = 1; i <= kNonPlatinumCount; ++i) {
            NSString* ident = GCIdentifierForXboxId(i);
            if (![complete containsObject:ident])
                return;  // at least one not done — no platinum yet
        }

        // All done — grant the platinum from the main queue
        dispatch_async(dispatch_get_main_queue(), ^{
            SubmitAchievement(kPlatinumIdentifier);
        });
    }];
}

// ── public API ────────────────────────────────────────────────────────────────

void LibertyGCInit()
{
    // dispatch_once ensures we only install the authenticateHandler once.
    static dispatch_once_t token;
    dispatch_once(&token, ^{
        s_initCalled = YES;

#if REX_PLATFORM_IOS
        // On iOS, GK may present a sign-in view controller on first launch.
        // We store it in the window's rootViewController when available.
        GKLocalPlayer.localPlayer.authenticateHandler =
            ^(UIViewController* viewController, NSError* error) {
                if (viewController) {
                    // Present the sign-in UI over the root controller.
                    UIWindow* win = [UIApplication sharedApplication].keyWindow;
                    [win.rootViewController presentViewController:viewController
                                                         animated:YES
                                                       completion:nil];
                } else if (GKLocalPlayer.localPlayer.isAuthenticated) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        s_gcAuthenticated = YES;
                        NSLog(@"[LibertyRecomp] Game Center authenticated: %@",
                              GKLocalPlayer.localPlayer.displayName);
                    });
                } else {
                    NSLog(@"[LibertyRecomp] Game Center auth failed: %@",
                          error ? error.localizedDescription : @"(unknown)");
                }
            };
#else
        // macOS: no view controller; block receives nil + auth result directly.
        GKLocalPlayer.localPlayer.authenticateHandler =
            ^(NSViewController* viewController, NSError* error) {
                if (viewController) {
                    // Rare on macOS but handle it: present as sheet.
                    NSApplication* app = NSApplication.sharedApplication;
                    if (app.keyWindow) {
                        [app.keyWindow.windowController presentViewControllerAsSheet:viewController];
                    }
                } else if (GKLocalPlayer.localPlayer.isAuthenticated) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        s_gcAuthenticated = YES;
                        NSLog(@"[LibertyRecomp] Game Center authenticated: %@",
                              GKLocalPlayer.localPlayer.displayName);
                    });
                } else {
                    NSLog(@"[LibertyRecomp] Game Center auth failed: %@",
                          error ? error.localizedDescription : @"(unknown)");
                }
            };
#endif
    });
}

void LibertyGCOnXboxAchievementUnlocked(uint32_t xbox_id)
{
    if (xbox_id < 1 || xbox_id > kNonPlatinumCount)
        return;

    // All GK calls must be dispatched to the main queue.
    NSString* identifier = GCIdentifierForXboxId(xbox_id);
    dispatch_async(dispatch_get_main_queue(), ^{
        SubmitAchievement(identifier);
        // After submitting the non-platinum, do the full scan to
        // see if the platinum should now be granted.
        CheckAndGrantPlatinumIfComplete();
    });
}

#endif  // LIBERTY_RECOMP_GAMECENTER
