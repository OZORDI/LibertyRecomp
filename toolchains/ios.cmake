# iOS CMake Toolchain
# Requirements:
#   - macOS host with Xcode (xcode-select --install)
#   - iOS SDK >= 9.0 (bundled with Xcode)
#   - SDL2 iOS project files (in thirdparty/SDL2)
#
# Usage:
#   cmake -DCMAKE_TOOLCHAIN_FILE=toolchains/ios.cmake \
#         -DLIBERTY_RECOMP_TARGET_PLATFORM=ios \
#         -DLIBERTY_RECOMP_EMBEDDED_GAME_PATH=tools/local_game_payload \
#         -G Xcode ..

set(CMAKE_SYSTEM_NAME iOS)
set(CMAKE_SYSTEM_PROCESSOR arm64)

# Minimum deployment target
set(CMAKE_OSX_DEPLOYMENT_TARGET "16.0" CACHE STRING "Minimum iOS version")
set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Target architecture")

# Use Xcode's embedded iOS SDK
set(CMAKE_OSX_SYSROOT iphoneos CACHE STRING "iOS SDK sysroot")

# Require Xcode generator for proper signing / xcframework support
if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
    message(WARNING "iOS builds are best built with -G Xcode. "
                    "Other generators may not handle code signing correctly.")
endif()

# We use Metal on iOS (same path as macOS)
set(LIBERTY_RECOMP_METAL ON CACHE BOOL "Use Metal renderer" FORCE)
set(LIBERTY_RECOMP_D3D12 OFF CACHE BOOL "" FORCE)
set(LIBERTY_RECOMP_VULKAN OFF CACHE BOOL "" FORCE)

# No installer wizard on iOS
set(LIBERTY_RECOMP_EMBEDDED_ASSETS ON CACHE BOOL "Package assets at build time (no installer UI)" FORCE)

# ── User-settable iOS signing ──────────────────────────────────────────────
# Can be overridden via -D flags on the cmake command line or in the CMake GUI.
# The DEVELOPMENT_TEAM is required for on-device builds; simulator builds work
# without it. Find your Team ID at https://developer.apple.com/account (Membership).
set(LIBERTY_IOS_DEVELOPMENT_TEAM ""
    CACHE STRING "Apple Developer Team ID (e.g. ABCDE12345) — required for on-device builds")
set(LIBERTY_IOS_BUNDLE_IDENTIFIER "com.libertyrecomp"
    CACHE STRING "iOS bundle identifier (reverse-DNS, must match provisioning profile)")
set(LIBERTY_IOS_PROVISIONING_PROFILE ""
    CACHE STRING "Provisioning profile UUID (optional — leave blank for automatic signing)")
