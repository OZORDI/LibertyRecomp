project("LibertyRecompLib")

add_compile_options(
    -fno-strict-aliasing
)

if (WIN32)
    add_compile_options(/fp:strict)
else()
    add_compile_options(-ffp-model=strict)
endif()

target_compile_definitions(XenonRecomp PRIVATE 
    XENON_RECOMP_CONFIG_FILE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/config/GTA4.toml\"
    XENON_RECOMP_HEADER_FILE_PATH=\"${LIBERTY_RECOMP_TOOLS_ROOT}/XenonRecomp/XenonUtils/ppc_context.h\")

# =============================================================================
# PPC Recompiled Sources: use RexGlue's gta4-recomp generated code
# =============================================================================
# RexGlue's codegen output is authoritative. Legacy LibertyRecompLib/ppc/
# files are preserved but no longer built.
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

target_compile_definitions(XenosRecomp PRIVATE 
    XENOS_RECOMP_INPUT=\"${CMAKE_CURRENT_SOURCE_DIR}/private/shader\" 
    XENOS_RECOMP_OUTPUT=\"${CMAKE_CURRENT_SOURCE_DIR}/shader/shader_cache.cpp\"
    XENOS_RECOMP_INCLUDE_INPUT=\"${XENOS_RECOMP_INCLUDE}\"
    LIBERTY_RECOMP
)

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
