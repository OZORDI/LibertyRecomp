#!/usr/bin/env python3
"""
Complete Button Prompt XTD Builder
===================================
Converts all controller variant textures to DXT5 format and packs them into XTD files.

Workflow:
1. For each controller variant (PS3, PS4, PS5, Switch, etc.)
2. Copy/convert textures to match Xbox 360 naming (strip prefix)
3. Convert RGBA32 textures to DXT5 using nvcompress via Wine
4. Pack into XTD using xtd_cli.exe via Wine
5. Output to gta_iv/button_prompts/<controller>/buttons_360.xtd
"""

import os
import sys
import shutil
import subprocess
import struct
from pathlib import Path
from datetime import datetime

# ============================================================================
# CONFIGURATION
# ============================================================================

WINE = "/opt/homebrew/bin/wine"
NVCOMPRESS = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/nvidia-texture-tools-2.1.2-win/bin/nvcompress.exe")
XTD_CLI = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/xtd_cli.exe")

FUSIONFIX_SOURCE = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/GTAIV.EFLC.FusionFix-master/textures/pc/textures/buttons_360.wtd")
XBOX360_FALLBACK = Path("/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/original_extracted_textures")
ORIGINAL_XTD = Path("/Users/ozordi/Library/Application Support/LibertyRecomp/game/xbox360/textures/buttons_360.xtd")

OUTPUT_BASE = Path("/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/button_prompts")
WORK_DIR = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/build_work")

# All texture names (Xbox 360 naming - what the game expects)
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

# Controller variants: (folder_name, prefix, display_name, fallback_prefix)
# fallback_prefix is used when textures are missing (e.g., XSX uses Xbox One)
CONTROLLERS = [
    ("ps3", "ps3_", "PlayStation 3", None),
    ("ps4", "ps4_", "PlayStation 4", None),
    ("ps5", "ps5_", "PlayStation 5", None),
    ("switch", "switch_", "Nintendo Switch", None),
    ("steam_controller", "sc_", "Steam Controller", None),
    ("steam_deck", "sd_", "Steam Deck", None),
    ("xbox_one", "xbone_", "Xbox One", None),
    ("xbox_series_x", "xsx_", "Xbox Series X", "xbone_"),  # Fallback to Xbox One
    ("xbox360", "", "Xbox 360", None),  # Base textures
]

# ============================================================================
# UTILITY FUNCTIONS
# ============================================================================

def log(msg, level="INFO"):
    timestamp = datetime.now().strftime("%H:%M:%S")
    print(f"[{timestamp}] [{level}] {msg}")

def wine_path(unix_path):
    """Convert Unix path to Wine Z: drive path."""
    path_str = str(unix_path).replace('/', '\\')
    return f"Z:{path_str}"

def analyze_dds(filepath):
    """Analyze a DDS file to determine if it needs conversion."""
    if not filepath.exists():
        return None
    
    with open(filepath, 'rb') as f:
        data = f.read(128)
    
    if len(data) < 128 or data[:4] != b'DDS ':
        return {"error": "Invalid DDS"}
    
    height = struct.unpack('<I', data[12:16])[0]
    width = struct.unpack('<I', data[16:20])[0]
    pf_flags = struct.unpack('<I', data[80:84])[0]
    fourcc = data[84:88]
    
    with open(filepath, 'rb') as f:
        file_size = len(f.read())
    pixel_data_size = file_size - 128
    
    # Check if it's DXT5
    is_dxt5 = (pf_flags & 0x4) and fourcc == b'DXT5'
    
    return {
        "width": width,
        "height": height,
        "is_dxt5": is_dxt5,
        "pixel_data_size": pixel_data_size,
        "needs_conversion": not is_dxt5,
    }

