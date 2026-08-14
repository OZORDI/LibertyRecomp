set(REXGLUE_USE_VULKAN ON CACHE BOOL "" FORCE)
set(REXGLUE_USE_D3D12 OFF CACHE BOOL "" FORCE)
set(REXGLUE_ENABLE_FIDELITYFX OFF CACHE BOOL "" FORCE)
set(REXGLUE_BUILD_TESTS OFF CACHE BOOL "")

set(REXSDK_DIR "${CMAKE_SOURCE_DIR}/glue/rexglue-sdk-main"
    CACHE PATH "Path to the Graine RexGlue SDK source tree")

if(NOT EXISTS "${REXSDK_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "Graine RexGlue SDK not found at ${REXSDK_DIR}")
endif()

add_subdirectory("${REXSDK_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/rexglue-sdk")
add_subdirectory("${REXSDK_DIR}/gta4-recomp" "${CMAKE_CURRENT_BINARY_DIR}/gta4-recomp")
