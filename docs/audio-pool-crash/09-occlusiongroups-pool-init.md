# Agent 9 — OcclusionGroups Pool Init / Audio Update Thread Race Analysis

Investigation focus: locate the F8000CD4 mutex/event creation site, decompile the
OcclusionGroups pool init and the audio update thread, then evaluate the race-window
hypothesis advanced by the parent investigation.

This file is self-contained — agents 1-8 / 10 outputs were not visible to the author.
All addresses are PPC guest VAs. All arithmetic was performed via `python3`.

---

## 1. String / RTTI search results

### "OcclusionGroups" literal

```
Address: 0x8209D9E0  (in .rdata)
asm symbol: aOcclusiongroup
xref:       sub_829186F8 + 0x24  (single referrer)
```

The string occupies the slot at file-offset `103130` of
`gta_iv/xex_excavation_retail/default_decrypted.xex.asm` and is referenced
only by `sub_829186F8`.

### Other occlusion strings

```
0x8203D194  "OCCLUSION_FACTOR_TO_CUTOFF_FREQUENCY_VERTICAL"
0x8203D1C4  "OCCLUSION_FACTOR_TO_LINEAR_VOLUME_VERTICAL"
0x8203D1F0  "OCCLUSION_FACTOR_TO_CUTOFF_FREQUENCY"
0x8203D218  "OCCLUSION_FACTOR_TO_LINEAR_VOLUME"
0x8209C36C  "audioocclusion"
0x8209C37C  "noaudioocclusion"
```

### RTTI / vtable hits — full chain found

|symbol|address|kind|
|-|-|-|
|`audOcclusionGroupManager_rage` vtable|0x8200A6AC|vtable|
|`audGtaOcclusionGroupManager` vtable|0x8200A820|vtable|
|`audOcclusionGroupInterface_rage` vtable|0x8203D186|vtable|
|`audGtaOcclusionGroup` vtable|0x8203D19A|vtable|
|`___R4audOcclusionGroupManager_rage__6B_`|0x820C190C|RTTI COL|
|`___R4audGtaOcclusionGroupManager__6B_`|0x820C1950|RTTI COL|

### Audio config-load strings (relevant to the crash chain)

|address|literal|sole referrer|
|-|-|-|
|0x8200A610|`AudioConfig`|—|
|0x8200A959|`GTA Audio Update`|sub_82299500 + 0x3E0|
|0x8209B3E8|`config/curves.dat`|—|
|0x8209B410|`config/categories.dat`|—|
|0x82A992E0|`audio:/config/`|sub_82299500 + 0x74|

### Memory layout hits (computed)

```python
base   = 0x82B40000   # r11 = -2095251456
oc_ptr = base - 29084 # 0x82B38C24  -> OcclusionGroups pool struct ptr
oc_alt = base - 29088 # 0x82B38C20  -> sub_82849778 sem returned by sub_829186F8 tail call
sem_a  = base - 29872 # 0x82B38B50  written by sub_8290D338
sem_b  = base - 29876 # 0x82B38B4C written by sub_8290D338
```

Note: the original investigation referenced an arbitrary `0x831C8E64`-style address.
After re-walking the recomp constants, the OcclusionGroups manager pointer actually
lives at **0x82B38C24** (`r11 = -2095251456 = 0x82B40000`, displacement `-29084`).

---

## 2. INIT — `sub_829186F8` (OcclusionGroups pool ctor)

The recomp scaffold is a small leaf-style ctor. Its decompiled C++ form is:

