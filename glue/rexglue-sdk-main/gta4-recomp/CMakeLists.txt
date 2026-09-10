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
    find_package(CURL REQUIRED)
    find_package(OpenSSL 3.0 REQUIRED COMPONENTS Crypto)
    # The Apple SDK is already supplied through CMAKE_OSX_SYSROOT. Re-adding
    # its usr/include directory through CURL::libcurl makes Homebrew libc++
    # combine headers outside and inside the sysroot, which breaks fundamental
    # Darwin typedefs with current Xcode SDKs.
    if(CURL_INCLUDE_DIR MATCHES "/MacOSX[^/]*/usr/include$")
        set_property(TARGET CURL::libcurl PROPERTY INTERFACE_INCLUDE_DIRECTORIES "")
    endif()

    set(LIBERTY_GC_MACOS_ENTITLEMENTS
        "${CMAKE_CURRENT_SOURCE_DIR}/res/macos/LibertyRecomp.entitlements")
    set(LIBERTY_BUTTON_PROMPT_SOURCE
        "${LIBERTY_RECOMP_WORKSPACE_ROOT}/LibertyRecompLib/private/button_prompts")
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
        "${LIBERTY_RECOMP_WORKSPACE_ROOT}/LibertyRecompLib/font_atlases")
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
        "${LIBERTY_RECOMP_WORKSPACE_ROOT}/LibertyRecompLib/aes_key.bin")
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

    set(LIBERTY_DESKTOP_ICON
        "${LIBERTY_RECOMP_WORKSPACE_ROOT}/LibertyRecomp/res/macos/game_icon.icns")
    set_source_files_properties("${LIBERTY_DESKTOP_ICON}" PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(LibertyRecomp MACOSX_BUNDLE
        "${LIBERTY_DESKTOP_ICON}"
        src/main.cpp
        src/gta4_app.cpp
        src/gta4_frontend_hooks.cpp
        src/gta4_font_selection_trace.cpp
        src/gta4_presentation_options.cpp
        src/gta4_presentation_hooks.cpp
        src/gta4_aspect_hooks.cpp
        src/gta4_quicksave_hooks.cpp
        src/gta4_input_hooks.cpp
        src/gta4_loading_parallax_fix.cpp
        src/gta4_motion_bridge.cpp
        src/gta4_motion_hooks.cpp
        src/gta4_motion_vehicle_hooks.cpp
        src/gta4_audio_lifetime_trace.cpp
        src/gta4_lzx_hooks.cpp
        src/gta4_multiplayer_64_hooks.cpp
        src/gta4_online_hooks.cpp
        src/gta4_native_hooks.cpp
        src/gta4_population_hooks.cpp
        src/gta4_streaming_diagnostics.cpp
        src/gta4_touch_coordinator.cpp
        src/gta4_transition_hooks.cpp
        src/install/gta4_install_dialog.cpp
        src/install/gta4_installer.cpp
        src/install/gta4_rpf_extractor.cpp
        src/install/gta4_source_inspector.cpp
        src/legal_screen_trace.cpp
        src/rpf_button_prompts.cpp
        src/achievement_bridge_gc.mm
        src/input/text_chat_dialog.cpp
        src/input/context_touch_controls.cpp
        src/input/context_touch_layout.cpp
        src/input/context_touch_overlay.cpp
        src/input/user_music_player.mm
        src/network/community_multiplayer.cpp
        src/network/gta4_microphone_permission.mm
        src/network/gta4_voice_audio.cpp
        "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/crypto/rijndael-alg-fst.c"
        "${LIBERTY_GC_MACOS_ENTITLEMENTS}"
        ${LIBERTY_BUTTON_PROMPT_RESOURCES}
        ${LIBERTY_VECTOR_FONT_RESOURCES}
        ${LIBERTY_RPF_AES_KEY_RESOURCES}
        ${GENERATED_SOURCES}
    )

    set_source_files_properties(
        src/achievement_bridge_gc.mm
        src/network/gta4_microphone_permission.mm
        src/input/user_music_player.mm
        PROPERTIES
        COMPILE_OPTIONS "-fobjc-arc"
    )

    target_include_directories(LibertyRecomp PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
        ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/crypto
        ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/inja/third_party/include
    )

    target_compile_definitions(LibertyRecomp PRIVATE
        GTA4_RECOMP_ASSET_XEX="${CMAKE_CURRENT_SOURCE_DIR}/assets/default_v8.xex"
        GTA4_BUTTON_PROMPT_ASSET_ROOT="${LIBERTY_BUTTON_PROMPT_SOURCE}"
        GTA4_RPF_AES_KEY_SOURCE="${LIBERTY_RPF_AES_KEY_SOURCE}"
    )

    target_link_libraries(LibertyRecomp PRIVATE
        rex::runtime
        xxHash::xxhash
        CURL::libcurl
        OpenSSL::Crypto
        SDL3::SDL3
        utf8cpp
        z
        "-framework AVFoundation"
        "-framework GameKit"
    )

    rexglue_configure_target(LibertyRecomp GPU_PLUGINS xenos gta4-native)

    # rexglue_configure_target stages the Vulkan loader and its companion
    # runtime from the selected SDK.  MoltenVK itself is built from the
    # workspace, however, and may contain title-required fixes that are not in
    # the installed SDK copy.  Make the workspace build authoritative in the
    # same runtime directory after the generic staging command has run.
    if(TARGET MoltenVK)
        add_dependencies(LibertyRecomp MoltenVK)
        # The target dependency establishes build order, but by itself it does
        # not rerun this target's POST_BUILD events when only MoltenVK changes.
        # Make the driver a file-level link dependency so the app is relinked
        # and the authoritative workspace driver is restaged after the generic
        # SDK runtime staging step.
        set_property(TARGET LibertyRecomp APPEND PROPERTY LINK_DEPENDS
            "$<TARGET_FILE:MoltenVK>"
        )
        add_custom_command(TARGET LibertyRecomp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_BUNDLE_CONTENT_DIR:LibertyRecomp>/Resources/vulkan/lib"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:MoltenVK>"
                "$<TARGET_BUNDLE_CONTENT_DIR:LibertyRecomp>/Resources/vulkan/lib/libMoltenVK.dylib"
            COMMENT "Staging the workspace MoltenVK runtime..."
            VERBATIM
        )
    endif()

    set_target_properties(LibertyRecomp PROPERTIES
        OUTPUT_NAME "Liberty Recompiled"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LibertyRecomp"
        MACOSX_BUNDLE_INFO_PLIST
            "${CMAKE_CURRENT_SOURCE_DIR}/res/macos/MacOSXBundleInfo.plist.in"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${LIBERTY_MACOS_BUNDLE_IDENTIFIER}"
        MACOSX_BUNDLE_BUNDLE_NAME "Liberty Recompiled"
        MACOSX_BUNDLE_ICON_FILE "game_icon.icns"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${LIBERTY_MACOS_BUNDLE_IDENTIFIER}"
    )

    # An icon-only update must rerun the bundle signing step too.
    set_property(TARGET LibertyRecomp APPEND PROPERTY LINK_DEPENDS "${LIBERTY_DESKTOP_ICON}")
    # Atlas-only updates also change the bundle seal. Relink so the existing
    # post-build signing step runs after CMake copies the new font resources.
    set_property(TARGET LibertyRecomp APPEND PROPERTY LINK_DEPENDS
        ${LIBERTY_VECTOR_FONT_RESOURCES})

    if(LIBERTY_MACOS_CODE_SIGN_IDENTITY)
        set_target_properties(LibertyRecomp PROPERTIES
            XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${LIBERTY_GC_MACOS_ENTITLEMENTS}"
        )
        add_custom_command(TARGET LibertyRecomp POST_BUILD
            COMMAND /usr/bin/codesign
                --force
                --sign "${LIBERTY_MACOS_CODE_SIGN_IDENTITY}"
                --entitlements "${LIBERTY_GC_MACOS_ENTITLEMENTS}"
                "$<TARGET_BUNDLE_DIR:LibertyRecomp>"
            COMMENT "Signing macOS bundle with Game Center and audio-input entitlements..."
        )
    else()
        # Game Center is a restricted entitlement. Attaching it to an ad-hoc
        # signature causes AMFI to kill the process before main. Unsandboxed
        # microphone access is authorized by TCC using the bundle usage
        # description, so development bundles must be signed without the
        # restricted entitlement set.
        add_custom_command(TARGET LibertyRecomp POST_BUILD
            COMMAND /usr/bin/codesign
                --force
                --sign "-"
                "$<TARGET_BUNDLE_DIR:LibertyRecomp>"
            COMMENT "Ad-hoc signing macOS development bundle..."
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
