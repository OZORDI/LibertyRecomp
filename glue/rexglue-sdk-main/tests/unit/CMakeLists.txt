# Unit Tests for ReXGlue core components

add_executable(unit_tests
    memory/heap_allocation_test.cpp
    kernel/object_table_test.cpp
    core/cvar_test.cpp
    core/diagnostics_policy_test.cpp
    core/fiber_test.cpp
    core/gta4_transition_test.cpp
    core/gpu_flight_recorder_test.cpp
    core/sha256_test.cpp
    core/stream_test.cpp
    core/byte_order_test.cpp
    core/chrono_test.cpp
    audio/audio_conversion_test.cpp
    audio/audio_ring_test.cpp
    audio/audio_clock_test.cpp
    graphics/postfx_resource_pool_test.cpp
    graphics/depth_handoff_util_test.cpp
    graphics/alpha_to_coverage_util_test.cpp
    graphics/native_clip_control_test.cpp
    graphics/native_fixed_function_policy_test.cpp
    graphics/native_pipeline_policy_test.cpp
    graphics/native_pipeline_recipe_test.cpp
    graphics/native_pipeline_compiler_test.cpp
    graphics/native_pipeline_lookup_memo_test.cpp
    graphics/native_draw_state_cache_test.cpp
    graphics/native_resource_content_policy_test.cpp
    graphics/native_sampler_cache_key_test.cpp
    graphics/native_sampler_lod_bias_test.cpp
    graphics/smaa_source_sync_test.cpp
    graphics/lighting_semantics_test.cpp
    graphics/native_lighting_transport_test.cpp
    graphics/native_spirv_reflection_test.cpp
    graphics/shader_override_policy_test.cpp
    graphics/dirty_state_delta_test.cpp
    graphics/native_frame_scheduling_test.cpp
    graphics/native_msaa_policy_test.cpp
    graphics/hdr_policy_test.cpp
    graphics/texture_filtering_policy_test.cpp
    graphics/anisotropic_filtering_policy_test.cpp
    graphics/native_black_anomaly_test.cpp
    graphics/native_buffer_arena_test.cpp
    graphics/native_owner_retirement_test.cpp
    graphics/native_optimization_lifetime_test.cpp
    graphics/native_buffer_metadata_test.cpp
    graphics/native_descriptor_backend_test.cpp
    graphics/native_working_set_test.cpp
    graphics/native_hotpath_policy_test.cpp
    graphics/native_hotpath_cache_test.cpp
    graphics/native_texture_eviction_index_test.cpp
    graphics/native_working_set_vulkan_contract_test.cpp
    graphics/native_descriptor_tuple_cache_test.cpp
    graphics/native_frame_context_test.cpp
    graphics/native_gpu_attribution_test.cpp
    graphics/native_gpu_pass_origin_test.cpp
    graphics/native_reflection_registry_test.cpp
    graphics/native_resolve_policy_test.cpp
    graphics/native_submission_lifetime_test.cpp
    graphics/vulkan_submission_completion_test.cpp
    graphics/native_virtual_resource_registry_test.cpp
    graphics/shadow_distance_util_test.cpp
    graphics/gta4_native_surface_view_test.cpp
    graphics/native_performance_samples_test.cpp
  graphics/native_cpu_profile_test.cpp
    graphics/native_memory_samples_test.cpp
    graphics/pipeline_cache_file_test.cpp
    graphics/primitive_restart_test.cpp
    input/absolute_pointer_test.cpp
    input/controller_compatibility_test.cpp
    input/encoded_action_test.cpp
    input/gta4_input_action_routing_test.cpp
    input/gta4_keyboard_controller_test.cpp
    input/gta4_vehicle_weapon_policy_test.cpp
    input/gta4_vehicle_mouse_policy_test.cpp
    input/context_touch_layout_test.cpp
    input/controls_reliability_test.cpp
    input/input_rumble_routing_test.cpp
    input/input_state_polling_test.cpp
    input/mnk_input_driver_test.cpp
    input/sdl_axis_policy_test.cpp
    input/pointer_motion_test.cpp
    input/sdl_hotplug_policy_test.cpp
    input/sdl_rumble_policy_test.cpp
    codegen/config_test.cpp
    codegen/template_registry_test.cpp
    ppc/import_function_test.cpp
    ppc/stack_test.cpp
    system/guest_path_test.cpp
    system/gta4_frontend_menu_policy_test.cpp
    system/gta4_aspect_policy_test.cpp
    system/gta4_quicksave_policy_test.cpp
    system/gta4_draw_distance_policy_test.cpp
    system/gta4_frame_limiter_test.cpp
    system/gta4_touch_policy_test.cpp
    system/gta4_loading_parallax_fix_test.cpp
    system/gta4_source_inspector_test.cpp
    system/gta4_population_policy_test.cpp
    system/gta4_multiplayer_64_policy_test.cpp
    system/gta4_voice_policy_test.cpp
    system/content_manager_test.cpp
    system/achievement_manager_test.cpp
    system/arbitration_async_test.cpp
    system/community_profile_policy_test.cpp
    system/community_transport_reliability_test.cpp
    system/function_dispatcher_test.cpp
    system/live_compatibility_test.cpp
    system/multiplayer_validation_test.cpp
    system/lzx_test.cpp
    system/online_services_test.cpp
    system/title_storage_test.cpp
    system/user_profile_test.cpp
    system/xam_user_identity_policy_test.cpp
    system/xgi_stats_abi_test.cpp
    system/xfile_io_test.cpp
    codegen/manifest_test.cpp
    rexglue/migration_scan_test.cpp
    rexglue/sdl_mouse_motion_policy_test.cpp
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
    ${PROJECT_SOURCE_DIR}/gta4-recomp/src/input/context_touch_layout.cpp
    ${PROJECT_SOURCE_DIR}/gta4-recomp/src/install/gta4_source_inspector.cpp
    ${PROJECT_SOURCE_DIR}/src/graphics/gta4_native/native_performance_samples.cpp
    ${PROJECT_SOURCE_DIR}/src/graphics/gta4_native/native_memory_samples.cpp
    ${PROJECT_SOURCE_DIR}/src/graphics/gta4_native/native_buffer_arena.cpp
    ${PROJECT_SOURCE_DIR}/src/graphics/gta4_native/native_descriptor_backend.cpp
    ${PROJECT_SOURCE_DIR}/src/graphics/gta4_native/native_frame_context.cpp
    ${PROJECT_SOURCE_DIR}/src/graphics/gta4_native/native_gpu_attribution.cpp
    ${PROJECT_SOURCE_DIR}/src/graphics/gta4_native/native_submission_lifetime.cpp
    ${PROJECT_SOURCE_DIR}/src/rexglue/commands/migration_scan.cpp
    ${PROJECT_SOURCE_DIR}/src/rexglue/ui/ui.cpp
    ${PROJECT_SOURCE_DIR}/src/rexglue/ui/progress.cpp
)