```cpp
// Address: 0x829186F8.  Single caller: sub_8290D338 (audio engine ctor stage 2)
// Reads: aOcclusiongroup at 0x8209D9E0.
// Writes pool ptr to *(uint32_t*)0x82B38C24
// Writes per-pool semaphore handle to *(uint32_t*)0x82B38C20

extern "C" void* sub_821B3510(uint32_t size);   // game heap alloc (3611 callers)
extern "C" uint32_t sub_82849778(uint32_t init_count); // ExCreateSemaphore(initCount, 32767)

struct AudPool;                                  // see sub_829185D8 below
extern "C" AudPool* sub_829185D8(AudPool* zero_init,
                                 uint32_t element_count,
                                 const char* name);

struct AudOcclusionGroupManagerStorage
{
    AudPool* pool;          // 0x82B38C24
    uint32_t lock_handle;   // 0x82B38C20  (semaphore handle, NOT an event)
};

void audOcclusionGroupManager_rage__Init()
{
    // 1) Allocate the manager-owned pool wrapper (sizeof = 56)
    AudPool* p = static_cast<AudPool*>(sub_821B3510(56));

    if (p) {
        // 2) Build pool with 400 elements, name "OcclusionGroups"
        sub_829185D8(p, /*element_count=*/400, /*name=*/"OcclusionGroups");
        *(AudPool**)0x82B38C24 = p;
    } else {
        *(AudPool**)0x82B38C24 = nullptr;
    }

    // 3) Allocate the manager-level semaphore  initial_count = 1
    //    sub_82849778 -> sub_82A12EB8(0, count=1, max=32767, 0)
    //                 -> NtCreateSemaphore(...)
    uint32_t sem = sub_82849778(1);
    *(uint32_t*)0x82B38C20 = sem;
}
```

That semaphore created with **initial count = 1** is what the parent investigation has
been calling the "F8000CD4 mutex" — it is in fact a counting semaphore created via
`NtCreateSemaphore` (the kernel allocates the F8000CDx-style handle on the host side).

### Pool ctor `sub_829185D8` (the work-horse)

```cpp
struct AudPool {
    void*    free_array;        // +0  (3*N*128 bytes, see derivation)
    uint8_t* slot_array;        // +4  (N bytes; high bit pre-set to mark "free")
    int32_t  cs_storage[8];     // +8  RTL_CRITICAL_SECTION (initialized via 0x8285FE48)
    uint32_t elem_count_a;      // +40
    uint32_t elem_count_b;      // +44
    int32_t  free_head;         // +48 = -1 sentinel
    uint8_t  state_flag;        // +52 = 1
    uint8_t  pad[3];
    // tail (+56..) holds debug name copy etc.
};

AudPool* sub_829185D8(AudPool* p, int N, const char* name)
{
    sub_8285FE48(&p->cs_storage);                  // RtlInitializeCriticalSection
    uint32_t buf_bytes = (3u * (uint32_t)N * 128u) & 0xFFFFFF80; // 153600 for N=400
    p->free_array  = sub_821B3510(buf_bytes);
    p->slot_array  = (uint8_t*)sub_821B3510(N);
    p->elem_count_a = (uint32_t)N;
    p->elem_count_b = (uint32_t)N;
    p->state_flag   = 1;
    p->free_head    = -1;
    for (int i = 0; i < N; ++i) {
        p->slot_array[i] = (uint8_t)(p->slot_array[i] | 0x80); // mark free
    }
    sub_822BCA90(/*pool name table*/0x82918664, name, buf_bytes);
    return p;
}
```

