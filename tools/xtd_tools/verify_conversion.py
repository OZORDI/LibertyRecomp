#!/usr/bin/env python3
"""Analyze converted texture and compare with Xbox 360 original."""

import struct

# Read converted file
with open('/Users/Ozordi/Downloads/LibertyRecomp/tools/xtd_tools/test_convert/ps4_lt_butt_dxt5.dds', 'rb') as f:
    data = f.read()

# Parse header
magic = data[0:4]
height = struct.unpack('<I', data[12:16])[0]
width = struct.unpack('<I', data[16:20])[0]
pf_flags = struct.unpack('<I', data[80:84])[0]
fourcc = data[84:88]

print("=== CONVERTED ps4_lt_butt_dxt5.dds ===")
print(f"Magic: {magic}")
print(f"Dimensions: {width}x{height}")
print(f"PF Flags: {hex(pf_flags)}")
print(f"FourCC: {fourcc}")
print(f"File size: {len(data)} bytes")
print(f"Pixel data: {len(data) - 128} bytes")

# Compare with Xbox 360 original
with open('/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/original_extracted_textures/lt_butt.dds', 'rb') as f:
    orig = f.read()

print("\n=== XBOX 360 ORIGINAL lt_butt.dds ===")
print(f"Magic: {orig[0:4]}")
height2 = struct.unpack('<I', orig[12:16])[0]
width2 = struct.unpack('<I', orig[16:20])[0]
pf_flags2 = struct.unpack('<I', orig[80:84])[0]
fourcc2 = orig[84:88]
print(f"Dimensions: {width2}x{height2}")
print(f"PF Flags: {hex(pf_flags2)}")
print(f"FourCC: {fourcc2}")
print(f"File size: {len(orig)} bytes")
print(f"Pixel data: {len(orig) - 128} bytes")

print("\n=== MATCH CHECK ===")
print(f"Format matches: {fourcc == fourcc2}")
print(f"Size matches: {len(data) == len(orig)}")
print(f"Dimensions match: {width == width2 and height == height2}")
