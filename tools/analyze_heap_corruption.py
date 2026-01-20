#!/usr/bin/env python3
"""
Analyze heap corruption in LibertyRecomp.

The heap structure at 0xA0101000 is being corrupted:
1. Critical section pointer at offset 1408 (0x580) is zeroed
2. Free list at offsets 384-388 may be corrupted causing infinite loop

Key observations:
- Heap base: 0xA0101000 (physical heap region starts at 0xA0000000)
- Critical section at heap+1408 = 0xA0101580
- Free list head at heap+384 = 0xA0101180
- Free list next at heap+388 = 0xA0101184

The corruption happens between heap allocation calls #5 and #22.
Something is zeroing memory in the range 0xA0101180 - 0xA0101610.
"""

def analyze():
    heap_base = 0xA0101000
    
    print("=" * 60)
    print("HEAP STRUCTURE ANALYSIS")
    print("=" * 60)
    
    print(f"\n[HEAP LAYOUT]")
    print(f"  Base:              0x{heap_base:08X}")
    print(f"  Free count:        offset +48   = 0x{heap_base + 48:08X}")
    print(f"  Bitmap:            offset +88   = 0x{heap_base + 88:08X}")
    print(f"  Free list buckets: offset +384  = 0x{heap_base + 384:08X}")
    print(f"  Free list next:    offset +388  = 0x{heap_base + 388:08X}")
    print(f"  Size buckets:      offset +768  = 0x{heap_base + 768:08X}")
    print(f"  Critical section:  offset +1408 = 0x{heap_base + 1408:08X}")
    
    print(f"\n[CORRUPTION RANGE]")
    print(f"  Free list start:   0x{heap_base + 384:08X}")
    print(f"  Critical section:  0x{heap_base + 1408:08X}")
    print(f"  Corruption span:   {1408 - 384} bytes (0x{1408 - 384:X})")
    
    print(f"\n[POSSIBLE CAUSES]")
    print(f"  1. memset zeroing a region that includes heap metadata")
    print(f"  2. Buffer overflow from earlier allocation")
    print(f"  3. Uninitialized pointer writing to heap region")
    print(f"  4. Xbox memory layout difference - static data overlapping heap")
    
    print(f"\n[MEMORY REGIONS]")
    print(f"  PPC code/data:     0x82000000 - 0x831F0000 (now writable)")
    print(f"  Physical heap:     0xA0000000 - 0xFFFFFFFF")
    print(f"  Heap structure:    0xA0101000 - 0xA0101800 (approx)")
    
    print(f"\n[DEBUG APPROACH]")
    print(f"  1. Add memory watchpoint on 0xA0101180 to catch who's zeroing it")
    print(f"  2. Or add wrapper to sub_829A5F10 to log all heap creations")
    print(f"  3. Check if second heap is being created at same address")

if __name__ == "__main__":
    analyze()