Field offsets verified by walking each `PPC_STORE_*` against the prologue's `r31 =
&pool`. The `0x80` high-bit-on-each-byte loop is the freelist init.

### Caller `sub_8290D338` (audio engine "stage 2" init)

```cpp
// Single caller: sub_829029A0 (audio config loader)
void sub_8290D338()
{
    // 1) Clear five global flags (see python-derived constants below)
    *(uint8_t*) 0x82B38AFC = 0;   // base-29860
    *(uint32_t*)0x82B38AF4 = 0;   // base-29868
    *(uint32_t*)0x82B38AE4 = 0;   // base-29884
    *(uint32_t*)0x82B38AE8 = 0;   // base-29880
    *(uint32_t*)0x82B38AF8 = 0;   // base-29864

    // 2) Build OcclusionGroups pool + manager semaphore (the racy step!)
    sub_829186F8();

    // 3) Two more semaphores with initial=1 stored at base-29872 / -29876
    *(uint32_t*)0x82B38B50 = sub_82849778(1);
    *(uint32_t*)0x82B38B4C = sub_82849778(1);
    // returns r3=1 (truthy)
}
```

Three counting semaphores are created with initial_count == 1 in this single function.
Each behaves like a binary mutex (one waiter unblocks immediately).

---

## 3. AUDIO UPDATE THREAD — `sub_82299490` (entry) + `sub_82298E70` (body)

### Spawn site (verified in xex_excavation_retail/default_decrypted.xex.asm:791556-791573)

```
li    r3, 0
bl    sub_82849778            # ExCreateSemaphore(0, 32767) -> wake sem
stw   r3, dword_82CB1BC8@l    # semaphore handle saved
li    r3, 0
bl    sub_82849778            # second wake/quit semaphore
stw   r3, dword_82CB1BC4@l
addi  r7, r11, aGtaAudioUpdate@l  # "GTA Audio Update"
addi  r3, r11, sub_82299490@l
li    r9, 5
li    r8, 1
li    r6, 2
ori   r5, r5, 0x8000            # stack 0x8000
li    r4, 0
bl    sub_82849A50              # ExCreateThread(entry=sub_82299490, ...)
stw   r3, dword_82CB1BC0@l      # thread handle
```

Key observation: the audio update thread is created **inside** `sub_82299500` at offset
+0x3E0, which is *after* the call to `sub_82902AF8` at +0x40 (which ultimately leads
through `sub_829029A0 -> sub_8290D338 -> sub_829186F8 -> sub_829185D8` — i.e. the
OcclusionGroups pool is built well before the update thread exists).

So the spawn ordering in the canonical init path is **safe**:
1. `sub_82902AF8` ⇒ `sub_829029A0` ⇒ `sub_8290D338` ⇒ pool init + 3 semaphores at init=1
2. `sub_82215188`, `sub_8262B508`, `sub_8234B928`, `sub_825F0E90`, ... (entity ctors)
3. Many `sub_82915158` curve calls
4. `sub_82849A50` spawns the GTA Audio Update thread *last*

### Thread main `sub_82299490`

```cpp
extern "C" void  sub_821B39F0(uint32_t size);     // alloc TLS scratch
extern "C" void  sub_82298E70();                  // audEngine::Update()
extern "C" int   sub_828497D8(uint32_t handle);   // wait sem, INFINITE
extern "C" int   sub_82849860(uint32_t handle);   // release sem, count=1

void GTAAudioUpdate_thread()
{
    sub_821B39F0(8);                                            // tiny scratch alloc
    sub_828497D8(*(uint32_t*)(/*r13_base*/ + 7112));            // wait initial gate

    while (!*(uint8_t*)(0x82B40000 - 22110) /*shutdown flag*/) {
        if (*(uint8_t*)(0x82B40000 - 27859 + g28) /*paused?*/) {
            sub_82298E70();                                     // audEngine::Update
        }
        sub_82849860(*(uint32_t*)(/*r13_base*/ + 7108));        // signal completion
        sub_828497D8(*(uint32_t*)(/*r13_base*/ + 7112));        // wait next tick
    }
}
```

The two `r13`-relative semaphore handles (`+7108`, `+7112`) are the ones stored to
`dword_82CB1BC4` / `dword_82CB1BC8` during init, accessed via the per-thread global
table.

### Update body `sub_82298E70` — RTTI evidence

`sub_82298E70` carries string refs `audController` and `audEngine` — definitive
identification as `audEngine::Update`. Its callees include the audio entity vfuncs
`sub_82215188` (audFrontendAudioEntity::Update), `sub_8234B928`
(audEmitterAudioEntity::Update) and the time/clock helpers
(`sub_82917FB8`, `GameClockHours`, `GameClockMinutes`, `GameClockBirdCycle`).

It does NOT directly read the OcclusionGroups pool. The pool is consumed indirectly
through `audGtaOcclusionGroupManager` virtual dispatch from inside the entity update
vfuncs, which is many levels deep in the call graph.

---

## 4. Cross-reference: does `sub_8227F2E8` operate on the OcclusionGroups pool?

**No.** This is the cleanest negative finding in the investigation.

`sub_8227F2E8` calls `sub_828C2300`, which is one of **71 callers** of that helper.
Examining `sub_828C2300` and its companion `sub_828C2290` (267 callers) reveals:

```cpp
// sub_828C2290 — appends a 36-byte record (8 floats + r9) to a ring at 0x831C2D2C
//               (base = 0x831C0000 + 11580 - 16, derived via python)
// sub_828C2300 — flushes/resets that ring; calls sub_828BF270 (free hook).
// sub_828BF270 — reads ptr at 0x831C22A4 (= 0x831C0000 + 8868), tail-calls sub_82A3DF50.
```

The 47 callers of `sub_828C2290` include `CDrawTriShapeDC::vfunc[0]`,
`CDrawRadarMapSectionDC::vfunc[0]`, `CDrawRadioHudTextDC::vfunc[0]` and many other
`CDraw*DC::vfunc[0]` symbols — i.e. these are **debug / immediate-mode draw
primitives** dispatched from a draw command list, not audio.

`sub_8227F2E8` itself takes **9 floating-point arguments** (`f1..f9`), shuffles them
into 4 sets and calls `sub_828C2290` four times before flushing with
`sub_828C2300`. That signature matches "draw quad with 4 vertices in 3D" far more
than any audio API.

Conclusion: the `LR=0x8227F3AC` (= `sub_8227F2E8 + 0xC4`) chain in the original
crash report points to a **debug-draw or immediate-mode rendering path**, not to
audio pool teardown. The "freelist counter" being corrupted in the crash report is
the draw-list element counter at `0x831C2D3C`, which is **not** the OcclusionGroups
pool's `free_head`.

---

## 5. Race-window analysis

### What the manager init writes (step by step, after the semaphore creation)

```cpp
sub_829186F8():
    p = sub_821B3510(56);          // (a) heap alloc
    if (p) {
        sub_829185D8(p, 400, ...); // (b) full init — see below
        store_to(0x82B38C24, p);   // (c) publish pointer to GLOBAL
    } else {
        store_to(0x82B38C24, 0);
    }
    sem = sub_82849778(1);         // (d) NtCreateSemaphore initial=1
    store_to(0x82B38C20, sem);     // (e) publish semaphore handle to GLOBAL
