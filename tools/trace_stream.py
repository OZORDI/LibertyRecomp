#!/usr/bin/env python3
"""
Trace stream position and data flow through audio config processing.
Analyze the categories.dat15 file format to understand expected vs actual reads.
"""

import struct
import os

def read_file_as_big_endian(filepath):
    """Read file and interpret as big-endian (Xbox 360 format)."""
    with open(filepath, 'rb') as f:
        return f.read()

def analyze_categories_dat():
    """Analyze categories.dat15 file structure."""
    filepath = os.path.expanduser(
        "~/Library/Application Support/LibertyRecomp/game/audio/config/categories.dat15"
    )
    
    if not os.path.exists(filepath):
        print(f"File not found: {filepath}")
        return
    
    data = read_file_as_big_endian(filepath)
    file_size = len(data)
    
    print("=" * 60)
    print(f"ANALYZING: categories.dat15 ({file_size} bytes)")
    print("=" * 60)
    
    # Header analysis
    print("\n[HEADER - First 64 bytes]")
    for i in range(0, min(64, file_size), 4):
        val_be = struct.unpack('>I', data[i:i+4])[0]  # Big-endian
        val_le = struct.unpack('<I', data[i:i+4])[0]  # Little-endian
        ascii_repr = ''.join(chr(b) if 32 <= b < 127 else '.' for b in data[i:i+4])
        print(f"  Offset {i:4d} (0x{i:04X}): BE=0x{val_be:08X} LE=0x{val_le:08X} '{ascii_repr}'")
    
    # The first 4 bytes after sub_8289DDB0 reads the header
    # sub_8289D1D0 then reads more data
    # Let's trace what each function should read
    
    print("\n[EXPECTED READ SEQUENCE]")
    pos = 0
    
    # sub_8289DDB0 reads 4 bytes header
    header = struct.unpack('>I', data[pos:pos+4])[0]
    print(f"  sub_8289DDB0: 4 bytes header at pos {pos} -> 0x{header:08X}")
    pos += 4
    
    # sub_8289D1D0: Reads size value and then data
    # From decompiled code: sub_827E8420(a2, &v10, 4) - reads 4 bytes
    size_val = struct.unpack('>I', data[pos:pos+4])[0]
    print(f"  sub_8289D1D0: 4 bytes at pos {pos} -> 0x{size_val:08X} ({size_val})")
    pos += 4
    
    # Then it reads 'size_val' more bytes
    print(f"  sub_8289D1D0: {size_val} bytes of data at pos {pos}")
    pos += size_val
    
    print(f"\n  After sub_8289D1D0: stream pos = {pos}")
    
    # sub_8289DD40: Reads more data
    # Need to trace the decompiled code to understand exact format
    
    print("\n[DATA AT KEY POSITIONS]")
    for check_pos in [0, 4, 8, 12, 16, pos, pos+4, pos+8]:
        if check_pos + 4 <= file_size:
            val = struct.unpack('>I', data[check_pos:check_pos+4])[0]
            print(f"  Pos {check_pos:5d}: 0x{val:08X}")
    
    # Look for patterns that might be v53 (offset values)
    print("\n[SCANNING FOR OFFSET VALUES]")
    print("  Looking for values 0-8114 (valid buffer offsets)...")
    offset_count = 0
    for i in range(0, file_size-4, 4):
        val = struct.unpack('>I', data[i:i+4])[0]
        if 0 < val < 8114:
            if offset_count < 20:
                print(f"    Pos {i:5d}: 0x{val:08X} ({val})")
            offset_count += 1
    print(f"  Found {offset_count} potential offset values")
    
    # Look for the garbage pattern
    print("\n[CHECKING FOR GARBAGE PATTERNS]")
    print("  Looking for 0xBAFFFFF3 or similar patterns...")
    target = 0xBAFFFFF3
    for i in range(0, file_size-4, 4):
        val_be = struct.unpack('>I', data[i:i+4])[0]
        val_le = struct.unpack('<I', data[i:i+4])[0]
        if val_be == target or val_le == target:
            print(f"    FOUND at pos {i}: BE=0x{val_be:08X} LE=0x{val_le:08X}")
    
    # Analyze stream position issue
    print("\n[STREAM POSITION ANALYSIS]")
    print("  File size: 14319 bytes")
    print("  Buffer size (from logs): 8114 bytes")
    print("  ")
    print("  If sub_8289D1D0 reads 4 + 8114 = 8118 bytes,")
    print("  sub_8289DD40 and sub_8289D490 would start at pos 8118+4 = 8122")
    print("  Remaining: 14319 - 8122 = 6197 bytes for DD40 and D490")

def analyze_curves_dat():
    """Analyze curves.dat12 for comparison (this one works)."""
    filepath = os.path.expanduser(
        "~/Library/Application Support/LibertyRecomp/game/audio/config/curves.dat12"
    )
    
    if not os.path.exists(filepath):
        print(f"\nFile not found: {filepath}")
        return
    
    data = read_file_as_big_endian(filepath)
    file_size = len(data)
    
    print("\n" + "=" * 60)
    print(f"COMPARING: curves.dat12 ({file_size} bytes) - THIS ONE WORKS")
    print("=" * 60)
    
    print("\n[HEADER - First 32 bytes]")
    for i in range(0, min(32, file_size), 4):
        val_be = struct.unpack('>I', data[i:i+4])[0]
        print(f"  Offset {i:4d}: 0x{val_be:08X}")

if __name__ == "__main__":
    analyze_categories_dat()
    analyze_curves_dat()
