# LibertyRecomp Documentation Index

> **Project:** LibertyRecomp - GTA IV Xbox 360 Static Recompilation
> **Last Updated:** December 20, 2024

---

## Quick Links

| Need To | Read This |
|---------|-----------|
| **READ THE PLAYBOOK:** Rewrite strategy | [REWRITE_PLAYBOOK.md](REWRITE_PLAYBOOK.md) |
| Understand what's blocking rendering | [blocking_issues_summary.md](blocking_issues_summary.md) |
| See fix priorities | [fix_priority_matrix.md](fix_priority_matrix.md) |
| Find a specific function | [xbox360_function_inventory.md](xbox360_function_inventory.md) |
| Understand threading issues | [worker_threads_analysis.md](worker_threads_analysis.md) |
| Learn about shader format | [rage_fxc_format.md](rage_fxc_format.md) |

---

## Documentation Overview

### Total Size: ~530KB across 17 new documents

| Document | Size | Purpose |
|----------|------|---------|
| **REWRITE_PLAYBOOK.md** | 90KB | **Master rewrite strategy & module inventory** |
| **ARCHITECTURE_REFERENCE.md** | 18KB | Consolidated subsystem/call graph/structures |
| **RUNTIME_BEHAVIOR.md** | 16KB | State machines, threading, async I/O reference |
| **RENDERER_REFERENCE.md** | 80KB | Combined renderer analysis |
| xbox360_function_inventory.md | 108KB | Master function inventory (267 functions) |
| PPC_REWRITE_GUIDE.md | 25KB | Developer rewrite guide (43,651 functions mapped) |
| sonic06_vs_gta4_functions.md | 10KB | Function address differences between games |
| dependency_graph.md | 23KB | Visual dependency chains |
| kernel_imports_analysis.md | 11KB | Import implementation status |
| blocking_issues_summary.md | 10KB | Executive summary of blockers |
| device_context_structure.md | 9KB | GPU context layout |
| ppc_file_index.md | 9KB | PPC file organization |
| rage_fxc_format.md | 9KB | Shader file format |
| fix_priority_matrix.md | 9KB | Priority-ranked fixes |
| magic_values_reference.md | 8KB | Return value meanings |

---

## By Category

### 🚨 Critical Issues

| Document | Description |
|----------|-------------|
| [blocking_issues_summary.md](blocking_issues_summary.md) | Executive summary of all blocking issues |
| [fix_priority_matrix.md](fix_priority_matrix.md) | Priority-ranked list of required fixes |

### 📋 Function Reference

| Document | Description |
|----------|-------------|
| [xbox360_function_inventory.md](xbox360_function_inventory.md) | Complete inventory of 155 Xbox 360 functions |
| [kernel_imports_analysis.md](kernel_imports_analysis.md) | Analysis of ~84 kernel imports |
| [ppc_file_index.md](ppc_file_index.md) | Index of 90 PPC recompiled files |

### 🔄 State Machines & Flow

| Document | Description |
|----------|-------------|
| [state_machines_analysis.md](state_machines_analysis.md) | 10 state machines documented |
| [dependency_graph.md](dependency_graph.md) | Visual call chains and dependencies |
| [magic_values_reference.md](magic_values_reference.md) | Return value protocol (996, 997, 258, 259) |

### 🧵 Threading & Runtime

| Document | Description |
|----------|-------------|
| [RUNTIME_BEHAVIOR.md](RUNTIME_BEHAVIOR.md) | State machines, worker threads, and XXOVERLAPPED semantics |

### 🎮 GPU & Rendering

| Document | Description |
|----------|-------------|
| [device_context_structure.md](device_context_structure.md) | D3D-like device context (14KB struct) |
| [rage_fxc_format.md](rage_fxc_format.md) | RAGE shader container format |
| [RENDERER_REFERENCE.md](RENDERER_REFERENCE.md) | Combined renderer analysis |

### 🔧 Build & Installation

