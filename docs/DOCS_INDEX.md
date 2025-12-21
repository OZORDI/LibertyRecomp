# LibertyRecomp Documentation Index

> **Project:** LibertyRecomp - GTA IV Xbox 360 Static Recompilation
> **Last Updated:** December 20, 2025

---

## Quick Links

| Need To | Read This |
|---------|-----------|
| **NEW DEVELOPER START HERE** | [REWRITE_PLAYBOOK.md](REWRITE_PLAYBOOK.md) |
| Understand runtime behavior | [RUNTIME_BEHAVIOR.md](RUNTIME_BEHAVIOR.md) |
| Find function addresses | [ARCHITECTURE_REFERENCE.md](ARCHITECTURE_REFERENCE.md) |
| Understand GPU strategy | [RENDERER_REFERENCE.md](RENDERER_REFERENCE.md) |
| Learn shader pipeline | [SHADER_PIPELINE.md](SHADER_PIPELINE.md) |
| Build the project | [BUILDING.md](BUILDING.md) |
| Detailed dev notes | [NOTES.md](NOTES.md) |

---

## Documentation Overview

### Core Handoff Documents (Start Here)

| Document | Purpose |
|----------|---------|
| **[REWRITE_PLAYBOOK.md](REWRITE_PLAYBOOK.md)** | **Master module handoff document** - Per-module rewrite specs with functions, test cases, implementation notes |
| **[RUNTIME_BEHAVIOR.md](RUNTIME_BEHAVIOR.md)** | State machines, kernel API behavior, cooperative polling patterns, worker threads |
| **[ARCHITECTURE_REFERENCE.md](ARCHITECTURE_REFERENCE.md)** | Subsystem address map, call graphs, data structures |
| **[RENDERER_REFERENCE.md](RENDERER_REFERENCE.md)** | GPU rendering strategy, PM4 bypass, D3D hook functions |

### Build & Installation

| Document | Purpose |
|----------|---------|
| [BUILDING.md](BUILDING.md) | Build instructions for all platforms |
| [INSTALLATION_ARCHITECTURE.md](INSTALLATION_ARCHITECTURE.md) | Installation system design |
| [SHADER_PIPELINE.md](SHADER_PIPELINE.md) | Shader extraction, XenosRecomp, cache format |
| [RPF_EXTRACTION_DESIGN.md](RPF_EXTRACTION_DESIGN.md) | RPF archive handling |
| [VFS_ADVANCED_FEATURES.md](VFS_ADVANCED_FEATURES.md) | Virtual filesystem features |
| [DUMPING-en.md](DUMPING-en.md) | Game dumping instructions |

### Reference & Notes

| Document | Purpose |
|----------|---------|
| [NOTES.md](NOTES.md) | Comprehensive development notes (123KB) - boot analysis, crash investigation, RAGE engine docs |
| [TITLE_UPDATE_SYSTEM.md](TITLE_UPDATE_SYSTEM.md) | Title update handling |

---

## Module Priority & Status

### P0 - Critical (Blocking Boot)

| Module | Status | Key Functions |
|--------|--------|---------------|
| **Timing & Sync** | Partial | `KeWaitForSingleObject`, VBlank callback |
| **XAM Task System** | Implemented | `XamTaskSchedule`, `XamTaskShouldExit` |

### P1 - High (Blocking Render)

| Module | Status | Key Functions |
|--------|--------|---------------|
| **GPU Rendering** | Partial | `DrawPrimitive`, `Present`, shader binding |
| **Boot State Machine** | Running | `sub_82120000`, `sub_8218C600` |
| **Worker Threads** | Blocked | Render workers on semaphores |

### P2 - Medium (Feature Completion)

| Module | Status | Key Functions |
|--------|--------|---------------|
| **Shader System** | Stubbed | `EffectManager::Load`, shader table |
| **File System** | Implemented | `NtCreateFile`, `NtReadFile`, async I/O |

### P3 - Low (Polish)

| Module | Status | Key Functions |
|--------|--------|---------------|
| **Audio System** | Stub | Audio buffer submission |
| **Input System** | Implemented | `XamInputGetState` |

### Port Features (Future Enhancements)

