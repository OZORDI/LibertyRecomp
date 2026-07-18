cmake_minimum_required(VERSION 3.25)
project(gta4_recomp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generated/sources.cmake")
    message(FATAL_ERROR
        "gta4-recomp generated sources are missing. Build rexglue and run "
        "'rexglue codegen gta4_manifest.toml'.")
endif()

include(generated/sources.cmake)

if(APPLE AND LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "desktop")
    add_executable(LibertyRecomp MACOSX_BUNDLE
        src/main.cpp
        src/dlc_layout.cpp
        ${GENERATED_SOURCES}
    )

    target_include_directories(LibertyRecomp PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
    )

    target_compile_definitions(LibertyRecomp PRIVATE
        GTA4_RECOMP_ASSET_XEX="${CMAKE_CURRENT_SOURCE_DIR}/assets/default_v8.xex"
    )

    target_link_libraries(LibertyRecomp PRIVATE
        rex::core
        rex::system
        rex::kernel
        rex::ui
    )

    rexglue_configure_target(LibertyRecomp GPU_PLUGINS xenos)

    set_target_properties(LibertyRecomp PROPERTIES
        OUTPUT_NAME "Liberty Recompiled"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LibertyRecomp"
    )
else()
    add_library(gta4_recomp INTERFACE)
    target_include_directories(gta4_recomp INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
    )
    set(GTA4_RECOMP_GENERATED_SOURCES ${GENERATED_SOURCES} PARENT_SCOPE)
endif()

if(TARGET rex::rexglue)
    add_custom_target(gta4_codegen
        COMMAND $<TARGET_FILE:rex::rexglue> codegen
                ${CMAKE_CURRENT_SOURCE_DIR}/gta4_manifest.toml
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating recompiled code for GTA IV"
        VERBATIM
    )
endif()
