#!/usr/bin/env python3
"""
Comprehensive texture format review for all controller platforms.
Identifies exactly which textures need RGBA32 -> DXT5 conversion.
"""

import os
import struct
from pathlib import Path
from collections import defaultdict

FUSIONFIX_SOURCE = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/GTAIV.EFLC.FusionFix-master/textures/pc/textures/buttons_360.wtd")

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

# Controller prefixes and display names
CONTROLLERS = [
    ("", "Xbox 360 (base)"),
    ("ps3_", "PlayStation 3"),
    ("ps4_", "PlayStation 4"),
    ("ps5_", "PlayStation 5"),
    ("switch_", "Nintendo Switch"),
    ("sc_", "Steam Controller"),
    ("sd_", "Steam Deck"),
    ("xbone_", "Xbox One"),
    ("xsx_", "Xbox Series X"),
]

def analyze_dds(filepath):
    """Analyze a DDS file and return format info."""
    if not filepath.exists():
        return None
    
    with open(filepath, 'rb') as f:
        data = f.read()
    
    if len(data) < 128 or data[:4] != b'DDS ':
        return {"error": "Invalid DDS"}
    
    height = struct.unpack('<I', data[12:16])[0]
    width = struct.unpack('<I', data[16:20])[0]
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
        format_name = f"UNKNOWN"
    
    return {
        "width": width,
        "height": height,
        "format": format_name,
        "mips": mip_count,
        "data_size": pixel_data_size,
        "file_size": file_size,
        "needs_conversion": not (format_name == "DXT5" and pixel_data_size == 4096 and width == 64 and height == 64),
    }

def main():
    output_lines = []
    
    output_lines.append("=" * 100)
    output_lines.append("COMPREHENSIVE TEXTURE FORMAT REVIEW - ALL CONTROLLER PLATFORMS")
    output_lines.append("=" * 100)
    output_lines.append("")
    output_lines.append("Target Format: DXT5, 64x64, 4096 bytes pixel data")
    output_lines.append("")
    
    # Summary data
    platform_summary = {}
    all_needs_conversion = []
    all_ready = []
    all_missing = []
    
    for prefix, controller_name in CONTROLLERS:
        output_lines.append("=" * 100)
        output_lines.append(f"CONTROLLER: {controller_name}")
        output_lines.append(f"Prefix: '{prefix}'" if prefix else "Prefix: (none - base textures)")
        output_lines.append("=" * 100)
        
        needs_conversion = []
        ready = []
        missing = []
        
        for tex in ALL_TEXTURES:
            dds_name = f"{prefix}{tex}.dds"
            path = FUSIONFIX_SOURCE / dds_name
            info = analyze_dds(path)
            
            if info is None:
                missing.append(tex)
            elif info.get("error"):
                missing.append(tex)
            elif info["needs_conversion"]:
                needs_conversion.append((tex, info))
                all_needs_conversion.append((prefix, tex, info))
            else:
                ready.append(tex)
                all_ready.append((prefix, tex))
        
        # Output for this controller
        output_lines.append("")
        output_lines.append(f"✅ READY (DXT5 64x64): {len(ready)}/{len(ALL_TEXTURES)}")
        if ready:
            output_lines.append(f"   {', '.join(ready)}")
        
        output_lines.append("")
        output_lines.append(f"⚠️  NEEDS CONVERSION: {len(needs_conversion)}/{len(ALL_TEXTURES)}")
        if needs_conversion:
            for tex, info in needs_conversion:
                output_lines.append(f"   {tex:25} | {info['width']}x{info['height']} | {info['format']:8} | {info['data_size']:6} bytes")
        
        output_lines.append("")
        output_lines.append(f"❌ MISSING: {len(missing)}/{len(ALL_TEXTURES)}")
        if missing:
            output_lines.append(f"   {', '.join(missing)}")
        
        output_lines.append("")
        
        platform_summary[controller_name] = {
            "ready": len(ready),
            "needs_conversion": len(needs_conversion),
            "missing": len(missing),
            "total": len(ALL_TEXTURES),
        }
        all_missing.extend([(prefix, tex) for tex in missing])
    
    # Grand summary
    output_lines.append("")
    output_lines.append("=" * 100)
    output_lines.append("GRAND SUMMARY")
    output_lines.append("=" * 100)
    output_lines.append("")
    output_lines.append(f"{'Controller':<25} | {'Ready':>6} | {'Convert':>8} | {'Missing':>8} | {'Total':>6}")
    output_lines.append("-" * 70)
    
    total_ready = 0
    total_convert = 0
    total_missing = 0
    
    for controller_name, stats in platform_summary.items():
        output_lines.append(f"{controller_name:<25} | {stats['ready']:>6} | {stats['needs_conversion']:>8} | {stats['missing']:>8} | {stats['total']:>6}")
        total_ready += stats['ready']
        total_convert += stats['needs_conversion']
        total_missing += stats['missing']
    
    output_lines.append("-" * 70)
    output_lines.append(f"{'TOTAL':<25} | {total_ready:>6} | {total_convert:>8} | {total_missing:>8} | {len(ALL_TEXTURES) * len(CONTROLLERS):>6}")
    
    # Detailed conversion list by format
    output_lines.append("")
    output_lines.append("=" * 100)
    output_lines.append("TEXTURES REQUIRING CONVERSION (grouped by source format)")
    output_lines.append("=" * 100)
    
    by_format = defaultdict(list)
    for prefix, tex, info in all_needs_conversion:
        key = f"{info['format']} {info['width']}x{info['height']}"
        by_format[key].append((prefix, tex, info))
    
    for fmt, items in sorted(by_format.items()):
        output_lines.append("")
        output_lines.append(f"--- {fmt} ({len(items)} textures) ---")
        for prefix, tex, info in items:
            full_name = f"{prefix}{tex}.dds"
            output_lines.append(f"   {full_name}")
    
    # Missing textures that will use fallback
    output_lines.append("")
    output_lines.append("=" * 100)
    output_lines.append("MISSING TEXTURES (will use Xbox 360 base fallback)")
    output_lines.append("=" * 100)
    
    by_controller = defaultdict(list)
    for prefix, tex in all_missing:
        controller = next(name for p, name in CONTROLLERS if p == prefix)
        by_controller[controller].append(tex)
    
    for controller, textures in by_controller.items():
        if textures:
            output_lines.append(f"\n{controller}:")
            output_lines.append(f"   {', '.join(textures)}")
    
    # Write output
    output_text = "\n".join(output_lines)
    print(output_text)
    
    # Also save to file
    with open("/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/comprehensive_texture_review.txt", "w") as f:
        f.write(output_text)

if __name__ == "__main__":
    main()
