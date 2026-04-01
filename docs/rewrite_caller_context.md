# sub_821FC1F8 → sub_82478AF8: Caller Context Analysis

Source: `gta4_recomp.5.cpp` lines 989–1177 (sub_821FC1F8), `gta4_recomp.20.cpp` lines 83744–85184 (sub_82478AF8).

## Call Sequence Position

sub_82478AF8 is call #31 (0-indexed: #30) in sub_821FC1F8's linear init chain. It is preceded by:
- Call 29: `sub_8247E4C0(r3=0x82ff5518)` — sets up some string/state at that address
- Call 30: `sub_823C04B0()` — no explicit register setup; r3 is whatever #29 returned

## Registers at Point of Call (lines 1083–1095)

No explicit r3/r4/r5 setup immediately before the call to sub_82478AF8. The PPC ABI permits callee-saved register forwarding from a prior call, but the codegen shows **no `li`, `mr`, `lis/addi` sequence targeting r3/r4/r5** between line 1088 and 1093.

The only setup visible is for call 29 (`sub_8247E4C0`):
```
lis r11, -32001     ; r11 = 0x82ff0000
addi r3, r11, 21784 ; r3  = 0x82ff5518
```

sub_82478AF8 ignores r3/r4/r5 on entry — it immediately calls `sub_826225E0/648/6B0` with no register prep, then `sub_821B3510(r3=176)` (malloc 176 bytes). The function **does not use the incoming r3/r4/r5** as parameters. It is a void init function taking no arguments.

## What sub_82478AF8 Does Internally

1. Allocates 176 bytes via `sub_821B3510(r3=176)` → result in r3.
2. If alloc succeeds (r3 != 0): calls `sub_8294BD68` to initialize the object; stores result in r30. If alloc fails: r30 = 0 (r23).
3. Writes globals (all computed via Python):

| Address | Value | Comment |
|-|-|-|
| 0x831cc944 | r30 | alloc result or 0 (primary obj ptr) |
| 0x82ff541c | flags \|= 1 | bit 0 set unconditionally |
| 0x82ff5418 | r26+r27+128 | cache buffer address |
| 0x82ff5414 | 168 | pool size constant |
| 0x82ff5360 | r30 | net/renderer obj ptr duplicate |

Where r26 = `[0x82fb0700+8]` and r27 = `[0x831d5388+8]` — both loaded from existing global pointers before the flag check.

4. Performs platform/API string detection via four `rexcrt__stricmp` calls comparing a config string against: `"d3d11"`, `"d3d12"`, `"vulkan"`, `"gl"`. Result stored at `r30+152` (renderer type enum: 2=d3d11, 1=d3d12/vulkan, 0=gl/other).

5. Allocates a second object (192 bytes), stores at `0x82ff5360` if successful.

6. Returns **void** — no r3 set before `__restgprlr_23`.

## Return Value Handling

**sub_82478AF8 returns void.** The caller (sub_821FC1F8) does **not** read r3 after the call. Lines 1096–1110 (calls 31–35) contain no `cmpwi`, `cmplwi`, or branch on r3:

```cpp
// line 1095
sub_82478AF8(ctx, base);
// line 1096 — immediately next call, no r3 check
ctx.lr = 0x821FC288;
sub_8225C010(ctx, base);   // call 31
ctx.lr = 0x821FC28C;
sub_82556190(ctx, base);   // call 32
ctx.lr = 0x821FC290;
sub_822F3740(ctx, base);   // call 33
```

There is **no success/failure branch**. Returns 0 or -1 from sub_82478AF8 is not possible — it has no return value.

## Downstream Calls 31–33 and Required State

### Call 31: sub_8225C010 (line 1097)
No register setup. Reads globals initialized by earlier calls in the chain (calls 1–29). Does not depend directly on sub_82478AF8's globals.

### Call 32: sub_82556190 (line 1100)
No register setup. Analysis in `ppc_analysis_sub_82556190.md`.

### Call 33: sub_822F3740 (line 1103)
No register setup. A separate subsystem init; does not reference the 0x82ff5xxx globals written by sub_82478AF8.

### Globals sub_82478AF8 must write for downstream calls 34–47

The globals `0x82ff5360` (net obj ptr) and `0x82ff541c` (flags) are likely read by subsystems initialized later in the chain. The renderer type at `r30+152` feeds into the graphics backend selection. These writes must happen before any rendering subsystem init that follows (calls 34+).

## Summary

sub_82478AF8 is a **void, no-argument** renderer/network-object init function. The caller passes no meaningful registers to it. It allocates two objects, detects the graphics API by string comparison, and writes 5 globals. The caller never checks its return value and execution is unconditional — no fallback path exists if sub_82478AF8 fails internally (alloc failure results in null obj ptr being stored silently).
