# LibertyRecomp Rewrite Playbook

## Purpose
This single reference replaces the previous module inventory and handoff documents. It maps every rewrite-critical module, explains the Xbox 360 assumptions, and presents a prioritized rewrite path for modern host implementations.

### Key Details
- **Total estimated effort:** 12–15 days for the core stack (timing, boot, GPU)
- **Priority order:** Timing/Sync > Boot/XAM Tasks > GPU Rendering > Runtime services (threads, async I/O)
- **Dependencies:** Timing foundations enable event/fence handling; GPU depends on timing+shader cache; runtime behavior references the same synchronization primitives.

---

## 1. Module Inventory & Priority
| Module | Scope | Complexity | Effort | Status |
|--------|-------|------------|--------|--------|
| Timing & Synchronization | Full | Critical | 2–3 days | P0 Blocking |
| XAM Task System | Full | Critical | 1–2 days | P0 Blocking |
| GPU Rendering | Full | High | 5–7 days | P1 |
| Boot State Machine | Partial | High | 2–3 days | P1 |
| Shader System | Full | Medium | 3–4 days | P2 |
| File System/Async I/O | Partial | Medium | 2–3 days | P2 |
| Worker Threads | Partial | High | 2–3 days | P1 |
| Audio System | Full | Medium | 3–4 days | P3 |
| Input System | Partial | Low | 1 day | P3 |

**Main dependencies:** timing → boot/xam → GPU, with runtime behaviors underpinning everything.

---

## 2. Timing & Synchronization Rewrite Case
_Content from TIMING_REWRITE.md and relevant handoff sections._
- Fixed 60 Hz model, VBlank callback, event/fence signaling, frame-flag forcing, direct `VdSwap`, event tracker, watchdog.
- Must hook: `sub_829DDC90`, `sub_82169400`, `sub_829D7368`, `sub_829D4C48`, `sub_82673718`, `sub_82671E40`.
- Implementation: timing thread, `KeWaitForSingleObject` bypass, `KeSetEvent` tracking, forced `VdSwap`.
- Validate: 60 logs/second, 256 events signaled/frame, stable frame times (14–20 ms), no infinite loops.

---

## 3. Boot & XAM Task Rewrite Case
_Content from BOOT_REWRITE.md and handoff sections._
- Boot path: `_xstart` → `sub_8218BEA8` → `sub_827D89B8` → `sub_82120000` → `sub_8218C600` with async XAM tasks.
- Problem: `XamTaskSchedule` stub, `XamTaskShouldExit` returns 1, busy-waits poll job counters (`sub_82673718`/`sub_82975608`), semaphores never signaled.
- Rewrite: implement `XamTaskSchedule/ShouldExit/CloseHandle`, cache `sub_82120000` result, implement loop around `sub_8218BEA8`, signal semaphores after init, optionally collapse blocked loops.
- Validate: `sub_82120000` returns success once, log `sub_828529B0` entry, `VdSwap` invoked ~60 FPS, worker semaphores get signaled, XAM tasks report proper states.

---

## 4. GPU Rendering Rewrite Case
_Content from GPU_REWRITE.md and handoff sections._
- Hook layer 1 D3D wrappers; bypass PM4 command builders and ring-buffer handling.
- Key functions: `CreateDevice` (`0x829D87E8`), `DrawPrimitive` (`0x829D8860`), `UnifiedDraw` (`0x829D4EE0`), `Present` (`0x829D5388`), shader/state setters (`0x829CD350`, `0x829D6690`, `0x829C9070`, `0x829C96D0`, `0x829D3728`, `0x829C9440`, `0x829C8000`/`0x829C9368`, etc.).
- Host renderer must translate primitive types, track dirty state, connect to shader cache, bind textures/buffers, and issue draw calls.
- Validation: ~961 draws per frame, shader handles non-null, visuals match reference, <16.7 ms frames, pipeline changes minimized.

---

## 5. Runtime Services & Data
- **Runtime Behavior:** state machines (XAM tasks, worker readiness, init completion), worker thread lifecycle (semaphores 0xA82487F0/B0, `sub_827DE858`, `sub_827DACD8`), `XXOVERLAPPED` semantics (Results, event handles, completion routines); referenced in `RUNTIME_BEHAVIOR.md` and `ARCHITECTURE_REFERENCE.md`.
- **Data structures:** device context, render targets, streaming objects, heap layout; captured in architecture doc.
- **Shaders & audio:** extracted via `SHADER_PIPELINE.md`; `EffectManager` stub must create real GuestShader handles.

---

## 6. Implementation Roadmap
1. **Phase 1:** Timing/event/fence system (P0).
2. **Phase 2:** XAM task/boot loop improvements (P0/P1).
3. **Phase 3:** GPU renderer hooks & shader binding (P1).
4. **Phase 4:** Runtime services (threads, async I/O, audio, input) as needed.

---

## Document History
- 2025-12-20: Consolidated `MODULE_REWRITE_INDEX.md` + `REWRITE_HANDOFF.md` into this playbook.
