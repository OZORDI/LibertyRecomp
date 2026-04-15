// iOS Core Haptics + GCController vibration bridge.
//
// Implementation notes:
//   * CHHapticEngine vended through GameController (GCDeviceHaptics)
//     does NOT support advanced pattern players — only basic pattern
//     players and only non-audio events. audioContinuous / audioCustom
//     events are silently ignored (Apple iOS 14+ doc note).
//   * We keep two engines per controller when possible — one bound to
//     GCHapticsLocalityLeftHandle and one to RightHandle — so low and
//     high frequency rumble motors are driven independently. On
//     controllers that only report a single locality (e.g. some MFi
//     gamepads), both sides fall back to GCHapticsLocalityDefault and
//     we mix the intensities.
//   * Engine lifetime: created lazily in Init, restarted from the
//     resetHandler (kernel audio server restart) and stoppedHandler
//     (user pressed home, audio session interruption, etc.).
//   * All Core Haptics / GameController calls are marshalled onto the
//     main queue because CHHapticEngine is not documented as
//     thread-safe and engine start/stop can block on XPC.

#include "haptics_ios.h"

#if defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV)

#import <CoreHaptics/CoreHaptics.h>
#import <GameController/GameController.h>
#import <Foundation/Foundation.h>

#include <algorithm>
#include <cmath>

API_AVAILABLE(ios(14.0), tvos(14.0))
@interface LRIOSHapticsEngine : NSObject
- (instancetype)initWithController:(GCController*)controller;
- (void)setRumbleLeft:(float)left right:(float)right duration:(float)duration;
- (void)stop;
- (void)teardown;
@end

API_AVAILABLE(ios(14.0), tvos(14.0))
@implementation LRIOSHapticsEngine {
    __weak GCController* _controller;

    // One engine per physical motor bank. _leftEngine drives the
    // left-handle locality (low-frequency on Xbox), _rightEngine the
    // right-handle (high-frequency). If the controller only exposes
    // GCHapticsLocalityDefault, _leftEngine == _rightEngine and both
    // channels are mixed into a single event.
    CHHapticEngine* _leftEngine;
    CHHapticEngine* _rightEngine;
    BOOL _sharedEngine;

    // Currently-playing basic pattern players so we can stop the
    // previous rumble before starting a new one. Basic players are
    // cheap to recreate each SetRumble call.
    id<CHHapticPatternPlayer> _leftPlayer;
    id<CHHapticPatternPlayer> _rightPlayer;
}

- (instancetype)initWithController:(GCController*)controller {
    self = [super init];
    if (!self) return nil;
    _controller = controller;

    GCDeviceHaptics* haptics = controller.haptics;
    if (haptics == nil) {
        // Older controller (iOS < 14, or MFi without haptics firmware).
        // Nothing to build — callers must check the returned handle.
        return nil;
    }

    NSSet<GCHapticsLocality>* supported = haptics.supportedLocalities;

    // Prefer split left/right engines. Fall back to Default locality
    // if either handle is not exposed.
    if ([supported containsObject:GCHapticsLocalityLeftHandle]) {
        _leftEngine = [haptics createEngineWithLocality:GCHapticsLocalityLeftHandle];
    }
    if ([supported containsObject:GCHapticsLocalityRightHandle]) {
        _rightEngine = [haptics createEngineWithLocality:GCHapticsLocalityRightHandle];
    }
    if (!_leftEngine || !_rightEngine) {
        CHHapticEngine* shared = [haptics createEngineWithLocality:GCHapticsLocalityDefault];
        if (!shared) {
            return nil;
        }
        _leftEngine = shared;
        _rightEngine = shared;
        _sharedEngine = YES;
    }

    [self configureEngine:_leftEngine];
    if (!_sharedEngine) {
        [self configureEngine:_rightEngine];
    }
    return self;
}

- (void)configureEngine:(CHHapticEngine*)engine {
    __weak LRIOSHapticsEngine* weakSelf = self;
    engine.stoppedHandler = ^(CHHapticEngineStoppedReason reason) {
        // System stopped us (audio session interruption, app background,
        // etc). We'll lazily restart in the next setRumble call.
        (void)reason;
        (void)weakSelf;
    };
    engine.resetHandler = ^{
        // Haptic server crashed / was killed. Restart and re-register
        // handlers on the main queue.
        LRIOSHapticsEngine* strong = weakSelf;
        if (!strong) return;
        dispatch_async(dispatch_get_main_queue(), ^{
            NSError* err = nil;
            [engine startAndReturnError:&err];
            (void)err;
        });
    };

    NSError* err = nil;
    [engine startAndReturnError:&err];
    (void)err;
}

- (id<CHHapticPatternPlayer>)makePlayerOnEngine:(CHHapticEngine*)engine
                                      intensity:(float)intensity
                                       duration:(float)duration {
    if (!engine || intensity <= 0.0f) return nil;

    CHHapticEventParameter* intensityParam =
        [[CHHapticEventParameter alloc] initWithParameterID:CHHapticEventParameterIDHapticIntensity
                                                      value:intensity];
    // Sharpness at 0.5 gives a reasonable generic "rumble" texture.
    // Games that want nuance can be extended later to pass through.
    CHHapticEventParameter* sharpnessParam =
        [[CHHapticEventParameter alloc] initWithParameterID:CHHapticEventParameterIDHapticSharpness
                                                      value:0.5f];

    CHHapticEvent* event =
        [[CHHapticEvent alloc] initWithEventType:CHHapticEventTypeHapticContinuous
                                      parameters:@[intensityParam, sharpnessParam]
                                    relativeTime:0
                                        duration:duration];

    NSError* err = nil;
    CHHapticPattern* pattern =
        [[CHHapticPattern alloc] initWithEvents:@[event] parameters:@[] error:&err];
    if (!pattern) return nil;

    id<CHHapticPatternPlayer> player = [engine createPlayerWithPattern:pattern error:&err];
    return player;
}

