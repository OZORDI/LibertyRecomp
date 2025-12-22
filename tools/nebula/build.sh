#!/bin/bash
# Build script for Nebula VPN binaries
#
# This script builds Nebula from source or downloads pre-built binaries.
# Requires Go 1.21+ for building from source.

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="${SCRIPT_DIR}/src"
BIN_DIR="${SCRIPT_DIR}/bin"
NEBULA_VERSION="v1.9.0"

# Detect platform
detect_platform() {
    case "$(uname -s)" in
        Darwin*)
            PLATFORM="darwin"
            ;;
        Linux*)
            PLATFORM="linux"
            ;;
        MINGW*|MSYS*|CYGWIN*)
            PLATFORM="windows"
            ;;
        *)
            echo "Unsupported platform: $(uname -s)"
            exit 1
            ;;
    esac

    case "$(uname -m)" in
        x86_64|amd64)
            ARCH="amd64"
            ;;
        arm64|aarch64)
            ARCH="arm64"
            ;;
        *)
            echo "Unsupported architecture: $(uname -m)"
            exit 1
            ;;
    esac

    if [ "$PLATFORM" = "windows" ]; then
        EXE_SUFFIX=".exe"
    else
        EXE_SUFFIX=""
    fi

    echo "Detected platform: ${PLATFORM}-${ARCH}"
}

# Build from source using Go
build_from_source() {
    echo "Building Nebula from source..."
    
    if ! command -v go &> /dev/null; then
        echo "Error: Go compiler not found. Please install Go 1.21+ or use --download"
        exit 1
    fi

    GO_VERSION=$(go version | grep -oE 'go[0-9]+\.[0-9]+' | head -1)
    echo "Found Go: ${GO_VERSION}"

    OUTPUT_DIR="${BIN_DIR}/${PLATFORM}-${ARCH}"
    mkdir -p "${OUTPUT_DIR}"

    cd "${SOURCE_DIR}"

    echo "Building nebula..."
    CGO_ENABLED=0 GOOS=${PLATFORM} GOARCH=${ARCH} \
        go build -trimpath -ldflags "-s -w" \
        -o "${OUTPUT_DIR}/nebula${EXE_SUFFIX}" \
        ./cmd/nebula

    echo "Building nebula-cert..."
    CGO_ENABLED=0 GOOS=${PLATFORM} GOARCH=${ARCH} \
        go build -trimpath -ldflags "-s -w" \
        -o "${OUTPUT_DIR}/nebula-cert${EXE_SUFFIX}" \
        ./cmd/nebula-cert

    echo "Build complete: ${OUTPUT_DIR}"
    ls -la "${OUTPUT_DIR}"
}

# Download pre-built binaries from GitHub releases
download_binaries() {
    echo "Downloading Nebula ${NEBULA_VERSION} binaries..."

    OUTPUT_DIR="${BIN_DIR}/${PLATFORM}-${ARCH}"
    mkdir -p "${OUTPUT_DIR}"

    # Construct download URL
    if [ "$PLATFORM" = "darwin" ]; then
        ARCHIVE_NAME="nebula-${PLATFORM}-${ARCH}.zip"
    else
        ARCHIVE_NAME="nebula-${PLATFORM}-${ARCH}.tar.gz"
    fi

    DOWNLOAD_URL="https://github.com/slackhq/nebula/releases/download/${NEBULA_VERSION}/${ARCHIVE_NAME}"

    echo "Downloading from: ${DOWNLOAD_URL}"

    TEMP_DIR=$(mktemp -d)
    ARCHIVE_PATH="${TEMP_DIR}/${ARCHIVE_NAME}"

    if command -v curl &> /dev/null; then
        curl -L -o "${ARCHIVE_PATH}" "${DOWNLOAD_URL}"
    elif command -v wget &> /dev/null; then
        wget -O "${ARCHIVE_PATH}" "${DOWNLOAD_URL}"
    else
        echo "Error: curl or wget required for download"
        exit 1
    fi

    echo "Extracting..."
    if [[ "${ARCHIVE_NAME}" == *.zip ]]; then
        unzip -o "${ARCHIVE_PATH}" -d "${TEMP_DIR}"
    else
        tar -xzf "${ARCHIVE_PATH}" -C "${TEMP_DIR}"
    fi

    # Copy binaries to output directory
    cp "${TEMP_DIR}/nebula${EXE_SUFFIX}" "${OUTPUT_DIR}/"
    cp "${TEMP_DIR}/nebula-cert${EXE_SUFFIX}" "${OUTPUT_DIR}/"

    # Make executable
    chmod +x "${OUTPUT_DIR}/nebula${EXE_SUFFIX}"
    chmod +x "${OUTPUT_DIR}/nebula-cert${EXE_SUFFIX}"

    # Cleanup
    rm -rf "${TEMP_DIR}"

    echo "Download complete: ${OUTPUT_DIR}"
    ls -la "${OUTPUT_DIR}"
}

# Build for all platforms (cross-compilation)
build_all() {
    echo "Building for all platforms..."

    PLATFORMS=("darwin-amd64" "darwin-arm64" "linux-amd64" "windows-amd64")

    for PLAT_ARCH in "${PLATFORMS[@]}"; do
        PLATFORM="${PLAT_ARCH%-*}"
        ARCH="${PLAT_ARCH#*-}"
        
        if [ "$PLATFORM" = "windows" ]; then
            EXE_SUFFIX=".exe"
        else
            EXE_SUFFIX=""
        fi

        OUTPUT_DIR="${BIN_DIR}/${PLATFORM}-${ARCH}"
        mkdir -p "${OUTPUT_DIR}"

        echo "Building for ${PLATFORM}-${ARCH}..."

        cd "${SOURCE_DIR}"

        CGO_ENABLED=0 GOOS=${PLATFORM} GOARCH=${ARCH} \
            go build -trimpath -ldflags "-s -w" \
            -o "${OUTPUT_DIR}/nebula${EXE_SUFFIX}" \
            ./cmd/nebula

        CGO_ENABLED=0 GOOS=${PLATFORM} GOARCH=${ARCH} \
            go build -trimpath -ldflags "-s -w" \
            -o "${OUTPUT_DIR}/nebula-cert${EXE_SUFFIX}" \
            ./cmd/nebula-cert

        echo "Done: ${OUTPUT_DIR}"
    done

    echo "All platforms built successfully!"
}

# Print usage
usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --build       Build from source (requires Go 1.21+)"
    echo "  --download    Download pre-built binaries"
    echo "  --all         Build for all platforms (requires Go)"
    echo "  --help        Show this help message"
    echo ""
    echo "If no option is specified, defaults to --build"
}

# Main
detect_platform

case "${1:-}" in
    --download)
        download_binaries
        ;;
    --all)
        build_all
        ;;
    --help|-h)
        usage
        ;;
    --build|"")
        build_from_source
        ;;
    *)
        echo "Unknown option: $1"
        usage
        exit 1
        ;;
esac
