# rexcore - Core utilities library
# Foundation types, data structures, and threading primitives

add_library(rexcore STATIC
    arena.cpp
    bit_map.cpp
    bit_stream.cpp
    byte_stream.cpp
    clock.cpp
    cvar.cpp
    exception_handler.cpp
    filesystem.cpp
    filesystem_wildcard.cpp
    keyboard_dialog.cpp
    logging.cpp
    memory.cpp
    mutex.cpp
    ring_buffer.cpp
    sha256.cpp
    string.cpp
    string_buffer.cpp
    threading.cpp
    timer_queue.cpp
    utf8.cpp
    vec128.cpp
)
add_library(rex::core ALIAS rexcore)

# Platform-specific sources
if(WIN32)
    target_sources(rexcore PRIVATE
        atomic_win.cpp
        clock_win.cpp
        dbg_win.cpp
        dynlib_win.cpp
        exception_handler_win.cpp
        filesystem_win.cpp
        fiber_win32.cpp
        mapped_memory_win.cpp
        math_msvc.cpp
        memory_win.cpp
        seh_win.cpp
        socket_win.cpp
        string_win.cpp
        threading_win.cpp
    )
elseif(APPLE AND CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # iOS: Darwin/AArch64, BSD signals. Shares POSIX surface with macOS but
    # uses iOS-specific memory/fiber/net/logging/filesystem/dynlib files.
    target_sources(rexcore PRIVATE
        atomic_posix.cpp
        clock_posix.cpp
        dbg_posix.cpp
        math_gcc.cpp
        mapped_memory_posix.cpp
        seh_posix.cpp
        socket_posix.cpp
        string_posix.cpp
        threading_posix.cpp
        exception_handler_posix.cpp
    )
    foreach(_ios_src
        memory_ios.cpp
        fiber_ios.cpp
        net_ios.cpp
        logging_ios.cpp
        filesystem_ios.mm
        dynlib_ios.cpp)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_ios_src}")
            target_sources(rexcore PRIVATE ${_ios_src})
        endif()
    endforeach()
elseif(APPLE)
    target_sources(rexcore PRIVATE
        atomic_posix.cpp
        clock_posix.cpp
        dbg_posix.cpp
        dynlib_posix.cpp
        exception_handler_mac.cpp
        filesystem_posix.cpp
        fiber_mac.cpp
        mapped_memory_posix.cpp
        math_gcc.cpp
        memory_mac.cpp
        seh_posix.cpp
        socket_posix.cpp
        string_posix.cpp
        threading_mac.cpp
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    # Android: bionic libc, POSIX signals, AArch64/ARMv7. Shares the POSIX
    # sources but uses Android-specific memory/net/logging/filesystem.
    target_sources(rexcore PRIVATE
        atomic_posix.cpp
        clock_posix.cpp
        dbg_posix.cpp
        dynlib_posix.cpp
        exception_handler_posix.cpp
        fiber_posix.cpp
        mapped_memory_posix.cpp
        math_gcc.cpp
        seh_posix.cpp
        socket_posix.cpp
        string_posix.cpp
        threading_posix.cpp
    )
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/memory_android.cpp")
        target_sources(rexcore PRIVATE memory_android.cpp)
    else()
        target_sources(rexcore PRIVATE memory_posix.cpp)
    endif()
    foreach(_android_src
        net_android.cpp
        logging_android.cpp
        filesystem_android.cpp)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_android_src}")
            target_sources(rexcore PRIVATE ${_android_src})
        endif()
    endforeach()
elseif(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch")
    # Switch (libnx/newlib/AArch64): fork of POSIX sources using libnx kernel
    # APIs for threading + setjmp/longjmp-based fibers. threading_posix.cpp
    # and fiber_posix.cpp are NOT compiled on this target.
    target_sources(rexcore PRIVATE
        atomic_posix.cpp
        clock_posix.cpp
        dbg_posix.cpp
        exception_handler_posix.cpp
        filesystem_posix.cpp
        fiber_switch.cpp
        mapped_memory_posix.cpp
        math_gcc.cpp
        memory_switch.cpp
        seh_posix.cpp
        socket_posix.cpp
        string_posix.cpp
        threading_switch.cpp
    )
    foreach(_switch_src
        dynlib_switch.cpp
        exception_handler_switch.cpp
        mmio_handler_switch.cpp)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_switch_src}")
            target_sources(rexcore PRIVATE ${_switch_src})
        endif()
    endforeach()
