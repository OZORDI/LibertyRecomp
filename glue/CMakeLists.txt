# =============================================================================
# Phase 3a — minimal glue shim.
#
# The official ReXGlue SDK (graine) now handles all SDK work itself:
# version.h generation, thirdparty (fmt/spdlog/utf8cpp/SDL3/imgui/o1heap/etc),
# target creation, and exports rex::core / rex::system / rex::kernel / rex::audio
# / rex::input / rex::graphics / rex::ui plus legacy rex* aliases.
#
# This file only does three things now:
#   1. Point REXSDK_DIR at graine and add_subdirectory it.
#   2. add_subdirectory the gta4-recomp consumer project (sibling dir).
#   3. LIBERTY_RECOMP_OVERRIDES compile def on rex::kernel
#
# NOTE: LR intentionally does NOT link rex::ui (we provide our own UI).
# ImGuiDialog linker stubs live in LibertyRecomp/kernel/compat_stubs.cpp.
# =============================================================================

# --- Graphics backend selection (consumed by graine) -------------------------
set(REXGLUE_USE_VULKAN     ON  CACHE BOOL "" FORCE)
set(REXGLUE_USE_D3D12      OFF CACHE BOOL "" FORCE)
set(REXGLUE_ENABLE_FIDELITYFX OFF CACHE BOOL "" FORCE)
set(REXGLUE_BUILD_TESTS    OFF CACHE BOOL "" FORCE)

# --- Path to the graine SDK source tree --------------------------------------
set(REXSDK_DIR "${CMAKE_SOURCE_DIR}/glue/rexglue-sdk-main"
    CACHE PATH "Path to rexglue-sdk source tree (graine)")

if(NOT EXISTS "${REXSDK_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "Graine SDK not found at ${REXSDK_DIR}. "
        "Set REXSDK_DIR to the rexglue-sdk source tree.")
endif()

# --- Bring in graine ----------------------------------------------------------
# Graine registers rexcore/rexsystem/rexkernel/rexaudio/rexinput/rexgraphics/rexui
# plus rex::* aliases and every thirdparty target LibertyRecomp needs
# (fmt, spdlog, utf8cpp, SDL3, imgui, o1heap, simde, tomlplusplus, mspack, aes128, ...).
add_subdirectory("${REXSDK_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/rexglue-sdk")

# --- Bring in the gta4-recomp consumer project -------------------------------
# Sibling to this file. Provides GENERATED_SOURCES from generated/sources.cmake
# and is configured via the SDK-exported rexglue_setup_target helper.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gta4-recomp/CMakeLists.txt")
    add_subdirectory(gta4-recomp)
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gta4-recomp/generated/sources.cmake")
    # Consumer project doesn't have its own CMakeLists yet — expose the
    # generated sources list to the parent scope so LibertyRecomp can absorb them.
    include("${CMAKE_CURRENT_SOURCE_DIR}/gta4-recomp/generated/sources.cmake")
    set(GTA4_RECOMP_GENERATED_SOURCES "${GENERATED_SOURCES}" PARENT_SCOPE)
    set(GTA4_RECOMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/gta4-recomp" PARENT_SCOPE)
    message(STATUS "glue: gta4-recomp consumer has no CMakeLists.txt yet — "
                   "exporting GTA4_RECOMP_GENERATED_SOURCES only")
endif()

# --- LIBERTY_RECOMP_OVERRIDES --------------------------------------------------
# rexkernel's Vd*/Xam* PPC_FUNC hooks are conditionally compiled out when this
# is defined, so LibertyRecomp can own them directly without duplicate symbols.
if(TARGET rexkernel)
    target_compile_definitions(rexkernel PRIVATE LIBERTY_RECOMP_OVERRIDES)
endif()
