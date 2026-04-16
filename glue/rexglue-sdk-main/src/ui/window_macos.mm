#include <rex/ui/window.h>

#include <AppKit/AppKit.h>
#include <QuartzCore/CAMetalLayer.h>

#include <rex/logging.h>
#include <rex/ui/surface_macos.h>

namespace rex::ui {
class MacWindow;
}

@interface RexContentView : NSView
@property(nonatomic, assign) rex::ui::MacWindow* owner;
- (void)updateDrawableSize;
- (void)requestOwnerPaint;
@end

@interface RexWindowDelegate : NSObject <NSWindowDelegate>
@property(nonatomic, assign) rex::ui::MacWindow* owner;
@end

namespace rex::ui {

class MacMenuItem final : public MenuItem {
 public:
  MacMenuItem(Type type, const std::string& text, const std::string& hotkey,
              std::function<void()> callback)
      : MenuItem(type, text, hotkey, std::move(callback)) {}
};

class MacWindow final : public Window {
 public:
  MacWindow(WindowedAppContext& app_context, const std::string_view title,
            uint32_t desired_logical_width, uint32_t desired_logical_height)
      : Window(app_context, title, desired_logical_width, desired_logical_height) {}

  ~MacWindow() override {
    EnterDestructor();
    if (paint_timer_ != nil) {
      [paint_timer_ invalidate];
      paint_timer_ = nil;
    }
    if (window_ != nil) {
      [window_ setDelegate:nil];
      [window_ orderOut:nil];
      window_ = nil;
    }
    if (window_delegate_ != nil) {
      window_delegate_.owner = nullptr;
    }
    if (content_view_ != nil) {
      content_view_.owner = nullptr;
    }
    metal_layer_ = nil;
  }

  uint32_t GetLatestDpiImpl() const override {
    NSScreen* screen = window_ != nil ? window_.screen : [NSScreen mainScreen];
    CGFloat scale = screen != nil ? screen.backingScaleFactor : 1.0;
    return static_cast<uint32_t>(96.0 * scale);
  }