elseif(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4")
    # PS4 (OpenOrbis / FreeBSD-like): fork of POSIX sources plus the local
    # musl-compatible ucontext shim (libc ships headers but no impl).
    # threading_posix.cpp and fiber_posix.cpp are NOT compiled on this target.
    target_sources(rexcore PRIVATE
        atomic_posix.cpp
        clock_posix.cpp
        dbg_posix.cpp
        exception_handler_posix.cpp
        filesystem_posix.cpp
        fiber_ps4.cpp
        ucontext_ps4.c
        mapped_memory_posix.cpp
        math_gcc.cpp
        memory_ps4.cpp
        seh_posix.cpp
        socket_posix.cpp
        string_posix.cpp
        threading_ps4.cpp
    )
    target_sources(rexcore PRIVATE dynlib_ps4.cpp)
elseif(UNIX)
    target_sources(rexcore PRIVATE
        atomic_posix.cpp
        clock_posix.cpp
        dbg_posix.cpp
        dynlib_posix.cpp
        exception_handler_posix.cpp
        filesystem_posix.cpp
        fiber_posix.cpp
        mapped_memory_posix.cpp
        math_gcc.cpp
        memory_posix.cpp
        seh_posix.cpp
        socket_posix.cpp
        string_posix.cpp
        threading_posix.cpp
    )
endif()

# Platform-specific networking (console targets)
if(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4")
    target_sources(rexcore PRIVATE net_ps4.cpp)
elseif(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch")
    target_sources(rexcore PRIVATE net_switch.cpp)
endif()

# Platform-specific keyboard dialog backends for XamShowKeyboardUI.
# Each file is self-guarded by #if REX_PLATFORM_* so unused TUs compile out.
if(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/keyboard_dialog_ps4.cpp")
        target_sources(rexcore PRIVATE keyboard_dialog_ps4.cpp)
    endif()
elseif(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/keyboard_dialog_switch.cpp")
        target_sources(rexcore PRIVATE keyboard_dialog_switch.cpp)
    endif()
elseif(APPLE AND CMAKE_SYSTEM_NAME STREQUAL "iOS")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/keyboard_dialog_ios.mm")
        target_sources(rexcore PRIVATE keyboard_dialog_ios.mm)
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/keyboard_dialog_android.cpp")
        target_sources(rexcore PRIVATE keyboard_dialog_android.cpp)
    endif()
endif()

target_include_directories(rexcore PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(rexcore PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/thirdparty/cli11/include
)

if(UNIX)
    target_link_libraries(rexcore PRIVATE Threads::Threads)
    if(CMAKE_DL_LIBS)
        target_link_libraries(rexcore PRIVATE ${CMAKE_DL_LIBS})
    endif()
    if(NOT APPLE)
        target_link_libraries(rexcore PRIVATE rt)
    endif()
endif()

target_link_libraries(rexcore
    PUBLIC
        fmt::fmt
        simde
        tomlplusplus
        spdlog::spdlog
    PRIVATE
        disruptorplus
        utf8cpp
)

if(REXGLUE_ENABLE_TRACY)
    target_link_libraries(rexcore PUBLIC TracyClient)
    target_compile_definitions(rexcore PUBLIC
        $<$<NOT:$<CONFIG:Release>>:REXGLUE_ENABLE_PROFILING>)
endif()

if(REXGLUE_ENABLE_PERF_COUNTERS)
    target_sources(rexcore PRIVATE perf/counter.cpp)
    target_compile_definitions(rexcore PUBLIC
        $<$<NOT:$<CONFIG:Release>>:REXGLUE_ENABLE_PERF_COUNTERS>)
endif()
