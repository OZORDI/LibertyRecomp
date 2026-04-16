# Nintendo Switch / libnx (devkitPro) Toolchain
# Requirements:
#   - devkitPro with devkitA64 and libnx installed
#     Install: https://devkitpro.org/wiki/Getting_Started
#     Then:    dkp-pacman -S switch-dev
#   - Environment: DEVKITPRO and DEVKITA64 set by devkitPro's env.sh
#
# Setup:
#   source $DEVKITPRO/devkita64/environment-setup-aarch64-none-elf
#   cmake -DCMAKE_TOOLCHAIN_FILE=toolchains/switch-libnx.cmake \
#         -DLIBERTY_RECOMP_TARGET_PLATFORM=switch \
#         -DLIBERTY_RECOMP_EMBEDDED_GAME_PATH=tools/local_game_payload \
#         ..
#
# Verify installed packages:
#   dkp-pacman -Q switch-dev

# Tell rexglue SDK we're targeting Switch console
# (lowercase — graine's console branches check for "switch")
set(LIBERTY_RECOMP_TARGET_PLATFORM "switch" CACHE STRING "" FORCE)

# Resolve devkitPro paths
if(NOT DEFINED DEVKITPRO)
    if(DEFINED ENV{DEVKITPRO})
        set(DEVKITPRO "$ENV{DEVKITPRO}")
    else()
        set(DEVKITPRO "/opt/devkitpro")  # common default
    endif()
endif()

if(NOT DEFINED DEVKITA64)
    if(DEFINED ENV{DEVKITA64})
        set(DEVKITA64 "$ENV{DEVKITA64}")
    else()
        set(DEVKITA64 "${DEVKITPRO}/devkitA64")
    endif()
endif()

