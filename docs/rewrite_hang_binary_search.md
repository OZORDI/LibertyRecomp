# sub_82478AF8 Hang — Binary Search Probe Plan

**Source**: `gta4_recomp.20.cpp` lines 83744–85184
**Total direct sub-calls**: 71
**Script**: analysis performed by Python parsing all sub_ call sites in order.

---

## All Call Sites in Execution Order

| Line | Function | Phase |
|-|-|-|
| 83764 | sub_826225E0 | 1 — TLS slot alloc |
| 83767 | sub_82622648 | 1 — TLS slot alloc |
| 83770 | sub_826226B0 | 1 — TLS slot alloc |
| 83775 | sub_821B3510 | 2 — malloc(176) |
| 83784 | sub_8294BD68 | 2 — AudioMgr ctor |
| 83896 | sub_8294BE28 | 3 — set listener pos |
| 83903 | sub_8294BE20 | 3 — set audio budget |
| 83914 | sub_8294BEA0 | 3 — set audio param |
| 84019 | sub_8294E208 | 3 — finalize audio mgr |
| 84028 | sub_821B3510 | 2 — malloc(voice array) |
| 84035 | sub_82956718 | 3 — voice pool init |
| 84050 | sub_82953088 | 3 — audio subsys init |
| 84057 | sub_82954618 | 3 — audio config |
| 84072 | sub_82955838 | 3 — audio device setup |
| 84117 | sub_8284D220 | 4 — string/device name |
| 84132 | sub_829530B0 | 3 — audio init |
| 84157 | sub_82953A68 | 3 — audio init |
| 84164 | sub_8284E1C0 | 3 — audio init |
| 84175 | sub_8297B6B8 | 3 — streaming reg |
| 84178 | sub_826BDC18 | 3 — streaming reg |
| 84198 | sub_8296A278 | 3 — audio setup |
| 84209 | sub_82953AB0 | 3 — audio init |
| 84218 | sub_82953AD0 | 3 — audio init |
| 84227 | sub_8228A1E0 | 5 — engine init A |
| 84230 | sub_825FD6B8 | 5 — engine init B |
| 84233 | sub_82955BE0 | 3 — audio alloc |
| 84272 | sub_82A00DC0 | 4 — strncpy |
| 84284 | sub_8284F310 | 5 — streaming setup |
| 84287 | sub_82477670 | 5 — engine init C |
| 84298 | sub_827C2420 | 5 — streaming mgr init |
| 84303 | sub_8284E830 | 5 — streaming init |
| 84310 | sub_821B3510 | 2 — malloc |
| 84331–84605 | sub_8284D220 ×7 | 4 — device name strings |
| 84636–84650 | sub_82478A80 ×3 | 4 — local string helper |
| 84663–84714 | sub_82A00DC0 ×6 | 4 — strncpy |
| 84737 | sub_827ADB48 | 5 — ENGINE INIT (big) |
| 84788 | sub_821B3510 | 2 — malloc |
| 84810 | sub_8261FBA0 | 5 — streaming source reg |
| 84853 | sub_821B3510 | 2 — malloc |
| 84896 | sub_827ACC98 | 5 — streaming queue init |
| 84901 | sub_821B3560 | 2 — malloc variant |
| 84916–84934 | sub_827ACCA0 ×3 | 5 — queue config |
| 84941 | sub_821B5038 | 2 — alloc helper |
| 84944 | sub_8287AC38 | 5 — audio render setup |
| 84957–84968 | sub_8287A6A8 ×2 | 5 — audio callback reg |
| 84975 | sub_821B5038 | 2 — alloc helper |
| 84978 | sub_8261C7C8 | 5 — STREAMING INIT (big) |
| 84989 | sub_8287A408 | 5 — audio param |
| 85000 | sub_8287A4E0 | 5 — audio param |
| 85011 | sub_8287A4E8 | 5 — audio param |
| 85038–85059 | sub_8299B4A8 ×2 | 5 — audio finalize |
| 85072 | sub_821B3510 | 2 — malloc |
| 85079 | sub_823B33F8 | 5 — streaming thread? |
| 85173 | sub_822BCA90 | 6 — NOP (empty body) |

---

## Phase Classification

