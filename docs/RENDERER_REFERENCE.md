# Renderer Reference

## Purpose
Summarizes the GTA IV rendering analysis from both renderer_analysis.md and renderer_analysis_claude.md so developers see the high-level anti-goals, confirmed hooks, missing functions, PM4 bypass strategy, shader pipeline status, and boot-block implications in one place.

---

## 1. Anti-Goals & Strategy
- **Reject PM4 decoding** and ring-buffer emulation; hook the D3D-like wrapper layer instead.
- **Adopt Sonic Unleashed model:** intercept `CreateDevice`, `Draw*`, `Set*`, use precompiled shaders, bypass command buffers, stub kernel/VdSwap interactions.
- **Hook layer 1 functions** (addresses in 0x829C/0x829D ranges); treat PM4 builders as bypassed and kernel ring buffer as stubbed.

---

## 2. Confirmed Hooks & Candidates
| Address | Function | Action | Notes |
|---------|----------|--------|-------|
| `0x829D87E8` | `CreateDevice` | Hooked | Calls `VdInitializeEngines` |
| `0x829D5388` | `Present`/`VdSwap` | Hooked | Calls `__imp__VdSwap` |
| `0x829D3400` | `CreateTexture` | Stubbed | Resource creation pattern |
| `0x829D3520` | `CreateVertexBuffer` | Stubbed | Resource creation pattern |
| `0x829DFAD8` | `GpuMemAlloc` | Stubbed | Fake GPU offsets |
| `0x8285E048` | `EffectManager::Load` | Stub | Shader loading entry |
| `0x829D8860` | `DrawPrimitive` | Hook ✓ | Candidate for native draws |
| `0x829D4EE0` | `UnifiedDraw` | Hook ✓ | Handles indexed and non-indexed draws |
| `0x829CD350`/`0x829D6690` | Shader binding | Hook ✓ | VS/PS state writes |
| `0x829C9070`/`0x829C96D0` | Vertex/index binding | Hook ✓ | Device state updates |
| `0x829D3728`/`0x829C9440` | Texture/decl | Hook ✓ | Frequent per-frame binds |

---

## 3. Missing Functions to Find
- DrawPrimitive/DrawIndexedPrimitive: should accept 4–5 args, call PM4 builders, located near 0x826Fxxxx.
- SetVertexShader/SetPixelShader: 2-arg functions storing shader pointers at offsets +10932/+10936.
- SetRenderState/SetViewport/Clear: many small functions in 0x829E range writing device context offsets.

---

## 4. Render Flow & PM4 Bypass
- **Call path:** `sub_827D89B8` → render dispatch → `sub_828529B0` → `sub_828507F8` → `sub_829D5388` → `VdSwap`.
- **Draw path:** Game calls `sub_829D8860`, loads state from device context, then writes to PM4 buffers via `sub_829D7E58`/`sub_829D8568`; host should hook before these builders and issue draws directly.
- **State functions:** offsets map to render state fields (blend, cull, depth, textures, streams, shaders, index buffer). Host tracks dirty flags and updates pipelines on change.
- **Hooks for defunct PM4 builders** should either be stubbed (`sub_829D7E58`, `sub_829D8568`, `sub_829D7740`) or made to return harmless success.

---

## 5. Shader Pipeline Notes
- FXC files (`common.rpf/shaders/fxl_final/`) contain Xbox 360 shader containers; 1132 shaders precompiled via XenosRecomp.
- `EffectManager::Load` needs to return real shader handles, not just `g_shaderCacheEntries` placeholders.
- Shader binding occurs via `SetVertexShader`/`SetPixelShader`; hooks must mark pipelines dirty and flush before draws.
- Dirty state optimization (like Sonic Unleashed) keeps pipeline/state changes minimal.

---

## 6. Blocking Conditions
- Boot never reaches draw path because XAM tasks stubbed (`XamTaskSchedule` no-op, `XamTaskShouldExit` returns 1) — async workers exit before signaling completion.
- Without valid shaders, draw loops never issue commands; PM4 buffers remain static.
- GPU/boot busy-waits (e.g., `sub_8298E810`, `sub_82120000`) idle waiting for signals that never arrive; host must force event/fence completions.

---

## 7. Validation Checklist
1. `DrawPrimitive`/`UnifiedDraw` hooks receive ~961 calls per frame.
2. Shader binding logs show non-null host shaders before draws.
3. Frame graph: `CreateDevice` → host pipeline creation → `Present` calls `VdSwap` consistently.
4. PM4 builders never executed (returns early) while host issues real draw calls.

---

## Document History
- 2025-12-20: Consolidated both renderer analyses into `RENDERER_REFERENCE.md`.
