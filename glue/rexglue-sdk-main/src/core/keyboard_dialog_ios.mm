/**
 ******************************************************************************
 * ReXGlue : Runtime glue layer                                               *
 ******************************************************************************
 * iOS keyboard dialog backend using UIAlertController + UITextField.
 *
 * The dialog must be presented from the main thread, but XamShowKeyboardUI is
 * invoked from guest worker threads. We dispatch_async onto the main queue to
 * present, then block the calling thread on a dispatch_semaphore that the
 * UIAlertAction handlers signal. Matches the console backends' synchronous
 * contract.
 ******************************************************************************/

#include <rex/platform.h>

#if REX_PLATFORM_IOS

#include <rex/kernel/xam/keyboard_dialog.h>
#include <rex/logging.h>

#import <UIKit/UIKit.h>
#include <dispatch/dispatch.h>

#include <atomic>
#include <string>

namespace rex {
namespace kernel {
namespace xam {

namespace {

static UIViewController* FindTopViewController() {
  UIWindow* keyWindow = nil;
  for (UIScene* scene in [[UIApplication sharedApplication] connectedScenes]) {
    if ([scene isKindOfClass:[UIWindowScene class]] &&
        scene.activationState == UISceneActivationStateForegroundActive) {
      UIWindowScene* ws = (UIWindowScene*)scene;
      for (UIWindow* w in ws.windows) {
        if (w.isKeyWindow) { keyWindow = w; break; }
      }
      if (keyWindow) break;
    }
  }
  UIViewController* vc = keyWindow.rootViewController;
  while (vc.presentedViewController) {
    vc = vc.presentedViewController;
  }
  return vc;
}

static KeyboardDialogResult RunBackend(const KeyboardDialogParams& params) {
  __block KeyboardDialogResult result;
  result.accepted = false;
  result.text = params.default_text;

  dispatch_semaphore_t sem = dispatch_semaphore_create(0);

  NSString* nsTitle =
      [NSString stringWithUTF8String:params.title.c_str() ?: ""];
  NSString* nsDesc =
      [NSString stringWithUTF8String:params.description.c_str() ?: ""];
  NSString* nsDefault =
      [NSString stringWithUTF8String:params.default_text.c_str() ?: ""];
  const uint32_t max_length = params.max_length > 0 ? params.max_length : 256;
  const uint32_t flags = params.flags;

  dispatch_async(dispatch_get_main_queue(), ^{
    UIViewController* top = FindTopViewController();
    if (!top) {
      REXKRNL_WARN("[keyboard_dialog][ios] No top view controller");
      dispatch_semaphore_signal(sem);
      return;
    }

    UIAlertController* alert =
        [UIAlertController alertControllerWithTitle:nsTitle
                                            message:nsDesc
                                     preferredStyle:UIAlertControllerStyleAlert];

    [alert addTextFieldWithConfigurationHandler:^(UITextField* tf) {
      tf.text = nsDefault;
      if (flags & kKeyboardDialogFlag_Password) {
        tf.secureTextEntry = YES;
      }
      if (flags & kKeyboardDialogFlag_Numeric) {
        tf.keyboardType = UIKeyboardTypeNumberPad;
      } else if (flags & kKeyboardDialogFlag_Email) {
        tf.keyboardType = UIKeyboardTypeEmailAddress;
      } else if (flags & kKeyboardDialogFlag_Uri) {
        tf.keyboardType = UIKeyboardTypeURL;
      } else {
        tf.keyboardType = UIKeyboardTypeDefault;
      }
      tf.autocorrectionType = UITextAutocorrectionTypeNo;
      tf.autocapitalizationType = UITextAutocapitalizationTypeNone;
    }];

    UIAlertAction* ok =
        [UIAlertAction actionWithTitle:@"OK"
                                 style:UIAlertActionStyleDefault
                               handler:^(UIAlertAction*) {
          UITextField* tf = alert.textFields.firstObject;
          NSString* entered = tf.text ?: @"";
          if (entered.length > max_length) {
            entered = [entered substringToIndex:max_length];
          }
          result.accepted = true;
          result.text = std::string([entered UTF8String] ?: "");
          dispatch_semaphore_signal(sem);
        }];
    UIAlertAction* cancel =
        [UIAlertAction actionWithTitle:@"Cancel"
                                 style:UIAlertActionStyleCancel
                               handler:^(UIAlertAction*) {
          result.accepted = false;
          dispatch_semaphore_signal(sem);
        }];
    [alert addAction:ok];
    [alert addAction:cancel];
    [top presentViewController:alert animated:YES completion:nil];
  });

  dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);
  return result;
}

struct Register {
  Register() { RegisterKeyboardDialogBackend(&RunBackend); }
};
static Register s_register;

}  // namespace

}  // namespace xam
}  // namespace kernel
}  // namespace rex

#endif  // REX_PLATFORM_IOS
