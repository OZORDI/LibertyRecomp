# Deadlock Pattern Analysis: Audio Streaming Init Hang

Generated 2026-03-28.

---

## Hang Chain

```
sub_82478AF8 (audio init)
  → sub_827C2420 (activate streaming)
    → sub_82852DD0 (thread launch dispatch)
      → sub_82852D18 (operation) — HANGS
        → sub_82852A50 (get resource) — returned OK (probe #30556 fired)
        → sub_82851A10 (pre-vtable) — entered (probe #30557 fired)
        → PPC_CALL_INDIRECT_FUNC(vtable[2]) — HANG POINT
```

---

## Root Cause: Same "GPU vtable dispatch blocks forever" pattern

### sub_82852D18 internals (gta4_recomp.55.cpp:30962)

1. Calls `sub_82852A50(r3=context, r4=resource)` → returns resource ptr in r27
2. If r27 != null: loads `r31 = *(r27+0)` (dereference resource to get inner object)
3. If r31 != null: calls `sub_82851A10(r3=context+24, r4=*(r31+0))` — name lookup in resource table
4. Loads vtable: `r11 = *(*(r29+0) + 8)` — vtable slot 2 of the object passed as r5
5. Calls vtable[2] with args `(r3=r29_object, r4=r31_inner, r5=r28_flags)` — **THIS HANGS**

### What is r5/r29 (the vtable object)?

In the `sub_827C2420` call site (gta4_recomp.50.cpp:57438), r5 = `0x820BBFE4` — a static object in the .rdata section. Its vtable[2] (offset 8) is the function dispatched. This is NOT a code address we can identify statically from the recomp — it's initialized at runtime from the executable's data section.

### Why it hangs

The object at `0x820BBFE4` is a **GPU device dispatch interface** — same class family as the shader vtable objects. The vtable[2] function likely submits a "create streaming resource" command to the Xenos PM4 command ring and waits for GPU fence completion. Without a real GPU, the fence never completes.

Evidence:
- `sub_82852DD0` calls `sub_82852D18` then immediately calls `sub_8285B088` (the already-hooked GPU shader consumer). This confirms they operate on the same GPU device abstraction layer.
- The post-vtable code in `sub_82852D18` (lines 31031-31045) reads the same global at `0x831E55EC` offset 40, extracts bit 17, and conditionally calls `sub_828470E0` — the GPU thread lock pattern identical to `sub_82852FB0` (ShaderFinalise).
- `sub_82852DD0` passes through the resource manager at `0x82B07278` — same one used by `sub_82852B78` (ShaderBind).

---

## Comparison with Previous Hang

| Attribute | Previous (sub_82955BE0) | Current (sub_82852D18) |
|-|-|-|
| Pattern | "Wait for callback that never fires" (yield loop polling struct+2016) | "Wait for GPU fence that never completes" (vtable dispatch into PM4 ring) |
| Root cause | Xbox async kernel callback (XAudio2 device arrival) never fires on PC/macOS | Xenos GPU command submission waits for hardware fence response |
| Fix | Hook sub_82212EC0 to write struct+2016=0 (connected state) | Need to stub/hook sub_82852DD0 or sub_82852D18 |
| Category | Missing kernel callback | Missing GPU hardware |

**This is NOT the same "callback never fires" pattern.** It is the same "GPU hardware never responds" pattern as the shader hooks (sub_8285B088 / sub_8285A8B0).

---

## Why Existing Hooks Don't Cover This

The existing GPU stubs cover the shader pipeline:
- `sub_82852FB0` (ShaderFinalise) → hooked to return 1 (success)
- `sub_82852B78` (ShaderBind) → runs original (sub_8285B088 inside is safe because sub_8285A8B0 is stubbed)
- `sub_8285B088` (shader consumer) → runs original (sub_8285A8B0 GPU flush is stubbed)
- `sub_8285A8B0` (GPU buffer flush) → stubbed (no-op)

The problem: `sub_82852DD0` reaches the GPU through a DIFFERENT path. It calls `sub_82852D18` which dispatches through a vtable[2] call on a different object (`0x820BBFE4`). This vtable dispatch does NOT go through `sub_8285A8B0`. It is a separate GPU entry point — likely "create/bind streaming buffer on device" rather than "submit shader bytecode".

The existing `sub_8285B088` call in `sub_82852DD0` (line 31130-31132) would work fine AFTER `sub_82852D18` returns, but `sub_82852D18` never returns because it's stuck in the vtable[2] GPU dispatch.

---

## Recommended Fix Strategy

**Option A: Hook sub_82852DD0 to skip sub_82852D18 entirely**
- Return the allocated resource from sub_8284F468 and call sub_8285B088 for cleanup, but skip the vtable dispatch operation
- Risk: may leave streaming resource in uninitialized state

**Option B: Hook sub_82852D18 to skip the vtable[2] call**
- Let sub_82852A50 and sub_82851A10 run (pure memory operations), but skip the PPC_CALL_INDIRECT_FUNC
- Set r30=1 (success) to mimic normal completion
- Lower risk: resource lookup still happens, only GPU submission is skipped

**Option C: Hook sub_827C2420 (activate-streaming) to return immediately**
- Highest level, lowest risk of side effects from partial execution
- May break streaming audio if downstream code expects the resource manager to be initialized

Option B is closest to the existing pattern (sub_8285A8B0 stub skips GPU submission while letting surrounding logic run).

---

## Key Addresses

| Address | Role |
|-|-|
| 0x820BBFE4 | Static vtable object passed as r5 to sub_82852DD0 from sub_827C2420 |
| 0x831E55EC | Global pointer to GPU device state (loaded via `lis -31970 + lwz 21996`) |
| 0x82B07278 | Resource manager instance (used by both shader and streaming paths) |
| sub_82852D18 | Operation dispatcher — contains the blocking vtable[2] call |
| sub_82852DD0 | Thread launch dispatch — calls sub_8284F468 → sub_82852D18 → sub_8285B088 |
| sub_82851A10 | Pre-vtable name lookup (pure memory, safe) |
| sub_82852A50 | Resource getter (pure memory, safe) |
