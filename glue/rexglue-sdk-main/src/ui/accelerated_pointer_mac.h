/**
 * @file        ui/accelerated_pointer_mac.h
 * @brief       AppKit accelerated pointer motion bridge for macOS.
 */
#pragma once

namespace rex::ui {

using AcceleratedPointerCallback = void (*)(void* userdata, float delta_x, float delta_y);

// These functions must be called on the macOS application thread.
void* InstallAcceleratedPointerMonitor(void* native_window, AcceleratedPointerCallback callback,
                                       void* userdata);
void RemoveAcceleratedPointerMonitor(void* monitor);

}  // namespace rex::ui