target_include_directories(unit_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/gta4-recomp/src
    ${PROJECT_SOURCE_DIR}/thirdparty/crypto
)

target_link_libraries(unit_tests PRIVATE
    rexcodegen
    rexruntime
    rexinput
    Catch2::Catch2WithMain
    xxHash::xxhash
)

# Exercise the lighting command/transport policies without a running game or GPU.
# Normal application builds still compile the production hook and renderer users.
if(REXGLUE_USE_VULKAN)
add_executable(native_lighting_tests
    graphics/lighting_semantics_test.cpp
    graphics/native_lighting_transport_test.cpp
    graphics/native_fixed_function_policy_test.cpp
    graphics/native_clip_control_test.cpp
    graphics/native_resolve_policy_test.cpp
    graphics/shader_override_policy_test.cpp
    graphics/gta4_native_surface_view_test.cpp
)
target_include_directories(native_lighting_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/simde
    ${PROJECT_SOURCE_DIR}/thirdparty/fmt/include
)
target_link_libraries(native_lighting_tests PRIVATE Catch2::Catch2WithMain volk_headers)
endif()

# Exercise presentation policies and the real cvar registration/persistence path.
add_executable(presentation_options_tests
    system/gta4_presentation_options_test.cpp
    system/gta4_frontend_menu_policy_test.cpp
    ${PROJECT_SOURCE_DIR}/gta4-recomp/src/gta4_presentation_options.cpp
)
target_include_directories(presentation_options_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/gta4-recomp/src
)
target_link_libraries(presentation_options_tests PRIVATE rexcore Catch2::Catch2WithMain)
set_target_properties(presentation_options_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:rexruntime>
)

# Font instrumentation policies have no game or GPU dependency.
add_executable(font_selection_tests system/gta4_font_selection_policy_test.cpp)
target_include_directories(font_selection_tests PRIVATE ${PROJECT_SOURCE_DIR}/gta4-recomp/src)
target_link_libraries(font_selection_tests PRIVATE Catch2::Catch2WithMain)
set_target_properties(font_selection_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:rexruntime>)