| Document | Description |
|----------|-------------|
| [BUILDING.md](BUILDING.md) | Build instructions |
| [INSTALLATION_ARCHITECTURE.md](INSTALLATION_ARCHITECTURE.md) | Installation system design |
| [SHADER_PIPELINE.md](SHADER_PIPELINE.md) | Shader extraction and compilation |
| [RPF_EXTRACTION_DESIGN.md](RPF_EXTRACTION_DESIGN.md) | RPF archive handling |
| [VFS_ADVANCED_FEATURES.md](VFS_ADVANCED_FEATURES.md) | Virtual filesystem features |
| [DUMPING-en.md](DUMPING-en.md) | Game dumping instructions |

---

## Function Inventory Summary

### By Category (155 total)

| Category | Count | Status |
|----------|-------|--------|
| GPU | 35 | Mostly hooked |
| XAM/Kernel | 20 | Needs work |
| Boot | 20 | Hooked |
| Timing/Sync | 15 | Mixed |
| Threading | 15 | Partial |
| FileIO | 12 | Implemented |
| Memory | 8 | Implemented |
| Video | 8 | Partial |
| Audio | 5 | Stub |
| Network | 4 | Stub |
| Input | 3 | Needs impl |
| Profile | 3 | Stub |
| Error | 3 | Stub |
| Crypto | 2 | Needs impl |
| Config | 2 | Stub |

### Implementation Status

| Status | Count | % |
|--------|-------|---|
| ✅ Implemented | 45 | 29% |
| ⚠️ Partial | 35 | 23% |
| ❌ Stub | 50 | 32% |
| ❌ Broken | 10 | 6% |
| 🔄 Bypass | 15 | 10% |

---

## Critical Path

```
Fix These First (P0):
1. XamTaskShouldExit → return 0, not 1
2. XamTaskSchedule → implement task creation
3. EffectManager → parse FXC, load shaders

Then These (P1):
4. Semaphore signaling → wake render workers
5. XXOVERLAPPED events → complete async I/O
6. GPU memory allocation → proper resource creation
```

---

## Key Addresses

| Address | Purpose |
|---------|---------|
| 0x830E5900 | Shader table (128 slots) |
| 0xA82487F0 | Render worker #1 semaphore |
| 0xA82487B0 | Render worker #2 semaphore |
| 0xA824E9C0 | GPU ring buffer |
| 0x82003890 | Corrupted stream object |

---

## Key Functions

| Function | Purpose | Status |
|----------|---------|--------|
| sub_8218BEA8 | Main entry | ✅ Hooked |
| sub_82120000 | One-time init | ✅ Hooked |
| sub_8285E048 | EffectManager | ❌ Stubbed |
| sub_829D8860 | DrawPrimitive | ✅ Hooked (never called) |
| sub_827DE858 | Render worker | ❌ Blocked |

---

## Document History

| Date | Changes |
|------|---------|
| 2024-12-20 | Initial comprehensive documentation created |
| | 12 new documents, ~320KB total |
| | 155 functions documented |
| | All critical blocking issues identified |

---

## Navigation

### By Document Type

**Analysis Documents:**
- blocking_issues_summary.md
- state_machines_analysis.md
- worker_threads_analysis.md
- kernel_imports_analysis.md
- xxoverlapped_analysis.md

**Reference Documents:**
- xbox360_function_inventory.md
- device_context_structure.md
- magic_values_reference.md
- ppc_file_index.md
- rage_fxc_format.md

**Planning Documents:**
- fix_priority_matrix.md
- dependency_graph.md

**Build/Install Documents:**
- BUILDING.md
- INSTALLATION_ARCHITECTURE.md
- SHADER_PIPELINE.md
- RPF_EXTRACTION_DESIGN.md
- VFS_ADVANCED_FEATURES.md

---

## Contributing

When adding new documentation:

1. Follow the existing format with headers, tables, and code blocks
2. Include function addresses in `sub_XXXXXXXX` format
3. Document Xbox 360 expectations vs current behavior
4. Add to this index file
5. Update xbox360_function_inventory.md for new functions

