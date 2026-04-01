# Search: Writes to Guest Address 0x831C2458 via Base+Offset

**Result: NO writes found.** No generated recomp code stores to guest address `0x831C2458`.

## Search Criteria

- Direct literal `0x831C2458` in all 79 generated `.cpp` files
- `lis` loading `0x831C` (upper half) within 20 lines of a store with offset `0x2458` / `9304`
- Any `PPC_STORE` instruction using offset `0x2458` or decimal `9304`
- Variable tracking: assignment from `0x831C0000` followed by store at `+0x2458`
- Alternate lis splits (`0x831D` + negative, `0x831B` + positive) verified impossible (offsets exceed signed 16-bit range)

## Decimal 9304 Hits (4 locations, all false positives)

| File | Line | Base Register Value | Computed Address | Usage |
|-|-|-|-|-|
| gta4_recomp.31.cpp | 9657 | r10 = 0x82030000 | 0x82032458 | `addi r3,r10,9304` -- function arg, not a store |
| gta4_recomp.35.cpp | 90581 | r11 = 0x82040000 | 0x8203DBA8 | `addi r11,r11,-9304` -- stored on stack, wrong base |
| gta4_recomp.38.cpp | 37646 | r11 = 0x82690000 | 0x82692458 | `addi r11,r11,9304` -- stored on stack, wrong base |
| gta4_recomp.58.cpp | 115329 | r11 = 0x831C0000 | **0x831C2458** | Correct address, but used as LOAD source (`lwz r3,0(r11)`), not a store target |

## Closest Match: gta4_recomp.58.cpp:115329 (sub_828C9980)

`r11` is computed as `0x831C2458` at line 115330. However:

- **Line 115337**: `stw r10,-432(r11)` stores to `0x831C22A8` (not 0x831C2458)
- **Line 115353**: `lwz r3,0(r11)` READS from `0x831C2458` (a load, not a store)
- The nearby stores using base `r9 = 0x831C0000` write to `0x831C3DE4` and `0x831C3DDC`

## PPC_STORE with offset 9304/0x2458

Zero results across all 79 generated files. No `PPC_STORE_U8/U16/U32/U64` call uses offset 9304 or 0x2458.

## Conclusion

Address `0x831C2458` is **read** in the generated code (sub_828C9980) but never **written** via base+offset addressing. If this address is being written at runtime, it must be through:
1. An indirect store (pointer loaded from memory, not a lis+addi literal)
2. A memcpy/memset covering the region
3. A DMA or hardware operation
