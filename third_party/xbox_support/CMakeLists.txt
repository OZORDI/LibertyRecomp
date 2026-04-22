project("xbox_support")

# xbox_support: runtime helpers for Xbox 360 support (XEX/XDBF/big-endian types).
# Previously distributed as XenonUtils inside the XenonRecomp tool tree; moved here
# and made self-contained when the XenonRecomp tool was removed.

set(_XSUP_THIRDPARTY "${CMAKE_SOURCE_DIR}/third_party")

add_library(xbox_support STATIC
    "disasm.cpp"
    "xex.cpp"
    "image.cpp"
    "xdbf_wrapper.cpp"
    "xex_patcher.cpp"
    "memory_mapped_file.cpp"
    "${_XSUP_THIRDPARTY}/libmspack/libmspack/mspack/lzxd.c"
    "${_XSUP_THIRDPARTY}/tiny-AES-c/aes.c"
)
# Legacy alias — existing targets link against XenonUtils.
add_library(XenonUtils ALIAS xbox_support)

target_compile_definitions(xbox_support PRIVATE NOMINMAX)

target_include_directories(xbox_support
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${_XSUP_THIRDPARTY}/simde"
    PRIVATE
        "${_XSUP_THIRDPARTY}/libmspack/libmspack/mspack"
        "${_XSUP_THIRDPARTY}/tiny-AES-c"
        "${_XSUP_THIRDPARTY}/TinySHA1"
)

target_link_libraries(xbox_support PUBLIC disasm)
