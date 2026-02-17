#!/usr/bin/env python3
"""
Check all FusionFix source texture dimensions.
Verify if any textures are NOT 64x64.
"""

import os
import struct
from pathlib import Path
from collections import defaultdict

FUSIONFIX_SOURCE = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/GTAIV.EFLC.FusionFix-master/textures/pc/textures/buttons_360.wtd")

def analyze_dds(filepath):
    """Get dimensions and format of a DDS file."""
    with open(filepath, 'rb') as f:
        data = f.read(128)
    
    if data[:4] != b'DDS ':
        return None
    
    height = struct.unpack('<I', data[12:16])[0]
    width = struct.unpack('<I', data[16:20])[0]
    pf_flags = struct.unpack('<I', data[80:84])[0]
    fourcc = data[84:88]
    
    with open(filepath, 'rb') as f:
        file_size = len(f.read())
    
    if pf_flags & 0x4:
        fmt = fourcc.decode('ascii', errors='replace')
    else:
        fmt = "RGBA32"
    
    return {
        "width": width,
        "height": height,
        "format": fmt,
        "file_size": file_size,
    }

def main():
    print("=" * 80)
    print("TEXTURE DIMENSION CHECK - ALL FUSIONFIX SOURCE TEXTURES")
    print("=" * 80)
    
    by_dimension = defaultdict(list)
    total = 0
    
    for dds_file in sorted(FUSIONFIX_SOURCE.glob("*.dds")):
        # Skip controller images (these are large reference images, not button prompts)
        if "controller" in dds_file.name.lower():
            continue
        # Skip key_ textures (keyboard)
        if dds_file.name.startswith("key_"):
            continue
        # Skip arrow_*_pc textures (keyboard arrows)
        if "_pc.dds" in dds_file.name:
            continue
            
        info = analyze_dds(dds_file)
        if info:
            total += 1
            dim = f"{info['width']}x{info['height']}"
            by_dimension[dim].append((dds_file.name, info))
    
    print(f"\nTotal button textures checked: {total}\n")
    
    # Report by dimension
    for dim in sorted(by_dimension.keys(), key=lambda x: int(x.split('x')[0]), reverse=True):
        textures = by_dimension[dim]
        is_correct = dim == "64x64"
        status = "✓" if is_correct else "⚠️ WRONG SIZE"
        
        print(f"\n{'=' * 60}")
        print(f"{dim} - {len(textures)} textures {status}")
        print("=" * 60)
        
        if not is_correct or len(textures) < 50:  # Show details for wrong sizes or small groups
            for name, info in textures:
                print(f"  {name:40} | {info['format']:8} | {info['file_size']:6} bytes")
        else:
            # Just show count for correct 64x64 textures
            formats = defaultdict(int)
            for name, info in textures:
                formats[info['format']] += 1
            print(f"  Formats: {dict(formats)}")
            # Show first few as examples
            print(f"  Examples: {', '.join([t[0] for t in textures[:5]])}...")
    
    # Summary
    print("\n" + "=" * 80)
    print("SUMMARY")
    print("=" * 80)
    
    correct_64x64 = len(by_dimension.get("64x64", []))
    wrong_size = total - correct_64x64
    
    print(f"✓ 64x64 textures: {correct_64x64}")
    print(f"⚠️ Other sizes:    {wrong_size}")
    
    if wrong_size > 0:
        print("\n⚠️ WARNING: Some textures are NOT 64x64!")
        print("These will need to be resized before conversion.")
        for dim, textures in by_dimension.items():
            if dim != "64x64":
                print(f"\n  {dim}:")
                for name, info in textures:
                    print(f"    - {name}")

if __name__ == "__main__":
    main()
