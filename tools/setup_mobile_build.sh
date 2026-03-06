#!/bin/bash
# Setup script for mobile/console builds
# This helps users prepare their environment for building

set -e

echo "==================================="
echo "Liberty Recomp Mobile/Console Setup"
echo "==================================="
echo ""

# Detect platform
if [[ "$OSTYPE" == "darwin"* ]]; then
    HOST_OS="macos"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    HOST_OS="linux"
else
    echo "Unsupported host OS: $OSTYPE"
    exit 1
fi

echo "Detected host OS: $HOST_OS"
echo ""

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check CMake
echo "Checking CMake..."
if command_exists cmake; then
    CMAKE_VERSION=$(cmake --version | head -n1 | awk '{print $3}')
    echo "✓ CMake $CMAKE_VERSION found"
else
    echo "✗ CMake not found"
    echo "  Install: brew install cmake (macOS) or apt install cmake (Linux)"
    exit 1
fi

echo ""
echo "Which platform do you want to build for?"
echo "1) iOS (requires macOS + Xcode)"
echo "2) Android (requires Android SDK + NDK)"
echo "3) PS4 (requires LLVM/Clang + OpenOrbis)"
echo "4) Nintendo Switch (requires devkitPro)"
echo "5) Check all platforms"
read -p "Enter choice [1-5]: " choice

check_ios() {
    echo ""
    echo "=== iOS Build Requirements ==="
    
    if [[ "$HOST_OS" != "macos" ]]; then
        echo "✗ iOS builds require macOS"
        return 1
    fi
    
    if command_exists xcodebuild; then
        XCODE_VERSION=$(xcodebuild -version | head -n1)
        echo "✓ $XCODE_VERSION found"
    else
        echo "✗ Xcode not found"
        echo "  Install from App Store or: xcode-select --install"
        return 1
    fi
    
    echo "✓ iOS build environment ready"
    echo ""
    echo "To build:"
    echo "  cmake --preset ios-release"
    echo "  open out/build/ios-release/LibertyRecomp.xcodeproj"
}

check_android() {
    echo ""
    echo "=== Android Build Requirements ==="
    
    if [[ -z "$ANDROID_HOME" ]] && [[ -z "$ANDROID_SDK_ROOT" ]]; then
        echo "✗ ANDROID_HOME or ANDROID_SDK_ROOT not set"
        echo "  Set: export ANDROID_HOME=/path/to/android-sdk"
        return 1
    fi
    
    SDK_PATH="${ANDROID_HOME:-$ANDROID_SDK_ROOT}"
    echo "✓ Android SDK: $SDK_PATH"
    
    if [[ -z "$ANDROID_NDK" ]]; then
        echo "⚠ ANDROID_NDK not set, will try to find in SDK"
        # Try to find NDK
        if [[ -d "$SDK_PATH/ndk" ]]; then
            LATEST_NDK=$(ls -1 "$SDK_PATH/ndk" | sort -V | tail -n1)
            if [[ -n "$LATEST_NDK" ]]; then
                export ANDROID_NDK="$SDK_PATH/ndk/$LATEST_NDK"
                echo "✓ Found NDK: $ANDROID_NDK"
            fi
        fi
    else
        echo "✓ Android NDK: $ANDROID_NDK"
    fi
    
    if [[ -z "$JAVA_HOME" ]]; then
        echo "⚠ JAVA_HOME not set"
        if command_exists java; then
            JAVA_VERSION=$(java -version 2>&1 | head -n1)
            echo "  Found: $JAVA_VERSION"
        fi
    else
        echo "✓ Java: $JAVA_HOME"
    fi
    
    echo ""
    echo "To build:"
    echo "  cd android && ./gradlew assembleRelease"
}

