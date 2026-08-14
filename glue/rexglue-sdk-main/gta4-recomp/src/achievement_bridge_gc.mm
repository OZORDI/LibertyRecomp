#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

#include "achievement_bridge_gc.h"

namespace gta4::game_center {
namespace {

constexpr uint32_t kFirstAchievementId = 1;
constexpr uint32_t kLastAchievementId = 65;
NSString* const kPlatinumIdentifier = @"ach_platinum";

bool g_initialized = false;
bool g_authenticated = false;

NSMutableSet<NSString*>* PendingIdentifiers() {
  static NSMutableSet<NSString*>* identifiers = [[NSMutableSet alloc] init];
  return identifiers;
}

NSString* IdentifierForXboxId(uint32_t xbox_id) {
  return [NSString stringWithFormat:@"ach_%03u", xbox_id];
}

NSViewController* ActivePresenter() {
  NSApplication* application = NSApplication.sharedApplication;
  NSWindow* window = application.keyWindow ?: application.mainWindow;
  if (window == nil) {
    for (NSWindow* candidate in application.windows) {
      if (candidate.isVisible) {
        window = candidate;
        break;
      }
    }
  }
  return window.contentViewController;
}

void PresentAuthenticationController(NSViewController* controller) {
  NSViewController* presenter = ActivePresenter();
  if (presenter == nil) {
    NSLog(@"[LibertyRecomp] Game Center authentication UI has no active window.");
    return;
  }
  [presenter presentViewControllerAsSheet:controller];
}

void CheckAndGrantPlatinum() {
  if (!g_authenticated) {
    return;
  }

  [GKAchievement loadAchievementsWithCompletionHandler:
      ^(NSArray<GKAchievement*>* achievements, NSError* error) {
        dispatch_async(dispatch_get_main_queue(), ^{
          if (error != nil || achievements == nil) {
            if (error != nil) {
              NSLog(@"[LibertyRecomp] Game Center achievement load failed: %@",
                    error.localizedDescription);
            }
            return;
          }

          NSMutableSet<NSString*>* completed = [NSMutableSet setWithCapacity:achievements.count];
          for (GKAchievement* achievement in achievements) {
            if (achievement.percentComplete >= 100.0) {
              [completed addObject:achievement.identifier];
            }
          }

          for (uint32_t id = kFirstAchievementId; id <= kLastAchievementId; ++id) {
            if (![completed containsObject:IdentifierForXboxId(id)]) {
              return;
            }
          }

          GKAchievement* platinum =
              [[GKAchievement alloc] initWithIdentifier:kPlatinumIdentifier];
          platinum.percentComplete = 100.0;
          platinum.showsCompletionBanner = YES;
          [GKAchievement
              reportAchievements:@[ platinum ]
              withCompletionHandler:^(NSError* report_error) {
                if (report_error != nil) {
                  NSLog(@"[LibertyRecomp] Game Center platinum report failed: %@",
                        report_error.localizedDescription);
                }
              }];
        });
      }];
}

void ReportIdentifier(NSString* identifier, bool check_platinum) {
  GKAchievement* achievement = [[GKAchievement alloc] initWithIdentifier:identifier];
  achievement.percentComplete = 100.0;
  achievement.showsCompletionBanner = YES;
  [GKAchievement reportAchievements:@[ achievement ] withCompletionHandler:^(NSError* error) {
    dispatch_async(dispatch_get_main_queue(), ^{
      if (error != nil) {
        NSLog(@"[LibertyRecomp] Game Center achievement report failed (%@): %@", identifier,
              error.localizedDescription);
        return;
      }
      if (check_platinum) {
        CheckAndGrantPlatinum();
      }
    });
  }];
}

void FlushPendingIdentifiers() {
  if (!g_authenticated) {
    return;
  }

  NSArray<NSString*>* pending = PendingIdentifiers().allObjects;
  if (pending.count == 0) {
    CheckAndGrantPlatinum();
    return;
  }

  [PendingIdentifiers() removeAllObjects];
  NSMutableArray<GKAchievement*>* achievements =
      [NSMutableArray arrayWithCapacity:pending.count];
  for (NSString* identifier in pending) {
    GKAchievement* achievement = [[GKAchievement alloc] initWithIdentifier:identifier];
    achievement.percentComplete = 100.0;
    achievement.showsCompletionBanner = YES;
    [achievements addObject:achievement];
  }

  [GKAchievement reportAchievements:achievements withCompletionHandler:^(NSError* error) {
    dispatch_async(dispatch_get_main_queue(), ^{
      if (error != nil) {
        [PendingIdentifiers() addObjectsFromArray:pending];
        NSLog(@"[LibertyRecomp] Game Center pending achievement report failed: %@",
              error.localizedDescription);
        return;
      }
      CheckAndGrantPlatinum();
    });
  }];
}

}  // namespace

void Initialize() {
  dispatch_async(dispatch_get_main_queue(), ^{
    if (g_initialized) {
      return;
    }
    g_initialized = true;

    GKLocalPlayer.localPlayer.authenticateHandler =
        ^(NSViewController* controller, NSError* error) {
          dispatch_async(dispatch_get_main_queue(), ^{
            if (controller != nil) {
              PresentAuthenticationController(controller);
              return;
            }

            g_authenticated = GKLocalPlayer.localPlayer.isAuthenticated;
            if (g_authenticated) {
              NSLog(@"[LibertyRecomp] Game Center authenticated: %@",
                    GKLocalPlayer.localPlayer.displayName);
              FlushPendingIdentifiers();
            } else {
              NSLog(@"[LibertyRecomp] Game Center authentication failed: %@",
                    error != nil ? error.localizedDescription : @"unknown error");
            }
          });
        };
  });
}

void SubmitAchievement(uint32_t xbox_id) {
  if (xbox_id < kFirstAchievementId || xbox_id > kLastAchievementId) {
    NSLog(@"[LibertyRecomp] Ignoring unknown Xbox achievement ID: %u", xbox_id);
    return;
  }

  NSString* identifier = IdentifierForXboxId(xbox_id);
  dispatch_async(dispatch_get_main_queue(), ^{
    if (!g_authenticated) {
      [PendingIdentifiers() addObject:identifier];
      return;
    }
    ReportIdentifier(identifier, true);
  });
}

}  // namespace gta4::game_center
