# rexinput - Input system library
# Input system and drivers for Xbox 360 controller emulation

add_library(rexinput OBJECT
    absolute_pointer.cpp
    input_system.cpp
    sony_feedback.cpp
    sony_feedback_options.cpp
    mnk/controller_compatibility.cpp
    mnk/mnk_input_driver.cpp
    mnk/pointer_motion.cpp
    nop/nop_input_driver.cpp
)
add_library(rex::input ALIAS rexinput)

target_include_directories(rexinput PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
if(WIN32)
    target_sources(rexinput PRIVATE sdl/sdl_input_driver.cpp sdl/physical_device_inventory.cpp xinput/xinput_input_driver.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Switch")
    target_sources(rexinput PRIVATE switch/switch_input_driver.cpp)
else()
    target_sources(rexinput PRIVATE sdl/sdl_input_driver.cpp sdl/physical_device_inventory.cpp)
endif()

target_link_libraries(rexinput PUBLIC rexcore rexui SDL3::SDL3)
if(APPLE AND NOT IOS)
    target_link_libraries(rexinput PUBLIC "-framework IOKit" "-framework CoreFoundation")
endif()