| Feature | Status | See Section |
|---------|--------|-------------|
| **Save Data System** | Basic stubs | REWRITE_PLAYBOOK.md §14.1 |
| **Achievements** | Stub only | REWRITE_PLAYBOOK.md §14.2 |
| **Network/Multiplayer** | Stub (offline) | REWRITE_PLAYBOOK.md §14.3 |
| **Localization** | Not implemented | REWRITE_PLAYBOOK.md §14.4 |
| **High Fidelity Renderer** | Foundation exists | REWRITE_PLAYBOOK.md §14.5-14.6 |
| **High Frame Rate** | Requires timing rewrite | REWRITE_PLAYBOOK.md §14.7 |
| **Ultrawide Support** | Not implemented | REWRITE_PLAYBOOK.md §14.8 |
| **Extended Controller** | Basic SDL | REWRITE_PLAYBOOK.md §14.9 |
| **Low Input Latency** | Not implemented | REWRITE_PLAYBOOK.md §14.10 |

---

## Key Addresses Quick Reference

| Address | Purpose |
|---------|---------|
| 0x82120000 | One-time init function |
| 0x8218BEA8 | Game entry point |
| 0x829D5388 | Present/VdSwap |
| 0x829D8860 | DrawPrimitive |
| 0x8285E048 | EffectManager::Load |
| 0x830E5900 | Shader table (128 slots) |
| 0xA82487F0 | Render worker #1 semaphore |
| 0xA82487B0 | Render worker #2 semaphore |

---

## Magic Return Values

| Value | Meaning | Action |
|-------|---------|--------|
| 996 | No progress | Retry later |
| 997 | Pending | Store state, return |
| 258 | Mapped to 996 | Retry |
| 259 | Wait required | Call NtWaitForSingleObjectEx |
| 257 | Retry trigger | Re-attempt wait |

---

## PPC Recompiled Code Quick Reference

The recompiled PowerPC code in `/LibertyRecompLib/ppc/` contains **43,650 functions** across 54 source files (~300,000 lines).

| Address Range | Content | Key Functions |
|---------------|---------|---------------|
| 0x82120000-0x821FFFFF | Boot/init | `sub_82120000` (one-time init) |
| 0x8218xxxx | Thread creation | `sub_8218C600` (creates 9 workers) |
| 0x827Dxxxx | Workers | `sub_827DE858` (render worker) |
| 0x829Axxxx | Async I/O | `sub_829A1F00`, `sub_829A3318` |
| 0x829Dxxxx | **GPU/D3D** | `sub_829D8860` (DrawPrimitive), `sub_829D5388` (Present) |

See **REWRITE_PLAYBOOK.md §11-13** for complete PPC code documentation.

---

## Document History

| Date | Changes |
|------|---------|
| 2025-12-21 | **Added Port Features Roadmap (§14-15)** |
| | Save data, achievements, network, localization |
| | Renderer enhancements, HFR, ultrawide, controller features |
| | XAM subsystem reference with function map |
| 2025-12-20 | Major documentation overhaul |
| | REWRITE_PLAYBOOK.md expanded with per-module handoff specs |
| | RUNTIME_BEHAVIOR.md updated with kernel API details |
| | **Added PPC recompiled code reference (§11-13)** |
| | Documented renderer, thread system, async I/O rewrites |

---

## For New Developers

**Start with these documents in order:**

1. **[REWRITE_PLAYBOOK.md](REWRITE_PLAYBOOK.md)** - Module inventory, function tables, test cases
2. **[RUNTIME_BEHAVIOR.md](RUNTIME_BEHAVIOR.md)** - Kernel API behavior, polling patterns
3. **[ARCHITECTURE_REFERENCE.md](ARCHITECTURE_REFERENCE.md)** - Address maps, data structures
4. **[NOTES.md](NOTES.md)** - Detailed investigation history

**Key source files:**
- `LibertyRecomp/kernel/imports.cpp` - Kernel API implementations
- `LibertyRecomp/kernel/xam.cpp` - XAM API implementations
- `LibertyRecomp/gpu/video.cpp` - GPU hooks
- `LibertyRecompLib/ppc/` - Recompiled PPC code (43,650 functions)

