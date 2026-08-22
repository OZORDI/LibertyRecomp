# rexgpu-xenos: Xenos GPU emulation plugin (ported from Xenia)
# Namespace: rex::graphics
#
# Runtime-loaded via rex::system::LoadGpuPlugin; consumers never link this
# target. The Xenos format layer is compiled into rexcore.

set(REXGPU_XENOS_SOURCES
    # Core GPU types
    flags.cpp
    register_file.cpp
    registers.cpp
    pipeline/shader/shader.cpp
    format/ucode.cpp
    sampler_info.cpp

    # GPU system
    graphics_system.cpp
    command_processor.cpp
    trace_writer.cpp
    util/draw_extent_estimator.cpp
    util/draw.cpp
    packet_disassembler.cpp
    primitive_processor.cpp
    pipeline/render_target/cache.cpp
    shared_memory.cpp
    pipeline/texture/cache.cpp
    pipeline/shader/interpreter.cpp
    pipeline/shader/translator.cpp
    pipeline/shader/translator_disasm.cpp
)

# SPIR-V shader translation
set(REXGPU_XENOS_SPIRV_SOURCES
    pipeline/shader/spirv_builder.cpp
    pipeline/shader/spirv.cpp
    pipeline/shader/spirv_translator.cpp
    pipeline/shader/spirv_translator_alu.cpp
    pipeline/shader/spirv_translator_fetch.cpp
    pipeline/shader/spirv_translator_memexport.cpp
    pipeline/shader/spirv_translator_rb.cpp
)

# Vulkan backend
set(REXGPU_XENOS_VULKAN_SOURCES
    vulkan/graphics_system.cpp
    vulkan/deferred_command_buffer.cpp
    vulkan/command_processor.cpp
    vulkan/pipeline_cache.cpp
    vulkan/primitive_processor.cpp
    vulkan/render_target_cache.cpp
    vulkan/shader.cpp
    vulkan/shared_memory.cpp
    vulkan/texture_cache.cpp
)

# D3D12 backend
if(REXGLUE_USE_D3D12)
    # DXBC third-party dependency
    set(REXGPU_XENOS_DXBC_THIRDPARTY_SOURCES
        ${REXGLUE_ROOT}/thirdparty/dxbc/DXBCChecksum.cpp
    )

    # DXBC shader translation (required for D3D12)
    set(REXGPU_XENOS_DXBC_SOURCES
        pipeline/shader/dxbc.cpp
        pipeline/shader/dxbc_translator.cpp
        pipeline/shader/dxbc_translator_alu.cpp
        pipeline/shader/dxbc_translator_fetch.cpp
        pipeline/shader/dxbc_translator_memexport.cpp
        pipeline/shader/dxbc_translator_om.cpp
    )

    # D3D12 GPU backend
    set(REXGPU_XENOS_D3D12_SOURCES
        d3d12/graphics_system.cpp
        d3d12/command_processor.cpp
        d3d12/primitive_processor.cpp
        d3d12/render_target_cache.cpp
        d3d12/shader.cpp
        d3d12/shared_memory.cpp
        d3d12/texture_cache.cpp
        d3d12/deferred_command_list.cpp
        d3d12/pipeline_cache.cpp
    )
endif()

if(REXGLUE_USE_VULKAN)
    list(APPEND REXGPU_XENOS_SOURCES ${REXGPU_XENOS_SPIRV_SOURCES} ${REXGPU_XENOS_VULKAN_SOURCES})
endif()

if(REXGLUE_USE_D3D12)
    list(APPEND REXGPU_XENOS_SOURCES
        ${REXGPU_XENOS_DXBC_THIRDPARTY_SOURCES}
        ${REXGPU_XENOS_DXBC_SOURCES}
        ${REXGPU_XENOS_D3D12_SOURCES})
endif()

function(rexgpu_configure_plugin target export_name)
    set_target_properties(${target} PROPERTIES
        EXPORT_NAME ${export_name}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )

    target_include_directories(${target} PRIVATE
        ${REXGLUE_ROOT}/thirdparty/renderdoc
        ${REXGLUE_ROOT}  # For thirdparty/dxbc/DXBCChecksum.h include
    )

    target_compile_options(${target} PRIVATE -Wno-unused-variable)

    # Everything PRIVATE: plugins have no link-time consumers. Never link the
    # OBJECT libraries (rexcore/rexui) here; rexruntime already contains those
    # objects and relinking them would duplicate global state.
    target_link_libraries(${target} PRIVATE
        rexruntime
        xxHash::xxhash
        snappy
    )

    if(REXGLUE_USE_VULKAN)
        target_link_libraries(${target} PRIVATE
            volk::volk
            glslang::SPIRV
            GPUOpen::VulkanMemoryAllocator
            glslang
            MachineIndependent
            GenericCodeGen
            OSDependent
            OGLCompiler
        )
    endif()

    if(REXGLUE_USE_D3D12)
        target_link_libraries(${target} PRIVATE
            d3d12
            dxgi
            dxguid
        )
    endif()