- (void)setRumbleLeft:(float)left right:(float)right duration:(float)duration {
    dispatch_block_t block = ^{
        // Stop any currently-playing rumble first — overlapping players
        // on the same engine double the perceived intensity.
        NSError* err = nil;
        if (self->_leftPlayer)  { [self->_leftPlayer  stopAtTime:0 error:&err]; self->_leftPlayer  = nil; }
        if (self->_rightPlayer) { [self->_rightPlayer stopAtTime:0 error:&err]; self->_rightPlayer = nil; }

        if (left <= 0.0f && right <= 0.0f) {
            return; // pure stop
        }

        // Make sure engines are running; restart if stoppedHandler fired.
        [self->_leftEngine startAndReturnError:&err];
        if (!self->_sharedEngine) {
            [self->_rightEngine startAndReturnError:&err];
        }

        if (self->_sharedEngine) {
            // Single motor: mix the two channels so both rumble inputs
            // contribute something audible.
            float mixed = std::min(1.0f, std::max(left, right) * 0.7f + std::min(left, right) * 0.3f);
            self->_leftPlayer = [self makePlayerOnEngine:self->_leftEngine
                                               intensity:mixed
                                                duration:duration];
            [self->_leftPlayer startAtTime:0 error:&err];
        } else {
            self->_leftPlayer  = [self makePlayerOnEngine:self->_leftEngine
                                                intensity:left
                                                 duration:duration];
            self->_rightPlayer = [self makePlayerOnEngine:self->_rightEngine
                                                intensity:right
                                                 duration:duration];
            if (self->_leftPlayer)  [self->_leftPlayer  startAtTime:0 error:&err];
            if (self->_rightPlayer) [self->_rightPlayer startAtTime:0 error:&err];
        }
        (void)err;
    };

    if ([NSThread isMainThread]) {
        block();
    } else {
        dispatch_async(dispatch_get_main_queue(), block);
    }
}

- (void)stop {
    dispatch_block_t block = ^{
        NSError* err = nil;
        if (self->_leftPlayer)  { [self->_leftPlayer  stopAtTime:0 error:&err]; self->_leftPlayer  = nil; }
        if (self->_rightPlayer) { [self->_rightPlayer stopAtTime:0 error:&err]; self->_rightPlayer = nil; }
        (void)err;
    };
    if ([NSThread isMainThread]) block();
    else dispatch_async(dispatch_get_main_queue(), block);
}

- (void)teardown {
    dispatch_block_t block = ^{
        NSError* err = nil;
        if (self->_leftPlayer)  [self->_leftPlayer  stopAtTime:0 error:&err];
        if (self->_rightPlayer) [self->_rightPlayer stopAtTime:0 error:&err];
        self->_leftPlayer  = nil;
        self->_rightPlayer = nil;

        [self->_leftEngine stopWithCompletionHandler:^(NSError* _Nullable e){ (void)e; }];
        if (!self->_sharedEngine) {
            [self->_rightEngine stopWithCompletionHandler:^(NSError* _Nullable e){ (void)e; }];
        }
        self->_leftEngine  = nil;
        self->_rightEngine = nil;
        (void)err;
    };
    if ([NSThread isMainThread]) block();
    else dispatch_sync(dispatch_get_main_queue(), block);
}

@end

// ---------------------------------------------------------------------------
// C++ facade
// ---------------------------------------------------------------------------

namespace lr::haptics {

LRHapticsHandle Create(GCController* controller) {
    if (controller == nil) return nullptr;
    if (@available(iOS 14.0, tvOS 14.0, *)) {
        LRIOSHapticsEngine* engine = [[LRIOSHapticsEngine alloc] initWithController:controller];
        if (!engine) return nullptr;
        return (__bridge_retained void*)engine;
    }
    return nullptr;
}

void SetRumble(LRHapticsHandle handle, uint16_t leftMotor, uint16_t rightMotor, float durationSec) {
    if (!handle) return;
    if (@available(iOS 14.0, tvOS 14.0, *)) {
        LRIOSHapticsEngine* engine = (__bridge LRIOSHapticsEngine*)handle;

        // Xbox convention 0-65535 → 0.0-1.0. Clamp and saturate.
        float left  = std::min(1.0f, std::max(0.0f, leftMotor  / 65535.0f));
        float right = std::min(1.0f, std::max(0.0f, rightMotor / 65535.0f));

        // Core Haptics continuous events cap at 30s and need a positive
        // duration. Default 0.25s if caller passed garbage.
        float duration = durationSec;
        if (!std::isfinite(duration) || duration <= 0.0f) duration = 0.25f;
        if (duration > 30.0f) duration = 30.0f;

        [engine setRumbleLeft:left right:right duration:duration];
    }
}

void Stop(LRHapticsHandle handle) {
    if (!handle) return;
    if (@available(iOS 14.0, tvOS 14.0, *)) {
        LRIOSHapticsEngine* engine = (__bridge LRIOSHapticsEngine*)handle;
        [engine stop];
    }
}

void Destroy(LRHapticsHandle handle) {
    if (!handle) return;
    if (@available(iOS 14.0, tvOS 14.0, *)) {
        LRIOSHapticsEngine* engine = (__bridge_transfer LRIOSHapticsEngine*)handle;
        [engine teardown];
        engine = nil;
    }
}

} // namespace lr::haptics

#endif // iOS / tvOS