# Keep the failure recorder testable without linking the game or a GPU driver.
add_executable(gpu_flight_recorder_tests
    core/gpu_flight_recorder_test.cpp
    ${PROJECT_SOURCE_DIR}/src/core/gpu_flight_recorder.cpp
)
target_include_directories(gpu_flight_recorder_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(gpu_flight_recorder_tests PRIVATE Catch2::Catch2WithMain)

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

# Keep the generated-PPC stats ABI contract runnable even when unrelated
# runtime/platform objects prevent the broad unit_tests target from linking.
add_executable(xgi_stats_abi_tests
    system/xgi_stats_abi_test.cpp
)
target_include_directories(xgi_stats_abi_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/thirdparty/fmt/include
    ${PROJECT_SOURCE_DIR}/thirdparty/simde
)
target_link_libraries(xgi_stats_abi_tests PRIVATE
    Catch2::Catch2WithMain
)
set_target_properties(xgi_stats_abi_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:rexruntime>
)

add_executable(community_stats_policy_tests
    system/community_stats_policy_test.cpp
    system/community_transport_reliability_test.cpp
)
target_include_directories(community_stats_policy_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/gta4-recomp/src
)
target_link_libraries(community_stats_policy_tests PRIVATE
    Catch2::Catch2WithMain
)
set_target_properties(community_stats_policy_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:rexruntime>
)

add_executable(profile_scalar_setting_tests
    system/profile_scalar_setting_test.cpp
)
target_include_directories(profile_scalar_setting_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/thirdparty/fmt/include
    ${PROJECT_SOURCE_DIR}/thirdparty/simde
)
target_link_libraries(profile_scalar_setting_tests PRIVATE
    rexcore
    Catch2::Catch2WithMain
)
set_target_properties(profile_scalar_setting_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:rexruntime>
)

add_executable(arbitration_async_tests
    system/arbitration_async_test.cpp
)
target_include_directories(arbitration_async_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/gta4-recomp/src
)
target_link_libraries(arbitration_async_tests PRIVATE
    rexruntime
    Catch2::Catch2WithMain
)
set_target_properties(arbitration_async_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:rexruntime>
)

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
if(TARGET native_lighting_tests)
catch_discover_tests(native_lighting_tests
    TEST_PREFIX "unit.native-lighting."
    PROPERTIES LABELS "unit;native-lighting" TIMEOUT 60
)
endif()
catch_discover_tests(unit_tests
    TEST_PREFIX "unit."
    PROPERTIES
        LABELS "unit"
        TIMEOUT 60
)
catch_discover_tests(xgi_stats_abi_tests
    TEST_PREFIX "unit.xgi-stats-abi."
    PROPERTIES
        LABELS "unit"
        TIMEOUT 60
)
catch_discover_tests(community_stats_policy_tests
    TEST_PREFIX "unit.community-stats-policy."
    PROPERTIES
        LABELS "unit"
        TIMEOUT 60
)
catch_discover_tests(presentation_options_tests
    TEST_PREFIX "unit.presentation-options."
    PROPERTIES LABELS "unit" TIMEOUT 60
)
catch_discover_tests(profile_scalar_setting_tests
    TEST_PREFIX "unit.profile-scalar-setting."
    PROPERTIES
        LABELS "unit"
        TIMEOUT 60
)
catch_discover_tests(arbitration_async_tests
    TEST_PREFIX "unit.arbitration-async."
    PROPERTIES
        LABELS "unit"
        TIMEOUT 60
)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_test(
    NAME unit.diagnostics-source-audit
    COMMAND ${Python3_EXECUTABLE}
            ${PROJECT_SOURCE_DIR}/tools/check_diagnostics_gates.py
)
set_tests_properties(unit.diagnostics-source-audit PROPERTIES LABELS "unit" TIMEOUT 60)

add_test(
    NAME unit.gta4-frontend-advanced-menu-layout
    COMMAND ${Python3_EXECUTABLE}
            ${PROJECT_SOURCE_DIR}/gta4-recomp/tools/derive_frontend_advanced_menu_layout.py
)
set_tests_properties(unit.gta4-frontend-advanced-menu-layout
    PROPERTIES LABELS "unit" TIMEOUT 60)

add_test(
    NAME unit.gta4-presentation-original-contracts
    COMMAND ${Python3_EXECUTABLE} -B
            ${PROJECT_SOURCE_DIR}/gta4-recomp/tools/verify_presentation_contracts.py
)
set_tests_properties(unit.gta4-presentation-original-contracts
    PROPERTIES LABELS "unit;presentation" TIMEOUT 60)

add_test(
    NAME unit.gta4-touch-layout
    COMMAND ${Python3_EXECUTABLE}
            ${PROJECT_SOURCE_DIR}/gta4-recomp/tools/verify_gta4_touch_layout.py
)
set_tests_properties(unit.gta4-touch-layout PROPERTIES LABELS "unit" TIMEOUT 60)

add_test(
    NAME unit.gta4-title-profile-contract
    COMMAND ${Python3_EXECUTABLE}
            ${PROJECT_SOURCE_DIR}/gta4-recomp/tools/derive_title_profile_contract.py
)
set_tests_properties(unit.gta4-title-profile-contract PROPERTIES LABELS "unit" TIMEOUT 60)

add_test(
    NAME unit.gta4-stats-retry-contract
    COMMAND ${Python3_EXECUTABLE}
            ${PROJECT_SOURCE_DIR}/gta4-recomp/tools/derive_stats_retry_contract.py
)
set_tests_properties(unit.gta4-stats-retry-contract PROPERTIES LABELS "unit" TIMEOUT 60)

add_test(
    NAME unit.gta4-social-cache-contract
    COMMAND ${Python3_EXECUTABLE}
            ${PROJECT_SOURCE_DIR}/gta4-recomp/tools/derive_social_cache_contract.py
)
set_tests_properties(unit.gta4-social-cache-contract PROPERTIES LABELS "unit" TIMEOUT 60)

add_test(
    NAME unit.gta4-community-reconnect-contract
    COMMAND ${Python3_EXECUTABLE}
            ${PROJECT_SOURCE_DIR}/gta4-recomp/tools/derive_community_reconnect_contract.py
)
set_tests_properties(unit.gta4-community-reconnect-contract PROPERTIES LABELS "unit" TIMEOUT 60)

add_test(
    NAME unit.gta4-network-transport-contract
    COMMAND ${Python3_EXECUTABLE}
            ${PROJECT_SOURCE_DIR}/../../gta_iv/xex_excavation_retail/audit_network_transport_contracts.py
)
set_tests_properties(unit.gta4-network-transport-contract
    PROPERTIES LABELS "unit" TIMEOUT 60)

add_test(
    NAME unit.gta4-arbitration-async-contract
    COMMAND ${Python3_EXECUTABLE}
            ${PROJECT_SOURCE_DIR}/gta4-recomp/tools/derive_arbitration_async_contract.py
)
set_tests_properties(unit.gta4-arbitration-async-contract
    PROPERTIES LABELS "unit" TIMEOUT 60)

# Compile the real community adapter for isolated loopback acceptance tests.
# It creates temporary identities and never starts generated game code.
if(UNIX AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS" AND NOT ANDROID)
  find_package(CURL QUIET)
  find_package(OpenSSL 3.0 QUIET COMPONENTS Crypto)
  if(TARGET CURL::libcurl AND TARGET OpenSSL::Crypto)
    if(CURL_INCLUDE_DIR MATCHES "/MacOSX[^/]*/usr/include$")
      set_property(TARGET CURL::libcurl PROPERTY INTERFACE_INCLUDE_DIRECTORIES "")
    endif()
    add_executable(community_backend_integration
        ../regression/community_backend_integration.cpp
        ${PROJECT_SOURCE_DIR}/gta4-recomp/src/network/community_multiplayer.cpp)
    target_include_directories(community_backend_integration PRIVATE
        ${PROJECT_SOURCE_DIR}/gta4-recomp/src
        ${PROJECT_SOURCE_DIR}/thirdparty/inja/third_party/include)
    target_link_libraries(community_backend_integration PRIVATE
        rexruntime CURL::libcurl OpenSSL::Crypto utf8cpp)
    set_target_properties(community_backend_integration PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:rexruntime>)
  endif()
endif()

# Live menu settings, shader-family selection, frame latching and TLAD precedence.
add_executable(modern_shader_tests
    graphics/modern_shader_policy_test.cpp
    graphics/shader_override_policy_test.cpp
    graphics/native_pipeline_lookup_memo_test.cpp
    system/gta4_frontend_menu_policy_test.cpp
    ${PROJECT_SOURCE_DIR}/src/graphics/gta4_native/modern_shader_options.cpp
    ${PROJECT_SOURCE_DIR}/gta4-recomp/src/gta4_presentation_options.cpp
)
target_include_directories(modern_shader_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/gta4-recomp/src
)
target_link_libraries(modern_shader_tests PRIVATE rexcore Catch2::Catch2WithMain)
set_target_properties(modern_shader_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:rexruntime>)
