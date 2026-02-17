#!/usr/bin/env python3
"""
Compare DDS texture formats between Xbox 360 originals and FusionFix sources.
This helps identify why some textures are corrupted after conversion.
"""

import os
import struct
from pathlib import Path

# Paths
XBOX360_ORIGINAL = Path("/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/original_extracted_textures")
FUSIONFIX_SOURCE = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/GTAIV.EFLC.FusionFix-master/textures/pc/textures/buttons_360.wtd")
PROCESSED_DDS = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/processed_dds")

def analyze_dds(filepath):
    """Analyze a DDS file and return format info."""
    with open(filepath, 'rb') as f:
        data = f.read()
    
    if data[:4] != b'DDS ':
        return {"error": "Not a DDS file"}
    
    height = struct.unpack('<I', data[12:16])[0]
    width = struct.unpack('<I', data[16:20])[0]
    linear_size = struct.unpack('<I', data[20:24])[0]
    mip_count = struct.unpack('<I', data[28:32])[0]
    pf_flags = struct.unpack('<I', data[80:84])[0]
    fourcc = data[84:88]
    rgb_bit_count = struct.unpack('<I', data[88:92])[0]
    
    file_size = len(data)
    pixel_data_size = file_size - 128
    
    # Decode fourcc
    if pf_flags & 0x4:  # DDPF_FOURCC
        fourcc_str = fourcc.decode('ascii', errors='replace')
    else:
        fourcc_str = f"NONE (bits={rgb_bit_count})"
    
    return {
        "width": width,
        "height": height,
        "fourcc": fourcc_str,
        "mip_count": mip_count,
        "pixel_data_size": pixel_data_size,
        "pf_flags": pf_flags,
        "linear_size": linear_size,
    }

def print_comparison(texture_name, prefix=""):
    """Compare a texture across all sources."""
    
    # Xbox 360 original
    xbox_path = XBOX360_ORIGINAL / f"{texture_name}.dds"
    # FusionFix variant
    fusion_path = FUSIONFIX_SOURCE / f"{prefix}{texture_name}.dds"
    # Processed (what we converted)
    proc_path = PROCESSED_DDS / f"{prefix}{texture_name}.dds"
    
    print(f"\n{'='*60}")
    print(f"TEXTURE: {texture_name} (prefix: '{prefix}')")
    print(f"{'='*60}")
    
    if xbox_path.exists():
        info = analyze_dds(xbox_path)
        print(f"XBOX 360 ORIGINAL:")
        print(f"  Dimensions: {info['width']}x{info['height']}")
        print(f"  Format:     {info['fourcc']}")
        print(f"  Mipmaps:    {info['mip_count']}")
        print(f"  Data size:  {info['pixel_data_size']} bytes")
    else:
        print(f"XBOX 360 ORIGINAL: NOT FOUND")
    
    if fusion_path.exists():
        info = analyze_dds(fusion_path)
        print(f"FUSIONFIX SOURCE ({prefix}{texture_name}.dds):")
        print(f"  Dimensions: {info['width']}x{info['height']}")
        print(f"  Format:     {info['fourcc']}")
        print(f"  Mipmaps:    {info['mip_count']}")
        print(f"  Data size:  {info['pixel_data_size']} bytes")
        
        # Flag issues
        if info['width'] != 64 or info['height'] != 64:
            print(f"  ⚠️  ISSUE: Wrong dimensions (need 64x64)")
        if info['fourcc'] != 'DXT5':
            print(f"  ⚠️  ISSUE: Wrong format (need DXT5)")
        if info['mip_count'] > 0:
            print(f"  ⚠️  ISSUE: Has mipmaps (Xbox 360 has none)")
    else:
        print(f"FUSIONFIX SOURCE: NOT FOUND")
    
    if proc_path.exists():
        info = analyze_dds(proc_path)
        print(f"PROCESSED DDS:")
        print(f"  Dimensions: {info['width']}x{info['height']}")
        print(f"  Format:     {info['fourcc']}")
        print(f"  Mipmaps:    {info['mip_count']}")
        print(f"  Data size:  {info['pixel_data_size']} bytes")

def main():
    print("DDS FORMAT COMPARISON REPORT")
    print("=" * 60)
    print(f"Xbox 360 Original: {XBOX360_ORIGINAL}")
    print(f"FusionFix Source:  {FUSIONFIX_SOURCE}")
    print(f"Processed DDS:     {PROCESSED_DDS}")
    
    # Textures to check - focus on the ones that were corrupted
    textures = [
        "lt_butt", "rt_butt", "lb_butt", "rb_butt",  # Triggers/shoulders
        "a_butt", "b_butt", "x_butt", "y_butt",      # Face buttons
        "start_butt", "back_butt",                    # Menu buttons
        "lstick_none", "rstick_none",                 # Sticks
    ]
    
    prefixes = ["", "ps4_", "ps5_", "switch_"]
    
    for tex in textures:
        for prefix in prefixes:
            # Only check base for Xbox, variants for others
            if prefix == "":
                print_comparison(tex, "")
                break  # Just show Xbox 360 original once
    
    # Now show full comparison for PS4 (the problematic one)
    print("\n" + "=" * 80)
    print("DETAILED PS4 COMPARISON (PROBLEMATIC TEXTURES)")
    print("=" * 80)
    
    for tex in textures:
        print_comparison(tex, "ps4_")

if __name__ == "__main__":
    main()
