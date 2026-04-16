/**
 ******************************************************************************
 * ReXGlue : Runtime glue layer                                               *
 ******************************************************************************
 * Android (JNI) keyboard dialog backend.
 *
 * Shows a native AlertDialog with an EditText on the main (UI) thread and
 * blocks the calling thread until the user confirms or cancels, using a
 * CountDownLatch handed back from Java.
 *
 * Expects the app to expose a Java helper class:
 *   com.libertyrecomp.AndroidDialogFragment
 *     public static String showKeyboardBlocking(
 *         String title, String description, String defaultText,
 *         int maxLength, int flags);
 * returning the entered string, or null on cancel. Flags bits match the
 * KeyboardDialogFlag enum.
 *
 * If a JavaVM/env cannot be obtained (unusual), falls back to default_text.
 ******************************************************************************/

#include <rex/platform.h>

#if REX_PLATFORM_ANDROID

#include <rex/kernel/xam/keyboard_dialog.h>
#include <rex/logging.h>

#include <jni.h>

#include <cstring>
#include <string>

// Provided by the Android app glue (same mechanism SDL/NativeActivity use).
// LibertyRecomp's android startup stashes the JavaVM here.
extern "C" JavaVM* rex_android_get_jvm();

namespace rex {
namespace kernel {
namespace xam {

namespace {

struct JniAttach {
  JavaVM* vm = nullptr;
  JNIEnv* env = nullptr;
  bool detach = false;

  explicit JniAttach(JavaVM* v) : vm(v) {
    if (!vm) return;
    int status = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    if (status == JNI_EDETACHED) {
      if (vm->AttachCurrentThread(&env, nullptr) == JNI_OK) {
        detach = true;
      } else {
        env = nullptr;
      }
    } else if (status != JNI_OK) {
      env = nullptr;
    }
  }
  ~JniAttach() {
    if (detach && vm) vm->DetachCurrentThread();
  }
};

static KeyboardDialogResult RunBackend(const KeyboardDialogParams& params) {
  KeyboardDialogResult result;
  result.accepted = false;
  result.text = params.default_text;

  JavaVM* vm = rex_android_get_jvm();
  JniAttach att(vm);
  if (!att.env) {
    REXKRNL_WARN("[keyboard_dialog][android] No JNIEnv available");
    return result;
  }
  JNIEnv* env = att.env;

  jclass cls = env->FindClass("com/libertyrecomp/AndroidDialogFragment");
  if (!cls) {
    if (env->ExceptionCheck()) env->ExceptionClear();
    REXKRNL_WARN("[keyboard_dialog][android] AndroidDialogFragment not found");
    return result;
  }

  jmethodID mid = env->GetStaticMethodID(
      cls, "showKeyboardBlocking",
      "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;II)"
      "Ljava/lang/String;");
  if (!mid) {
    if (env->ExceptionCheck()) env->ExceptionClear();
    env->DeleteLocalRef(cls);
    REXKRNL_WARN(
        "[keyboard_dialog][android] showKeyboardBlocking method missing");
    return result;
  }

  jstring j_title = env->NewStringUTF(params.title.c_str());
  jstring j_desc = env->NewStringUTF(params.description.c_str());
  jstring j_default = env->NewStringUTF(params.default_text.c_str());
  jint j_max_length = static_cast<jint>(
      params.max_length > 0 ? params.max_length : 256);
  jint j_flags = static_cast<jint>(params.flags);

  jobject ret = env->CallStaticObjectMethod(cls, mid, j_title, j_desc,
                                            j_default, j_max_length, j_flags);
  if (env->ExceptionCheck()) {
    env->ExceptionDescribe();
    env->ExceptionClear();
    ret = nullptr;
  }

  if (ret) {
    const char* utf = env->GetStringUTFChars(static_cast<jstring>(ret), nullptr);
    if (utf) {
      result.accepted = true;
      result.text = std::string(utf);
      env->ReleaseStringUTFChars(static_cast<jstring>(ret), utf);
    }
    env->DeleteLocalRef(ret);
  }

  env->DeleteLocalRef(j_title);
  env->DeleteLocalRef(j_desc);
  env->DeleteLocalRef(j_default);
  env->DeleteLocalRef(cls);
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

#endif  // REX_PLATFORM_ANDROID
