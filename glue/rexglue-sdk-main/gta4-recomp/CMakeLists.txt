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
    if(NOT TARGET GameNetworkingSockets::static)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
        set(ENABLE_ICE ON CACHE BOOL "" FORCE)
        set(USE_STEAMWEBRTC OFF CACHE BOOL "" FORCE)
        add_subdirectory(
            "${CMAKE_SOURCE_DIR}/thirdparty/GameNetworkingSockets"
            "${CMAKE_BINARY_DIR}/thirdparty/GameNetworkingSockets")
    endif()

    set(LIBERTY_GC_MACOS_ENTITLEMENTS
        "${CMAKE_SOURCE_DIR}/LibertyRecomp/res/macos/LibertyRecomp.entitlements")
    set(LIBERTY_BUTTON_PROMPT_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../LibertyRecompLib/private/button_prompts")
    set(LIBERTY_BUTTON_PROMPT_RESOURCES)
    foreach(LIBERTY_BUTTON_PROMPT_VARIANT
            xbox360 xbox_one xbox_series_x ps3 ps4 ps5 switch steam_controller steam_deck)
        set(LIBERTY_BUTTON_PROMPT_RESOURCE
            "${LIBERTY_BUTTON_PROMPT_SOURCE}/${LIBERTY_BUTTON_PROMPT_VARIANT}/buttons_360.xtd")
        list(APPEND LIBERTY_BUTTON_PROMPT_RESOURCES "${LIBERTY_BUTTON_PROMPT_RESOURCE}")
        set_source_files_properties("${LIBERTY_BUTTON_PROMPT_RESOURCE}" PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources/button_prompts/${LIBERTY_BUTTON_PROMPT_VARIANT}")
    endforeach()

    set(LIBERTY_VECTOR_FONT_RESOURCES)
    set(LIBERTY_VECTOR_FONT_SOURCE
        "${CMAKE_SOURCE_DIR}/LibertyRecompLib/font_atlases")
    foreach(LIBERTY_VECTOR_FONT_NAME font1 font2 font3)
        set(LIBERTY_VECTOR_FONT_RESOURCE
            "${LIBERTY_VECTOR_FONT_SOURCE}/${LIBERTY_VECTOR_FONT_NAME}.png")
        list(APPEND LIBERTY_VECTOR_FONT_RESOURCES "${LIBERTY_VECTOR_FONT_RESOURCE}")
        set_source_files_properties("${LIBERTY_VECTOR_FONT_RESOURCE}" PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources/font_atlases")
    endforeach()
    set(LIBERTY_VECTOR_FONT_MANIFEST
        "${LIBERTY_VECTOR_FONT_SOURCE}/manifest.json")
    list(APPEND LIBERTY_VECTOR_FONT_RESOURCES "${LIBERTY_VECTOR_FONT_MANIFEST}")
    set_source_files_properties("${LIBERTY_VECTOR_FONT_MANIFEST}" PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources/font_atlases")
    foreach(LIBERTY_VECTOR_FONT_EPISODE tlad tbogt)
        foreach(LIBERTY_VECTOR_FONT_NAME font1 font2 font3)
            set(LIBERTY_VECTOR_FONT_RESOURCE
                "${LIBERTY_VECTOR_FONT_SOURCE}/${LIBERTY_VECTOR_FONT_EPISODE}/${LIBERTY_VECTOR_FONT_NAME}.png")
            list(APPEND LIBERTY_VECTOR_FONT_RESOURCES "${LIBERTY_VECTOR_FONT_RESOURCE}")
            set_source_files_properties("${LIBERTY_VECTOR_FONT_RESOURCE}" PROPERTIES
                MACOSX_PACKAGE_LOCATION
                    "Resources/font_atlases/${LIBERTY_VECTOR_FONT_EPISODE}")
        endforeach()
        set(LIBERTY_VECTOR_FONT_EPISODE_MANIFEST
            "${LIBERTY_VECTOR_FONT_SOURCE}/${LIBERTY_VECTOR_FONT_EPISODE}/manifest.json")
        list(APPEND LIBERTY_VECTOR_FONT_RESOURCES
            "${LIBERTY_VECTOR_FONT_EPISODE_MANIFEST}")
        set_source_files_properties("${LIBERTY_VECTOR_FONT_EPISODE_MANIFEST}" PROPERTIES
            MACOSX_PACKAGE_LOCATION
                "Resources/font_atlases/${LIBERTY_VECTOR_FONT_EPISODE}")
    endforeach()

    set(LIBERTY_RPF_AES_KEY_SOURCE
        "${CMAKE_SOURCE_DIR}/LibertyRecompLib/aes_key.bin")
    set(LIBERTY_RPF_AES_KEY_RESOURCES)
    if(EXISTS "${LIBERTY_RPF_AES_KEY_SOURCE}")
        list(APPEND LIBERTY_RPF_AES_KEY_RESOURCES
            "${LIBERTY_RPF_AES_KEY_SOURCE}")
        set_source_files_properties("${LIBERTY_RPF_AES_KEY_SOURCE}" PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources")
    else()
        message(STATUS
            "LibertyRecompLib/aes_key.bin is not present; the app will build "
            "without the optional game-install key resource")
    endif()

    add_executable(LibertyRecomp MACOSX_BUNDLE
        src/main.cpp
        src/gta4_app.cpp
        src/gta4_frontend_hooks.cpp
        src/gta4_loading_parallax_fix.cpp
        src/gta4_motion_bridge.cpp
        src/gta4_motion_hooks.cpp
        src/gta4_motion_vehicle_hooks.cpp
        src/gta4_native_hooks.cpp
        src/gta4_physics_trace.cpp
        src/gta4_transition_hooks.cpp
        src/install/gta4_install_dialog.cpp
        src/install/gta4_installer.cpp
        src/install/gta4_rpf_extractor.cpp
        src/legal_screen_trace.cpp
        src/rpf_button_prompts.cpp
        src/achievement_bridge_gc.mm
        "${CMAKE_SOURCE_DIR}/LibertyRecomp/network/community_multiplayer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/crypto/rijndael-alg-fst.c"
        "${CMAKE_SOURCE_DIR}/LibertyRecomp/patches/gta4_input_patches.cpp"
        "${LIBERTY_GC_MACOS_ENTITLEMENTS}"
        ${LIBERTY_BUTTON_PROMPT_RESOURCES}
        ${LIBERTY_VECTOR_FONT_RESOURCES}
        ${LIBERTY_RPF_AES_KEY_RESOURCES}
        ${GENERATED_SOURCES}
    )

    set_source_files_properties(src/achievement_bridge_gc.mm PROPERTIES
        COMPILE_OPTIONS "-fobjc-arc"
    )

    target_include_directories(LibertyRecomp PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
        ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/crypto
        ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui
        "${CMAKE_SOURCE_DIR}/LibertyRecomp"
        "${CMAKE_SOURCE_DIR}/thirdparty/json/include"
    )

    target_compile_definitions(LibertyRecomp PRIVATE
        GTA4_RECOMP_ASSET_XEX="${CMAKE_CURRENT_SOURCE_DIR}/assets/default_v8.xex"
        GTA4_BUTTON_PROMPT_ASSET_ROOT="${LIBERTY_BUTTON_PROMPT_SOURCE}"
        GTA4_RPF_AES_KEY_SOURCE="${LIBERTY_RPF_AES_KEY_SOURCE}"
    )

    target_link_libraries(LibertyRecomp PRIVATE
        rex::runtime
        xxHash::xxhash
        curl
        GameNetworkingSockets::static
        z
        "-framework GameKit"
    )

    rexglue_configure_target(LibertyRecomp GPU_PLUGINS xenos gta4-native)

    set_target_properties(LibertyRecomp PROPERTIES
        OUTPUT_NAME "Liberty Recompiled"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LibertyRecomp"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${LIBERTY_MACOS_BUNDLE_IDENTIFIER}"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${LIBERTY_MACOS_BUNDLE_IDENTIFIER}"
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${LIBERTY_GC_MACOS_ENTITLEMENTS}"
    )

    if(LIBERTY_MACOS_CODE_SIGN_IDENTITY)
        add_custom_command(TARGET LibertyRecomp POST_BUILD
            COMMAND /usr/bin/codesign
                --force
                --sign "${LIBERTY_MACOS_CODE_SIGN_IDENTITY}"
                --entitlements "${LIBERTY_GC_MACOS_ENTITLEMENTS}"
                "$<TARGET_BUNDLE_DIR:LibertyRecomp>"
            COMMENT "Signing macOS bundle with Game Center entitlement..."
        )
    endif()
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
