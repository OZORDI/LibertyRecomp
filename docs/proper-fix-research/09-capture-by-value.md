# Agent 09: Capture-By-Reference vs Capture-By-Value Hypothesis

## Verdict: HYPOTHESIS REFUTED

The claim that the DC color UAF stems from "capture-by-reference where capture-by-value is required" does NOT match the actual guest code. The guest ALREADY performs capture-by-value INSIDE `sub_8227F2E8` before any long-lived host GPU state is produced. The 0xFFE1E1E1 poison signature observed at the READ site (`sub_828BF270`) is NOT the color word — it is an UNRELATED singleton manager pointer at guest address 0x831C22A4 that the batcher dereferences to obtain its current vertex pool.

## Evidence Chain

### 1. Caller pattern (sub_8229ECxx, gta4_recomp.12.cpp:5420)

```
li   r31, -1              ; color = 0xFFFFFFFF (opaque white)
addi r4,  r1, 212         ; r4 = &stack[212]
stw  r31, 212(r1)          ; stack[212] = 0xFFFFFFFF
bl   sub_8227F5B8          ; pass r4 = pointer to caller's stack slot
```

The color DOES arrive by reference — but the pointee is a CALLER STACK LOCAL (`r1+212`), NOT a DC-allocated heap object. RAGE's draw-pool 0xFFE1E1E1 poison never touches this slot; it cannot, because it is stack memory owned exclusively by the current thread's call frame.

### 2. sub_8227F5B8 stashes the pointer (gta4_recomp.10.cpp)

```
stwu r1, -112(r1)
stw  r4,  92(r1)          ; save the incoming pointer at stack[-20 from original r1]
bl   sub_8227F2E8
```

### 3. sub_8227F2E8 DEREFERENCES the pointer ONCE, early

```
stwu r1, -208(r1)
lwz  r30, 300(r1)         ; r30 = reload saved r4 (= caller's &stack[212])
lwz  r9,  0(r30)          ; r9 = *r30 = 0xFFFFFFFF  [VALUE captured]
;  ... 4 calls to sub_828C2290 — all receive r9 by register
```

The dereference happens SYNCHRONOUSLY inside the same call frame, before any host GPU queue ever sees this frame. Verified via Python that `300 - 208 = 92`, which matches exactly the `stw r4, 92(r1)` in `sub_8227F5B8`.

### 4. sub_828C2290 stores the VALUE into the host vertex pool

```
loc_828C22C0:
  stfs f1..f6, 0..20(r11)  ; xyz + uv floats
  stw  r9,     24(r11)     ; COLOR WORD — stored BY VALUE
  stfs f7..f8, 28..32(r11)
```

`r11` is the game-managed vertex pool slot (the `mgr` at 0x831C22A4 + offset). Once the 32 bytes are written, the caller's stack contents are irrelevant.

## Hardware vs Host Execution Model

Task 1 asks whether Xbox 360 RAGE commits vertices immediately or queues them. The code above demonstrates the answer is the same on both:

- Both Xbox and host perform capture-by-value at the `sub_828C2290` store.
- The vertex pool (managed at guest address 0x831C22A4) is a GAME data structure that batches immediate-mode vertices until `sub_828BF270` → `sub_82A3DF50` flushes. This flush logic is IDENTICAL whether running on Xbox or on the host; the recomp does not alter it.
- No host-side asynchronous command buffer sits between the guest vertex emit and the guest flush. RexGlue's command_processor consumes PM4 packets from a ring buffer written by the guest MUCH later (when the game actually calls into the GPU via sub_82A3DAB0 equivalents).

## Task 2 — Host enqueue and capture semantics

`glue/rexglue-sdk-main/src/graphics/d3d12/shared_memory.cpp:368 UploadRanges` and the Vulkan twin at line 343 — when the host GPU needs guest vertex memory, it snapshots via `memcpy` from `memory().TranslatePhysical(...)` into an upload buffer IMMEDIATELY at draw time. This is pure copy semantics at host level.

