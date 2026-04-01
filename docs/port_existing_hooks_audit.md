# Existing Hooks Audit: Streaming/Resource Loading Path

Audit date: 2026-03-28. Source files: `kernel/imports.cpp`, `gpu/video.cpp`.

## Call Chain Analysis: sub_82478AF8 -> sub_827C2420 -> sub_82852DD0 -> sub_82852D18

### sub_82478AF8 — Engine init orchestrator
- **Hook type**: INIT_PROBE (probe #2830, "821FC1F8 call 30")
- **Behavior**: Pass-through with printf trace. Calls original `__imp__` and logs ENTER/EXIT.
- **Status**: Diagnostic only. No behavioral modification.

### sub_827C2420 — Activate streaming
- **Hook type**: INIT_PROBE (probe #3055, "82478AF8 tail activate-streaming")
- **Behavior**: Pass-through with printf trace. Calls original.
- **Status**: Diagnostic only. No behavioral modification.

### sub_82852DD0 — "OpenAndProcess" streaming resource loader
- **Hook type**: Full PPC_FUNC_HOOK (custom, replaces INIT_PROBE)
- **Behavior**: Pass-through with detailed tracing. Logs all 6 register args, thread ID, caller address, and return value. Calls original.
- **Flow**: sub_8284F468 (find) -> sub_82852D18 (process) -> sub_8285B088 (flush)
- **Status**: Diagnostic only. No behavioral modification.

### sub_82852D18 — Resource processor (vtable[2] dispatch)
- **Hook type**: Full PPC_FUNC_HOOK (custom, replaces INIT_PROBE)
- **Behavior**: Pass-through with vtable introspection. Reads r5 object's vtable pointer and slot[2] target before calling original. Logs thread ID.
- **Status**: Diagnostic only. Known to hang at vtable[2] dispatch per comment.

## All Hooks in 0x8284xxxx-0x8286xxxx Range

### imports.cpp

| Address | Name | Type | Behavior | Purpose |
|-|-|-|-|-|
| 0x828497D8 | NtWait dispatcher | PPC_FUNC_HOOK | Pass-through | Non-GPU waits still work; GPU fences short-circuited by higher-level stubs |
| 0x82848750 | Heap alloc (TLS debug) | PPC_FUNC_HOOK | Pass-through + log | Checks TLS[1684] debug flag on first call |
| 0x82848B68 | Heap free | PPC_FUNC_HOOK | Pass-through + log | Double-free detector; tracks freed addresses, flags re-frees |
| 0x828507F8 | Frame presentation | PPC_FUNC_HOOK | Pass-through | Was clearing 0x83124CCC; fix removed that (sync done in video.cpp) |
| 0x8284A7E8 | Content creation | PPC_FUNC_HOOK | Pass-through + log | Traces XamContentCreateEx slot state (16=sync/bad, 17=async/good) |
| 0x8284C290 | TLS alloc guard | PPC_FUNC_HOOK | Pass-through + fix | Clears corrupt TLS[1676] allocator pointers after call |
| 0x8284CFD8 | Ring-buffer worker init | PPC_FUNC_HOOK | Pass-through + fix | Seeds XSemaphore handles into worker structs at unk_8319F2F8 to prevent deadlock |
| 0x8284E830 | Finalize streaming | INIT_PROBE (#3056) | Pass-through + log | Diagnostic only |
| 0x8284F310 | Start streaming mgr | INIT_PROBE (#30551) | Pass-through + log | Diagnostic only |
| 0x8284F468 | Alloc resource | INIT_PROBE (#30554) | Pass-through + log | Diagnostic only (called by sub_82852DD0) |
| 0x82849918 | Yield/sleep | PPC_FUNC_HOOK | Pass-through + log | Logs every 1000th yield iteration |
| 0x82851A10 | Pre-vtable call | INIT_PROBE (#30557) | Pass-through + log | Diagnostic only (called by sub_82852D18) |
| 0x82852A50 | Get resource ptr | INIT_PROBE (#30556) | Pass-through + log | Diagnostic only (called by sub_82852D18) |
| 0x82852B78 | ShaderBind | PPC_FUNC_HOOK | Pass-through | Lets original run; sub_8285B088 inside is stubbed |
| 0x82852D18 | Resource processor | PPC_FUNC_HOOK | Pass-through + log | Detailed vtable introspection; known hang point |
| 0x82852DD0 | OpenAndProcess | PPC_FUNC_HOOK | Pass-through + log | Detailed trace of streaming resource load |
| 0x8285A8B0 | GPU buffer flush | PPC_FUNC_HOOK | **STUB (no-op)** | Skips Xenos shader bytecode submission (vtable slots 9+13) |
| 0x8285B088 | Shader registration | PPC_FUNC_HOOK | Pass-through + log | Lets original run; sub_8285A8B0 inside is now stubbed |
| 0x8285C648 | GPU fence wait | PPC_FUNC_HOOK | **STUB (returns 1)** | Immediately signals completion |
| 0x8285CF98 | Fence create+wait | PPC_FUNC_HOOK | **STUB (returns 1)** | Returns success without waiting |
| 0x8285D018 | Ring buffer submit+wait | PPC_FUNC_HOOK | **STUB (returns 0)** | Skips all GPU command submission; returns fence=0 |

### video.cpp

| Address | Name | Type | Behavior | Purpose |
|-|-|-|-|-|
| 0x828574A0 | Shader reload | PPC_FUNC_HOOK | **STUB (no-op)** | Liberty shaders are immutable |
| 0x8285BDC8 | Shader FXC open | PPC_FUNC_HOOK | **STUB (returns 1)** | XEX shaders not in cache; GPU funcs would trap |
| 0x8285DF10 | Shader fixup | PPC_FUNC_HOOK | **STUB (no-op)** | Depends on FXC loading (also stubbed) |
| 0x82858758 | Shader preload | PPC_FUNC_HOOK | **STUB (returns 0)** | Liberty's CreateShader handles all shaders |
| 0x82869620 | SPS DB index->entry | PPC_FUNC_HOOK | **Replace** | Lazy-creates FXC effect objects via shader manager vtable |
| 0x82869F30 | Shader DB scanner | PPC_FUNC_HOOK | **Replace** | Populates guest SPS DB from embedded preset table (no file I/O) |
| 0x82856F08 | Render tick | (called by main loop) | Indirect | Called in sub_8218BEA8 infinite loop; render path root |
| 0x828BEC78 | (GPU related) | PPC_FUNC_HOOK | (in video.cpp) | GPU-side hook |

## INIT_PROBE Inventory (All 46 Probes)

All INIT_PROBE entries are pass-through with printf trace. They call the original function and log sequence number + description.

### Top-level init sequence (10 probes)
- sub_82302308 (#2, early init), sub_826CA440 (#3, engine mid-level), sub_82299500 (#4, renderer init)
- sub_821B4768 (#7, player/controller), sub_82214E00 (#10, game systems), sub_82266EA8 (#11, subsystem)
- sub_8233C480 (#13, subsystem r3=2000), sub_82140F38 (#15, subsystem), sub_821E34C0 (#20, subsystem)
- sub_82206BB8 (#25, subsystem)

### Mid-level init (8 probes)
- sub_8223F458 (#26), sub_8223C848 (#27), sub_821FC1F8 (#28), sub_82267948 (#31)
- sub_822446A8 (#35), sub_821C61D8 (#40), sub_82220FC8 (#46), sub_822A1028 (#50)

### Late/final init (3 probes)
- sub_82146A68 (#59, late init), sub_82227D50 (#63, late init), sub_82329C90 (#68, final init)

### sub_821FC1F8 internal calls (10 probes)
- sub_8251BA08 (#2801, call 1), sub_82504318 (#2805, call 5), sub_82446BA8 (#2810, call 10)
- sub_82446DB0 (#2815, call 15), sub_8254A610 (#2820, call 20), sub_82163F38 (#2825, call 25)
- sub_82478AF8 (#2830, call 30), sub_822B2010 (#2835, call 35), sub_823A2108 (#2840, call 40)
- sub_825030B8 (#2845, call 45)

### sub_82478AF8 internal phases (15 probes)
- sub_826225E0 (#3001, env-A), sub_826226B0 (#3003, env-C)
- sub_8294BD68 (#3005, audio-mgr-ctor), sub_8294E208 (#3010, finalize-audio-mgr)
- sub_82956718 (#3012, xaudio-obj-ctor), sub_82953088 (#3013, audio-graph-init)
- sub_82955838 (#3019, CreateSourceVoices), sub_827ADB48 (#3026, create-voice-block)
- sub_8287AC38 (#3036, streaming-graph), sub_8261C7C8 (#3039, audio3D-init)
- sub_8228A1E0 (#3049, start-stream-thread), sub_825FD6B8 (#3050, start-RPF-stream)
- sub_82955BE0 (#3051, xaudio-stream-HANG), sub_82477670 (#3054, streaming-tick)
- sub_827C2420 (#3055, activate-streaming)

### sub_827C2420 / sub_82852DD0 / sub_82852D18 subprobes (4 probes)
- sub_8284F310 (#30551, start-streaming-mgr)
- sub_8284F468 (#30554, alloc-resource in sub_82852DD0)
- sub_82852A50 (#30556, get-resource-ptr in sub_82852D18)
- sub_82851A10 (#30557, pre-vtable-call in sub_82852D18)

### Custom non-INIT_PROBE init hook
- sub_822BCA90 (#33, "between 25-35") — PPC_FUNC_HOOK with custom storage-init tracing

## Gap Analysis: Missing Hooks on the Streaming Hang Path

### Functions with hooks (in the hang chain):

| Function | Hook | Sufficient? |
|-|-|-|
| sub_82478AF8 | INIT_PROBE | Yes (orchestrator, no fix needed) |
| sub_827C2420 | INIT_PROBE | Yes (orchestrator, no fix needed) |
| sub_8284F310 | INIT_PROBE | Diagnostic only — may need behavioral hook if it creates bad state |
| sub_82852DD0 | Pass-through trace | **NO** — traces the hang but does not prevent it |
| sub_82852D18 | Pass-through trace | **NO** — traces the vtable[2] hang but does not prevent it |
| sub_8284F468 | INIT_PROBE | Diagnostic only |
| sub_82852A50 | INIT_PROBE | Diagnostic only |
| sub_82851A10 | INIT_PROBE | Diagnostic only |
| sub_8285B088 | Pass-through (sub_8285A8B0 stubbed) | **Partially** — GPU flush is stubbed, but vtable slot 10 runs (memory-only) |

### Functions WITHOUT hooks (potential gaps):

1. **sub_8284F310** ("start-streaming-mgr") — Only has INIT_PROBE. If this function sets up the streaming manager state that sub_82852DD0 depends on, corrupt init could cause the hang.

2. **sub_8284E830** ("finalize-streaming") — Only INIT_PROBE. Runs after the hang point. If the hang in sub_82852DD0/D18 is resolved, this function might also need inspection.

3. **vtable[2] target of sub_82852D18** — The actual function dispatched through the vtable (address resolved at runtime, printed by the hook). This is the ACTUAL hang point. It likely routes into sub_8285B088 or a similar GPU-bound path. The vtable target itself has no hook unless it happens to be one of the already-hooked functions (e.g., sub_8285B088).

4. **sub_828529B0** — Referenced in video.cpp as part of render path (sub_82856F08 -> sub_828529B0 -> sub_828507F8). No dedicated hook.

## Summary

The streaming hang path sub_82478AF8 -> sub_827C2420 -> sub_82852DD0 -> sub_82852D18 has **extensive diagnostic instrumentation** (6 INIT_PROBEs + 2 detailed trace hooks) but **no behavioral fixes** on the actual hang point. The hang occurs inside sub_82852D18 at a vtable[2] indirect dispatch. The GPU-side stubs (sub_8285A8B0, sub_8285D018, sub_8285C648, sub_8285CF98) handle the ring buffer / fence path, and sub_8285B088's GPU flush is neutered. However, if the vtable[2] dispatch in sub_82852D18 routes to a function OTHER than sub_8285B088 (e.g., a resource-specific handler), it will bypass all existing stubs and potentially hit raw GPU code.

**Key finding**: The hooks are structured for diagnosis, not for resolution. The next step would be to examine the runtime vtable[2] target address from the trace logs and determine whether it already has a hook or needs one.
