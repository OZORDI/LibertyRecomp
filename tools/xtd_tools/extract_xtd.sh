#!/bin/bash
# Extract XTD texture container using RAGE Console Texture Editor via Wine
#
# Usage: ./extract_xtd.sh <input.xtd> <output_folder>
#
# This script launches the GUI editor. You'll need to:
# 1. File -> Open the XTD file
# 2. Select all textures
# 3. Right-click -> Export
# 4. Choose the output folder

INPUT_XTD="${1:-/Users/Ozordi/Downloads/LibertyRecomp/buttons_360.xtd}"
OUTPUT_DIR="${2:-/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/extracted_textures}"

EDITOR_DIR="/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/full/RAGE-Console-Texture-Editor-master"
EDITOR_EXE="GTA V Console Texture Editor 1.3.1.exe"

# Create output directory
mkdir -p "$OUTPUT_DIR"

echo "==================================="
echo "XTD Texture Extractor"
echo "==================================="
echo "Input XTD: $INPUT_XTD"
echo "Output Dir: $OUTPUT_DIR"
echo ""
echo "Launching RAGE Console Texture Editor via Wine..."
echo ""
echo "MANUAL STEPS REQUIRED:"
echo "1. File -> Open -> Navigate to: $INPUT_XTD"
echo "2. Select all textures (Ctrl+A)"
echo "3. Right-click -> Export"
echo "4. Save to: $OUTPUT_DIR"
echo ""

# Check for wine
if command -v wine64 &> /dev/null; then
    WINE_CMD="wine64"
elif command -v wine &> /dev/null; then
    WINE_CMD="wine"
elif [ -x "/opt/homebrew/bin/wine64" ]; then
    WINE_CMD="/opt/homebrew/bin/wine64"
elif [ -x "/usr/local/bin/wine64" ]; then
    WINE_CMD="/usr/local/bin/wine64"
else
    echo "ERROR: Wine not found. Please install Wine first:"
    echo "  brew install --cask wine-stable"
    exit 1
fi

echo "Using Wine: $WINE_CMD"

cd "$EDITOR_DIR"
"$WINE_CMD" "$EDITOR_EXE"
