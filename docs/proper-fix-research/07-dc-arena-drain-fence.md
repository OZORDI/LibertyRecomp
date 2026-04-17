# Agent 07 — DC Arena Drain/Fence Hypothesis

## TL;DR

CBaseDC UAF is **NOT** caused by missing host-side fence between render-queue submission and GPU drain. The host-side fence chain `g_executedCommandList.wait(false)` (video.cpp:3676) → `g_queue->waitForCommandFence()` (video.cpp:3700) is sound: the guest submit thread blocks until the render thread has fully processed `ExecuteCommandList` (which memcpy's all guest pointer payloads into `g_intermediaryUploadAllocator` via `ProcDrawPrimitiveUP`) before calling `.reset()` on upload allocators.

The UAF root cause lives **inside the guest**: `CDrawCommandAllocator` / arena allocator `sub_821BB3D8` silently wraps the 1MB DC arena to offset 0 mid-frame when full, while `CEndDrawListDC`-delimited draw lists are still being consumed on the guest render thread (`sub_821B5B20` / `sub_821B5A68` via `sub_821BB2D0`). There is **no fence** between the arena producer (game thread building DCs) and the arena consumer (render thread executing DC vtable slot[1]).

## Evidence (Python-verified numbers, no pseudocode)

| Field | Address / Value | Role |
|-|-|-|
| `CBaseDC` vtable | 0x82000974 | 6-slot DC base |
| `CEndDrawListDC` vtable | 0x82001054 | terminator DC; slot[2]=sub_821BD840 (getType=8); slot[5]=sub_82175650 (getType=2) |
| `CNewDrawListDC` vtable | 0x82001038 | drawlist-open DC; slot[2]=sub_821BD988 (getType=16) |
| `CDrawCommandAllocator` vtable | 0x82000F2C | inherits rage::sysMemAllocator; 19-slot; slot[0]=sub_821BB220 (ctor); slot[2]=sub_821BF978 (bound wrapper to sub_821BDB48) |
| Arena size threshold | `(15 << 16) \| 40960` = **0xFA000 = 1024000 bytes** | 1 MB arena |
| DC size field mask (tag word [1]) | `(tag >> 3) & 0x7FF0` | max DC = **0x7FF0 = 32752 bytes** (~32KB) |
| DC sequence counter | `dword_82A925F0` | monotonic, packed `(seq << 18) \| (sizeField & 0x3FFFF)` in DC[+4]. Reset to 1 by `CEndDrawListDC::ctor` (sub_821BB568) |
| Global arena base array | `dword_82B38B58[]` | per-buffer base pointers |
| Current buffer index | `dword_82B38B6C` | index into B58[] — **never rotated by retail code** |
| Bump cursor | `dword_82B38B74` | byte offset into current buffer |
| "Overflow pending" flag | `dword_82B38B60[B6C]` | **write-only** in retail pseudocode (debug trace, never read) |
| Reserve cursor | `dword_82B38B70` | used by `sub_821BB9A0` pre-check allocator |
| DrawList queue control array base | allocator + 7196..7344 | per-sub-queue heads/counts (sub_821BAA50) |

## Key code locations (all absolute paths)

Guest allocator / arena:
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BB3D8_0x821BB3D8.c` — **arena overflow-wrap** (resets `dword_82B38B74=0` at line 23 when `cursor+aligned+orig >= 0xFA000`; sets write-only debug flag `B60[B6C]=1`; does NOT rotate buffer index `B6C`; does NOT wait for consumer).
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BB9A0_0x821BB9A0.c` — secondary pre-check allocator using `dword_82B38B70`.
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.4.cpp` — `sub_821BDB48` (member-level per-descriptor arena, separate from global; also bump-wraps on overflow; 16-byte alignment; `rexcrt_memcpy` payload in).
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BB4C8_0x821BB4C8.c` — `CNewDrawListDC::ctor`; captures `dword_82B38B74 - 16` as drawlist start.
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BB568_0x821BB568.c` — `CEndDrawListDC::ctor`; calls `sub_821BA750` to finalize drawlist.
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BA750_0x821BA750.c` — writes terminator marker via TLS`[0]+20`: `*(*(r13+0)+20)+12 = 2; *(..+20)+4 = result; *(r13+0)+20 = 0`. The drawlist slot is detached (`+20 = 0`).

Guest DC enqueue / ring-buffer:
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BA858_0x821BA858.c` — writes to allocator struct at `+7196` (sub-queue descriptor ID), calls `sub_821BA7E0`.
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BA7E0_0x821BA7E0.c` — **128-slot × 28-byte ring buffer**. When `count==128`, returns 0 (drops DC). Wraps at head==128.
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BAEF0_0x821BAEF0.c` — per-type secondary queue append (model-info refs, txd refs, etc).
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BAA50_0x821BAA50.c` — allocator init: zeroes all 10 sub-queue descriptors at +7196..+7274.

Guest DC consumer (render thread):
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BB2D0_0x821BB2D0.c` — **DC execution loop**. Reads `v13 = arena_buf + cursor`, calls `(*v13.vtable)(+4)(v13)` (vtable slot[1]), advances cursor by `((v13[1] >> 3) & 0x7FF0)`, terminates when `v13[1] >> 18 == a3` (expected end-seq from CEndDrawListDC).
- `gta_iv/xex_excavation_retail/pseudocode/sub_821B5B20_0x821B5B20.c` — **render-thread main loop**. Waits semaphore `[a1+4012]`, calls callback `[a1+4076]`, dispatches via `sub_821BA928` (rotating ring-buffer entries) or `sub_821BAA18` (clear ring pointers).
- `gta_iv/xex_excavation_retail/pseudocode/sub_821B5A68_0x821B5A68.c` — 4-iteration drain around `sub_821BAF28` (double-buffer swap).
- `gta_iv/xex_excavation_retail/pseudocode/sub_821B5A08_0x821B5A08.c` — single-iteration variant.
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BAF28_0x821BAF28.c` — **queue-swap on `byte_82A925EC` set**, only swaps the per-DC-type SECONDARY queue heads/counts (offsets +7204..+7274), not the primary DC arena pointers.
- `gta_iv/xex_excavation_retail/pseudocode/sub_821BA928_0x821BA928.c` — drains the 128-slot ring, calls `sub_821BB2D0(&dword_82B38B58, *v8, v8[1], v8[4])` — i.e. passes arena-base-array, start offset, expected-end-seq, and terminator-seq. Reads DCs FROM the arena identified by `dword_82B38B58`.

Host render thread (fenced path — **not the bug site**):
- `LibertyRecomp/gpu/video.cpp:597-710` — `UploadAllocator`/`IntermediaryUploadAllocator` (16MB ring).
- `LibertyRecomp/gpu/video.cpp:1285` — `g_renderQueue` (moodycamel BlockingConcurrentQueue<RenderCommand>).
- `LibertyRecomp/gpu/video.cpp:3438,3676,3881` — `g_executedCommandList` atomic<bool> used as a two-way fence: submit thread waits at 3676, render thread notifies at 3881 inside `ProcExecuteCommandList`.
- `LibertyRecomp/gpu/video.cpp:3710-3712` — the ONLY site where `g_intermediaryUploadAllocator.reset()` is called. This executes AFTER the `wait(false)` returned, so the render thread has already memcpy'd all guest payloads. **Host-side is correctly fenced.**
- `LibertyRecomp/gpu/video.cpp:5480,5498,5894` — allocate-and-copy sites that snapshot guest memory (VS/PS constants, UP vertex data).

## The actual race

1. Game thread A (`AddToDrawListEntityList`, sub_821EE700 → sub_825444F0, line 53 spawns `EntityRenderCacheTask` via `sub_8285D9D8`) builds CDrawEntityDC / CDrawPedDC / CDrawFragDC instances by calling `sub_821BB3D8(sizeof(DC))` → writes into arena bytes `[B58[B6C] + B74 .. +B74+size]`, advances `B74`.
2. When `B74 + size >= 0xFA000`, `sub_821BB3D8` line 23 sets `B74 = 0` — arena wraps. Subsequent DC writes overwrite address range `[B58[B6C] + 0 .. +size]`.
3. Meanwhile render thread B (`sub_821B5B20`) has been signaled to run earlier drawlists via `sub_821BB2D0`. It holds `a1[6] = v12` = a cursor somewhere IN the arena, and dereferences `a1[a1[4]] + v12` each iteration. The DC bytes it reads can be clobbered by thread A's wrap.
4. `CEndDrawListDC` terminator-seq check `a3 == v13[1] >> 18` is the only exit condition. If a clobbered DC has a valid-looking but wrong seq, the loop reads garbage vtables, jumps to garbage, or loops past the intended end — this is the UAF/crash surface.
5. `dword_82A925F0` (global seq counter) gets reset to 1 by `CEndDrawListDC::ctor`, so seq values recycle rapidly — increasing the chance that a stale DC's packed seq coincidentally matches.

## What a proper fix requires

**NOT a host-side fence.** The host `g_executedCommandList` chain is correct.

**The correct fix is host-side enforcement of arena-producer/consumer ordering in the guest recomp.** Options from least to most invasive:

1. **Per-arena shadow copy (safest, most work).** In a hook on `sub_821BB3D8`, detect the overflow-wrap condition (`B74 + aligned_size >= 0xFA000`) and instead of resetting `B74=0`, allocate a fresh host-side backing buffer, copy the arena out, swap the base in `dword_82B38B58[B6C]`, and retire the old buffer into a ring guarded by the render thread's completion. Effectively add the missing triple-buffering that the field layout hints at but retail never implemented.

2. **Producer-stall hook.** In `sub_821BB3D8`, at the overflow branch, block the calling (game) thread on a host-side event that the render-thread dispatcher (`sub_821BB2D0`) sets when it observes a `CEndDrawListDC` seq match. This forces arena drain before overwrite. Risk: deadlock if the producer and consumer are the same thread (sub_821B5A08 is called from sub_82140088 which looks like game thread).

3. **DC-payload copy-out on render-side.** Hook `sub_821BB2D0` to `memcpy` each DC's bytes into a render-thread-local scratch before calling `(*v13.vtable)(+4)(v13)`. Size is known: `((v13[1] >> 3) & 0x7FF0)`. This eliminates the liveness requirement on the arena mid-dispatch but requires intercepting every `this` pointer the DC vtable slot[1] reads.

4. **Increase arena size.** Raise `0xFA000` threshold to e.g. 8MB via patching the immediate in `sub_821BB3D8`. Makes overflow-within-frame rare but doesn't eliminate the race; not a proper fix.

Recommended: **Option 1** (shadow copy / real triple-buffer) in a patches/ file hooking `sub_821BB3D8`. Field layout already reserves `dword_82B38B58[]` as an array and `dword_82B38B60[]` as a flag array; retail's bug is that the index `B6C` is never advanced. A host-managed index rotation with reference-counting on render-thread drain is the right shape.

## Cross-references

- Agent 11's `CDrawCommandAllocator` / 0x82000F2C finding: confirmed. The *per-instance* vtable-0x82000F2C allocator is separate from the *global* `dword_82B38B58` arena. Per-instance uses `sub_821BDB48`; global uses `sub_821BB3D8`. Both have identical overflow-wrap semantics with no fence. The 228-byte DCs in question can come from either.
- Agent 15's hypothesis 2 "CEndDrawListDC wipes the DC arena before GPU submit": **refuted in detail**. `CEndDrawListDC::ctor` (sub_821BB568) does NOT wipe the arena. It only (a) resets `dword_82A925F0 = 1` (seq counter), and (b) via `sub_821BA750`, detaches the active drawlist slot at TLS`[0]+20`. The arena wipe/wrap only happens on overflow in `sub_821BB3D8`, unrelated to CEndDrawListDC enqueue timing.
