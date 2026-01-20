#!/usr/bin/env python3
"""
Analyze memmove crash #2 - updated with LLDB data.

Crash details from LLDB:
- x0 (dest) = 0x383942000
- x1 (src)  = 0x3cbfb7ab8
- x19 (base) = 0x300000000

sub_8286BBE8 #4 ENTER r3=0xCBFB82B0 r4=0x82086550
"""

def analyze_crash():
    # LLDB values
    dest_host = 0x383942000
    src_host = 0x3cbfb7ab8
    g_memory_base = 0x300000000
    
    # Calculate guest addresses
    dest_guest = dest_host - g_memory_base
    src_guest = src_host - g_memory_base
    
    print("=" * 60)
    print("MEMMOVE CRASH ANALYSIS - LLDB DATA")
    print("=" * 60)
    
    print(f"\n[CRASH REGISTERS]")
    print(f"  x0 (dest host):    0x{dest_host:X}")
    print(f"  x1 (src host):     0x{src_host:X}")
    print(f"  x19 (base):        0x{g_memory_base:X}")
    
    print(f"\n[GUEST ADDRESSES]")
    print(f"  Dest guest:        0x{dest_guest:08X}")
    print(f"  Src guest:         0x{src_guest:08X}")
    
    # Check regions
    print(f"\n[MEMORY REGION CHECK]")
    
    # PPC code/data region (read-only)
    ppc_start = 0x82000000
    ppc_end = 0x831F0000
    
    print(f"  PPC region:        0x{ppc_start:08X} - 0x{ppc_end:08X} (READ-ONLY)")
    
    if ppc_start <= dest_guest < ppc_end:
        print(f"  ** DEST IS IN PPC REGION (READ-ONLY)! **")
        offset = dest_guest - ppc_start
        print(f"     Offset in region: 0x{offset:X} ({offset:,} bytes)")
    elif dest_guest >= ppc_end and dest_guest < 0x90000000:
        print(f"  ** DEST IS PAST PPC END BUT BEFORE HEAP! **")
        print(f"     This region may not be mapped as writable")
        beyond = dest_guest - ppc_end
        print(f"     Beyond PPC end by: 0x{beyond:X} ({beyond:,} bytes)")
    
    # Heap region
    heap_start = 0xC0000000
    print(f"  Heap region:       0x{heap_start:08X}+ (WRITABLE)")
    
    if src_guest >= heap_start:
        print(f"  Source is in heap region (valid)")
    
    # Analyze the call parameters
    print(f"\n[sub_8286BBE8 PARAMETERS]")
    r3 = 0xCBFB82B0
    r4 = 0x82086550
    print(f"  r3 (texture obj):  0x{r3:08X}")
    print(f"  r4 (buffer ptr):   0x{r4:08X}")
    
    if ppc_start <= r4 < ppc_end:
        print(f"  ** r4 IS IN PPC REGION (READ-ONLY)! **")
        print(f"     This is the SOURCE of the bad address!")
        print(f"     The code is using PPC code/data as a destination buffer!")
    
    print(f"\n[ROOT CAUSE ANALYSIS]")
    print(f"  The destination address 0x{dest_guest:08X} is derived from")
    print(f"  a pointer that points into the PPC executable region.")
    print(f"  ")
    print(f"  On Xbox 360, this region was writable (XEX format).")
    print(f"  In our recompilation, it's read-only (protection mismatch).")
    print(f"  ")
    print(f"  Solutions:")
    print(f"  1. Make the PPC region writable (security risk)")
    print(f"  2. Redirect writes to this region to a shadow buffer")
    print(f"  3. Hook sub_8286BBE8 to use heap memory instead")

if __name__ == "__main__":
    analyze_crash()
