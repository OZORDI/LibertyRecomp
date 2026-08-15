/**
 * @file        ui/accelerated_pointer_mac.mm
 * @brief       AppKit accelerated pointer motion bridge for macOS.
 */
#include "accelerated_pointer_mac.h"

#import <AppKit/AppKit.h>

namespace rex::ui {

namespace {

struct AcceleratedPointerMonitor {
  NSWindow* window = nil;
  id event_monitor = nil;
  AcceleratedPointerCallback callback = nullptr;
  void* userdata = nullptr;
};

}  // namespace

void* InstallAcceleratedPointerMonitor(void* native_window, AcceleratedPointerCallback callback,
                                       void* userdata) {
  if (![NSThread isMainThread] || !native_window || !callback) {
    return nullptr;
  }

  auto* monitor = new AcceleratedPointerMonitor();
  monitor->window = (__bridge NSWindow*)native_window;
  monitor->callback = callback;
  monitor->userdata = userdata;

  const NSEventMask mask = NSEventMaskMouseMoved | NSEventMaskLeftMouseDragged |
                           NSEventMaskRightMouseDragged | NSEventMaskOtherMouseDragged;
  monitor->event_monitor = [NSEvent addLocalMonitorForEventsMatchingMask:mask
                                                                 handler:^NSEvent*(NSEvent* event) {
    NSWindow* event_window = event.window;
    if ((event_window == monitor->window ||
         (!event_window && NSApp.keyWindow == monitor->window)) &&
        monitor->callback) {
      monitor->callback(monitor->userdata, static_cast<float>(event.deltaX),
                        static_cast<float>(event.deltaY));
    }
    return event;
  }];

  if (!monitor->event_monitor) {
    delete monitor;
    return nullptr;
  }
  return monitor;
}

void RemoveAcceleratedPointerMonitor(void* opaque_monitor) {
  if (!opaque_monitor) {
    return;
  }
  auto* monitor = static_cast<AcceleratedPointerMonitor*>(opaque_monitor);
  if (monitor->event_monitor) {
    [NSEvent removeMonitor:monitor->event_monitor];
    monitor->event_monitor = nil;
  }
  monitor->callback = nullptr;
  monitor->userdata = nullptr;
  monitor->window = nil;
  delete monitor;
}

}  // namespace rex::ui
