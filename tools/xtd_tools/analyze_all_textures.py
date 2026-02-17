#!/usr/bin/env python3
"""
Comprehensive DDS texture format analyzer.
Compares original Xbox 360 textures with FusionFix source textures.
"""

import os
import struct
from pathlib import Path

# Paths
XBOX360_ORIGINAL = Path("/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/original_extracted_textures")
FUSIONFIX_SOURCE = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/GTAIV.EFLC.FusionFix-master/textures/pc/textures/buttons_360.wtd")
PROCESSED_DDS = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/processed_dds")

# All texture names in the XTD
ALL_TEXTURES = [
    "a_butt", "b_butt", "x_butt", "y_butt",
    "lb_butt", "rb_butt", "lt_butt", "rt_butt",
    "back_butt", "start_butt",
    "dpad_all", "dpad_up", "dpad_down", "dpad_left", "dpad_right",
    "dpad_updown", "dpad_leftright", "dpad_none",
    "lstick_all", "lstick_up", "lstick_down", "lstick_left", "lstick_right",
    "lstick_updown", "lstick_leftright", "lstick_none",
    "rstick_all", "rstick_up", "rstick_down", "rstick_left", "rstick_right",
    "rstick_updown", "rstick_leftright", "rstick_none",
    "up_arrow", "down_arrow", "left_arrow", "right_arrow",
]

# Controller prefixes
PREFIXES = ["", "ps3_", "ps4_", "ps5_", "switch_", "sc_", "sd_", "xbone_", "xsx_"]

def analyze_dds(filepath):
    """Analyze a DDS file and return format info."""
    if not filepath.exists():
        return None
    
    with open(filepath, 'rb') as f:
        data = f.read()
    
    if data[:4] != b'DDS ':
        return {"error": "Not DDS"}
    
    height = struct.unpack('<I', data[12:16])[0]
    width = struct.unpack('<I', data[16:20])[0]
    linear_size = struct.unpack('<I', data[20:24])[0]
    mip_count = struct.unpack('<I', data[28:32])[0]
    pf_flags = struct.unpack('<I', data[80:84])[0]
    fourcc = data[84:88]
    rgb_bit_count = struct.unpack('<I', data[88:92])[0]
    
    file_size = len(data)
    pixel_data_size = file_size - 128
    
    # Determine format
    if pf_flags & 0x4:  # DDPF_FOURCC
        format_name = fourcc.decode('ascii', errors='replace')
    elif pf_flags & 0x40:  # DDPF_RGB
        if pf_flags & 0x1:  # DDPF_ALPHAPIXELS
            format_name = f"RGBA{rgb_bit_count}"
        else:
            format_name = f"RGB{rgb_bit_count}"
    else:
        format_name = f"UNKNOWN(flags={hex(pf_flags)})"
    
    return {
        "width": width,
        "height": height,
        "format": format_name,
        "mips": mip_count,
        "data_size": pixel_data_size,
        "file_size": file_size,
        "pf_flags": pf_flags,
        "rgb_bits": rgb_bit_count,
    }

def main():
    print("=" * 100)
    print("DDS TEXTURE FORMAT ANALYSIS REPORT")
    print("=" * 100)
    
    # First, analyze all original Xbox 360 textures
    print("\n" + "=" * 60)
    print("ORIGINAL XBOX 360 TEXTURES (Target Format)")
    print("=" * 60)
    
    xbox_formats = {}
    for tex in ALL_TEXTURES:
        path = XBOX360_ORIGINAL / f"{tex}.dds"
        info = analyze_dds(path)
        if info:
            xbox_formats[tex] = info
            print(f"{tex:25} | {info['width']:3}x{info['height']:<3} | {info['format']:8} | {info['data_size']:6} bytes")
    
    # Summarize Xbox 360 formats
    unique_formats = set((v['format'], v['width'], v['height'], v['data_size']) for v in xbox_formats.values())
    print(f"\nUnique Xbox 360 formats: {unique_formats}")
    
    # Now check each prefix variant
    problematic_textures = []
    
    for prefix in PREFIXES:
        if prefix == "":
            prefix_name = "Xbox 360 (base)"
        else:
            prefix_name = prefix.rstrip('_').upper()
        
        print(f"\n{'=' * 60}")
        print(f"FUSIONFIX: {prefix_name}")
        print("=" * 60)
        
        wrong_format = []
        for tex in ALL_TEXTURES:
            dds_name = f"{prefix}{tex}.dds"
            path = FUSIONFIX_SOURCE / dds_name
            info = analyze_dds(path)
            
            if info:
                is_ok = info['format'] == 'DXT5' and info['data_size'] == 4096
                status = "✓" if is_ok else "✗ NEEDS CONVERSION"
                
                if not is_ok:
                    wrong_format.append((tex, info))
                    print(f"{tex:25} | {info['width']:3}x{info['height']:<3} | {info['format']:8} | {info['data_size']:6} bytes | {status}")
        
        if wrong_format:
            problematic_textures.extend([(prefix, t, i) for t, i in wrong_format])
            print(f"\n  ⚠ {len(wrong_format)} textures need format conversion for {prefix_name}")
        else:
            print(f"  ✓ All textures already in correct format")
    
    # Summary
    print("\n" + "=" * 100)
    print("SUMMARY: TEXTURES REQUIRING DXT5 CONVERSION")
    print("=" * 100)
    
    if problematic_textures:
        by_format = {}
        for prefix, tex, info in problematic_textures:
            fmt = info['format']
            if fmt not in by_format:
                by_format[fmt] = []
            by_format[fmt].append((prefix, tex, info))
        
        for fmt, items in by_format.items():
            print(f"\n{fmt} -> DXT5 ({len(items)} textures):")
            for prefix, tex, info in items[:10]:  # Show first 10
                print(f"  {prefix}{tex}.dds ({info['width']}x{info['height']}, {info['data_size']} bytes)")
            if len(items) > 10:
                print(f"  ... and {len(items) - 10} more")
    else:
        print("All textures are already in the correct format!")
    
    print("\n" + "=" * 100)
    print("REQUIRED CONVERSION COMMAND (using nvcompress or texconv):")
    print("=" * 100)
    print("""
For RGBA -> DXT5 conversion, you need a DXT compressor tool:

Option 1: NVIDIA Texture Tools (nvcompress)
  brew install nvidia-texture-tools
  nvcompress -bc3 input.dds output.dds

Option 2: DirectXTex texconv (via Wine)
  texconv -f BC3_UNORM -o output_dir input.dds

Option 3: Python with Pillow + texture compression
  pip install Pillow
  (Pillow can read RGBA but needs external lib for DXT encoding)

Option 4: Use 'crunch' (open source DXT compressor)
  git clone https://github.com/BinomialLLC/crunch.git
  crunch -file input.dds -out output.dds -DXT5
""")

if __name__ == "__main__":
    main()