```

`sub_829185D8` order (single-thread, no acquire/release fences):
1. `RtlInitializeCriticalSection(&p->cs_storage)`        — kernel-side init
2. `p->free_array = malloc(153600)`                      — heap alloc
3. `p->slot_array = malloc(400)`                         — heap alloc
4. plain stores to `+40,+44,+48,+52`
5. **byte loop** to OR `0x80` into every entry of `p->slot_array`
6. `sub_822BCA90` to register the pool name in a global table

There is **no memory barrier** between (b) and (c) — i.e. between filling out
`*p` and publishing `p` into `*(uint32_t*)0x82B38C24`. On Xbox 360's PowerPC, that
opens a classic publication race: a reader in another core could observe the
non-null pointer **before** observing all of `*p`.

However: the pointer is stored *strictly after* `sub_829185D8` returns, and
`sub_829185D8` ends in a tail call. Even more importantly, the manager semaphore
(stored at `0x82B38C20`) is created **last**, so any reader that takes the
semaphore is guaranteed (by hardware-level happens-before via NtReleaseSemaphore
implementation in the kernel) to see the published pool pointer.

### Engine reader path

The audio engine update thread (`sub_82299490`) is **not started until after**
`sub_8290D338` returns (verified by tracing `sub_82299500` line-by-line). The
thread is created at `+0x3E0` of `sub_82299500`, while the pool init happens at
`+0x40 → sub_82902AF8 → sub_829029A0 → sub_8290D338`.

Therefore, by the time the GTA Audio Update thread reaches `sub_82298E70` for
the first time:
- `0x82B38C24` (pool ptr) is fully written
- `0x82B38C20` (manager semaphore) is fully created
- Pool's `cs_storage`, `free_array`, `slot_array`, `elem_count_*`, `free_head`,
  `state_flag` are all initialized
- Every `slot_array[i]` has bit 7 set (free marker)

There is **no realistic window** in which the audio update thread can reach the
OcclusionGroups pool with `r9 = 0xFFE1E1E1` from a half-built slot. `0xFFE1E1E1`
also does not match any byte filled during init (which would be `0xFF` if all
slots had been bytewise initialised to 0xFF, but the actual init only sets the
high bit, so a virgin byte that *happened* to come from un-zeroed heap could be
arbitrary — that's a *separate* uninitialized-read concern, not a race).

### What the `0xFFE1E1E1` poison value really hints at

`0xE1E1E1E1` is the canonical free-block fill emitted by the Xbox 360 debug heap
(matching `MmFreeContiguousMemory` poisoning), and `0xFFE1E1E1` would arise from
a one-byte-off pointer dereference into freed memory. That points to a **double-
free or use-after-free**, not a publication-race on init.

---

## 6. Conclusion — is OcclusionGroups the root cause?

**No.** The OcclusionGroups pool init and the audio update thread spawn are
strictly ordered (init at `sub_82299500 + 0x40`, thread spawn at +0x3E0); the
update thread cannot observe a half-built pool. Evidence:

- `sub_8227F2E8` is the **debug-draw immediate-mode primitive** path, not audio
  teardown. Its companions `sub_828C2290` / `sub_828C2300` are called from 47-71
  `CDraw*DC::vfunc[0]` symbols.
- The "freelist counter" being trampled at `LR=0x8227F3AC` is a draw command
  list count at `0x831C2D3C`, not the audio pool `free_head` at
  `pool+48`.
- The manager semaphore at `0x82B38C20` is a counting semaphore created with
  `initial_count = 1` *after* the pool body is fully written; in this codebase
  there is no concurrent reader before the GTA Audio Update thread is started,
  which itself happens 0x3A0 bytes later in the same init function on the same
  thread.
- `0xFFE1E1E1` is consistent with reading from poisoned-freed Xbox 360 heap
  memory (`0xE1E1E1E1` fill), implicating a **use-after-free or double-free**,
  most likely in the renderer's debug-draw command list, not in the audio pool.

### Recommended next investigation steps

1. Re-read the crash dump and locate which **DC** vfunc is on the stack above
   `sub_8227F2E8` — that is the actual misbehaving draw object.
2. Inspect the `0x831C0000`-region pointers for who recently freed the buffer
   pointed to by `0x831C22A4` (`sub_828BF270` reads it; `sub_82A3DF50` is the
   tail-called consumer).
3. Audit the streaming/RPF subsystem for a free-while-in-use of debug-draw
   command pages — the project memory notes major streaming work landed
   around the time of this crash class.

The OcclusionGroups race-window hypothesis can be deprioritized.

---

## Appendix: addresses & files of interest

|kind|absolute path / address|
|-|-|
|recomp generated tree|`/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/`|
|asm dump|`/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/xex_excavation_retail/default_decrypted.xex.asm`|
|`OcclusionGroups` literal|`0x8209D9E0`|
|`audOcclusionGroupManager_rage` vtable|`0x8200A6AC`|
|`audGtaOcclusionGroupManager` vtable|`0x8200A820`|
|OcclusionGroups pool ptr global|`0x82B38C24`|
|OcclusionGroups manager sem global|`0x82B38C20`|
|audio update thread handle|`dword_82CB1BC0`|
|audio update wake sem A|`dword_82CB1BC8`|
|audio update wake sem B|`dword_82CB1BC4`|
|`sub_829186F8`|OcclusionGroups manager init|
|`sub_829185D8`|generic AudPool ctor (called with N=400, name)|
|`sub_8290D338`|audio engine stage-2 init (calls `sub_829186F8` + 2 more sems)|
|`sub_829029A0`|audio config loader (refs `config/`)|
|`sub_82902AF8`|audio init wrapper (ret-bool)|
|`sub_82299500`|audio engine root init (refs `audio:/config/`, `GTA Audio Update`)|
|`sub_82299490`|GTA Audio Update thread main|
|`sub_82298E70`|`audEngine::Update` (refs `audEngine`, `audController`)|
|`sub_82849778`|thin wrapper → `sub_82A12EB8(0, init, 32767, 0)` → `NtCreateSemaphore`|
|`sub_82849860`|thin wrapper → `sub_82A12F50(_, 1, 0)` → `NtReleaseSemaphore`|
|`sub_828497D8`|thin wrapper → `sub_82A13040(_, INFINITE)` → wait|
|`sub_82849A50`|`ExCreateThread` shim (creates thread, stores handle)|
|`sub_8227F2E8`|**debug-draw quad emitter** — NOT audio teardown|
|`sub_828C2290`|debug-draw vertex pusher (47 callers, mostly `CDraw*DC::vfunc[0]`)|
|`sub_828C2300`|debug-draw flush (71 callers, mostly `CDraw*DC::vfunc[0]`)|
