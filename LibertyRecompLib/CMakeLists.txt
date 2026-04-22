project("LibertyRecompLib")

add_compile_options(
    -fno-strict-aliasing
)

if (WIN32)
    add_compile_options(/fp:strict)
else()
    add_compile_options(-ffp-model=strict)
endif()

# macOS needs _XOPEN_SOURCE for ucontext (fibers) used by rex/thread/fiber.h
if(APPLE)
    add_compile_definitions(_XOPEN_SOURCE=600 _DARWIN_C_SOURCE)
endif()

if(TARGET XenonRecomp)
    target_compile_definitions(XenonRecomp PRIVATE
        XENON_RECOMP_CONFIG_FILE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/config/GTA4.toml\"
        XENON_RECOMP_HEADER_FILE_PATH=\"${LIBERTY_RECOMP_TOOLS_ROOT}/XenonRecomp/XenonUtils/ppc_context.h\")
endif()

# =============================================================================
# PPC Recompiled Sources: use RexGlue's gta4-recomp generated code
# =============================================================================
# RexGlue's codegen output is authoritative. Legacy LibertyRecompLib/ppc/
# files are preserved but no longer built.
# Lives inside the SDK tree (single canonical copy — the former out-of-tree
# consumer copy at glue/gta4-recomp was removed 2026-04-18).
set(REXGLUE_GTA4_RECOMP "${CMAKE_CURRENT_SOURCE_DIR}/../glue/rexglue-sdk-main/gta4-recomp")

include("${REXGLUE_GTA4_RECOMP}/generated/sources.cmake")
set(LIBERTY_RECOMP_PPC_RECOMPILED_SOURCES ${GENERATED_SOURCES})

# NOTE: GTA IV shader handling is different from Sonic '06
# The shader.arc/shader_lt.arc extraction is disabled for GTA IV
# A stub shader_cache.cpp is used until GTA IV shader extraction is implemented
#
# Original Sonic '06 shader extraction (disabled):
# add_custom_command(
#     OUTPUT
#         "${CMAKE_CURRENT_SOURCE_DIR}/private/shader"
#     COMMAND
#         $<TARGET_FILE:u8extract> "${CMAKE_CURRENT_SOURCE_DIR}/private/shader.arc" "${CMAKE_CURRENT_SOURCE_DIR}/private/shader"
#     COMMAND
#         $<TARGET_FILE:u8extract> "${CMAKE_CURRENT_SOURCE_DIR}/private/shader_lt.arc" "${CMAKE_CURRENT_SOURCE_DIR}/private/shader"
#     DEPENDS
#         "${CMAKE_CURRENT_SOURCE_DIR}/private/shader.arc"
#         "${CMAKE_CURRENT_SOURCE_DIR}/private/shader_lt.arc"
# )

set(XENOS_RECOMP_ROOT "${LIBERTY_RECOMP_TOOLS_ROOT}/XenosRecomp/XenosRecomp")
set(XENOS_RECOMP_INCLUDE "${XENOS_RECOMP_ROOT}/shader_common.h")

# XenosRecomp is a host-side shader-translator tool — skipped on console
# cross-compiles (see top-level CMakeLists.txt:143). Only tag the define when
# the target actually exists.
if(TARGET XenosRecomp)
    target_compile_definitions(XenosRecomp PRIVATE LIBERTY_RECOMP)
endif()
# NOTE: XENOS_RECOMP_INPUT / OUTPUT / INCLUDE_INPUT were previously hardcoded
# to LibertyRecompLib/private/shader (a dir that doesn't exist), which made
# the binary ignore argv and crash with SIGSEGV on readAllBytes(nullptr).
# We now pass the three paths on the command line when regenerating:
#   XenosRecomp <fxc_dir> <output.cpp> <shader_common.h>

file(GLOB XENOS_RECOMP_SOURCES 
    "${XENOS_RECOMP_ROOT}/*.h"
    "${XENOS_RECOMP_ROOT}/*.cpp"
)

# NOTE: Shader recompilation disabled for GTA IV
# Using stub shader_cache.cpp until GTA IV shader extraction is implemented
#
# add_custom_command(
#     OUTPUT 
#         "${CMAKE_CURRENT_SOURCE_DIR}/shader/shader_cache.cpp"
#     COMMAND
#         $<TARGET_FILE:XenosRecomp>
#     DEPENDS 
#         "${CMAKE_CURRENT_SOURCE_DIR}/private/shader"
#         ${XENOS_RECOMP_SOURCES}
#         ${XENOS_RECOMP_INCLUDE}
#     USES_TERMINAL
# )

add_library(LibertyRecompLib
    ${LIBERTY_RECOMP_PPC_RECOMPILED_SOURCES}
    "shader/shader_cache.h"
    "shader/shader_cache.cpp"
    "shader/postprocess_cache.h"
    "shader/postprocess_cache.cpp"
)

target_include_directories(LibertyRecompLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(LibertyRecompLib PRIVATE
    "${LIBERTY_RECOMP_TOOLS_ROOT}/XenonRecomp/thirdparty/simde"
    "${LIBERTY_RECOMP_TOOLS_ROOT}/XenonRecomp/XenonUtils"
    # RexGlue headers for generated PPC code
    "${REXGLUE_GTA4_RECOMP}"
    "${REXGLUE_GTA4_RECOMP}/generated"
)

# RexGlue generated code needs access to RexGlue SDK headers
if(TARGET rexcore)
    target_link_libraries(LibertyRecompLib PRIVATE rexcore rexruntime rexkernel)
endif()
