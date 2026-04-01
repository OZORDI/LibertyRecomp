# PPC Analysis: sub_821FC1F8 — Main World Init Chain

**Source**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.5.cpp` lines 989–1177
**Purpose**: Sequential initialization of all game subsystems during world startup
**Total calls**: 47 (46 unique — call 41 and 46 both call `sub_822BCA90`)

---

## Helper function reference

| Function | Role |
|-|-|
| `sub_821B3510` | `operator new` — reads TLS[r13+1676] → vtable[8] for heap dispatch |
| `sub_82507368` | `CPool::Init` — takes (pool_struct*, elem_size, count), mallocs + initializes pool |
| `sub_829FF840` | Unaligned memset (byte-fill loop) |
| `sub_8285D610` | `CPhysicsBody::Init` — initializes critical section + physics state struct |
| `sub_8285D948` | `CPhysicsSpace::Register` — creates rigid body, allocs 1080 bytes, calls D610 |
| `sub_82849A50` | `CPhysicsWorld::Create` — allocs physics world object (min 16384 budget) |

---

## Complete ordered call list

### PHASE 1 — PRE-HANG (calls 1–29, verified working)

These all follow the pattern: `operator new` → `CPool::Init(pool_struct, elem_size, count)`.
All have `D948=False, D610=False`.

| # | Address | File (line) | Pool/Global addr | Summary |
|-|-|-|-|-|
| 1 | `sub_8251BA08` | 25.cpp:29614 | 0x82023458 → pool 0x83030004 | Init streaming type-A pool; calls BA70+BAD8 sub-inits |
| 2 | `sub_8251BA70` | 25.cpp:29678 | 0x82023468 → pool 0x83030004 | Init streaming type-B pool; calls BAD8+BC50 |
| 3 | `sub_8254EE48` | 26.cpp:75365 | 0x82025918 → pool 0x83060010 | Init streaming pool-C; calls EEB0+EF68 |
| 4 | `sub_825256F8` | 25.cpp:53633 | 0x8202398C → pool 0x83030060 | Init streaming pool; calls 8244D9F8 + 8222CE48 |
| 5 | `sub_82504318` | 24.cpp:56631 | 0x82022584 → pool 0x83020060 | Init object pool (stride 1); calls 823B1BC0 + 823B62E0 |
| 6 | `sub_82326E10` | 12.cpp:11106 | 0x82011C70 → pool 0x82DE0060 | Init task/streaming2 pool; refs 0x82A9A164 (type name), calls 8244CE68 |
| 7 | `sub_82504900` | 24.cpp:57542 | 0x820226E4 → pool 0x83020060 | Init object pool-2; refs 0x820C0050 (vtable) |
| 8 | `sub_82374720` | 13.cpp:83071 | 0x820132E4 → pool 0x82FB0060 | Init drawable pool; calls 82372210 + 8251B9D0 |
| 9 | `sub_82523CF0` | 25.cpp:49682 | 0x82023488 → pool 0x83030060 | Init streaming-entity pool; refs 0x8202373C + 0x82B071A0 |
| 10 | `sub_82446BA8` | 19.cpp:37298 | 0x8201A7BC → pool 0x82FF0060 | CObject-group init A: calls sub-inits C10+C78+CE0+D48 sequentially |
| 11 | `sub_82446C78` | 19.cpp:37422 | 0x8201A7E0 → pool 0x82FF0060 | CObject-group init B: calls CE0+D48+DB0+E18 |
| 12 | `sub_82446C10` | 19.cpp:37360 | 0x8201A7CC → pool 0x82FF0060 | CObject-group init C: calls C78+CE0+D48+DB0 |
| 13 | `sub_82446CE0` | 19.cpp:37484 | 0x8201A800 → pool 0x82FF0060 | CObject-group init D: calls D48+DB0+E18+E80 |
| 14 | `sub_82446D48` | 19.cpp:37546 | 0x8201A818 → pool 0x82FF0060 | CObject-group init E: calls DB0+E18+E80; calls 82445380 (entity setup) |
| 15 | `sub_82446DB0` | 19.cpp:37608 | 0x8201A834 → pool 0x82FF0060 | CObject-group init F: calls 8279EB60/E9E0 (physical object VTBLs) |
| 16 | `sub_82446E18` | 19.cpp:37670 | 0x8201A84C → pool 0x82FF0060 | CObject-group init G: calls 8279EEE0 + 8244C388 (object type reg) |
| 17 | `sub_82412560` | 18.cpp:3409 | 0x82010D90 → pool 0x82FD0060 | CDecision/AI pool init; refs 0x820186CC (type data) |
| 18 | `sub_8254D230` | 26.cpp:71159 | 0x82025864 → pool 0x83060060 | Streaming resource pool; calls 8244C388 ×3 (register resource types) |
| 19 | `sub_8230AF58` | 11.cpp:21302 | 0x82010558 → pool 0x82D50060 | Task-manager pool init; calls 823C05E8 (mutex init) ×2 |
| 20 | `sub_8254A610` | 26.cpp:64396 | 0x8202549C → pool 0x83060060 | Streaming pool group: calls A678+A6E0 sub-inits |
| 21 | `sub_8254A6E0` | 26.cpp:64520 | 0x820254B8 → pool 0x83060060 | Streaming pool A6E0: calls 82624758 (GFX pool alloc), 82455C68 |
| 22 | `sub_8254A678` | 26.cpp:64458 | 0x820254A8 → pool 0x83060060 | Streaming pool A678: calls A6E0+A748+A758 + GFX alloc |
| 23 | `sub_823D46A0` | 15.cpp:83218 | 0x82015414 → pool 0x82FD0060 | Navigation/waypoint pool init; calls 826C4CA8 (path alloc) + 8227FAB0 |
| 24 | `sub_82549D88` | 26.cpp:63158 | 0x82025468 → pool 0x83060060 | Streaming pool-D; refs 0x82004680 (type), 0x82B08340 |
| 25 | `sub_82163F38` | 0.cpp:85484 | 0x820BC78C → pool 0x831D0060 | Core subsystem init (large BSS region); calls 82164B90 (register) |
| 26 | `sub_824E8C00` | 23.cpp:54518 | 0x82021294 → pool 0x83000060 | Entity-extended pool init; calls 823B62E0 (list init) + 82A02000 |
| 27 | `sub_823B31B8` | 15.cpp:2753 | 0x82FC94A0 | Sync/list init; calls 823B2F50 (uses float const 0x3333…≈0.2) + 823B3210 |
| 28 | `sub_8247E4C0` | 20.cpp:97715 | arg r3=0x82FF5518 | Display/renderer setup; takes display format string arg, calls 8247E4C8 + 821D5F70 |
| 29 | `sub_823C04B0` | 15.cpp:34111 | 0x82FD0001/0030 | Event/callback system init; calls 823C0558 ×2 + 823C05E8 (mutex) |

---

### HANG (call 30)

| # | Address | File (line) | Status |
|-|-|-|-|
| **30** | **`sub_82478AF8`** | **20.cpp:83744** | **THE HANG — XAudio device init** |

**Why it hangs**: Opens XAudio3D device — calls `sub_826225E0` (pool 4690 elems, 96B), `sub_82622648` (pool 864 elems, 176B), `sub_826226B0` (pool 90 elems, 752B), then `sub_8294BD68` → `sub_8293EA08` (XAudio3D init).
On PC the XAudio3D device does not exist; the Xbox 360 XAudio API call blocks or returns fatal error.
After the device allocation it calls `sub_82953088` (XAudio source voice), `sub_82954618` (set voice params), `sub_82955838` (buffer submit), `sub_8297B6B8` (start engine), `sub_82956718` (further setup).
**This call does NOT call `sub_8285D948` or `sub_8285D610` directly.**

---

### PHASE 2 — POST-HANG (calls 31–47)

| # | Address | File (line) | D948 | D610 | Summary |
|-|-|-|-|-|-|
| 31 | `sub_8225C010` | 7.cpp:45767 | N | N | Script/task world init; calls 829FF840 (memset), 8225C038+C070+C158 (scripting pools), 823B4148 |
| **32** | **`sub_82556190`** | **26.cpp:93071** | **Y** | **N** | **Physics space init — DANGEROUS; calls `sub_8285D948` (CPhysicsSpace::Register, allocs 1080 bytes, creates rigid body). Also calls 82849778 (physics memory pool, 32767 entries) and 82849A50 (CPhysicsWorld::Create, min 16384 budget).** |
| 33 | `sub_822F3740` | 10.cpp:73484 | N | N | World/streaming dispatch init; calls 824502D8 (ped slot table reset, uses 0.2 constant), 82434D60, 824FFAC0, 825ED090, 823C1E28, 822EEDF8 (calls 8235C370 + 82146690 = timer/thread init), 822EF7C0 |
| 34 | `sub_8237C250` | 14.cpp:3809 | N | N | Drawable/debug render init; calls 822BBF90, 8237AA00, then sub_8284E060 ×3 (stores to 0x82FC0000 offsets 1812/1816/1820 = 3 render class registrations) |
| 35 | `sub_822B2010` | 9.cpp:35039 | N | N | Audio manager init; calls 82146448 (audio type reg), 82288490 (bank lookup), reads audio pool via 82146690 + 82288490; refs 0x82D10050 (audio globals) |
| 36 | `sub_823BFA98` | 15.cpp:32626 | N | N | CWorld init (takes CWorld* r3, arg r3=0x82FC97A0 from lis/addi in caller); calls 824FFF28 + 821D0488, writes zeros to world struct fields, stores 0x40 entries |
| 37 | `sub_8228E3A0` | 8.cpp:54248 | N | N | Audio channel init; args r3=0x820024F0, r4=0x82003700 passed from caller; calls 8284F310 + 8284E830 (audio engine setup) ×2, 828C8898 ×8 (channel register loop), refs 0x82B07278 |
| 38 | `sub_822BCC20` | 9.cpp:60774 | N | N | Audio buffer pool init; calls 828C2448, 825232C0/E0, 8284F310 + 8284E830, 828C9728, 828C8898 ×3; refs 0x82D30070, 0x831C0070 (audio pool structs) |
| 39 | `sub_822BDD98` | 9.cpp:63396 | N | N | Audio secondary init; calls 828C2448, 8284F310 + 8284E830, 828C9728 ×2; refs 0x82D2B5E0, 0x82D2B190 |
| 40 | `sub_823A2108` | 14.cpp:95530 | N | N | Debug/log system init (args r3=2, r4-r7=0 from caller); calls 823A0DA8 (uses sprintf=0x82A00108 + 82A00DC0 = log formatting). Refs 0x82FB64B0/B8 (log buffers), 0x82FB0350 |
| 41 | `sub_822BCA90` | 9.cpp:60535 | N | N | Audio global table reset; calls CA98/AA8/AB8/AC8/B18/BA8/B18 chain then BCC20; refs 0x82D2B0B0, 0x82D30001 (audio controller structs) |
| 42 | `sub_82552D08` | 26.cpp:85067 | N | N | Streaming load-list pool init (328 slots × 82 entries, memset-zeroed); calls 8251BB98 (streaming reg), 821B3560 (free), refs 0x8305C5F0 (load list header) |
| 43 | `sub_821CA7E8` | 3.cpp:45378 | N | N | Render target init A; calls 821D0488 (returns 0x82CC1558 module ptr) + 8250D080 + 8250D168 ×2 |
| 44 | `sub_825169C8` | 25.cpp:17498 | N | N | Render target init B / LOD setup; calls 82516440, 821D0488, 8250D080, 82516B70, 8225CF80, 822F27C0; refs 0x82AB337C, 0x83015110 |
| 45 | `sub_825030B8` | 24.cpp:53975 | N | N | Streaming/LOD finalizer; calls 824FFF28 (streaming alloc), 825030E0, 82502058 ×2 (LOD table fill), 826C4CA8 ×2 (path/nav pool), 822A3AA0; refs 0x82B3A1A0 |
| 46 | `sub_822BCA90` | 9.cpp:60535 | N | N | Audio global table reset (second call — identical to call 41) |
| 47 | `sub_8251CA08` | 25.cpp:32144 | N | N | Final streaming/pool registration; calls 8251CA28+CA78+CA88, 8251BAD8 ×4 (streaming sub-init), 823F1B90, 82238C28 (object type lookup via table), 82371AB0+A88 (drawable type setup), 8251CB40; refs 0x83032964–70 |

---

## Danger summary

| Call | Address | Dangerous? | Reason |
|-|-|-|-|
| 30 | `sub_82478AF8` | **HANG** | XAudio3D device open — blocks on PC |
| 32 | `sub_82556190` | **D948=YES** | Calls `CPhysicsSpace::Register` (sub_8285D948) which calls `CPhysicsBody::Init` (sub_8285D610) |
| all others | various | Safe | No 8285D948/D610 calls in first 300 instructions |

---

## Pool region to subsystem mapping

| Pool global region | Subsystem |
|-|-|
| 0x83030xxx | Streaming type-A/B/D and streaming-entity pools |
| 0x83060xxx | Streaming extended (resource, LOD, drawables) |
| 0x83020xxx | Object pools |
| 0x82FF0xxx | CObject entity group pools (10–16) |
| 0x82FD0xxx | AI/Decision + Navigation pools |
| 0x82DE0xxx | Task/streaming2 pool |
| 0x82D5xxxx | Task-manager pool |
| 0x83000xxx | Entity-extended pool |
| 0x831D0xxx | Core subsystem (large BSS) |
| 0x82FB0xxx | Drawable/vehicle pools |

---

## Behavior after the hang is fixed

**Calls 31–29 (pre-hang)** — already working. These are all CPool::Init calls: safe, deterministic, no I/O.

**Call 30 (hang)** — once XAudio hook is in place, the function should proceed through all sub_8294/8296 calls and return normally. The function has no loop/spin wait visible in 600 lines; the hang is in the XAudio3D device open.

**Calls 31–47 (post-hang) — expected outcomes:**

- Calls 31, 33–35, 40–47: Fully safe, no dangerous sub-calls. Will succeed after hang is fixed.
- **Call 32 (`sub_82556190`)**: Calls `sub_8285D948` (physics body creation). This allocates 1080 bytes via `operator new` and calls `RtlInitializeCriticalSection` (via `sub_8285FE48`). This should work on PC provided `operator new` (TLS[1676] heap) is set up before call 30. Given this is post-hang and call 30 uses the same heap, it will work once call 30 completes.
- Calls 37–39 (audio channel/buffer inits): Reference 0x8284xxxx / 0x828Cxxxx audio engine functions. These are the RAGE audio subsystem inits (not XAudio directly). Should succeed if XAudio device (call 30) succeeded.
- Call 36 (`sub_823BFA98`): Takes the CWorld pointer as argument (r3 loaded from 0x82FC97A0 region). Writes zeros to world struct at offsets up to 3592+. Should succeed.
- Call 44 (`sub_825169C8`): Calls `sub_8225CF80` and `sub_822F27C0` which appear to be CStreaming callbacks — safe.
- Call 47 (`sub_8251CA08`): Final registration call — calls `sub_82238C28` which dispatches through a function table. If the table is populated (calls 1–29 complete), this is safe.

**Conclusion**: Call 32 is the only post-hang call with `sub_8285D948` involvement; it should work on PC because it only uses heap allocation + critical section init (both PC-native via rexcrt hooks). The remaining post-hang calls 33–47 are deterministic inits with no platform-specific blocking.

---

## Key addresses for hooks/investigation

| Address | Likely role |
|-|-|
| `sub_82478AF8` | XAudio device init — THE hang; needs XAudio hook |
| `sub_826225E0` | XAudio pool alloc (4690 × 96B) |
| `sub_82622648` | XAudio pool alloc (864 × 176B) |
| `sub_826226B0` | XAudio pool alloc (90 × 752B) |
| `sub_8294BD68` | XAudio3D engine create |
| `sub_8293EA08` | XAudio3D internal init (deepest blocked call) |
| `sub_8285D948` | CPhysicsSpace::Register (post-hang, call 32) |
| `sub_8285D610` | CPhysicsBody::Init (called by D948) |
| `sub_82849A50` | CPhysicsWorld::Create (post-hang, call 32) |