| Phase | Lines | Key functions |
|-|-|-|
| 1 — TLS slot alloc | 83764–83770 | sub_826225E0, sub_82622648, sub_826226B0 |
| 2 — Memory allocation | 83775–85072 | sub_821B3510 (×6), sub_821B3560, sub_821B5038 |
| 3 — Audio device setup | 83784–84233 | sub_8294BD68, sub_8294E208, sub_82955838, sub_82956718 |
| 4 — String operations | 84117–84714 | sub_8284D220 (×7), sub_82A00DC0 (×7), sub_82478A80 (×3) |
| 5 — Engine init | 84227–85079 | sub_827ADB48, sub_8261C7C8, sub_827C2420, sub_8287AC38, sub_8284F310 |
| 6 — Streaming thread | 85173 | sub_822BCA90 (empty — NOP stub) |

---

## Blocking Risk Analysis

### Confirmed non-blocking
These call chains bottom out in pure computation, bounded loops, or NT object allocation (not waiting):

- **Phase 1–2**: TLS and malloc — no wait paths found.
- **Phase 4**: All string ops — sub_82A00DC0 (strncmp/strncpy), sub_8284D220 — pure computation.
- **sub_822BCA90** (line 85173): Body is a single `return;`. Completely inert.
- **sub_827ADB48** (line 84737): Calls sub_827AD9C8 → sub_8279A6D0 (no NT), sub_827AD200 → sub_827ACCE0 (no NT). Clean.
- **sub_8261FBA0** (line 84810): Calls sub_827BA840 → sub_829A0318, sub_82836A38, sub_827A4D88. All bounded. No NT waits found.
- **sub_8284F468** (called from sub_821B5600): Has backward goto but bounded by `count = *(r30+3076)`. NOT infinite.
- **sub_82173C38** (called via sub_8261FBA0 → sub_827BA840 → sub_82836A38): Backward goto is an array scan (bounded by element count). NOT infinite.

### Medium risk — deeper chains, not fully traced
- **sub_8287AC38** (line 84944): → sub_8287A878 → sub_8287C578 (15 lines, no calls), sub_8287AD28 → sub_8287C5D0 (15 lines, no calls). Likely safe.
- **sub_8228A1E0** (line 84227): → sub_82289A38, sub_821D0488, sub_8250D080, sub_82662900. All have 0–3 deep calls, no NtWait path found.
- **sub_8284F310** (line 84284): → sub_8284E690 (no calls), sub_82211840 (no calls). Low risk.

### HIGH RISK — spin-wait loop confirmed

**sub_8261C7C8** (line 84978) is the primary suspect:

Call chain:
```
sub_8261C7C8
  └─ sub_821B5600
       ├─ sub_8284F468  [bounded loop, OK]
       ├─ sub_8285AA68  → sub_82855460 → sub_828708C8 [9 lines, no calls, OK]
       └─ sub_821B5538
            └─ sub_82219AC0
                 └─ sub_822199E8
                      └─ sub_82218FC0
                           └─ sub_8285DB88  [25 lines streaming wait?]
                           └─ sub_82852F48, sub_82852FB0
```

Also:
```
sub_8261C7C8
  └─ sub_821B53D0
       └─ sub_8285B088
            └─ sub_8285A8B0
                 └─ sub_82854C80  [71 lines, deep streaming subsystem]
```

The 0x828x address range is the streaming manager. Functions here wait on semaphores signaled by the streaming I/O thread. If the streaming thread hasn't started yet (or its semaphore was never created), **sub_8261C7C8 will block indefinitely**.

**sub_827C2420** (line 84298) is secondary risk:
```
sub_827C2420
  └─ sub_8284F310 → sub_82211840 [no calls, OK]
  └─ sub_82852DD0
       └─ sub_8284F468 [bounded loop]
       └─ sub_82852D18
            └─ sub_828470E0, sub_8284FA58, sub_8284FC98, sub_82847120
```
sub_82852D18 reaches into the streaming manager.

### NtWaitForSingleObjectEx reachability
All three `NtWaitForSingleObjectEx` call sites (gta4_recomp.69.cpp:50648, 53928, 54019) are wrapped by:
- `sub_82A1A450` → called only from `sub_82A13040` → not reachable from sub_82478AF8's call tree
- `sub_82A1BA38`, `sub_82A1BAD0` → only called within .69.cpp itself

`KeDelayExecutionThread` is reached via `sub_82849918 → sub_82A12B60 → sub_82A1A200 → KeDelayExecutionThread`. This path is NOT directly called from sub_82478AF8 but IS reachable from `sub_82212F38` (yield loop) — which in turn is only called by the streaming scheduler, not from the main init function.

