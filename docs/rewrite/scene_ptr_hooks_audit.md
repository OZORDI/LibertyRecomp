# Scene Pointer Hooks Audit (2026-03-28)

## Search Results

### 831C2458 (scene pointer global)
| File | Line | Context |
|-|-|
| kernel/imports.cpp | 1928 | Comment: scene pointer at 0x831C2458 remains NULL if state machine never reaches state 3+ |
| kernel/imports.cpp | 2032 | `PPC_LOAD_U32(0x831C2458)` in FRAME-UPDATE hook (sub_82142F90) — read-only diagnostic |
| gpu/video.cpp | 9399 | Comment: Scene ptr: r11=0x831C2458 |
| gpu/video.cpp | 9402 | `PPC_LOAD_U32(0x831C2458)` in RENDER-GATE diagnostic — read-only |

### RENDER-GATE
| File | Line | Context |
|-|-|
| gpu/video.cpp | 9409 | printf("[RENDER-GATE] frame#%d gate=%d scene@831C2458=...") — diagnostic only |

### 8290B48C (WRONG address) vs 82B0B48C (CORRECT render gate)
| File | Line | Context |
|-|-|
| gpu/video.cpp | 9398 | Comment references 0x8290B48C (wrong) |
| gpu/video.cpp | 9401 | `PPC_LOAD_U32(0x8290B48C)` — **BUG: reads wrong address** |
| docs/rewrite/frame_tick_scene_dispatch.md | multiple | Documents the typo: 0x8290B48C is 2 MB below correct 0x82B0B48C |

**The render gate address bug in video.cpp:9401 is known but NOT yet fixed.** The diagnostic reads garbage from 0x8290B48C instead of the actual gate at 0x82B0B48C.

### "scene" (case-insensitive) — key hits only
| File | Area | Purpose |
|-|-|
| kernel/imports.cpp | Lines 1924-2141 | SCENE CREATION PATH DIAGNOSTICS — full state machine tracing (states 0-6) |
| kernel/imports.cpp | sub_82240B08 hook | Sets g_sceneReady (0x82BF3A77) and g_contentReady (0x82BF3CDA) |
| kernel/imports.cpp | sub_822438B0 hook | STATE 6 INNER — reads sceneState (0x82BF9848) and sceneObj (0x82BF3A88) |
| kernel/memory.cpp | Line 165-169 | Comment about scene never created if state 0 loops |
| gpu/video.cpp | Lines 9397-9412 | RENDER-GATE diagnostic reading scene ptr and vtable |
| kernel/game_init.cpp | N/A | No scene references — handles phases 1-7 of engine init |
| ui/options_menu.cpp | Line 873 | CutsceneAspectRatio option (unrelated) |

### sub_828C15C8, sub_828BF3C8, sub_828C1228 — render dispatch functions
| Function | Hooked? | References |
|-|-|
| sub_828C15C8 | **NO** | Only appears as a comment in video.cpp:9397 ("trace the render dispatch path in sub_828C15C8") |
| sub_828BF3C8 | **NO** | Zero references in any .cpp/.h file |
| sub_828C1228 | **NO** | Zero references in any .cpp/.h file |

**None of these three render dispatch functions have hooks.** They exist only as recompiled PPC code and run unmodified.

## Summary

1. **0x831C2458** (scene pointer) is read in two places (imports.cpp, video.cpp) — both are diagnostic-only printf statements. No hook writes to it; it is populated by the game's scene creation state machine.

2. **Render gate address bug**: video.cpp:9401 reads 0x8290B48C instead of 0x82B0B48C. This makes the RENDER-GATE diagnostic line print misleading gate values. The bug is documented in `frame_tick_scene_dispatch.md` but not yet patched in code.

3. **No hooks exist** for sub_828C15C8 (main render dispatch), sub_828BF3C8, or sub_828C1228. These functions run as vanilla recompiled PPC code. If the scene pointer is NULL when they execute, they will skip rendering (null check in the vtable dispatch path).

4. **game_init.cpp/h** handles engine initialization phases 1-7 but does not touch scene creation or the render gate. Scene creation is driven by the front-end state machine (sub_82142230, states 4-6) which has extensive hooks in imports.cpp.

5. **Scene creation flow**: The state machine hooks (sub_822414E8 through sub_822438B0) trace but do not force scene creation. The actual scene object at 0x82BF3A88 is created by the 15-state sub_82242910. The global scene list pointer at 0x831C2458 is set later when the scene is registered.