endfunction()

add_library(rexgpu-xenos SHARED
    plugin_main.cpp
    ${REXGPU_XENOS_SOURCES}
)
add_library(rex::gpu-xenos ALIAS rexgpu-xenos)
rexgpu_configure_plugin(rexgpu-xenos gpu-xenos)

if(REXGLUE_USE_VULKAN)
    set(GTA4_NATIVE_SHADER_CACHE_ROOT "${CMAKE_SOURCE_DIR}/LibertyRecompLib")
    set(GTA4_NATIVE_SMOLV_ROOT "${CMAKE_SOURCE_DIR}/tools/XenosRecomp/thirdparty/smol-v/source")
    set(GTA4_NATIVE_ZSTD_ROOT "${CMAKE_SOURCE_DIR}/tools/XenosRecomp/thirdparty/zstd")
    set(GTA4_NATIVE_SHADER_OVERRIDE_ROOT
        "${GTA4_NATIVE_SHADER_CACHE_ROOT}/shader_overrides")
    set(GTA4_NATIVE_SHADER_OVERRIDE_MANIFEST
        "${GTA4_NATIVE_SHADER_OVERRIDE_ROOT}/manifest.json")
    set(GTA4_NATIVE_SHADER_OVERRIDE_COMPILER
        "${REXGLUE_ROOT}/tools/gta4_shader_override_compiler.py")
    set(GTA4_NATIVE_SHADER_OVERRIDE_CPP
        "${CMAKE_CURRENT_BINARY_DIR}/generated/gta4_native/shader_override_cache.cpp")
    set(GTA4_NATIVE_SHADER_OVERRIDE_DEPFILE
        "${CMAKE_CURRENT_BINARY_DIR}/generated/gta4_native/shader_override_cache.d")
    if(NOT EXISTS "${GTA4_NATIVE_SHADER_CACHE_ROOT}/shader/shader_cache.cpp")
        message(FATAL_ERROR "GTA IV native shader cache source was not found")
    endif()
    if(NOT EXISTS "${GTA4_NATIVE_SHADER_OVERRIDE_MANIFEST}" OR
       NOT EXISTS "${GTA4_NATIVE_SHADER_OVERRIDE_COMPILER}")
        message(FATAL_ERROR "GTA IV native shader override inputs were not found")
    endif()

    find_package(Python3 3.10 REQUIRED COMPONENTS Interpreter)
    find_program(GTA4_NATIVE_GLSLANG_VALIDATOR
        NAMES glslangValidator
        HINTS "$ENV{VULKAN_SDK}/bin" "$ENV{VULKAN_SDK}/Bin")
    find_program(GTA4_NATIVE_SPIRV_VAL
        NAMES spirv-val
        HINTS "$ENV{VULKAN_SDK}/bin" "$ENV{VULKAN_SDK}/Bin")

    string(TOLOWER "${CMAKE_HOST_SYSTEM_PROCESSOR}" GTA4_NATIVE_HOST_PROCESSOR)
    if(GTA4_NATIVE_HOST_PROCESSOR MATCHES "arm64|aarch64")
        set(GTA4_NATIVE_DXC_ARCH arm64)
    else()
        set(GTA4_NATIVE_DXC_ARCH x64)
    endif()
    set(GTA4_NATIVE_DXC_ROOT "${CMAKE_SOURCE_DIR}/tools/XenosRecomp/thirdparty/dxc-bin")
    if(CMAKE_HOST_WIN32)
        set(GTA4_NATIVE_DXC
            "${GTA4_NATIVE_DXC_ROOT}/bin/${GTA4_NATIVE_DXC_ARCH}/dxc.exe")
    elseif(CMAKE_HOST_APPLE)
        set(GTA4_NATIVE_DXC
            "${GTA4_NATIVE_DXC_ROOT}/bin/${GTA4_NATIVE_DXC_ARCH}/dxc-macos")
        set(GTA4_NATIVE_DXC_LIBRARY_PATH
            "${GTA4_NATIVE_DXC_ROOT}/lib/${GTA4_NATIVE_DXC_ARCH}")
    elseif(CMAKE_HOST_UNIX AND GTA4_NATIVE_DXC_ARCH STREQUAL "x64")
        set(GTA4_NATIVE_DXC "${GTA4_NATIVE_DXC_ROOT}/bin/x64/dxc-linux")
        set(GTA4_NATIVE_DXC_LIBRARY_PATH "${GTA4_NATIVE_DXC_ROOT}/lib/x64")
    endif()

    set(GTA4_NATIVE_SHADER_OVERRIDE_COMMAND
        "${Python3_EXECUTABLE}"
        "${GTA4_NATIVE_SHADER_OVERRIDE_COMPILER}"
        --manifest "${GTA4_NATIVE_SHADER_OVERRIDE_MANIFEST}"
        --source-root "${GTA4_NATIVE_SHADER_OVERRIDE_ROOT}"
        --output "${GTA4_NATIVE_SHADER_OVERRIDE_CPP}"
        --depfile "${GTA4_NATIVE_SHADER_OVERRIDE_DEPFILE}")
    if(GTA4_NATIVE_DXC AND EXISTS "${GTA4_NATIVE_DXC}")
        list(APPEND GTA4_NATIVE_SHADER_OVERRIDE_COMMAND --dxc "${GTA4_NATIVE_DXC}")
        if(GTA4_NATIVE_DXC_LIBRARY_PATH)
            list(APPEND GTA4_NATIVE_SHADER_OVERRIDE_COMMAND
                --dxc-library-path "${GTA4_NATIVE_DXC_LIBRARY_PATH}")
        endif()
    endif()
    if(GTA4_NATIVE_GLSLANG_VALIDATOR)
        list(APPEND GTA4_NATIVE_SHADER_OVERRIDE_COMMAND
            --glslang-validator "${GTA4_NATIVE_GLSLANG_VALIDATOR}")
    endif()
    if(GTA4_NATIVE_SPIRV_VAL)
        list(APPEND GTA4_NATIVE_SHADER_OVERRIDE_COMMAND
            --spirv-val "${GTA4_NATIVE_SPIRV_VAL}")
    endif()

    add_custom_command(
        OUTPUT "${GTA4_NATIVE_SHADER_OVERRIDE_CPP}"
        COMMAND ${GTA4_NATIVE_SHADER_OVERRIDE_COMMAND}
        DEPENDS
            "${GTA4_NATIVE_SHADER_OVERRIDE_MANIFEST}"
            "${GTA4_NATIVE_SHADER_OVERRIDE_COMPILER}"
            "${GTA4_NATIVE_SHADER_OVERRIDE_ROOT}/shader_override_cache.h"
        DEPFILE "${GTA4_NATIVE_SHADER_OVERRIDE_DEPFILE}"
        COMMENT "Compiling GTA IV hash-specific shader overrides"
        VERBATIM)

    if(NOT TARGET libzstd_static)
        set(ZSTD_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
        set(ZSTD_BUILD_SHARED OFF CACHE BOOL "" FORCE)
        set(ZSTD_BUILD_STATIC ON CACHE BOOL "" FORCE)
        set(ZSTD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        add_subdirectory("${GTA4_NATIVE_ZSTD_ROOT}/build/cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/gta4_native_zstd")
    endif()

    add_library(rexgpu-gta4-native SHARED
        gta4_native/graphics_system.cpp
        gta4_native/native_performance_samples.cpp
        gta4_native/postfx_resource_pool.cpp
        gta4_native/smaa_pipeline.cpp
        gta4_native/split_postfx_pass.cpp
        gta4_native/sun_shafts_pass.cpp
        gta4_native/plugin_main.cpp
        "${GTA4_NATIVE_SHADER_CACHE_ROOT}/shader/shader_cache.cpp"
        "${GTA4_NATIVE_SHADER_OVERRIDE_CPP}"
        "${GTA4_NATIVE_SMOLV_ROOT}/smolv.cpp"
    )
    set_source_files_properties(
        "${GTA4_NATIVE_SHADER_CACHE_ROOT}/shader/shader_cache.cpp"
        PROPERTIES COMPILE_OPTIONS "-Wno-missing-field-initializers"
    )
    add_library(rex::gpu-gta4-native ALIAS rexgpu-gta4-native)
    rexgpu_configure_plugin(rexgpu-gta4-native gpu-gta4-native)
    target_include_directories(rexgpu-gta4-native PRIVATE
        "${GTA4_NATIVE_SHADER_CACHE_ROOT}"
        "${GTA4_NATIVE_SMOLV_ROOT}"
    )
    target_compile_definitions(rexgpu-gta4-native PRIVATE
        GTA4_NATIVE_FONT_ASSET_ROOT="${CMAKE_SOURCE_DIR}/LibertyRecompLib/font_atlases"
    )
    target_link_libraries(rexgpu-gta4-native PRIVATE
        libzstd_static
        SPIRV-Headers::SPIRV-Headers
    )
endif()
