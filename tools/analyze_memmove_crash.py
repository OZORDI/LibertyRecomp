#!/usr/bin/env python3
"""
Analyze memmove crash in sub_829E4970.

Crash details:
- Exception: EXC_BAD_ACCESS (SIGBUS) / KERN_PROTECTION_FAILURE
- Crash address: 0x383afa000
- VM Region: 0x3831f0000-0x3843d8000 [17.9M] r--/rwx (READ-ONLY!)

The code is trying to WRITE to a read-only memory region.
"""

def analyze_crash():
    # Known values from crash report
    crash_host_addr = 0x383afa000
    g_memory_base = 0x300000000
    
    # Calculate guest address
    guest_addr = crash_host_addr - g_memory_base
    
    print("=" * 60)
    print("MEMMOVE CRASH ANALYSIS")
    print("=" * 60)
    
    print(f"\n[CRASH DETAILS]")
    print(f"  Host address:      0x{crash_host_addr:X}")
    print(f"  g_memory.base:     0x{g_memory_base:X}")
    print(f"  Guest address:     0x{guest_addr:08X}")
    
    # VM Region analysis
    print(f"\n[VM REGION ANALYSIS]")
    print(f"  Region start:      0x3831f0000")
    print(f"  Region end:        0x3843d8000")
    print(f"  Region size:       17.9 MB")
    print(f"  Protection:        r-- (READ-ONLY)")
    print(f"  Max protection:    rwx")
    print(f"")
    print(f"  The crash address is in a READ-ONLY region!")
    print(f"  memmove is trying to WRITE to this address.")
    
    # Guest address range check
    print(f"\n[GUEST ADDRESS ANALYSIS]")
    print(f"  Guest address:     0x{guest_addr:08X}")
    
    # Check if it's in the PPC data region (0x82000000 - 0x831F0000)
    # This region is 17.9 MB which matches the crash region!
    ppc_data_start = 0x82000000
    ppc_data_end = 0x831F0000
    ppc_data_size = ppc_data_end - ppc_data_start
    
    print(f"  PPC data region:   0x{ppc_data_start:08X} - 0x{ppc_data_end:08X}")
    print(f"  PPC data size:     {ppc_data_size / (1024*1024):.1f} MB")
    
    if ppc_data_start <= guest_addr < ppc_data_end:
        print(f"  STATUS: Guest address IS in PPC data region")
        offset_in_region = guest_addr - ppc_data_start
        print(f"  Offset in region:  0x{offset_in_region:X} ({offset_in_region:,} bytes)")
    else:
        print(f"  STATUS: Guest address is OUTSIDE PPC data region!")
        if guest_addr >= ppc_data_end:
            beyond = guest_addr - ppc_data_end
            print(f"  Beyond PPC end by: 0x{beyond:X} ({beyond:,} bytes)")
    
    # Heap region check
    print(f"\n[HEAP REGION CHECK]")
    heap_start = 0xC0000000  # Typical heap start
    print(f"  Heap typically at: 0x{heap_start:08X}+")
    if guest_addr >= heap_start:
        print(f"  Guest address could be in heap region")
    else:
        print(f"  Guest address is BEFORE heap region")
    
    # What's at 0x83afa000?
    print(f"\n[ADDRESS INTERPRETATION]")
    print(f"  0x83afa000 is ~1.9 MB past the end of PPC data region (0x831F0000)")
    print(f"  This suggests the code is computing an address that goes")
    print(f"  beyond the valid guest memory range.")
    print(f"")
    print(f"  Possible causes:")
    print(f"  1. Incorrect buffer offset calculation")
    print(f"  2. Uninitialized or corrupted pointer")
    print(f"  3. Buffer size mismatch (source thinks buffer is larger)")
    print(f"  4. Guest address computed from invalid base pointer")

def analyze_call_stack():
    """Analyze the crash call stack."""
    print("\n" + "=" * 60)
    print("CALL STACK ANALYSIS")
    print("=" * 60)
    
    stack = [
        ("_platform_memmove", "System memmove - doing the actual copy"),
        ("sub_829E4970", "IMMEDIATE CALLER - computes addresses for memmove"),
        ("sub_829E5110", "Parent function"),
        ("sub_8286ABF0", "GPU/texture related"),
        ("sub_8286BBE8", "GPU buffer setup (known from previous analysis)"),
        ("sub_8286CD48", "GPU command processing?"),
        ("sub_822E42D8", "Higher-level game code"),
        ("sub_82221410", "Game initialization"),
        ("sub_82120FB8", "Loading sequence"),
    ]
    
    print("\n[CALL STACK]")
    for i, (func, desc) in enumerate(stack):
        print(f"  {i}: {func:20s} - {desc}")
    
    print("\n[FOCUS POINTS]")
    print("  1. sub_829E4970 computes the memmove addresses")
    print("  2. Need to trace r3 (dest), r4 (src), r5 (size) at memmove call")
    print("  3. The dest address 0x83afa000 is invalid (read-only region)")

if __name__ == "__main__":
    analyze_crash()
    analyze_call_stack()
    
    print("\n" + "=" * 60)
    print("NEXT STEPS")
    print("=" * 60)
    print("""
1. Use LLDB to break at sub_829E4970 and trace address computation
2. Read decompiled code for sub_829E4970 to understand the memmove call
3. Find where the invalid guest address 0x83afa000 comes from
4. Check if PPC data region should be writable (rwx not r--)

LLDB Commands:
  lldb ./out/build/macos-release/...
  b sub_829E4970
  run
  # When it hits breakpoint, step to memmove call
  # Check registers: x0=dest, x1=src, x2=size
""")