# Resolve repo-local libnx
get_filename_component(_switch_repo_root "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(LIBNX_ROOT "${_switch_repo_root}/tools/libnx")

if(EXISTS "${DEVKITA64}")
    # ── Mode A: devkitPro toolchain ──────────────────────────────────────────
    message(STATUS "Using devkitA64: ${DEVKITA64}")
    message(STATUS "Using devkitPro: ${DEVKITPRO}")

    set(CMAKE_SYSTEM_NAME Switch)
    set(CMAKE_SYSTEM_PROCESSOR aarch64)

    set(CMAKE_C_COMPILER   "${DEVKITA64}/bin/aarch64-none-elf-gcc")
    set(CMAKE_CXX_COMPILER "${DEVKITA64}/bin/aarch64-none-elf-g++")
    set(CMAKE_AR           "${DEVKITA64}/bin/aarch64-none-elf-ar")
    set(CMAKE_RANLIB       "${DEVKITA64}/bin/aarch64-none-elf-ranlib")
    set(CMAKE_LINKER       "${DEVKITA64}/bin/aarch64-none-elf-ld")

    set(CMAKE_SYSROOT "${DEVKITA64}/aarch64-none-elf")

    include_directories(SYSTEM
        "${DEVKITPRO}/libnx/include"
        "${DEVKITA64}/aarch64-none-elf/include"
    )
    link_directories(
        "${DEVKITPRO}/libnx/lib"
        "${DEVKITA64}/aarch64-none-elf/lib"
    )

    set(_SWITCH_EXTRA_FLAGS "")
    set(_SWITCH_TP_FLAG "-mtp=soft")

    # ── Linker specs for Switch homebrew NRO ─────────────────────────────────
    # devkitPro ships switch.specs at $DEVKITPRO/libnx/switch.specs. It pulls in:
    #   - link.T           (Switch homebrew memory layout / aslr-friendly PIE)
    #   - rcrt0            (_start entry point implemented by libnx)
    #   - -lnx -lm -lc -lsysbase (libnx + newlib + BSD system-call shims)
    # Without these the link fails with undefined `_start`, missing libnx
    # symbols (svcGetInfo, nxlinkStdio, romfsInit, etc.), and the resulting
    # ELF cannot be wrapped into an NRO by elf2nro.
    set(_SWITCH_SPECS "${DEVKITPRO}/libnx/switch.specs")
    if(NOT EXISTS "${_SWITCH_SPECS}")
        message(WARNING
            "Switch (Mode A): ${_SWITCH_SPECS} not found. "
            "Install libnx via `dkp-pacman -S libnx` or set DEVKITPRO correctly. "
            "Link will fail without switch.specs.")
    endif()

    # -specs must be on the linker command line (gcc driver passes it through).
    # -Wl,-Map emits a map file next to the ELF for debugging symbol layout.
    # -fPIE is already in SWITCH_C_FLAGS below; repeat at link so the driver
    # forwards -pie to ld (homebrew NROs are position-independent ELFs).
    set(CMAKE_EXE_LINKER_FLAGS_INIT
        "-specs=${_SWITCH_SPECS} -fPIE -Wl,-Map,LibertyRecomp.map")
else()
    # ── Mode B: Clang cross-compile (no devkitPro) — REMOVED ─────────────────
    # Building Switch homebrew without devkitPro is not viable:
    #
    #   1. tools/libnx/nx/Makefile hard-requires $DEVKITPRO and includes
    #      $(DEVKITPRO)/devkitA64/base_rules — libnx cannot be built against
    #      a bare Homebrew LLVM toolchain (no newlib sysroot, no base_rules).
    #   2. No prebuilt libnx.a ships in this repo; tools/libnx/ is source-only,
    #      so there is nothing to link against without a devkitPro build first.
    #   3. .nro packaging requires elf2nro and nacptool from devkitPro — even
    #      a successful ELF link could not be turned into a runnable homebrew.
    #   4. The previous Mode B injected macOS libc++ headers into an
    #      aarch64-none-elf newlib build, which prior review flagged as
    #      fundamentally broken (ABI/layout mismatch, host-only intrinsics).
    #
    # For a Switch build, install devkitPro:
    #   https://devkitpro.org/wiki/Getting_Started
    # Then:
    #   dkp-pacman -S switch-dev
    #   source $DEVKITPRO/devkita64/environment-setup-aarch64-none-elf
    #   cmake -DCMAKE_TOOLCHAIN_FILE=toolchains/switch-libnx.cmake ...
    message(FATAL_ERROR
        "Switch build requires devkitPro (devkitA64 + libnx).\n"
        "devkitA64 not found at: ${DEVKITA64}\n"
        "\n"
        "Install devkitPro: https://devkitpro.org/wiki/Getting_Started\n"
        "  then:  dkp-pacman -S switch-dev\n"
        "  then:  source \$DEVKITPRO/devkita64/environment-setup-aarch64-none-elf\n"
        "\n"
        "If devkitPro is installed in a non-standard location, set the\n"
        "DEVKITPRO environment variable (or pass -DDEVKITPRO=<path>) before\n"
        "re-running CMake.\n"
        "\n"
        "The previous 'Mode B' Clang cross-compile path has been removed:\n"
        "it could not build libnx, could not package a .nro, and leaked\n"
        "host libc++ headers into the aarch64-none-elf newlib build."
    )
endif()

# Compile flags for Switch homebrew
# -D_GNU_SOURCE enables POSIX visibility in newlib (fileno, isatty, fwrite_unlocked, etc.)
# Without it, -std=c++23 hides these functions.
# -DSPDLOG_NO_TZ_OFFSET: newlib's struct tm lacks tm_gmtoff.
set(SWITCH_C_FLAGS   "-D__SWITCH__ -D_GNU_SOURCE -DSPDLOG_NO_TZ_OFFSET -march=armv8-a+crc+crypto -mtune=cortex-a57 ${_SWITCH_TP_FLAG} -fPIE ${_SWITCH_EXTRA_FLAGS}")
set(SWITCH_CXX_FLAGS "${SWITCH_C_FLAGS}")

set(CMAKE_C_FLAGS_INIT   "${SWITCH_C_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${SWITCH_CXX_FLAGS}")

# newlib requires GNU extensions for POSIX visibility (__POSIX_VISIBLE, __MISC_VISIBLE).
# Without this, -std=c++23 hides fileno, isatty, fwrite_unlocked, etc.
set(CMAKE_CXX_EXTENSIONS ON)

# Prevent CMake from finding host (macOS/Linux) libraries
set(CMAKE_FIND_ROOT_PATH "${DEVKITPRO}" "${DEVKITPRO}/libnx")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Switch uses Vulkan via deko3d or SDL2's libnx backend
set(LIBERTY_RECOMP_VULKAN ON CACHE BOOL "Use Vulkan/deko3d renderer" FORCE)
set(LIBERTY_RECOMP_D3D12 OFF CACHE BOOL "" FORCE)
set(LIBERTY_RECOMP_METAL OFF CACHE BOOL "" FORCE)

# No installer wizard on Switch
set(LIBERTY_RECOMP_EMBEDDED_ASSETS ON CACHE BOOL "Package assets at build time (no installer UI)" FORCE)

# NRO (homebrew) output settings
set(LIBERTY_SWITCH_APP_TITLE "Liberty Recomp" CACHE STRING "Switch homebrew title")
set(LIBERTY_SWITCH_APP_AUTHOR "LibertyRecomp Team" CACHE STRING "Switch homebrew author")
set(LIBERTY_SWITCH_APP_VERSION "1.0.0" CACHE STRING "Switch homebrew version")
