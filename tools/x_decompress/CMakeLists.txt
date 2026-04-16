project("x_decompress")
set(CMAKE_CXX_STANDARD 17)

add_executable(x_decompress
    "x_decompress.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/libmspack/libmspack/mspack/lzxd.c"
)

target_include_directories(x_decompress
    PRIVATE "${CMAKE_SOURCE_DIR}/third_party/libmspack/libmspack/mspack"
)