check_ps4() {
    echo ""
    echo "=== PS4 Build Requirements ==="
    
    if command_exists clang; then
        CLANG_VERSION=$(clang --version | head -n1)
        echo "✓ $CLANG_VERSION"
    else
        echo "✗ Clang not found"
        echo "  Install: brew install llvm (macOS) or apt install clang (Linux)"
        return 1
    fi
    
    if command_exists lld; then
        echo "✓ lld linker found"
    else
        echo "✗ lld not found"
        echo "  Install with LLVM package"
        return 1
    fi
    
    TOOLCHAIN_PATH="$(pwd)/tools/OpenOrbis-PS4-Toolchain"
    if [[ -d "$TOOLCHAIN_PATH/include" ]]; then
        echo "✓ OpenOrbis toolchain found"
    else
        echo "✗ OpenOrbis toolchain not found"
        echo "  Run: git submodule update --init --recursive"
        return 1
    fi
    
    echo "✓ PS4 build environment ready"
    echo ""
    echo "To build:"
    echo "  export OO_PS4_TOOLCHAIN=$(pwd)/tools/OpenOrbis-PS4-Toolchain"
    echo "  cmake --preset ps4-release"
    echo "  cmake --build out/build/ps4-release"
}

check_switch() {
    echo ""
    echo "=== Nintendo Switch Build Requirements ==="
    
    if [[ -z "$DEVKITPRO" ]]; then
        echo "✗ DEVKITPRO not set"
        echo "  Install from: https://devkitpro.org/wiki/Getting_Started"
        echo "  Then: export DEVKITPRO=/opt/devkitpro"
        return 1
    fi
    
    echo "✓ DEVKITPRO: $DEVKITPRO"
    
    if [[ -z "$DEVKITA64" ]]; then
        export DEVKITA64="$DEVKITPRO/devkitA64"
    fi
    
    if [[ -d "$DEVKITA64" ]]; then
        echo "✓ devkitA64 found"
    else
        echo "✗ devkitA64 not found"
        echo "  Install: dkp-pacman -S switch-dev"
        return 1
    fi
    
    if command_exists dkp-pacman; then
        if dkp-pacman -Q switch-dev >/dev/null 2>&1; then
            echo "✓ switch-dev package installed"
        else
            echo "✗ switch-dev not installed"
            echo "  Install: dkp-pacman -S switch-dev"
            return 1
        fi
    fi
    
    echo "✓ Switch build environment ready"
    echo ""
    echo "To build:"
    echo "  cmake --preset switch-release"
    echo "  cmake --build out/build/switch-release"
}

check_game_files() {
    echo ""
    echo "=== Game Files Check ==="
    
    PAYLOAD_DIR="tools/local_game_payload"
    
    if [[ ! -f "$PAYLOAD_DIR/default.xex" ]]; then
        echo "✗ Game files not found in $PAYLOAD_DIR"
        echo ""
        echo "You need to copy your GTA IV Xbox 360 files:"
        echo "  - default.xex"
        echo "  - common.rpf"
        echo "  - xbox360.rpf"
        echo "  - audio.rpf"
        echo ""
        echo "See $PAYLOAD_DIR/README.md for instructions"
        return 1
    fi
    
    echo "✓ default.xex found"
    
    for file in common.rpf xbox360.rpf audio.rpf; do
        if [[ -f "$PAYLOAD_DIR/$file" ]]; then
            SIZE=$(du -h "$PAYLOAD_DIR/$file" | cut -f1)
            echo "✓ $file ($SIZE)"
        else
            echo "✗ $file missing"
        fi
    done
    
    # Check DLC
    if [[ -d "$PAYLOAD_DIR/dlc/TLAD" ]]; then
        echo "✓ TLAD DLC found"
    fi
    
    if [[ -d "$PAYLOAD_DIR/dlc/TBOGT" ]]; then
        echo "✓ TBOGT DLC found"
    fi
}

case $choice in
    1)
        check_ios
        check_game_files
        ;;
    2)
        check_android
        check_game_files
        ;;
    3)
        check_ps4
        check_game_files
        ;;
    4)
        check_switch
        check_game_files
        ;;
    5)
        check_ios || true
        check_android || true
        check_ps4 || true
        check_switch || true
        check_game_files
        ;;
    *)
        echo "Invalid choice"
        exit 1
        ;;
esac

echo ""
echo "==================================="
echo "Setup check complete!"
echo "==================================="