  bool OpenImpl() override {
    NSRect frame = NSMakeRect(0, 0, GetDesiredLogicalWidth(), GetDesiredLogicalHeight());
    NSWindowStyleMask style_mask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
                                   NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;
    window_ = [[NSWindow alloc] initWithContentRect:frame
                                          styleMask:style_mask
                                            backing:NSBackingStoreBuffered
                                              defer:NO];
    if (window_ == nil) {
      REXLOG_ERROR("Failed to create NSWindow");
      return false;
    }

    content_view_ = [[RexContentView alloc] initWithFrame:frame];
    content_view_.owner = this;
    [content_view_ setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
    [content_view_ setWantsLayer:YES];
    metal_layer_ = static_cast<CAMetalLayer*>(content_view_.layer);
    if (metal_layer_ == nil) {
      REXLOG_ERROR("Failed to create CAMetalLayer");
      return false;
    }
    [window_ setContentView:content_view_];

    window_delegate_ = [[RexWindowDelegate alloc] init];
    window_delegate_.owner = this;
    [window_ setDelegate:window_delegate_];
    [window_ setTitle:[NSString stringWithUTF8String:GetTitle().c_str()]];
    [window_ makeKeyAndOrderFront:nil];
    [NSApp activateIgnoringOtherApps:YES];

    RexContentView* content_view = content_view_;
    paint_timer_ = [NSTimer scheduledTimerWithTimeInterval:(1.0 / 60.0)
                                                   repeats:YES
                                                     block:^(__unused NSTimer* timer) {
                                                       [content_view setNeedsDisplay:YES];
                                                       [content_view requestOwnerPaint];
                                                     }];

    UpdateMetalLayerMetrics();

    WindowDestructionReceiver destruction_receiver(this);
    NSRect bounds = [content_view_ bounds];
    CGFloat scale = window_ != nil ? window_.backingScaleFactor : 1.0;
    OnActualSizeUpdate(static_cast<uint32_t>(bounds.size.width * scale),
                       static_cast<uint32_t>(bounds.size.height * scale), destruction_receiver);
    if (!destruction_receiver.IsWindowDestroyed()) {
      OnFocusUpdate(window_.keyWindow, destruction_receiver);
    }
    return true;
  }

  void RequestCloseImpl() override { [window_ performClose:nil]; }

  void ApplyNewFullscreen() override {
    if (window_ != nil) {
      [window_ toggleFullScreen:nil];
    }
  }

  void ApplyNewTitle() override {
    if (window_ != nil) {
      [window_ setTitle:[NSString stringWithUTF8String:GetTitle().c_str()]];
    }
  }

  void ApplyNewMainMenu(MenuItem* old_main_menu) override { (void)old_main_menu; }

  void FocusImpl() override {
    if (window_ != nil) {
      [window_ makeKeyAndOrderFront:nil];
    }
  }

  std::unique_ptr<Surface> CreateSurfaceImpl(Surface::TypeFlags allowed_types) override {
    if (metal_layer_ != nil && (allowed_types & Surface::kTypeFlag_MetalLayer)) {
      return std::make_unique<MetalLayerSurface>(reinterpret_cast<void*>(metal_layer_));
    }
    return nullptr;
  }

  void RequestPaintImpl() override {
    if (content_view_ == nil) {
      return;
    }
    RexContentView* content_view = content_view_;
    if (app_context().IsInUIThread()) {
      [content_view requestOwnerPaint];
    } else {
      app_context().CallInUIThreadDeferred([content_view] { [content_view requestOwnerPaint]; });
    }
  }

  void OnNativeResize() {
    UpdateMetalLayerMetrics();
    WindowDestructionReceiver destruction_receiver(this);
    NSRect bounds = [content_view_ bounds];
    CGFloat scale = content_view_.window != nil ? content_view_.window.backingScaleFactor : 1.0;
    OnActualSizeUpdate(static_cast<uint32_t>(bounds.size.width * scale),
                       static_cast<uint32_t>(bounds.size.height * scale), destruction_receiver);
  }

  void OnNativeFocusChanged(bool focused) {
    WindowDestructionReceiver destruction_receiver(this);
    OnFocusUpdate(focused, destruction_receiver);
  }

  void OnNativeClose() {
    WindowDestructionReceiver destruction_receiver(this);
    OnBeforeClose(destruction_receiver);
    if (!destruction_receiver.IsWindowDestroyed()) {
      OnAfterClose();
    }
  }

  void OnNativePaint() { OnPaint(false); }

 private:
  void UpdateMetalLayerMetrics() {
    if (metal_layer_ == nil || content_view_ == nil) {
      return;
    }
    const NSSize logical_size = content_view_.bounds.size;
    NSSize backing_size = logical_size;
    if (content_view_.window != nil) {
      backing_size = [content_view_ convertSizeToBacking:logical_size];
    }
    const CGFloat scale =
        logical_size.height > 0.0 ? backing_size.height / logical_size.height : 1.0;
    [metal_layer_ setContentsScale:scale];
    [metal_layer_ setDrawableSize:NSSizeToCGSize(backing_size)];
    [metal_layer_ setFrame:content_view_.bounds];
    [metal_layer_ setFramebufferOnly:NO];
    [metal_layer_ setOpaque:YES];
  }

  NSWindow* window_ = nil;
  RexContentView* content_view_ = nil;
  RexWindowDelegate* window_delegate_ = nil;
  CAMetalLayer* metal_layer_ = nil;
  NSTimer* paint_timer_ = nil;
};

std::unique_ptr<Window> Window::Create(WindowedAppContext& app_context,
                                       const std::string_view title, uint32_t desired_logical_width,
                                       uint32_t desired_logical_height) {
  return std::make_unique<MacWindow>(app_context, title, desired_logical_width,
                                     desired_logical_height);
}

std::unique_ptr<MenuItem> MenuItem::Create(Type type, const std::string& text,
                                           const std::string& hotkey,
                                           std::function<void()> callback) {
  return std::make_unique<MacMenuItem>(type, text, hotkey, std::move(callback));
}

}  // namespace rex::ui

@implementation RexContentView

+ (Class)layerClass {
  return [CAMetalLayer class];
}

- (BOOL)wantsUpdateLayer {
  return YES;
}

- (CALayer*)makeBackingLayer {
  return [self.class.layerClass layer];
}

- (BOOL)isFlipped {
  return YES;
}

- (void)updateDrawableSize {
  if ([self.layer isKindOfClass:[CAMetalLayer class]] && self.owner != nullptr) {
    self.owner->OnNativeResize();
  }
}

- (void)viewDidChangeBackingProperties {
  [super viewDidChangeBackingProperties];
  [self updateDrawableSize];
}

- (void)setFrameSize:(NSSize)newSize {
  [super setFrameSize:newSize];
  (void)newSize;
  [self updateDrawableSize];
}

- (void)drawRect:(NSRect)dirtyRect {
  [super drawRect:dirtyRect];
  if (self.owner != nullptr) {
    self.owner->OnNativePaint();
  }
}

- (void)requestOwnerPaint {
  if (self.owner != nullptr) {
    self.owner->OnNativePaint();
  }
}

@end

@implementation RexWindowDelegate

- (void)windowDidResize:(NSNotification*)notification {
  (void)notification;
  if (self.owner != nullptr) {
    self.owner->OnNativeResize();
  }
}

- (void)windowDidBecomeKey:(NSNotification*)notification {
  (void)notification;
  if (self.owner != nullptr) {
    self.owner->OnNativeFocusChanged(true);
  }
}

- (void)windowDidResignKey:(NSNotification*)notification {
  (void)notification;
  if (self.owner != nullptr) {
    self.owner->OnNativeFocusChanged(false);
  }
}

- (void)windowWillClose:(NSNotification*)notification {
  (void)notification;
  if (self.owner != nullptr) {
    self.owner->OnNativeClose();
  }
}

@end
