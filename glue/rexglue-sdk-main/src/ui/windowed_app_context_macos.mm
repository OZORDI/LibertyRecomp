#include <rex/ui/windowed_app_context_macos.h>

#include <AppKit/AppKit.h>
#include <CoreFoundation/CoreFoundation.h>

namespace rex::ui {

namespace {

void PostWakeEvent() {
  NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                      location:NSZeroPoint
                                 modifierFlags:0
                                     timestamp:0
                                  windowNumber:0
                                       context:nil
                                       subtype:0
                                         data1:0
                                         data2:0];
  [NSApp postEvent:event atStart:NO];
}

}  // namespace

MacWindowedAppContext::~MacWindowedAppContext() {
  if (run_loop_ != nullptr) {
    CFRunLoopWakeUp(static_cast<CFRunLoopRef>(run_loop_));
  }
}

void MacWindowedAppContext::NotifyUILoopOfPendingFunctions() {
  if (run_loop_ != nullptr) {
    CFRunLoopWakeUp(static_cast<CFRunLoopRef>(run_loop_));
  }
  if (NSApp != nil) {
    PostWakeEvent();
  }
}

void MacWindowedAppContext::PlatformQuitFromUIThread() {
  [NSApp stop:nil];
  PostWakeEvent();
  if (run_loop_ != nullptr) {
    CFRunLoopWakeUp(static_cast<CFRunLoopRef>(run_loop_));
  }
}

void MacWindowedAppContext::RunMainCocoaLoop() {
  [NSApplication sharedApplication];
  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
  [NSApp finishLaunching];
  [NSApp activateIgnoringOtherApps:YES];

  run_loop_ = CFRunLoopGetCurrent();
  while (!HasQuitFromUIThread()) {
    @autoreleasepool {
      ExecutePendingFunctionsFromUIThread();
      NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                          untilDate:[NSDate distantFuture]
                                             inMode:NSDefaultRunLoopMode
                                            dequeue:YES];
      if (event != nil) {
        [NSApp sendEvent:event];
      }
      [NSApp updateWindows];
    }
  }

  ExecutePendingFunctionsFromUIThread();
}

}  // namespace rex::ui
