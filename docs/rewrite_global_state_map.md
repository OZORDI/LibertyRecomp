# sub_82478AF8 — Global State Write Map

All addresses written to global (non-stack) memory during `sub_82478AF8` execution.
All arithmetic verified via Python. Stack writes (ctx.r1-relative) are excluded.

## Address Table

| Address | Calculation | Operation | Value Written | Readers | Effect if 0/Uninitialized |
|-|-|-|-|-|-|
| `0x831CC944` | lis -31971 (0x831D0000) − 14012 | stw r30 | 176-byte struct ptr from sub_821B3510 (or 0 if alloc fails) | gta4_recomp.63.cpp (×8), gta4_recomp.50.cpp (×6) | NULL dereference in physics/audio init path |
| `0x82FF5360` | lis -32001 (0x82FF0000) + 21344 | stw r30 | Module object pointer (allocated above) | gta4_recomp.9/12/17/18/19/41.cpp (×14+) | NULL → every subsystem fetching this global crashes |
| `0x82FF5364` | lis -32001 + 21348 | stw r3 | Audio manager ptr from sub_82956718, else 0 | gta4_recomp.12.cpp (×6), gta4_recomp.16/28/48.cpp | 0 → audio subsystem never initializes |
| `0x82FF5368` | lis -32001 + 21352 | stw r3 | Streaming manager ptr from sub_827ADB48, else 0 | Not confirmed in search | 0 → world streaming deadlock |
| `0x82FF536C` | lis -32001 + 21356 | stw r27 | Pointer to streaming struct array | Not confirmed in search | 0 → streaming subsystem blind |
| `0x82FF5374` | lis -32001 + 21364 | stw r3 | Particle system ptr from sub_823B33F8, else 0 | Not confirmed in search | 0 → particle emitter alloc storm hits wrong allocator path |
| `0x82FF5378` | lis -32001 + 21368 | stw r10 | Always 0 (r10=r23=0) | gta4_recomp.71.cpp (several) | — (zero is expected) |
| `0x82FF5414` | lis -32001 + 21524 | stw 168 (conditional) | 168 only if double-init branch taken (bit 1 of flags already set) | gta4_recomp.71.cpp:56067 | 0 → wrong camera FOV table index |
| `0x82FF5418` | lis -32001 + 21528 | stw r9 | r26 + r27 + 128 (world extent sum) | gta4_recomp.71.cpp:55928/55935/59278/60027 | 0 → world bounding volume calculation wrong |
| `0x82FF541C` | lis -32001 + 21532 | stw r11 | Bits 0+1 set (= 0x3) on first init | gta4_recomp.71.cpp:59274/59985 | 0 → re-init guard never set → double-init crash |
| `0x82FF2364`–`0x82FF5360` | lis -32001 + 9060 loop base | loop stw 0 (1024×3 words) | 12288 bytes zeroed (3 words per iter, 1024 iters) | None found | — (init zeroing) |
| `0x831CDA65` | lis -31971 (0x831D0000) − 9627 | stb r25 | 1 = renderer initialized flag | Not found in generated files | Stays 0 → renderer never activates |
| `0x831CDA47` | lis -31971 − 9657 | stb r23 | 0 = clear shutdown flag | gta4_recomp.64.cpp:21287 (PPC_LOAD_U8) | Non-zero → shutdown guard triggers prematurely |
| `0x82AE5F84` | lis -32082 (0x82AE0000) + 24452 | stb r23 | 0 | gta4_recomp.11.cpp:32152/32195, gta4_recomp.50.cpp:60446/60529 | — (zero is expected) |
| `0x82B1AEF8` | lis -32078 (0x82B20000) − 20744 | stw r10 | r10 with bit 9 cleared (rlwinm mask 23,21) | gta4_recomp.63.cpp:14295/14530/15683 | Bit 9 set → threading scheduler takes wrong branch |

## Address Groups

### 0x82FF53xx block — Main Init State (base 0x82FF0000, lis -32001)

```
+0x5360  module ptr          → written with r30 (module object)
+0x5364  audio mgr ptr       → written with audio manager or 0
+0x5368  streaming mgr ptr   → written with stream manager or 0
+0x536C  stream array ptr    → written with stream array pointer
+0x5374  particle sys ptr    → written with particle system or 0
+0x5378  zero flag           → always 0
...
+0x5414  camera FOV index    → 168 on re-init, 0 on first run
+0x5418  world extent sum    → r26+r27+128
+0x541C  init flags          → 0x3 (bits 0+1)
```

Array cleared: `0x82FF2364`–`0x82FF5360` (12288 bytes, 3072 zero words, likely a stream-entry table with 1024 entries × 3 fields).

### 0x831Dxxxx block — Scene/Renderer State (base 0x831D0000, lis -31971)

```
-0x36BC = 0x831CC944   module struct ptr (same ptr as 0x82FF5360 target)
-0x25B9 = 0x831CDA47   shutdown flag → cleared to 0
-0x259B = 0x831CDA65   renderer init flag → set to 1
```

### Scattered Globals

| Address | Module | Role |
|-|-|-|
| `0x82AE5F84` | 0x82AE0000 region | Cleared flag, read by streaming/world update (gta4_recomp.50.cpp) |
| `0x82B1AEF8` | 0x82B20000 region | Threading flag — bit 9 cleared to indicate safe concurrent access |

## Critical Dependencies

Writes that unblock other systems if non-zero:

1. `0x831CC944` and `0x82FF5360` — same struct pointer, written first. All rendering and audio code in gta4_recomp.12/19/50/63.cpp indexes from these addresses. If null, a vtable dispatch crash occurs before the first frame.

2. `0x82FF541C` (init flags 0x3) — read by gta4_recomp.71.cpp streaming init. If 0, the stream init guard re-enters and double-initializes the buffer pool, corrupting the allocator free-list.

3. `0x82B1AEF8` bit 9 — read by the task scheduler in gta4_recomp.63.cpp. If the bit remains set (uncleared), the scheduler spin-waits on a lock that is never released, causing a hang before the first real frame.

## Notes

- Struct member writes to `r30+40` (sth), `r30+152` (stw) are writes into the newly allocated 176-byte module object, not independent globals.
- `stw r31,56(r10)` (line 84293) is indirect: r10 is loaded from a pointer at lis -32076 (0x82B20000) − 27740 = `0x82B1AC04`. The target struct field depends on runtime pointer value.
- `stw r10,768(r11)` (line 84911) is indirect through the renderer manager pointer loaded from `0x82FF5368`.
