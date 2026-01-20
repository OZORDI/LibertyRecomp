#!/usr/bin/env python3
"""
Crash analysis script for sub_8289D490 address computation issue.

The crash occurs at:
    v32 = a1[12] + a1[0] + v53
    *(v32 + 5) += a1[44]

Where v53 contains garbage data causing invalid address computation.
"""

def analyze_crash():
    # Known values from LLDB crash dump
    buffer_base = 0xC8559530  # a1[0] - correct buffer base
    buffer_size = 8114        # a1[8] - buffer size
    offset_12 = 0             # a1[12] - data offset (starts at 0)
    
    crash_addr_guest = 0x83559528  # w9 from LLDB - the guest address being accessed
    crash_addr_store = crash_addr_guest - 5  # The computed r11 value (v32)
    
    # Memory base
    g_memory_base = 0x300000000
    
    print("=" * 60)
    print("CRASH ANALYSIS - sub_8289D490 Address Computation")
    print("=" * 60)
    
    print(f"\n[KNOWN VALUES]")
    print(f"  Buffer base (a1[0]):     0x{buffer_base:08X}")
    print(f"  Buffer size (a1[8]):     {buffer_size} bytes (0x{buffer_size:04X})")
    print(f"  Buffer end:              0x{buffer_base + buffer_size:08X}")
    print(f"  Data offset (a1[12]):    {offset_12}")
    print(f"  g_memory.base:           0x{g_memory_base:X}")
    
    print(f"\n[CRASH DETAILS]")
    print(f"  Crash address (host):    0x{g_memory_base + crash_addr_guest:X}")
    print(f"  Guest address (w9):      0x{crash_addr_guest:08X}")
    print(f"  Computed r11 (v32):      0x{crash_addr_store:08X}")
    
    # Calculate v53
    # r11 = a1[0] + a1[12] + v53
    # v53 = r11 - a1[0] - a1[12]
    v53_computed = (crash_addr_store - buffer_base - offset_12) & 0xFFFFFFFF
    v53_signed = v53_computed if v53_computed < 0x80000000 else v53_computed - 0x100000000
    
    print(f"\n[V53 ANALYSIS]")
    print(f"  v53 (computed):          0x{v53_computed:08X}")
    print(f"  v53 (signed):            {v53_signed:,}")
    print(f"  v53 (as offset):         {v53_computed:,} bytes")
    
    # What v53 SHOULD be
    print(f"\n[EXPECTED VALUES]")
    print(f"  Valid v53 range:         0 to {buffer_size} (0x{buffer_size:04X})")
    print(f"  Actual v53:              0x{v53_computed:08X} ({v53_signed:,})")
    
    # Check if v53 looks like ASCII
    print(f"\n[DATA INTERPRETATION]")
    bytes_v53 = [(v53_computed >> (i*8)) & 0xFF for i in range(4)]
    ascii_chars = ''.join(chr(b) if 32 <= b < 127 else '.' for b in bytes_v53)
    print(f"  v53 as bytes (LE):       {' '.join(f'{b:02X}' for b in bytes_v53)}")
    print(f"  v53 as ASCII:            '{ascii_chars}'")
    
    # Check for pattern
    print(f"\n[OFFSET ANALYSIS]")
    diff = buffer_base - crash_addr_store
    print(f"  Distance from buffer:    0x{diff:08X} ({diff:,} bytes)")
    print(f"  ~1.1 GB before buffer - consistent wrap-around pattern")
    
    # Analyze what could produce this value
    print(f"\n[POSSIBLE CAUSES]")
    print(f"  1. Stream position mismatch - reading wrong data as offset")
    print(f"  2. Stream exhausted - reading past EOF returning garbage")
    print(f"  3. Buffer underrun - reading before buffer data starts")
    print(f"  4. Endianness issue - byte order wrong for offset values")
    
    return v53_computed, v53_signed

def analyze_stream_structure():
    """Analyze the FileStream structure layout."""
    print("\n" + "=" * 60)
    print("FILESTREAM STRUCTURE ANALYSIS")
    print("=" * 60)
    
    # Expected PC FileStream structure
    print(f"\n[PC FILESTREAM LAYOUT (my implementation)]")
    print(f"  Offset 0:   storageDevice (0x83132000 = PC_STORAGE_DEVICE_ADDR)")
    print(f"  Offset 4:   fileHandle (kernel handle index)")
    print(f"  Offset 8:   bufferAddr (guest address of read buffer)")
    print(f"  Offset 12:  filePos (current file position)")
    print(f"  Offset 16:  bufferCursor (position within buffer)")
    print(f"  Offset 20:  bytesInBuffer (valid bytes in buffer)")
    print(f"  Total:      24 bytes minimum")
    
    # If stream shows 0x0000FFFF at offset 0
    print(f"\n[CORRUPTION PATTERN]")
    print(f"  Invalid device: 0x0000FFFF")
    print(f"  0xFFFF = 65535 = -1 in 16-bit")
    print(f"  Could be: uninitialized memory, zeroed + partial write, or overwritten")
    
    # Stream addresses from logs
    print(f"\n[STREAM ADDRESSES (from logs)]")
    print(f"  Config #1 stream: 0xC8544EF0 or 0xC8644F70")
    print(f"  Config #2 stream: 0xC8544F30 or 0xC8644FB0")
    print(f"  Difference:       0x40 (64 bytes)")
    print(f"  If stream structure > 64 bytes, streams could OVERLAP!")

def analyze_file_format():
    """Analyze expected audio config file format."""
    print("\n" + "=" * 60)
    print("AUDIO CONFIG FILE FORMAT ANALYSIS")
    print("=" * 60)
    
    # categories.dat15 hex dump analysis
    print(f"\n[categories.dat15 header (from xxd)]")
    print(f"  00000000: 0000 000f 0000 1fb2 0000 0000 0000 0000")
    print(f"  Offset 0:  0x0000000f = 15 (version or count)")
    print(f"  Offset 4:  0x00001fb2 = 8114 (matches buffer size!)")
    
    print(f"\n[STREAM POSITION TRACKING]")
    print(f"  sub_8289DDB0: Opens file, reads 4 bytes header")
    print(f"  sub_8289D1D0: Reads config part 1")
    print(f"  sub_8289DD40: Reads config part 2")
    print(f"  sub_8289D490: Reads string table - CRASHES")
    print(f"  ")
    print(f"  If earlier functions read more than expected,")
    print(f"  sub_8289D490 will start at wrong position!")

if __name__ == "__main__":
    v53_u, v53_s = analyze_crash()
    analyze_stream_structure()
    analyze_file_format()
    
    print("\n" + "=" * 60)
    print("NEXT STEPS")
    print("=" * 60)
    print("""
1. Use LLDB to set breakpoint at sub_827E8420 (stream read)
2. Track stream position before/after each processing function
3. Compare actual bytes read vs expected file format
4. Find where stream position diverges from expected

LLDB Commands:
  b __imp__sub_8289D490
  b sub_827E8420
  watch set expression ctx.r31.u32 + 84  # Watch v53 location
""")