def convert_to_dxt5(input_path, output_path):
    """Convert a texture to DXT5 using nvcompress via Wine, then strip mipmaps."""
    input_path = Path(input_path)
    output_path = Path(output_path)
    
    # Step 1: Use nvcompress to convert to DXT5
    temp_converted = output_path.parent / "temp_nvcompress.dds"
    
    cmd = [
        WINE, str(NVCOMPRESS),
        "-bc3",      # BC3 = DXT5
        "-nomips",   # Try no mips (doesn't always work)
        wine_path(str(input_path)),
        wine_path(str(temp_converted))
    ]
    
    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=60,
            cwd=str(NVCOMPRESS.parent)
        )
    except Exception as e:
        log(f"Conversion error: {e}", "ERROR")
        return False
    
    if not temp_converted.exists():
        return False
    
    # Step 2: Read and strip mipmaps to match Xbox 360 format
    with open(temp_converted, 'rb') as f:
        data = bytearray(f.read())
    
    if data[:4] != b'DDS ':
        temp_converted.unlink()
        return False
    
    # Get dimensions
    width = struct.unpack('<I', data[16:20])[0]
    height = struct.unpack('<I', data[12:16])[0]
    
    # Calculate expected DXT5 size (no mipmaps)
    # DXT5: 16 bytes per 4x4 block
    blocks_x = (width + 3) // 4
    blocks_y = (height + 3) // 4
    expected_size = blocks_x * blocks_y * 16  # 4096 bytes for 64x64
    
    # Fix header to remove mipmap info
    struct.pack_into('<I', data, 28, 0)  # mipmap count = 0
    struct.pack_into('<I', data, 20, expected_size)  # linear size
    
    # Fix flags - remove DDSD_MIPMAPCOUNT
    flags = struct.unpack('<I', data[8:12])[0]
    flags = flags & ~0x20000
    struct.pack_into('<I', data, 8, flags)
    
    # Fix caps - remove DDSCAPS_MIPMAP and DDSCAPS_COMPLEX
    caps = struct.unpack('<I', data[108:112])[0]
    caps = caps & ~0x400000 & ~0x8
    struct.pack_into('<I', data, 108, caps)
    
    # Truncate to header + first mip level only
    final_data = bytes(data[:128]) + bytes(data[128:128 + expected_size])
    
    # Write output
    with open(output_path, 'wb') as f:
        f.write(final_data)
    
    # Cleanup
    temp_converted.unlink()
    
    return output_path.exists() and output_path.stat().st_size == 128 + expected_size

def run_xtd_cli(args):
    """Run xtd_cli.exe via Wine."""
    cmd = [WINE, str(XTD_CLI)] + args
    
    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=120,
            cwd=str(XTD_CLI.parent)
        )
        return result.returncode == 0
    except Exception as e:
        log(f"XTD CLI error: {e}", "ERROR")
        return False

# ============================================================================
# MAIN BUILD PROCESS
# ============================================================================

def prepare_textures_for_controller(folder_name, prefix, display_name, fallback_prefix):
    """
    Prepare all textures for a controller variant.
    Returns path to directory with prepared DDS files.
    """
    log(f"Preparing textures for {display_name}...")
    
    # Create work directory for this controller
    controller_work = WORK_DIR / folder_name / "dds"
    controller_work.mkdir(parents=True, exist_ok=True)
    
    stats = {"ready": 0, "converted": 0, "fallback": 0, "failed": 0}
    
    for tex_name in ALL_TEXTURES:
        # Target filename (Xbox 360 naming)
        target_file = controller_work / f"{tex_name}.dds"
        
        # Try to find source texture
        source_file = None
        used_fallback = False
        
        # 1. Try controller-specific texture
        if prefix:
            candidate = FUSIONFIX_SOURCE / f"{prefix}{tex_name}.dds"
            if candidate.exists():
                source_file = candidate
        
        # 2. Try fallback prefix (e.g., Xbox Series X -> Xbox One)
        if source_file is None and fallback_prefix:
            candidate = FUSIONFIX_SOURCE / f"{fallback_prefix}{tex_name}.dds"
            if candidate.exists():
                source_file = candidate
                used_fallback = True
        
        # 3. Try base Xbox 360 texture from FusionFix
        if source_file is None and prefix:  # Don't do this for base Xbox 360
            candidate = FUSIONFIX_SOURCE / f"{tex_name}.dds"
            if candidate.exists():
                source_file = candidate
                used_fallback = True
        
        # 4. Ultimate fallback: original Xbox 360 extracted texture
        if source_file is None:
            candidate = XBOX360_FALLBACK / f"{tex_name}.dds"
            if candidate.exists():
                source_file = candidate
                used_fallback = True
        
        if source_file is None:
            log(f"  ✗ {tex_name}: No source found!", "ERROR")
            stats["failed"] += 1
            continue
        
        # Analyze source
        info = analyze_dds(source_file)
        if info is None or info.get("error"):
            log(f"  ✗ {tex_name}: Invalid DDS", "ERROR")
            stats["failed"] += 1
            continue
        
        # Convert or copy
        if info["needs_conversion"]:
            # Need to convert RGBA32 -> DXT5
            success = convert_to_dxt5(source_file, target_file)
            if success:
                stats["converted"] += 1
                status = "converted"
            else:
                log(f"  ✗ {tex_name}: Conversion failed", "ERROR")
                stats["failed"] += 1
                continue
        else:
            # Already DXT5, just copy
            shutil.copy(source_file, target_file)
            stats["ready"] += 1
            status = "copied"
        
        if used_fallback:
            stats["fallback"] += 1
    
    log(f"  Prepared: {stats['ready']} copied, {stats['converted']} converted, "
        f"{stats['fallback']} used fallback, {stats['failed']} failed")
    
    return controller_work, stats

