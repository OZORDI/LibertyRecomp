#!/bin/bash
# Extract XTD textures using xtd_cli.exe via Wine
# Usage: ./run_xtd_extract.sh

set -e

cd /Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools

INPUT_XTD="/Users/Ozordi/Downloads/LibertyRecomp/buttons_360.xtd"
OUTPUT_DIR="extracted_textures"

echo "=== XTD CLI Extractor ==="
echo "Input: $INPUT_XTD"
echo "Output: $OUTPUT_DIR"
echo ""

mkdir -p "$OUTPUT_DIR"

# First list the textures
echo "--- Listing textures in XTD ---"
wine xtd_cli.exe list "$INPUT_XTD" 2>&1

echo ""
echo "--- Extracting textures ---"
wine xtd_cli.exe export "$INPUT_XTD" "$OUTPUT_DIR" 2>&1

echo ""
echo "--- Extraction complete ---"
ls -la "$OUTPUT_DIR"
