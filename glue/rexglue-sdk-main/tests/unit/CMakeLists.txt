# Unit Tests for ReXGlue core components

add_executable(unit_tests
    memory/heap_allocation_test.cpp
    kernel/object_table_test.cpp
    core/cvar_test.cpp
    core/fiber_test.cpp
    core/gta4_transition_test.cpp
    core/sha256_test.cpp
    core/stream_test.cpp
    core/byte_order_test.cpp
    core/chrono_test.cpp
    audio/audio_conversion_test.cpp
    audio/audio_ring_test.cpp
    audio/audio_clock_test.cpp
    graphics/postfx_resource_pool_test.cpp
    graphics/gta4_native_surface_view_test.cpp
    graphics/pipeline_cache_file_test.cpp
    graphics/primitive_restart_test.cpp
    input/pointer_motion_test.cpp
    codegen/config_test.cpp
    codegen/template_registry_test.cpp
    ppc/import_function_test.cpp
    ppc/stack_test.cpp
    system/guest_path_test.cpp
    system/gta4_loading_parallax_fix_test.cpp
    system/content_manager_test.cpp
    system/achievement_manager_test.cpp
    system/function_dispatcher_test.cpp
    system/live_compatibility_test.cpp
    codegen/manifest_test.cpp
    rexglue/migration_scan_test.cpp
    rexglue/ui_test.cpp
)

if(APPLE AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    target_sources(unit_tests PRIVATE audio/coreaudio_smoke_test.cpp)

    add_executable(coreaudio_smoke
        audio/coreaudio_smoke_main.cpp
        ${PROJECT_SOURCE_DIR}/src/audio/coreaudio/coreaudio_output.cpp
    )
    target_include_directories(coreaudio_smoke PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(coreaudio_smoke PRIVATE
        rexcore
        "-framework AudioToolbox"
        "-framework CoreAudio"
    )
    set_target_properties(coreaudio_smoke PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:rexruntime>
    )
endif()

target_sources(unit_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/src/rexglue/commands/migration_scan.cpp
    ${PROJECT_SOURCE_DIR}/src/rexglue/ui/ui.cpp
    ${PROJECT_SOURCE_DIR}/src/rexglue/ui/progress.cpp
)

target_include_directories(unit_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(unit_tests PRIVATE
    rexcodegen
    rexruntime
    rexinput
    Catch2::Catch2WithMain
)

# ReXApp is shipped as source for recomp projects, rather than compiled into
# rexui. Keep its SDK-consumer compilation path covered by the test build.
add_library(rexapp_compile_test OBJECT
    ${PROJECT_SOURCE_DIR}/src/ui/rex_app.cpp
)
target_link_libraries(rexapp_compile_test PRIVATE
    rexruntime
    rexui
    rexinput
    rexaudio
    rexgraphics
)
add_dependencies(unit_tests rexapp_compile_test)

# Strip Tracy/profiling instrumentation from test targets
target_compile_options(unit_tests PRIVATE
    -UTRACY_ENABLE -UREXGLUE_ENABLE_PROFILING
    -UREXGLUE_ENABLE_PERF_COUNTERS -UREXGLUE_PROFILE_GUEST_FUNCTIONS
)

set_target_properties(unit_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:rexruntime>
)
if(UNIX AND NOT APPLE)
    set_target_properties(unit_tests PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH ON
    )
endif()

# Discover tests
include(Catch)
catch_discover_tests(unit_tests
    TEST_PREFIX "unit."
    PROPERTIES
        LABELS "unit"
        TIMEOUT 60
)
