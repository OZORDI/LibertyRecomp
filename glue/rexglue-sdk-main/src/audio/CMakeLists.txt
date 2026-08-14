# rexaudio - Audio subsystem library
# Audio processing and XMA decoding for Xbox 360 emulation

add_library(rexaudio OBJECT
    
    audio_driver.cpp
    audio_system.cpp
    xma_context.cpp
    xma_decoder.cpp
    xma_register_file.cpp
    # NOP backend (always available)
    nop/nop_audio_system.cpp
    # SDL backend
    sdl/sdl_audio_system.cpp
    sdl/sdl_audio_driver.cpp
)
add_library(rex::audio ALIAS rexaudio)

if(APPLE AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    target_sources(rexaudio PRIVATE
        coreaudio/coreaudio_audio_system.cpp
        coreaudio/coreaudio_audio_driver.cpp
        coreaudio/coreaudio_output.cpp
    )
endif()

target_include_directories(rexaudio PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(rexaudio
    PUBLIC rexcore SDL3::SDL3
    PRIVATE libavcodec libavutil
)

if(APPLE AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    target_link_libraries(rexaudio PUBLIC
        "-framework AudioToolbox"
        "-framework CoreAudio"
    )
endif()