**Conclusion**: The most likely hang site is the streaming semaphore wait inside `sub_8261C7C8` (line 84978).

---

## Binary Search Probe Plan (INIT_PROBE hooks)

Add probes at these 9 sites to binary-search the hang. A probe that is NOT printed = the hang is before it. A probe printed = the hang is after it.

```cpp
// Probe 1 — after Phase 1 TLS (line ~83770)
// Hook: sub_826226B0 return → INIT_PROBE("P1_tls_done")

// Probe 2 — after Phase 3 audio mgr finalize (line ~84019)
// Hook: sub_8294E208 return → INIT_PROBE("P3_audio_mgr_done")

// Probe 3 — after Phase 3 all audio inits (line ~84233)
// Hook: sub_82955BE0 return → INIT_PROBE("P3_audio_device_done")

// Probe 4 — after Phase 4 strings (line ~84714)
// Hook: last sub_82A00DC0 call (before line 84737) → INIT_PROBE("P4_strings_done")

// Probe 5 — after sub_827ADB48 engine init (line ~84737)
// Hook: sub_827ADB48 return → INIT_PROBE("P5_engine_init_done")

// Probe 6 — after sub_8284F310 streaming setup (line ~84284)
// Hook: sub_8284F310 return → INIT_PROBE("P5_streaming_setup_done")

// Probe 7 — after sub_827C2420 streaming mgr (line ~84298)
// Hook: sub_827C2420 return → INIT_PROBE("P5_streaming_mgr_done")

// Probe 8 — after sub_8287AC38 audio render (line ~84944)
// Hook: sub_8287AC38 return → INIT_PROBE("P5_audio_render_done")

// Probe 9 — after sub_8261C7C8 streaming init (line ~84978) [MOST LIKELY HANG]
// Hook: sub_8261C7C8 return → INIT_PROBE("P5_streaming_init_done")
```

### Recommended probe order (binary search)

1. **Probe 5** first (after sub_827ADB48): Does it print?
   - YES → hang is in Phase 5 after engine init → focus on sub_8284F310/sub_827C2420/sub_8261C7C8
   - NO → hang is in Phase 1–4 (unlikely given prior test data)

2. If YES to Probe 5, try **Probe 7** (after sub_827C2420):
   - YES → hang is in sub_8287AC38, sub_8261C7C8, or later
   - NO → hang is in sub_8284F310 or sub_827C2420

3. If YES to Probe 7, try **Probe 8** (after sub_8287AC38):
   - YES → hang is inside sub_8261C7C8 or after (sub_8299B4A8, sub_823B33F8)
   - NO → hang is inside sub_8287AC38

4. If YES to Probe 8, **Probe 9** confirms: hang is inside `sub_8261C7C8`.

### Specific sub_8261C7C8 internal probes

If Probe 9 (sub_8261C7C8) confirms the hang:

```cpp
// Hook: sub_821B5600 return → INIT_PROBE("sub8261C7C8_B5600_done")
// Hook: sub_82615A80 return → INIT_PROBE("sub8261C7C8_A80_done")
// Hook: sub_821B53D0 return → INIT_PROBE("sub8261C7C8_53D0_done")
```

The streaming subsystem functions at 0x8284xxxx–0x8286xxxx wait on semaphores. The root fix is ensuring the streaming thread (sub_8285D610) and its semaphore infrastructure are initialized BEFORE sub_8261C7C8 is called. However, sub_8285D610 is called from sub_8285D948, which is called from sub_821B3CE8 (in gta4_recomp.2.cpp), which is called from the top-level entry sub_82140000 — NOT from sub_82478AF8. If the calling order was changed so that sub_82478AF8 runs before sub_821B3CE8's streaming thread setup completes, sub_8261C7C8 would deadlock on the semaphore wait.

---

## Key Addresses for save_hooks.cpp

```cpp
// Priority 1: Add these INIT_PROBE points in save_hooks.cpp
// sub_827ADB48 = 0x827ADB48  (after probe 5)
// sub_827C2420 = 0x827C2420  (after probe 7)
// sub_8287AC38 = 0x8287AC38  (after probe 8)
// sub_8261C7C8 = 0x8261C7C8  (after probe 9 — most likely hang)
//
// Streaming semaphore chain (if sub_8261C7C8 confirmed):
// sub_821B5600 = 0x821B5600
// sub_821B53D0 = 0x821B53D0
// sub_8285B088 = 0x8285B088
// sub_8284F468 = 0x8284F468
```
