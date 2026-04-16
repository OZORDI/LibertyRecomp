# rexinput - Input system library
# Input system and drivers for Xbox 360 controller emulation

add_library(rexinput STATIC
    input_system.cpp
    mnk/mnk_input_driver.cpp
    nop/nop_input_driver.cpp
)
add_library(rex::input ALIAS rexinput)

target_include_directories(rexinput PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Default backend: SDL on desktop. xinput additionally on Windows.
if(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4")
    target_sources(rexinput PRIVATE ps4/ps4_input_driver.cpp)
    # ScePad_stub / SceUserService_stub are linked globally via the
    # PS4 toolchain file (-lScePad -lSceUserService). Nothing extra needed here.
elseif(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch")
    # Switch (Nintendo hid) input backend — owned by A9.
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/switch/switch_input_driver.cpp")
        target_sources(rexinput PRIVATE switch/switch_input_driver.cpp)
    endif()
elseif(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ios")
    # iOS GameController.framework backend — owned by A10.
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ios/ios_input_driver.mm")
        target_sources(rexinput PRIVATE ios/ios_input_driver.mm)
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ios/ios_input_driver.cpp")
        target_sources(rexinput PRIVATE ios/ios_input_driver.cpp)
    endif()
elseif(ANDROID OR LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "android")
    # Android input backend — owned by A8.
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/android/android_input_driver.cpp")
        target_sources(rexinput PRIVATE android/android_input_driver.cpp)
    endif()
    # Desktop fallbacks still useful on some Android devtools; keep SDL only.
    target_sources(rexinput PRIVATE sdl/sdl_input_driver.cpp)
elseif(WIN32)
    target_sources(rexinput PRIVATE sdl/sdl_input_driver.cpp xinput/xinput_input_driver.cpp)
else()
    # macOS / Linux — SDL only.
    target_sources(rexinput PRIVATE sdl/sdl_input_driver.cpp)
endif()

target_link_libraries(rexinput PUBLIC rexcore)

# SDL is only linked on platforms that actually compile the SDL driver.
if(NOT (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR
        LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR
        LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ios"))
    target_link_libraries(rexinput PUBLIC SDL3::SDL3)
endif()
