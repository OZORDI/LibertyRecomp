# D3D Init Functions and 0x831C2458: No Write Path Found

## Analysis

Searched three GPU/D3D initialization functions and all their callees for any reference to `0x831C2458` or the `0x831C2400-0x831C2500` range.

## Functions Analyzed

| Function | Purpose | Lines | Callees | Refs to 0x831C24xx | Stores to 0x831C24xx |
|-|-|-|-|-|-|
| sub_82A50890 | GPU CreateDevice | 304 | 15 | 0 | 0 |
| sub_82A49D08 | GPU ring buffer init | 605 | 8 | 0 | 0 |
| sub_82A416B8 | D3D device setup | 132 | 7 | 0 | 0 |

## sub_82A50890 Callees (15 functions checked)

sub_82A507A8, sub_82A49D08, sub_82A4F560, sub_82A42020, sub_82A4F7E0, sub_82A503C8, sub_82A50160, sub_821B3700, sub_82A499B8, sub_82A153C0, sub_82A4DAB0, sub_82A52E38, sub_82A3BAA8, sub_82A4F9B0, sub_82A53058

**None of these callees reference the 0x831C2400-0x831C2500 range.**

## Search Criteria

For each function and its callees, searched for:

1. Direct literal `0x831C2458` -- **0 hits**
2. Offset `0x2458` -- **0 hits**
3. Decimal `9304` -- **0 hits**
4. Any hex literal in `[0x831C2400, 0x831C2500)` -- **0 hits**
5. `PPC_STORE` to any address in that range -- **0 hits**
6. Base+offset pairs (`0x831C0000 + 0x2458`, `0x831C2400 + 0x58`, etc.) -- **0 hits**

## Global Search

Searched all 71+ generated `.cpp` files for hex literal `831C2458` -- **0 hits**.
Searched all generated files for any hex literal in `[0x831C2400, 0x831C2500)` -- **0 hits**.

## Conclusion

The D3D device init code path (sub_82A50890 and all its callees) does **not** write to `0x831C2458`. Neither does any other function in the entire generated recomp codebase.

This address is written exclusively by the Xbox 360 D3D kernel during device creation -- code that was never part of the game binary and therefore never appears in the recompiled output. On the real hardware, the kernel's D3D driver populates this scene list slot as part of GPU device setup, which is an OS-level operation with no user-mode equivalent in the game code.

The existing hooks in `imports.cpp` (lines 1084-1119) wrap sub_82A50890, sub_82A416B8, and sub_82A49D08 with diagnostic logging but do not write to `0x831C2458`.

## Implications

To populate `0x831C2458`, a hook must explicitly write a valid scene list pointer into this address. This write cannot be delegated to any existing recompiled function because no such function exists. The write must happen in a host-side hook after scene object creation completes.