def build_xtd_for_controller(folder_name, display_name, dds_dir):
    """Build XTD file for a controller variant."""
    log(f"Building XTD for {display_name}...")
    
    # Output paths
    output_dir = OUTPUT_BASE / folder_name
    output_dir.mkdir(parents=True, exist_ok=True)
    output_xtd = output_dir / "buttons_360.xtd"
    
    # Work with a copy of the original XTD
    work_xtd = WORK_DIR / folder_name / "working.xtd"
    work_xtd.parent.mkdir(parents=True, exist_ok=True)
    
    if not ORIGINAL_XTD.exists():
        log(f"  ✗ Original XTD not found: {ORIGINAL_XTD}", "ERROR")
        return False
    
    shutil.copy(ORIGINAL_XTD, work_xtd)
    
    # Replace each texture
    success_count = 0
    for tex_name in ALL_TEXTURES:
        dds_file = dds_dir / f"{tex_name}.dds"
        if not dds_file.exists():
            log(f"  ⚠ Missing: {tex_name}.dds", "WARN")
            continue
        
        temp_xtd = WORK_DIR / folder_name / "temp.xtd"
        
        args = [
            "replace",
            wine_path(str(work_xtd)),
            wine_path(str(temp_xtd)),
            tex_name,
            wine_path(str(dds_file))
        ]
        
        if run_xtd_cli(args) and temp_xtd.exists():
            shutil.move(temp_xtd, work_xtd)
            success_count += 1
        else:
            log(f"  ⚠ Failed to replace: {tex_name}", "WARN")
    
    # Move final XTD to output
    if work_xtd.exists():
        shutil.move(work_xtd, output_xtd)
        log(f"  ✓ Built: {output_xtd} ({success_count}/{len(ALL_TEXTURES)} textures)")
        return True
    else:
        log(f"  ✗ Failed to build XTD", "ERROR")
        return False

def main():
    log("=" * 70)
    log("BUTTON PROMPT XTD BUILDER")
    log("=" * 70)
    
    # Check prerequisites
    if not WINE or not Path(WINE).exists():
        log("Wine not found! Install with: brew install wine", "ERROR")
        sys.exit(1)
    
    if not NVCOMPRESS.exists():
        log(f"nvcompress.exe not found at {NVCOMPRESS}", "ERROR")
        sys.exit(1)
    
    if not XTD_CLI.exists():
        log(f"xtd_cli.exe not found at {XTD_CLI}", "ERROR")
        sys.exit(1)
    
    if not ORIGINAL_XTD.exists():
        log(f"Original XTD not found at {ORIGINAL_XTD}", "ERROR")
        log("Please ensure the game is installed and textures are extracted.", "ERROR")
        sys.exit(1)
    
    # Clean work directory
    if WORK_DIR.exists():
        shutil.rmtree(WORK_DIR)
    WORK_DIR.mkdir(parents=True)
    
    # Process each controller
    results = {}
    for folder_name, prefix, display_name, fallback_prefix in CONTROLLERS:
        log("")
        log("=" * 70)
        log(f"PROCESSING: {display_name}")
        log("=" * 70)
        
        # Step 1: Prepare textures (convert if needed)
        dds_dir, stats = prepare_textures_for_controller(
            folder_name, prefix, display_name, fallback_prefix
        )
        
        # Step 2: Build XTD
        success = build_xtd_for_controller(folder_name, display_name, dds_dir)
        
        results[display_name] = {
            "success": success,
            "stats": stats
        }
    
    # Summary
    log("")
    log("=" * 70)
    log("BUILD SUMMARY")
    log("=" * 70)
    
    for display_name, result in results.items():
        status = "✓" if result["success"] else "✗"
        stats = result["stats"]
        log(f"{status} {display_name}: {stats['ready']} ready, {stats['converted']} converted, "
            f"{stats['fallback']} fallback, {stats['failed']} failed")
    
    # List output files
    log("")
    log("Output XTD files:")
    for folder_name, _, display_name, _ in CONTROLLERS:
        xtd_path = OUTPUT_BASE / folder_name / "buttons_360.xtd"
        if xtd_path.exists():
            size = xtd_path.stat().st_size
            log(f"  {xtd_path} ({size:,} bytes)")

if __name__ == "__main__":
    main()
