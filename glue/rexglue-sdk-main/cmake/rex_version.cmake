#==========================================================
# rex_compute_version(<out_var>
#     FLOOR_MAJOR <int>
#     FLOOR_MINOR <int>
#     GIT_DESCRIBE_LONG  <string>   # output of git describe --tags --long ...
#     GIT_DESCRIBE_EXACT <string>   # output of git describe --tags --exact-match
#     BRANCH_NAME        <string>)  # output of git symbolic-ref --short HEAD
#
# Emits a CMake-style version string: MAJOR.MINOR[.PATCH[.TWEAK]][-id].
# See https://cmake.org/cmake/help/latest/variable/CMAKE_VERSION.html
#
# Tagged commit (vX.Y[.Z[.W]]): emits the tag verbatim (without the v).
# Untagged commit: emits MAJOR.<floor-minor>.<derived-patch>.<commit-count>-<id>
#   where id is "dev.gSHA" on any branch and "rc.gSHA" on a release/* branch.
#==========================================================
function(rex_compute_version out_var)
    set(one_value FLOOR_MAJOR FLOOR_MINOR GIT_DESCRIBE_LONG GIT_DESCRIBE_EXACT BRANCH_NAME)
    cmake_parse_arguments(ARG "" "${one_value}" "" ${ARGN})

    if(NOT "${ARG_GIT_DESCRIBE_EXACT}" STREQUAL "")
        if(ARG_GIT_DESCRIBE_EXACT MATCHES "^v([0-9]+\\.[0-9]+(\\.[0-9]+)?(\\.[0-9]+)?)$")
            set(${out_var} "${CMAKE_MATCH_1}" PARENT_SCOPE)
            return()
        endif()
        message(FATAL_ERROR "rex_compute_version: unparseable exact tag '${ARG_GIT_DESCRIBE_EXACT}'")
    endif()

    if("${ARG_GIT_DESCRIBE_LONG}" STREQUAL "")
        message(WARNING
            "rex_compute_version: no v* tag reachable from HEAD. "
            "Falling back to ${ARG_FLOOR_MAJOR}.${ARG_FLOOR_MINOR}.0.0-dev.unknown. "
            "If this is a CI build, ensure tags are fetched (fetch-depth: 0 + fetch-tags: true). "
            "If this is a fork, push tags with: git push <fork-remote> --tags")
        set(${out_var} "${ARG_FLOOR_MAJOR}.${ARG_FLOOR_MINOR}.0.0-dev.unknown" PARENT_SCOPE)
        return()
    endif()

    if(NOT ARG_GIT_DESCRIBE_LONG MATCHES "^v([0-9]+)\\.([0-9]+)(\\.([0-9]+))?(\\.([0-9]+))?-([0-9]+)-g([0-9a-f]+)$")
        message(FATAL_ERROR "rex_compute_version: unparseable describe output '${ARG_GIT_DESCRIBE_LONG}'")
    endif()
    set(tag_minor ${CMAKE_MATCH_2})
    if(CMAKE_MATCH_4)
        set(tag_patch ${CMAKE_MATCH_4})
    else()
        set(tag_patch 0)
    endif()
    set(commit_count ${CMAKE_MATCH_7})
    set(short_sha ${CMAKE_MATCH_8})

    # Floor minor must never go backwards.
    if(ARG_FLOOR_MINOR LESS tag_minor)
        message(FATAL_ERROR
            "rex_compute_version: floor minor (${ARG_FLOOR_MINOR}) is behind tag minor (${tag_minor}). "
            "The floor in CMakeLists.txt must not regress.")
    endif()

    if(ARG_FLOOR_MINOR EQUAL tag_minor)
        math(EXPR derived_patch "${tag_patch} + 1")
    else()
        set(derived_patch 0)
    endif()

    if(ARG_BRANCH_NAME MATCHES "^release/")
        set(id "rc.g${short_sha}")
    else()
        set(id "dev.g${short_sha}")
    endif()

    set(${out_var} "${ARG_FLOOR_MAJOR}.${ARG_FLOOR_MINOR}.${derived_patch}.${commit_count}-${id}" PARENT_SCOPE)
endfunction()

#==========================================================
# rex_resolve_version(<out_var>
#     FLOOR_MAJOR <int>
#     FLOOR_MINOR <int>
#     [SOURCE_DIR <path>])    # defaults to CMAKE_SOURCE_DIR
#
# Runs git in the working tree, then delegates to rex_compute_version.
#==========================================================
function(rex_resolve_version out_var)
    set(one_value FLOOR_MAJOR FLOOR_MINOR SOURCE_DIR)
    cmake_parse_arguments(ARG "" "${one_value}" "" ${ARGN})

    if(NOT ARG_SOURCE_DIR)
        set(ARG_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
    endif()

    find_program(GIT_EXECUTABLE git)
    if(NOT GIT_EXECUTABLE)
        rex_compute_version(result
            FLOOR_MAJOR ${ARG_FLOOR_MAJOR}
            FLOOR_MINOR ${ARG_FLOOR_MINOR}
            GIT_DESCRIBE_LONG ""
            GIT_DESCRIBE_EXACT ""
            BRANCH_NAME "")
        set(${out_var} "${result}" PARENT_SCOPE)
        return()
    endif()

    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --exact-match
            --match "v[0-9]*.[0-9]*.[0-9]*"
            --exclude "*-*"
        WORKING_DIRECTORY "${ARG_SOURCE_DIR}"
        OUTPUT_VARIABLE describe_exact
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE describe_exact_rc)
    if(NOT describe_exact_rc EQUAL 0)
        set(describe_exact "")
    endif()

    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --long
            --match "v[0-9]*.[0-9]*"
            --exclude "*-*"
        WORKING_DIRECTORY "${ARG_SOURCE_DIR}"
        OUTPUT_VARIABLE describe_long
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE describe_long_rc)
    if(NOT describe_long_rc EQUAL 0)
        set(describe_long "")
    endif()

    execute_process(
        COMMAND ${GIT_EXECUTABLE} symbolic-ref --short HEAD
        WORKING_DIRECTORY "${ARG_SOURCE_DIR}"
        OUTPUT_VARIABLE branch_name
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE branch_rc)
    if(NOT branch_rc EQUAL 0)
        set(branch_name "")
    endif()

    rex_compute_version(result
        FLOOR_MAJOR ${ARG_FLOOR_MAJOR}
        FLOOR_MINOR ${ARG_FLOOR_MINOR}
        GIT_DESCRIBE_LONG "${describe_long}"
        GIT_DESCRIBE_EXACT "${describe_exact}"
        BRANCH_NAME "${branch_name}")
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()
