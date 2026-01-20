#!/usr/bin/env python3
"""
Analyze memory layout and function table protection.

The crash is at 0x83942000 which is in the protected function table region.
We need to understand the memory layout and find a fix.
"""

def analyze_layout():
    # From ppc_config.h
    PPC_IMAGE_BASE = 0x82000000
    PPC_IMAGE_SIZE = 0x11F0000  # 17.9 MB
    PPC_CODE_BASE = 0x82120000
    PPC_CODE_SIZE = 0x8F3D5C   # ~9 MB
    
    PPC_IMAGE_END = PPC_IMAGE_BASE + PPC_IMAGE_SIZE  # 0x831F0000
    
    # Function table layout (from memory.cpp)
    kFuncTableOffset = PPC_IMAGE_BASE + PPC_IMAGE_SIZE  # 0x831F0000
    kFuncTableSize = (PPC_CODE_SIZE * 2) + 8  # 0x11E7AC0 (~17.9 MB)
    
    kFuncTableEnd = kFuncTableOffset + kFuncTableSize  # 0x843D7AC0
    
    # Crash details
    crash_addr = 0x83942000
    texture_base = 0x82086550  # r4 from sub_8286BBE8
    
    print("=" * 60)
    print("MEMORY LAYOUT ANALYSIS")
    print("=" * 60)
    
    print(f"\n[PPC IMAGE REGION]")
    print(f"  Base:       0x{PPC_IMAGE_BASE:08X}")
    print(f"  Size:       0x{PPC_IMAGE_SIZE:X} ({PPC_IMAGE_SIZE / (1024*1024):.1f} MB)")
    print(f"  End:        0x{PPC_IMAGE_END:08X}")
    
    print(f"\n[FUNCTION TABLE REGION] (PROTECTED)")
    print(f"  Offset:     0x{kFuncTableOffset:08X}")
    print(f"  Size:       0x{kFuncTableSize:X} ({kFuncTableSize / (1024*1024):.1f} MB)")
    print(f"  End:        0x{kFuncTableEnd:08X}")
    
    print(f"\n[CRASH ANALYSIS]")
    print(f"  Crash addr: 0x{crash_addr:08X}")
    print(f"  Texture base: 0x{texture_base:08X}")
    print(f"  Offset from base: 0x{crash_addr - texture_base:X} ({(crash_addr - texture_base) / (1024*1024):.1f} MB)")
    
    print(f"\n[PROTECTION CONFLICT]")
    print(f"  Func table starts at: 0x{kFuncTableOffset:08X}")
    print(f"  Crash address:        0x{crash_addr:08X}")
    print(f"  Crash is {crash_addr - kFuncTableOffset:,} bytes into protected region")
    
    print(f"\n[HEAP REGION]")
    heap_start = 0xC0000000
    print(f"  Heap typically at:    0x{heap_start:08X}+")
    print(f"  Gap between func table and heap: {(heap_start - kFuncTableEnd) / (1024*1024):.1f} MB")
    
    print(f"\n[ROOT CAUSE]")
    print(f"  The texture system computes buffer addresses by adding offsets")
    print(f"  to a base address in the PPC data region (0x{texture_base:08X}).")
    print(f"  ")
    print(f"  With large textures, the computed address can exceed 0x831F0000")
    print(f"  and land in the protected function table region.")
    print(f"  ")
    print(f"  On Xbox 360, this entire region was writable.")
    print(f"  In our recompilation, we protect it to prevent function pointer corruption.")
    
    print(f"\n[SOLUTIONS]")
    print(f"  1. Move function table to a different location (e.g., after heap)")
    print(f"  2. Increase PPC_IMAGE_SIZE to include texture buffer space")
    print(f"  3. Make the region 0x831F0000-0x8XXXXXXX writable")
    print(f"  4. Redirect texture allocation to heap memory (0xC0000000+)")
    
    # Calculate how much extra space we need
    max_texture_offset = crash_addr - texture_base
    recommended_size = PPC_IMAGE_SIZE + max_texture_offset + 0x1000000  # Add 16 MB margin
    print(f"\n[RECOMMENDED FIX]")
    print(f"  Current PPC_IMAGE_SIZE:    0x{PPC_IMAGE_SIZE:X}")
    print(f"  Max texture offset seen:   0x{max_texture_offset:X}")
    print(f"  Recommended new size:      0x{recommended_size:X} ({recommended_size / (1024*1024):.1f} MB)")
    print(f"  ")
    print(f"  OR: Don't protect the region from 0x831F0000 to 0x90000000")

if __name__ == "__main__":
    analyze_layout()
