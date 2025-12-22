# Nebula VPN Integration for Liberty Recomp Online Multiplayer
# 
# Nebula is a scalable overlay networking tool with a focus on performance,
# simplicity and security. It lets you seamlessly connect computers anywhere
# in the world.
#
# Source: https://github.com/slackhq/nebula
# License: MIT

cmake_minimum_required(VERSION 3.20)

# Nebula is written in Go - we provide build scripts and pre-built binaries
# This CMakeLists.txt handles installation of binaries to the output directory

set(NEBULA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(NEBULA_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(NEBULA_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config")

# Detect platform
if(WIN32)
    set(NEBULA_PLATFORM "windows")
    set(NEBULA_EXE_SUFFIX ".exe")
    set(NEBULA_ARCH "amd64")
elseif(APPLE)
    set(NEBULA_PLATFORM "darwin")
    set(NEBULA_EXE_SUFFIX "")
    if(CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
        set(NEBULA_ARCH "arm64")
    else()
        set(NEBULA_ARCH "amd64")
    endif()
else()
    set(NEBULA_PLATFORM "linux")
    set(NEBULA_EXE_SUFFIX "")
    set(NEBULA_ARCH "amd64")
endif()

set(NEBULA_BINARY_NAME "nebula${NEBULA_EXE_SUFFIX}")
set(NEBULA_CERT_BINARY_NAME "nebula-cert${NEBULA_EXE_SUFFIX}")

# Option to build from source (requires Go toolchain)
option(NEBULA_BUILD_FROM_SOURCE "Build Nebula from source (requires Go 1.21+)" OFF)

if(NEBULA_BUILD_FROM_SOURCE)
    # Find Go compiler
    find_program(GO_EXECUTABLE go)
    if(NOT GO_EXECUTABLE)
        message(WARNING "Go compiler not found. Nebula will not be built from source.")
        set(NEBULA_BUILD_FROM_SOURCE OFF)
    else()
        # Get Go version
        execute_process(
            COMMAND ${GO_EXECUTABLE} version
            OUTPUT_VARIABLE GO_VERSION_OUTPUT
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "Found Go: ${GO_VERSION_OUTPUT}")
        
        # Create build directory
        set(NEBULA_BUILD_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/bin")
        file(MAKE_DIRECTORY ${NEBULA_BUILD_OUTPUT})
        
        # Build nebula binary
        add_custom_command(
            OUTPUT "${NEBULA_BUILD_OUTPUT}/${NEBULA_BINARY_NAME}"
            COMMAND ${CMAKE_COMMAND} -E env 
                GOOS=${NEBULA_PLATFORM} 
                GOARCH=${NEBULA_ARCH}
                CGO_ENABLED=0
                ${GO_EXECUTABLE} build -trimpath -ldflags "-s -w" 
                -o "${NEBULA_BUILD_OUTPUT}/${NEBULA_BINARY_NAME}"
                ./cmd/nebula
            WORKING_DIRECTORY ${NEBULA_SOURCE_DIR}
            COMMENT "Building Nebula VPN..."
            VERBATIM
        )
        
        # Build nebula-cert binary
        add_custom_command(
            OUTPUT "${NEBULA_BUILD_OUTPUT}/${NEBULA_CERT_BINARY_NAME}"
            COMMAND ${CMAKE_COMMAND} -E env 
                GOOS=${NEBULA_PLATFORM} 
                GOARCH=${NEBULA_ARCH}
                CGO_ENABLED=0
                ${GO_EXECUTABLE} build -trimpath -ldflags "-s -w" 
                -o "${NEBULA_BUILD_OUTPUT}/${NEBULA_CERT_BINARY_NAME}"
                ./cmd/nebula-cert
            WORKING_DIRECTORY ${NEBULA_SOURCE_DIR}
            COMMENT "Building Nebula Certificate Tool..."
            VERBATIM
        )
        
        # Custom target to build both
        add_custom_target(nebula_build ALL
            DEPENDS 
                "${NEBULA_BUILD_OUTPUT}/${NEBULA_BINARY_NAME}"
                "${NEBULA_BUILD_OUTPUT}/${NEBULA_CERT_BINARY_NAME}"
        )
        
        set(NEBULA_INSTALL_SOURCE "${NEBULA_BUILD_OUTPUT}")
    endif()
endif()

# Use pre-built binaries if not building from source
if(NOT NEBULA_BUILD_FROM_SOURCE)
    set(NEBULA_INSTALL_SOURCE "${NEBULA_BIN_DIR}/${NEBULA_PLATFORM}-${NEBULA_ARCH}")
    
    # Check if pre-built binaries exist
    if(NOT EXISTS "${NEBULA_INSTALL_SOURCE}/${NEBULA_BINARY_NAME}")
        message(STATUS "Pre-built Nebula binaries not found at ${NEBULA_INSTALL_SOURCE}")
        message(STATUS "Run 'tools/nebula/build.sh' to download or build binaries")
        message(STATUS "Or set NEBULA_BUILD_FROM_SOURCE=ON with Go 1.21+ installed")
    endif()
endif()

# Install binaries to output directory (alongside game executable)
if(EXISTS "${NEBULA_INSTALL_SOURCE}/${NEBULA_BINARY_NAME}" OR NEBULA_BUILD_FROM_SOURCE)
    install(
        FILES 
            "${NEBULA_INSTALL_SOURCE}/${NEBULA_BINARY_NAME}"
            "${NEBULA_INSTALL_SOURCE}/${NEBULA_CERT_BINARY_NAME}"
        DESTINATION bin/nebula
        PERMISSIONS 
            OWNER_READ OWNER_WRITE OWNER_EXECUTE 
            GROUP_READ GROUP_EXECUTE 
            WORLD_READ WORLD_EXECUTE
        OPTIONAL
    )
endif()

# Install configuration templates
install(
    FILES
        "${NEBULA_CONFIG_DIR}/lighthouse.yml.template"
        "${NEBULA_CONFIG_DIR}/client.yml.template"
    DESTINATION bin/nebula/config
    OPTIONAL
)

# Install README
install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    DESTINATION bin/nebula
    OPTIONAL
)
