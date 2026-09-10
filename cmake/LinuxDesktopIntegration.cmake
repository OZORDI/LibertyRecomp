include_guard(GLOBAL)

# The SDL app ID, desktop-file basename, and hicolor icon name agree so
# compositors can associate running windows with the installed application.
function(liberty_configure_linux_desktop target resource_root)
    include(GNUInstallDirs)
    set(desktop_file "${resource_root}/linux/io.github.ozordi.libertyrecomp.desktop")
    set(staged_desktop "${CMAKE_CURRENT_BINARY_DIR}/io.github.ozordi.libertyrecomp.desktop")
    configure_file("${desktop_file}" "${staged_desktop}" COPYONLY)

    set(staged_icons)
    foreach(size 16 24 32 48 64 128 256 512 1024)
        set(source "${resource_root}/icons/png/${size}.png")
        set(relative "icons/hicolor/${size}x${size}/apps")
        set(destination "${CMAKE_CURRENT_BINARY_DIR}/share/${relative}/io.github.ozordi.libertyrecomp.png")
        add_custom_command(OUTPUT "${destination}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory
                "${CMAKE_CURRENT_BINARY_DIR}/share/${relative}"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${source}" "${destination}"
            DEPENDS "${source}"
            VERBATIM)
        list(APPEND staged_icons "${destination}")
        install(FILES "${source}"
            DESTINATION "${CMAKE_INSTALL_DATADIR}/${relative}"
            RENAME io.github.ozordi.libertyrecomp.png
            COMPONENT LibertyRecompDesktop)
    endforeach()
    add_custom_target(${target}_desktop_assets DEPENDS ${staged_icons})
    add_dependencies(${target} ${target}_desktop_assets)
    install(FILES "${desktop_file}"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/applications"
        COMPONENT LibertyRecompDesktop)
    install(TARGETS ${target}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT LibertyRecompDesktop)
endfunction()
