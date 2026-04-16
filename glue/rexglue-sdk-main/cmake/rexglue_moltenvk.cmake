#==========================================================
# MoltenVK discovery helpers
#==========================================================

set(REXGLUE_MOLTENVK_LIBRARY "${REXGLUE_MOLTENVK_LIBRARY}" CACHE FILEPATH
    "Path to the MoltenVK runtime library on macOS")

function(rexglue_find_moltenvk_library out_var)
    set(options REQUIRED)
    cmake_parse_arguments(REXGLUE_MOLTENVK "${options}" "" "" ${ARGN})

    if(NOT APPLE)
        set(${out_var} "" PARENT_SCOPE)
        return()
    endif()

    if(REXGLUE_MOLTENVK_LIBRARY)
        if(EXISTS "${REXGLUE_MOLTENVK_LIBRARY}")
            set(${out_var} "${REXGLUE_MOLTENVK_LIBRARY}" PARENT_SCOPE)
            return()
        endif()
        message(FATAL_ERROR
            "REXGLUE_MOLTENVK_LIBRARY is set to '${REXGLUE_MOLTENVK_LIBRARY}', "
            "but that file does not exist.")
    endif()

    find_package(Vulkan QUIET)

    set(_search_roots)
    if(Vulkan_LIBRARY)
        get_filename_component(_vulkan_lib_dir "${Vulkan_LIBRARY}" DIRECTORY)
        list(APPEND _search_roots "${_vulkan_lib_dir}")
        get_filename_component(_vulkan_root "${_vulkan_lib_dir}" DIRECTORY)
        list(APPEND _search_roots "${_vulkan_root}")
    endif()
    if(DEFINED ENV{VULKAN_SDK} AND NOT "$ENV{VULKAN_SDK}" STREQUAL "")
        list(APPEND _search_roots "$ENV{VULKAN_SDK}")
    endif()
    list(REMOVE_DUPLICATES _search_roots)

    set(_candidates)
    foreach(_root IN LISTS _search_roots)
        if(EXISTS "${_root}")
            file(GLOB_RECURSE _root_candidates LIST_DIRECTORIES FALSE
                "${_root}/libMoltenVK*.dylib")
            list(APPEND _candidates ${_root_candidates})
        endif()
    endforeach()
    list(REMOVE_DUPLICATES _candidates)

    set(_preferred "")
    set(_fallback "")
    foreach(_candidate IN LISTS _candidates)
        get_filename_component(_candidate_name "${_candidate}" NAME)
        if(_candidate_name STREQUAL "libMoltenVK.dylib")
            set(_preferred "${_candidate}")
            break()
        endif()
        if(NOT _fallback AND _candidate_name MATCHES "^libMoltenVK(\\.[0-9]+)?\\.dylib$")
            set(_fallback "${_candidate}")
        endif()
    endforeach()

    if(_preferred)
        set(_selected "${_preferred}")
    else()
        set(_selected "${_fallback}")
    endif()

    if(_selected)
        set(REXGLUE_MOLTENVK_LIBRARY "${_selected}" CACHE FILEPATH
            "Path to the MoltenVK runtime library on macOS" FORCE)
        set(${out_var} "${_selected}" PARENT_SCOPE)
        return()
    endif()

    if(REXGLUE_MOLTENVK_REQUIRED)
        message(FATAL_ERROR
            "MoltenVK was not found in the Vulkan SDK.\n"
            "Set REXGLUE_MOLTENVK_LIBRARY to libMoltenVK.dylib or configure VULKAN_SDK "
            "so ReXGlue can copy the runtime on macOS.")
    endif()

    set(${out_var} "" PARENT_SCOPE)
endfunction()
