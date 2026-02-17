#!/usr/bin/env python3
"""
Convert RGBA32 DDS to DXT5 DDS without mipmaps.
Uses nvcompress via Wine, then strips any mipmaps to match Xbox 360 format.
"""

import os
import sys
import struct
import shutil
import subprocess
from pathlib import Path

WINE = "/opt/homebrew/bin/wine"
NVCOMPRESS = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/nvidia-texture-tools-2.1.2-win/bin/nvcompress.exe")

def wine_path(unix_path):
    """Convert Unix path to Wine Z: drive path."""
    path_str = str(unix_path).replace('/', '\\')
    return f"Z:{path_str}"

def convert_rgba_to_dxt5(input_path, output_path, work_dir):
    """
    Convert RGBA32 DDS to DXT5 with no mipmaps.
    Returns True on success.
    """
    input_path = Path(input_path)
    output_path = Path(output_path)
    work_dir = Path(work_dir)
    work_dir.mkdir(parents=True, exist_ok=True)
    
    # Step 1: Use nvcompress to convert to DXT5
    temp_converted = work_dir / "temp_converted.dds"
    
    cmd = [
        WINE, str(NVCOMPRESS),
        "-bc3",      # BC3 = DXT5
        "-nomips",   # Try no mips (may not work)
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
        print(f"Error running nvcompress: {e}")
        return False
    
    if not temp_converted.exists():
        print("nvcompress failed to create output")
        return False
    
    # Step 2: Read the converted file and strip mipmaps
    with open(temp_converted, 'rb') as f:
        data = bytearray(f.read())
    
    # Check it's a valid DDS
    if data[:4] != b'DDS ':
        print("Invalid DDS output from nvcompress")
        return False
    
    # Get dimensions
    width = struct.unpack('<I', data[16:20])[0]
    height = struct.unpack('<I', data[12:16])[0]
    
    # Calculate expected DXT5 size (no mipmaps)
    # DXT5: 16 bytes per 4x4 block
    blocks_x = (width + 3) // 4
    blocks_y = (height + 3) // 4
    expected_size = blocks_x * blocks_y * 16  # 4096 bytes for 64x64
    
    # Fix the header
    # Set mipmap count to 0
    struct.pack_into('<I', data, 28, 0)
    
    # Set linear size to expected size
    struct.pack_into('<I', data, 20, expected_size)
    
    # Fix flags - remove DDSD_MIPMAPCOUNT if present
    flags = struct.unpack('<I', data[8:12])[0]
    flags = flags & ~0x20000  # Remove DDSD_MIPMAPCOUNT
    struct.pack_into('<I', data, 8, flags)
    
    # Fix caps - remove DDSCAPS_MIPMAP and DDSCAPS_COMPLEX
    caps = struct.unpack('<I', data[108:112])[0]
    caps = caps & ~0x400000  # Remove DDSCAPS_MIPMAP
    caps = caps & ~0x8       # Remove DDSCAPS_COMPLEX
    struct.pack_into('<I', data, 108, caps)
    
    # Truncate to header + first mip level only
    final_data = bytes(data[:128]) + bytes(data[128:128 + expected_size])
    
    # Write output
    with open(output_path, 'wb') as f:
        f.write(final_data)
    
    # Cleanup
    temp_converted.unlink()
    
    # Verify
    final_size = len(final_data)
    expected_total = 128 + expected_size
    
    if final_size == expected_total:
        return True
    else:
        print(f"Size mismatch: got {final_size}, expected {expected_total}")
        return False

def main():
    if len(sys.argv) < 3:
        print("Usage: convert_to_dxt5.py <input.dds> <output.dds>")
        sys.exit(1)
    
    input_path = Path(sys.argv[1])
    output_path = Path(sys.argv[2])
    work_dir = Path("/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/convert_work")
    
    if not input_path.exists():
        print(f"Input file not found: {input_path}")
        sys.exit(1)
    
    success = convert_rgba_to_dxt5(input_path, output_path, work_dir)
    
    if success:
        print(f"✓ Converted: {output_path}")
        # Verify
        with open(output_path, 'rb') as f:
            data = f.read()
        width = struct.unpack('<I', data[16:20])[0]
        height = struct.unpack('<I', data[12:16])[0]
        fourcc = data[84:88].decode('ascii')
        mips = struct.unpack('<I', data[28:32])[0]
        pixel_size = len(data) - 128
        print(f"  {width}x{height} | {fourcc} | {pixel_size} bytes | {mips} mips")
    else:
        print(f"✗ Conversion failed")
        sys.exit(1)

if __name__ == "__main__":
    main()
