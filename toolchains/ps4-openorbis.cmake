# PS4 / OpenOrbis Toolchain
# Requirements:
#   - OpenOrbis PS4 Toolchain cloned into tools/OpenOrbis-PS4-Toolchain
#     (already present — see tools/OpenOrbis-PS4-Toolchain)
#   - LLVM/lld installed: brew install llvm  (macOS/Linux)
#   - Environment variable OO_PS4_TOOLCHAIN pointing at the toolchain root
#
# Setup:
#   export OO_PS4_TOOLCHAIN="$(pwd)/tools/OpenOrbis-PS4-Toolchain"
#   cmake -DCMAKE_TOOLCHAIN_FILE=toolchains/ps4-openorbis.cmake \
#         -DLIBERTY_RECOMP_TARGET_PLATFORM=ps4 \
#         -DLIBERTY_RECOMP_EMBEDDED_GAME_PATH=tools/local_game_payload \
#         ..

# Tell rexglue SDK we're targeting PS4 console
set(LIBERTY_RECOMP_TARGET_PLATFORM "ps4" CACHE STRING "" FORCE)

# Resolve toolchain root
if(NOT DEFINED OO_PS4_TOOLCHAIN)
    if(DEFINED ENV{OO_PS4_TOOLCHAIN})
        set(OO_PS4_TOOLCHAIN "$ENV{OO_PS4_TOOLCHAIN}")
    else()
        # Default: repo-local clone (use CMAKE_CURRENT_LIST_DIR for try_compile safety)
        get_filename_component(_toolchain_repo_root "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
        set(OO_PS4_TOOLCHAIN "${_toolchain_repo_root}/tools/OpenOrbis-PS4-Toolchain")
    endif()
endif()

if(NOT EXISTS "${OO_PS4_TOOLCHAIN}")
    message(FATAL_ERROR
        "OpenOrbis toolchain not found at: ${OO_PS4_TOOLCHAIN}\n"
        "Run: git clone https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain.git tools/OpenOrbis-PS4-Toolchain\n"
        "Or set OO_PS4_TOOLCHAIN to the correct path.")
endif()

message(STATUS "Using OpenOrbis PS4 Toolchain: ${OO_PS4_TOOLCHAIN}")

set(CMAKE_SYSTEM_NAME PS4)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# OpenOrbis sample builds target FreeBSD ELF and rely on link.x + create-fself
# for PS4-specific rewriting. Match that model instead of mixing in the
# x86_64-scei-ps4 target triple.
# Prefer Homebrew LLVM (newer, with matching libc++ headers)
find_program(CLANG_PATH "clang" PATHS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin NO_DEFAULT_PATH)
find_program(CLANGPP_PATH "clang++" PATHS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin NO_DEFAULT_PATH)
if(NOT CLANG_PATH)
    find_program(CLANG_PATH "clang" REQUIRED)
    find_program(CLANGPP_PATH "clang++" REQUIRED)
endif()
find_program(LLD_PATH "lld" REQUIRED)
find_program(LD_LLD_PATH "ld.lld" PATHS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin NO_DEFAULT_PATH)
if(NOT LD_LLD_PATH)
    find_program(LD_LLD_PATH "ld.lld" REQUIRED)
endif()

set(CMAKE_C_COMPILER   "${CLANG_PATH}")
set(CMAKE_CXX_COMPILER "${CLANGPP_PATH}")
set(CMAKE_LINKER        "${LD_LLD_PATH}")

set(CMAKE_C_COMPILER_TARGET   "x86_64-pc-freebsd12-elf")
set(CMAKE_CXX_COMPILER_TARGET "x86_64-pc-freebsd12-elf")
set(CMAKE_ASM_COMPILER        "${CLANG_PATH}")
set(CMAKE_ASM_COMPILER_TARGET "x86_64-pc-freebsd12-elf")

# The x86_64-scei-ps4 driver expects `orbis-ld` on the tool search path.
# We provide a thin wrapper at ${OO_PS4_TOOLCHAIN}/bin/orbis-ld that execs
# the host lld (which natively supports the PS4/orbis linker flavor).
# -B tells clang where to find toolchain programs.
# Link PS4 system libraries: C/C++ runtime, kernel, and SDK stubs.
set(_PS4_LINK_FLAGS "-B ${OO_PS4_TOOLCHAIN}/bin")
# PS4 CRT objects: crt1.o provides _start entry point, crti/crtn provide
# _init/_fini sections for constructors/destructors.
string(APPEND _PS4_LINK_FLAGS " ${OO_PS4_TOOLCHAIN}/lib/crt1.o")
string(APPEND _PS4_LINK_FLAGS " ${OO_PS4_TOOLCHAIN}/lib/crti.o")
# C/C++ standard libraries (musl libc, libc++, libc++abi, unwind, compiler-rt)
# libcxx_ps4_compat.a: LLVM 21 libc++/libc++abi compiled for PS4 (replaces old OO SDK versions)
string(APPEND _PS4_LINK_FLAGS " ${OO_PS4_TOOLCHAIN}/lib/libcxx_ps4_compat.a")
string(APPEND _PS4_LINK_FLAGS " -lc++ -lc++abi -lunwind -lc -lm")
string(APPEND _PS4_LINK_FLAGS " ${OO_PS4_TOOLCHAIN}/lib/libclang_rt.builtins-x86_64.a")
# PS4 kernel (provides pthread, sceKernel*, mmap, etc.)
string(APPEND _PS4_LINK_FLAGS " -lkernel")
# PS4 SDK stub libraries
string(APPEND _PS4_LINK_FLAGS " -lSceSystemService -lSceSysmodule -lScePad -lSceUserService")
string(APPEND _PS4_LINK_FLAGS " -lSceAudioOut -lSceAudioIn -lSceVideoOut")
string(APPEND _PS4_LINK_FLAGS " -lSceNet -lSceNetCtl -lSceHttp -lSceSsl")
string(APPEND _PS4_LINK_FLAGS " -lSceNpManager -lSceNpTrophy -lSceNpSignaling -lSceNpMatching2")
string(APPEND _PS4_LINK_FLAGS " -lSceSaveData -lSceCommonDialog -lSceMsgDialog")
string(APPEND _PS4_LINK_FLAGS " -lSceGnmDriver -lSceFios2 -lScePosix")
string(APPEND _PS4_LINK_FLAGS " -lSceRtc -lSceFreeType")
string(APPEND _PS4_LINK_FLAGS " ${OO_PS4_TOOLCHAIN}/lib/crtn.o")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld ${_PS4_LINK_FLAGS}")

# Cross-compilation: skip try_compile linking
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Add OpenOrbis include/lib paths (PS4-specific headers like orbis/*.h)
include_directories(SYSTEM "${OO_PS4_TOOLCHAIN}/include")
link_directories("${OO_PS4_TOOLCHAIN}/lib")

# Build include paths for cross-compilation.
# Order: Homebrew libc++ (C++23 features) → clang builtins → SDK C headers (musl).
# We use Homebrew's modern libc++ for C++23 support (std::bit_cast, etc.)
# The SDK-bundled libc++ is too old (~LLVM 10). We provide an xlocale.h shim
# and enable _GNU_SOURCE so musl exposes the POSIX APIs libc++ needs.
get_filename_component(_BREW_LLVM_DIR "${CLANG_PATH}/../.." ABSOLUTE)
execute_process(
    COMMAND "${CLANG_PATH}" -print-resource-dir
    OUTPUT_VARIABLE _CLANG_RESOURCE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(_PS4_C_ISYSTEM "")
set(_PS4_CXX_ISYSTEM "")
# Only C++ compilations should see libc++ headers.
# 0) PS4 __config_site overlay (shadows Homebrew's to override musl/pthread settings)
string(APPEND _PS4_CXX_ISYSTEM " -isystem ${OO_PS4_TOOLCHAIN}/include/c++/v1")
# 1) Homebrew libc++ C++ stdlib headers (modern, C++23 capable)
string(APPEND _PS4_CXX_ISYSTEM " -isystem ${_BREW_LLVM_DIR}/include/c++/v1")
message(STATUS "PS4: Using Homebrew libc++ headers from ${_BREW_LLVM_DIR}")
# 2) Clang builtins (stddef.h, stdint.h, etc.)
if(_CLANG_RESOURCE_DIR)
    string(APPEND _PS4_C_ISYSTEM   " -isystem ${_CLANG_RESOURCE_DIR}/include")
    string(APPEND _PS4_CXX_ISYSTEM " -isystem ${_CLANG_RESOURCE_DIR}/include")
endif()
# 3) OpenOrbis SDK C headers (musl: string.h, stdio.h, pthread.h, etc.)
string(APPEND _PS4_C_ISYSTEM   " -isystem ${OO_PS4_TOOLCHAIN}/include")
string(APPEND _PS4_CXX_ISYSTEM " -isystem ${OO_PS4_TOOLCHAIN}/include")

message(STATUS "PS4: Using Homebrew LLVM from ${_BREW_LLVM_DIR}")

# _GNU_SOURCE enables all POSIX extensions in musl (nanosleep, localtime_r,
# locale_t, uselocale, fileno, fsync, tm_gmtoff, etc.) that libc++ requires.
# -U__FreeBSD__: PS4 is FreeBSD-based but we use musl headers, not FreeBSD libc.
#   Without this, libc++ selects the FreeBSD locale backend which needs
#   strtoll_l, snprintf_l, asprintf_l, etc. that musl doesn't provide.
# _LIBCPP_PROVIDES_DEFAULT_RUNE_TABLE: provides generic ctype bitmasks
#   instead of platform-specific _CTYPE_* / _IS* constants.
# Note: _LIBCPP_HAS_MUSL_LIBC and _LIBCPP_HAS_THREAD_API_PTHREAD are set
# via the __config_site overlay in include/c++/v1/__config_site.
set(_PS4_DEFINES "-D__ORBIS__ -D_GNU_SOURCE -U__FreeBSD__ -D_LIBCPP_PROVIDES_DEFAULT_RUNE_TABLE")

# Suppress default includes and set our explicit ordered paths
# Modern clang may define wchar_t via stddef.h before OpenOrbis musl headers,
# or reach stdlib.h without it depending on include order. Force stddef.h in
# first for C and suppress the duplicate musl typedef in that mode.
set(CMAKE_C_FLAGS_INIT   "${_PS4_DEFINES} -D__DEFINED_wchar_t -include stddef.h -nostdinc ${_PS4_C_ISYSTEM}")
set(CMAKE_CXX_FLAGS_INIT "${_PS4_DEFINES} -fexceptions -fcxx-exceptions -frtti -nostdinc++ -nostdinc ${_PS4_CXX_ISYSTEM}")

# Prevent CMake from finding host (macOS) libraries during cross-compilation
set(CMAKE_FIND_ROOT_PATH "${OO_PS4_TOOLCHAIN}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# PS4 uses a custom Vulkan-like GNM API via OpenOrbis's SDL-PS4 or GNM headers.
# For now we target SDL-PS4 with software-emulated Vulkan layer.
set(LIBERTY_RECOMP_VULKAN ON CACHE BOOL "Use Vulkan renderer (via PS4 layer)" FORCE)
set(LIBERTY_RECOMP_D3D12 OFF CACHE BOOL "" FORCE)
set(LIBERTY_RECOMP_METAL OFF CACHE BOOL "" FORCE)

# No installer wizard on PS4
set(LIBERTY_RECOMP_EMBEDDED_ASSETS ON CACHE BOOL "Package assets at build time (no installer UI)" FORCE)

# PKG output target name
set(LIBERTY_PS4_TITLE_ID "LBTY00001" CACHE STRING "PS4 title ID for PKG packaging")
set(LIBERTY_PS4_APP_VERSION "01.00" CACHE STRING "PS4 application version")