See also `glue/rexglue-sdk-main/src/graphics/primitive_processor.cpp:926` — index buffers are requested via `shared_memory_.RequestRange` then uploaded synchronously. There is no asynchronous "hold this pointer for later" anywhere in the host draw path.

## Task 3 — Data flow trace

1. Caller writes `0xFFFFFFFF` to stack[212], passes `&stack[212]` to `sub_8227F5B8`.
2. `sub_8227F5B8` saves `r4` to stack[92]. Calls `sub_8227F2E8`.
3. `sub_8227F2E8` reads `r30 = caller's pointer`, then `r9 = *r30 = 0xFFFFFFFF` — VALUE in register.
4. Calls `sub_828C2290` 4x passing `r9` by register. Each call writes `r9` to `vertex[i].color = *(pool + 24)`.
5. Caller returns. Stack slot 212 dies. Vertex pool retains the VALUE.
6. Later, `sub_828BF270` flushes the pool (unrelated trigger path).
7. Later still, the GPU command processor reads the pool via PM4 fetch — by then the caller is long gone, but the values are in pool memory.

## Task 4 — Upload allocator

The host's `D3D12UploadBufferPool` / `VulkanUploadBufferPool` obtain a mapped upload buffer via `upload_buffer_pool_->RequestPartial()`, then immediately `std::memcpy` from guest translated physical memory (`shared_memory.cpp:393-395`). No pointer is retained past `memcpy` return. Capture-by-value at the host boundary is GUARANTEED.

## Task 5 — Bug classification

There is no UAF on the color data. The observed poison 0xFFE1E1E1 in the `draw_pool_watchpoint` corresponds to `*0x831C22A4`, which resolves to UN-named data (confirmed via `resolve_address`). That field is the singleton `mgr` pointer consumed by `sub_828BF270` (read site) and dereferenced at `sub_82A3DF50` by `lwz r11, 13428(r3)`. The UAF is on this singleton allocator, NOT on per-draw color words.

## Task 6 — Where to hook? (Answered: nowhere for capture-by-value)

Since capture-by-value is already correct, there is no architectural fix at this layer. Hooking `sub_82A3DAB0` or `sub_828C2290` to "copy the color earlier" is a no-op — the copy already happens at `stw r9, 24(r11)` inside `sub_828C2290`.

The correct POI is the SINGLETON LIFETIME at 0x831C22A4:
- Writer 1: `sub_828C01E0`  writes from `dword_831C3094` (actually `0x831C23E4`).
- Writer 2: `sub_828C0338`  writes from `0x831C2294`.
- Parent: `sub_828C47E8` (sole caller of both writers).
- Grandparent: `sub_828D4C88`.

Investigation should trace what produces the POISONED pointer value into `SRC1_ADDR` / `SRC2_ADDR`, then into the mgr slot. The pool manager itself is freed while still referenced from the singleton slot.

## Task 7 — Vertex-decl-NULL integration

The vertex-decl-NULL issue is orthogonal. The decl is a separate state register (SetVertexDeclaration), sourced from a different structure (`D3DDevice`-equivalent fields in the guest). A unified fix would not emerge from the color capture analysis because color capture is already correct. The vertex-decl NULL is a DIFFERENT lifetime bug — the device state struct at a different guest address.

## Summary Table (for Python verification)

|addr|role|value|source|
|-|-|-|-|
|0x831C22A4|singleton mgr ptr (UAF victim)|0xFFE1E1E1 poison|Writers 1/2 via sub_828C47E8|
|caller stack r1+212|color word|0xFFFFFFFF|immediate literal|
|pool[slot]+24|vertex color|captured VALUE|stw r9 in sub_828C2290|

## Conclusion

The hypothesis is incorrect. The color is captured by value inside `sub_8227F2E8` before the caller's stack frame disappears. The UAF is elsewhere — it is on the singleton mgr allocator at 0x831C22A4, NOT on DC-derived vertex colors. No architectural fix at the render-command enqueue layer is warranted based on this hypothesis. Effort should pivot to the singleton allocator lifetime (Writers 1/2 in `draw_pool_watchpoint.cpp`).
