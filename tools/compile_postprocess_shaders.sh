#!/bin/bash
# =============================================================================
# Post-Processing Shader Compilation Script
# Compiles SSAO, DoF, and other post-processing shaders to SPIR-V and Metal
# =============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
SHADER_DIR="$PROJECT_ROOT/LibertyRecomp/gpu/shader/hlsl"
MSL_DIR="$PROJECT_ROOT/LibertyRecomp/gpu/shader/msl"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Post-Processing Shader Compiler ===${NC}"
echo "Project root: $PROJECT_ROOT"
echo "Shader directory: $SHADER_DIR"

# Check for DXC (DirectX Shader Compiler)
if command -v dxc &> /dev/null; then
    DXC_CMD="dxc"
elif [ -f "/usr/local/bin/dxc" ]; then
    DXC_CMD="/usr/local/bin/dxc"
elif [ -f "/opt/homebrew/bin/dxc" ]; then
    DXC_CMD="/opt/homebrew/bin/dxc"
else
    echo -e "${YELLOW}Warning: DXC not found. Install via:${NC}"
    echo "  brew install dxc  (macOS)"
    echo "  apt install dxc   (Linux)"
    echo "  Or download from: https://github.com/microsoft/DirectXShaderCompiler"
    DXC_CMD=""
fi

# List of post-processing shaders to compile
POSTPROCESS_SHADERS=(
    "ssao_gtao_ps"
    "ssao_blur_ps"
    "ssao_composite_ps"
    "dof_ps"
    "ssr_raytrace_ps"
    "ssr_composite_ps"
)

# Compile HLSL to SPIR-V
compile_spirv() {
    local shader_name=$1
    local shader_type=$2  # vs or ps
    local entry_point="main"
    local profile="ps_6_0"
    
    if [ "$shader_type" == "vs" ]; then
        profile="vs_6_0"
    fi
    
    local input_file="$SHADER_DIR/${shader_name}.hlsl"
    local output_file="$SHADER_DIR/${shader_name}.hlsl.spirv"
    local header_file="$SHADER_DIR/${shader_name}.hlsl.spirv.h"
    
    if [ ! -f "$input_file" ]; then
        echo -e "${RED}Error: Shader not found: $input_file${NC}"
        return 1
    fi
    
    if [ -z "$DXC_CMD" ]; then
        echo -e "${YELLOW}Skipping SPIR-V compilation (DXC not available)${NC}"
        return 0
    fi
    
    echo "Compiling $shader_name to SPIR-V..."
    
    # Compile to SPIR-V binary
    $DXC_CMD -T $profile -E $entry_point -spirv \
        -fspv-target-env=vulkan1.1 \
        -fvk-use-dx-layout \
        -Fo "$output_file" \
        "$input_file"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}  ✓ Generated: $output_file${NC}"
        
        # Convert to C header
        generate_header "$output_file" "$header_file" "g_${shader_name}_spirv"
    else
        echo -e "${RED}  ✗ Failed to compile $shader_name${NC}"
        return 1
    fi
}

# Generate C header from binary
generate_header() {
    local input_file=$1
    local output_file=$2
    local array_name=$3
    
    echo "// Auto-generated shader binary" > "$output_file"
    echo "// Source: $(basename $input_file)" >> "$output_file"
    echo "#pragma once" >> "$output_file"
    echo "" >> "$output_file"
    echo "static const unsigned char ${array_name}[] = {" >> "$output_file"
    
    xxd -i < "$input_file" | sed 's/^/    /' >> "$output_file"
    
    echo "};" >> "$output_file"
    echo "" >> "$output_file"
    
    echo -e "${GREEN}  ✓ Generated header: $output_file${NC}"
}

# Compile HLSL to Metal (for macOS)
compile_metal() {
    local shader_name=$1
    
    local input_file="$SHADER_DIR/${shader_name}.hlsl"
    local metal_file="$MSL_DIR/${shader_name}.metal"
    local metallib_file="$MSL_DIR/${shader_name}.metallib"
    local header_file="$MSL_DIR/${shader_name}.metal.metallib.h"
    
    if [ ! -f "$input_file" ]; then
        echo -e "${RED}Error: Shader not found: $input_file${NC}"
        return 1
    fi
    
    # Check if we're on macOS
    if [[ "$OSTYPE" != "darwin"* ]]; then
        echo -e "${YELLOW}Skipping Metal compilation (not on macOS)${NC}"
        return 0
    fi
    
    # Check for SPIRV-Cross for HLSL->Metal conversion
    if ! command -v spirv-cross &> /dev/null; then
        echo -e "${YELLOW}Warning: spirv-cross not found. Install via:${NC}"
        echo "  brew install spirv-cross"
        return 0
    fi
    
    echo "Compiling $shader_name to Metal..."
    
    # First compile to SPIR-V, then cross-compile to Metal
    local spirv_temp="/tmp/${shader_name}.spv"
    
    if [ -n "$DXC_CMD" ]; then
        $DXC_CMD -T ps_6_0 -E main -spirv \
            -fspv-target-env=vulkan1.1 \
            -Fo "$spirv_temp" \
            "$input_file"
        
        if [ $? -eq 0 ]; then
            # Cross-compile SPIR-V to Metal
            spirv-cross --msl "$spirv_temp" --output "$metal_file"
            
            if [ $? -eq 0 ]; then
                echo -e "${GREEN}  ✓ Generated: $metal_file${NC}"
                
                # Compile Metal to metallib
                xcrun -sdk macosx metal -c "$metal_file" -o "/tmp/${shader_name}.air"
                xcrun -sdk macosx metallib "/tmp/${shader_name}.air" -o "$metallib_file"
                
                if [ $? -eq 0 ]; then
                    echo -e "${GREEN}  ✓ Generated: $metallib_file${NC}"
                    generate_header "$metallib_file" "$header_file" "g_${shader_name}_air"
                fi
                
                rm -f "/tmp/${shader_name}.air"
            fi
        fi
        
        rm -f "$spirv_temp"
    fi
}

# Main compilation loop
echo ""
echo -e "${GREEN}Compiling post-processing shaders...${NC}"
echo ""

for shader in "${POSTPROCESS_SHADERS[@]}"; do
    echo "--- Processing: $shader ---"
    compile_spirv "$shader" "ps"
    compile_metal "$shader"
    echo ""
done

echo -e "${GREEN}=== Shader compilation complete ===${NC}"
echo ""
echo "Next steps:"
echo "1. Uncomment shader includes in postprocess_renderer.cpp"
echo "2. Uncomment shader loading code in CreateShaders()"
echo "3. Rebuild the project"
