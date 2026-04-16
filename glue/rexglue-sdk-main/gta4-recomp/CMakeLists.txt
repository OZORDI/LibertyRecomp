# gta4-recomp - LibertyRecomp's GTA IV Xbox 360 PPC recompilation consumer project
#
# This file is yours to edit. 'rexglue migrate' will NOT overwrite it.
# SDK boilerplate lives in generated/rexglue.cmake.
#
# Unlike a standalone rexglue consumer (which builds its own executable via
# rexglue_setup_target), LibertyRecomp absorbs the generated PPC recomp sources
# directly into LibertyRecompLib. We therefore expose GTA4_RECOMP_GENERATED_SOURCES
# to the parent scope instead of producing an executable here.

cmake_minimum_required(VERSION 3.25)
project(gta4_recomp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include generated sources manifest if codegen has been run.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generated/sources.cmake")
    include(generated/sources.cmake)

    # Re-export under a less-generic variable name for the parent build.
    set(GTA4_RECOMP_GENERATED_SOURCES ${GENERATED_SOURCES} PARENT_SCOPE)
else()
    message(WARNING
        "gta4-recomp: generated/sources.cmake not found. "
        "Run the gta4_codegen target to generate PPC recomp sources.")
    set(GTA4_RECOMP_GENERATED_SOURCES "" PARENT_SCOPE)
endif()

# Interface target so consumers can pick up headers (gta4_init.h, gta4_config.h)
# without hardcoding the include path.
add_library(gta4_recomp INTERFACE)
target_include_directories(gta4_recomp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/generated
)

# Codegen target - run 'cmake --build . --target gta4_codegen'
# Only wired up when the rexglue CLI target is available in the build tree.
if(TARGET rex::rexglue)
    add_custom_target(gta4_codegen
        COMMAND $<TARGET_FILE:rex::rexglue> codegen ${CMAKE_CURRENT_SOURCE_DIR}/gta4_config.toml
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating recompiled code for gta4-recomp"
        VERBATIM
    )
endif()
