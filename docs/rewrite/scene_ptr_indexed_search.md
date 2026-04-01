# Scene Pointer Indexed Store Search (0x831C2458)

## Offset Computations

- `0x831C2458 - 0x831C2210 = 0x248 (584 decimal)`
- `0x831C2458 - 0x831C2258 = 0x200 (512 decimal)`

## Key Finding: 0x831C2458 is BEYOND Generated Code Range

The generated recomp code covers addresses up to **0x82A85F18**. The target address **0x831C2458** is in the BSS/data segment at ~0x831Cxxxx, which is **far outside** the recompiled code range. There are zero direct references to 0x831C2210, 0x831C2258, or 0x831C2458 in the generated code.

- Total occurrences of "831C" in generated code: **61** (all are coincidental matches in code addresses like `loc_82831C2C`, branch targets like `0x821831cc`, or return addresses like `ctx.lr = 0x82831C08` -- none reference the 0x831C2xxx data region)
- **0 direct stores** to 0x831C2458
- **0 references** to 0x831C2210 or 0x831C2258 as data addresses
- **0 indexed stores** (reg+reg stwx pattern)
- **0 double-dereference** patterns through 0x831C

## Offset +0x248 (584) Stores: 33 hits (none provably target 0x831C2210)

All 33 `PPC_STORE` hits at offset +584 use register bases (r1/r31/r11/etc.) whose runtime values cannot be statically determined from the generated code alone. Many are stack stores (r1-based). None have a nearby `lis`/`addi` loading 0x831C2210 into the base register.

## Offset +0x200 (512) Stores: 92 hits (none provably target 0x831C2258)

Same situation -- register-indirect stores at offset +512, mostly stack-frame (r1-based) or object-field stores. No static evidence ties any to the 0x831C2258 base.

## Conclusion

The address 0x831C2458 is **not written by any statically identifiable pattern** in the generated recompiled code. The write must occur through one of:

1. **Runtime-computed register values** -- a function loads the table base (0x831C2210 or similar) into a register via `lis`/`ori` which the codegen folds into decimal constants like `-2113863680` (0x82090000), then adds an offset. These cannot be traced statically without symbolic execution.
2. **Import/kernel functions** -- the address may be written by an imported Xbox kernel function or a hooked CRT function that is not part of the recompiled code.
3. **DMA/hardware write** -- GPU or system-level initialization may write to BSS globals before the recompiled code runs.

To find the actual writer, a **runtime watchpoint** (memory write breakpoint on 0x831C2458) would be the most effective approach.
