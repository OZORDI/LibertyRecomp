# Android NDK CMake Toolchain wrapper
# Requirements:
#   - Android SDK >= 34        (set ANDROID_HOME or ANDROID_SDK_ROOT)
#   - NDK r26b (26.1.10909125) (set ANDROID_NDK or let SDK manager install it)
#   - Java 17+                 (for Gradle)
#
# Usage:
#   cmake -DCMAKE_TOOLCHAIN_FILE=toolchains/android.cmake \
#         -DANDROID_ABI=arm64-v8a \
#         -DANDROID_PLATFORM=android-26 \
#         -DLIBERTY_RECOMP_TARGET_PLATFORM=android \
#         -DLIBERTY_RECOMP_EMBEDDED_GAME_PATH=tools/local_game_payload \
#         ..
#
# Or use Gradle (recommended for packaging):
#   cd android/ && ./gradlew assembleRelease

# Resolve NDK path
if(NOT DEFINED ANDROID_NDK)
    if(DEFINED ENV{ANDROID_NDK})
        set(ANDROID_NDK "$ENV{ANDROID_NDK}")
    elseif(DEFINED ENV{ANDROID_HOME})
        # Try to find latest NDK inside SDK
        file(GLOB _ndk_dirs "$ENV{ANDROID_HOME}/ndk/*")
        list(SORT _ndk_dirs ORDER DESCENDING)
        list(GET _ndk_dirs 0 ANDROID_NDK)
    elseif(DEFINED ENV{ANDROID_SDK_ROOT})
        file(GLOB _ndk_dirs "$ENV{ANDROID_SDK_ROOT}/ndk/*")
        list(SORT _ndk_dirs ORDER DESCENDING)
        list(GET _ndk_dirs 0 ANDROID_NDK)
    endif()
endif()

if(NOT EXISTS "${ANDROID_NDK}")
    message(FATAL_ERROR
        "Android NDK not found. Set ANDROID_NDK, ANDROID_HOME, or ANDROID_SDK_ROOT.\n"
        "Install via: sdkmanager --install 'ndk;26.1.10909125'")
endif()

# Canonical NDK version for this project (for docs/reference; not enforced):
#   NDK 26.1.10909125 (r26b) — matches Gradle `ndkVersion` and the CI workflow.

message(STATUS "Using Android NDK: ${ANDROID_NDK}")

# Chain-load the real NDK toolchain
include("${ANDROID_NDK}/build/cmake/android.toolchain.cmake")

# Defaults
if(NOT DEFINED ANDROID_ABI)
    set(ANDROID_ABI "arm64-v8a")
endif()
if(NOT DEFINED ANDROID_PLATFORM)
    set(ANDROID_PLATFORM "android-26")
endif()
if(NOT DEFINED ANDROID_STL)
    # c++_static: LibertyRecomp ships as a single native library (libLibertyRecomp.so).
    # With only one .so there is no risk of duplicated STL instances across libraries,
    # and static linking keeps APK packaging simpler. Matches Gradle build.gradle.kts.
    set(ANDROID_STL "c++_static")
endif()

# Force Vulkan on Android
set(LIBERTY_RECOMP_VULKAN ON CACHE BOOL "Use Vulkan renderer" FORCE)
set(LIBERTY_RECOMP_D3D12 OFF CACHE BOOL "" FORCE)
set(LIBERTY_RECOMP_METAL OFF CACHE BOOL "" FORCE)

# No installer wizard on Android
set(LIBERTY_RECOMP_EMBEDDED_ASSETS ON CACHE BOOL "Package assets at build time (no installer UI)" FORCE)
