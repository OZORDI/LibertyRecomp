# PPC Analysis: sub_82478AF8 — Audio/Streaming Initialization

**Source**: `gta4-recomp/generated/gta4_recomp.20.cpp`, lines 83744–85184
**Function name**: `sub_82478AF8`
**Purpose**: Full audio subsystem initialization — allocates the audio manager object, configures voice counts and buffer sizes, constructs the XAudio2 sound engine (Xbox 360), registers streaming sources, and launches streaming threads.

---

## Complete Call Graph (in execution order)

### Phase 1 — Environment setup (lines 83751–83770)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x826225E0` | `sub_826225E0` | (none) | Init unknown subsystem A |
| `0x82622648` | `sub_82622648` | (none) | Init unknown subsystem B |
| `0x826226B0` | `sub_826226B0` | (none) | Init unknown subsystem C |

### Phase 2 — Audio manager object allocation (lines 83772–83791)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x821B3510` | `sub_821B3510` | r3=176 | Allocate 176-byte audio manager struct |
| `0x8294BD68` | `sub_8294BD68` | r3=alloc | Construct audio manager (if alloc != null) |

**Global writes after this block:**
- `0x831CC944` ← r30 (audio manager object pointer)

### Phase 3 — Voice/buffer configuration (lines 83793–83847)
Reads global flags at `0x82FF541C` (audio flags word), loads voice count and buffer config:
- `0x82FF5414` ← 168 (max voices / polyphony)
- `0x82FF5418` ← computed audio buffer size (r26+r27+128)
- `0x82FF541C` ← flags |= 0x1, |= 0x2 (marks buffer size + voice count configured)

### Phase 4 — Audio manager initialization (lines 83894–83952)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x8294BE28` | `sub_8294BE28` | r3=audio_mgr, r4=stack structs, r5=stack | Set listener position (f0 loaded from float globals) |
| `0x8294BE20` | `sub_8294BE20` | r3=audio_mgr, r4=6000 | Set audio budget/distance |
| `0x8294BEA0` | `sub_8294BEA0` | r3=audio_mgr, r4=1500 | Set another audio parameter |

**String comparisons** (lines 83940–84004) — `rexcrt__stricmp` called 4–5 times comparing a config string (from `0x82FF53C0` area, "AudioType" or platform name) against:
- `0x82FF-16248` — likely `"XAudio2"` or `"software"`
- `0x82FF-16260` — another audio backend name
- `0x82FF-16268`, `0x82FF-16276` — fallbacks

Result stored at `r30+152` (audio backend enum: 0=software, 1=XAudio2, 2=another).

### Phase 5 — Audio device construction (lines 84015–84057)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x8294E208` | `sub_8294E208` | r3=audio_mgr | Finalize audio manager |
| `0x821B3510` | `sub_821B3510` | r3=192 | Alloc 192-byte audio device struct |
| `0x82956718` | `sub_82956718` | r3=alloc | Construct audio device |

**Global writes:**
- `0x82FF5360` ← audio_mgr (global audio manager pointer)
- `0x82FF5364` ← audio device pointer (base for streaming)

### Phase 6 — Streaming pool initialization (lines 84048–84132)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x82953088` | `sub_82953088` | (none) | Init streaming pool subsystem |
| `0x82954618` | `sub_82954618` | r3=device, r4=16 | Set streaming pool params |
| `0x82955838` | `sub_82955838` | r3=device, r4=audio_mgr, r5=voices, r6=lis(32), r7=buf_size*4 | Allocate streaming voice pool |

### Phase 7 — Audio format descriptors (lines 84117–84133)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x8284D220` | `sub_8284D220` (×7) | r3=stack struct, r4=callback_ptr, r5=0, r6=stack, r7=4 | Create 7 audio format/buffer descriptors |
| `0x829530B0` | `sub_829530B0` | r3=device, r4=result_ld, r5=stack | Commit descriptors |

### Phase 8 — Streaming object construction (lines 84155–84175)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x82953A68` | `sub_82953A68` | r3=stack | Init streaming metadata struct |
| `0x8284E1C0` | `sub_8284E1C0` | r3=global_str | Init streaming name/path |
| `0x8297B6B8` | `sub_8297B6B8` | r3=device+8, r4=512, r5=-1 | Set streaming buffer size 512 blocks |
| `0x826BDC18` | `sub_826BDC18` | (none) | Unknown streaming init |

### Phase 9 — Virtual dispatch (lines 84176–84198)
```
lwz r3,-14076(r26)   ; load audio renderer ptr @ 0x831CC904
lwz r11,0(r3)        ; vtable
lwz r11,4(r11)       ; vtable[1]
bctrl                ; virtual call — likely XAudio2::CreateSourceVoice or equivalent
```
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x8296A278` | `sub_8296A278` | r3=1 | Enable audio engine |

**Global writes:**
- `0x831CDA65` ← r25 (=1, flag: surround/stereo enabled)

### Phase 10 — Streaming configuration (lines 84207–84230)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x82953AB0` | `sub_82953AB0` | r3=1 | Enable streaming source |
| `0x82953AD0` | `sub_82953AD0` | r3=1, f1=float_const | Set streaming volume |
| `0x8228A1E0` | `sub_8228A1E0` | (none) | Init streaming scheduler |
| `0x825FD6B8` | `sub_825FD6B8` | (none) | Init radio/ambient streaming |
| `0x82955BE0` | `sub_82955BE0` | (none) | Init streaming subsystem final |

**Global writes:**
- `0x831CDA47` ← 0 (r23)

### Phase 11 — Sound bank allocation (lines 84232–84316)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x8284F310` | `sub_8284F310` | r3=global_str, r4=bank_ptr | Load sound bank |
| `0x82477670` | `sub_82477670` | (none) | Init sound bank subsystem |
| `0x827C2420` | `sub_827C2420` | r3=obj | Init sound object |
| `0x8284E830` | `sub_8284E830` | r3=global_str | Finalize sound bank path |
| `0x821B3510` | `sub_821B3510` | r3=1024 | Alloc 1024-byte streaming channel array |

### Phase 12 — Streaming channel descriptors (lines 84317–84636)
Seven calls to `sub_8284D220` building streaming channel format structs, then:
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x82478A80` | `sub_82478A80` (×3) | r3=stack_struct | Convert format struct → channel descriptor |

### Phase 13 — **HANG POINT**: Audio engine construction (lines 84715–84743)
```
bl 0x827ADB48   ; sub_827ADB48 — XAudio2 engine constructor
stw r3,0(r31)   ; store result @ 0x82FF5368
```

**sub_827ADB48** calls:
- `sub_82A00DC0` ×6 — copy format descriptors
- `sub_827AD9C8` — deeper audio engine constructor

**sub_827AD9C8** calls:
- `sub_82A00DC0` ×6 — copy descriptors again
- `sub_827AD200` — **allocates/creates the XAudio2 source voice pool**
- `sub_8279A6D0` — **launches streaming I/O thread**

**sub_827AD200** → `sub_8285D610` (via `sub_8285D500` inner call chain):

```
sub_827ADB48
  └─ sub_827AD9C8
       ├─ sub_827AD200
       │    └─ [creates voice pool, calls sub_8285D610 for each voice]
       └─ sub_8279A6D0  ← launches streaming thread
```

### Phase 14 — XMA/streaming pool setup (lines 84786–84934)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x821B3510` | alloc | r3 = r30*176+16 | Alloc voice descriptor array |
| `0x8261FBA0` | `sub_8261FBA0` (×r30) | r3=slot | Init each voice slot (176 bytes each) |
| `0x821B3510` | alloc | r3 = r30*4 | Alloc voice pointer array |
| `0x827ACC98` | `sub_827ACC98` | r3=device, r4=ptr_array | Register voice pointer array |
| `0x821B3560` | `sub_821B3560` | r3=ptr | Possibly free temp |
| `0x827ACCA0` | `sub_827ACCA0` (×3) | r3=device, r4=8192/4096/2048, r5=200/300/500 | Set 3 streaming buffer tiers |

### Phase 15 — Final audio subsystem init (lines 84935–85172)
| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x821B5038` | `sub_821B5038` (×2) | r3=string_ptr | Load audio config strings |
| `0x8287AC38` | `sub_8287AC38` | (none) | Init effects/DSP chain |
| `0x8287A6A8` | `sub_8287A6A8` (×2) | r3=engine, r4=string, r5=6/4 | Register DSP effect channels |
| `0x8261C7C8` | `sub_8261C7C8` | (none) | Init reverb subsystem |
| `0x8287A408` | `sub_8287A408` | r3=engine, f1=float | Set master volume |
| `0x8287A4E0` | `sub_8287A4E0` | r3=engine, f1=float | Set send volume |
| `0x8287A4E8` | `sub_8287A4E8` | r3=engine, f1=float | Set reverb volume |
| Two vtable calls | indirect via `0x831CC904` vtable[4] | r4=string | Set audio device string params |
| `0x8299B4A8` | `sub_8299B4A8` (×2) | f1=f30 | Apply volume/pan |
| `0x821B3510` | alloc | r3=112 | Alloc XMA context block |
| `0x823B33F8` | `sub_823B33F8` | r3=alloc | Construct XMA decoder context |
| `0x822BCA90` | `sub_822BCA90` | r3=1 | Start audio processing loop |

---

## Global State Written

| Address | Value | Meaning |
|-|-|-|
| `0x831CC944` | audio_mgr ptr | Global audio manager object |
| `0x82FF5360` | audio_mgr ptr | Audio manager global (second copy) |
| `0x82FF5364` | device ptr | Audio device/router |
| `0x82FF5368` | engine ptr | Sound engine (XAudio2 equivalent) |
| `0x82FF536C` | slot_array ptr | Voice slot array base |
| `0x82FF5368` | engine ptr | Also result of sub_827ADB48 |
| `0x82FF5414` | 168 | Max polyphony (voices) |
| `0x82FF5418` | computed | Streaming buffer size |
| `0x82FF541C` | flags | Audio config flags (bits 0,1) |
| `0x831CDA65` | 1 | Stereo/surround enabled flag |
| `0x831CDA47` | 0 | Audio pause flag |
| `0x82FF5374` | 0 | Init array (1024 slots × 12 bytes zeroed) |

---

## Path to the Hang: sub_8285D610

**sub_8285D610** is the **streaming voice initializer**. It is called from the `sub_8285D500` → `sub_8285D488` chain inside `sub_8285D610`'s loop:

```
sub_82478AF8
  └─ sub_827ADB48   (line 84735, lr=0x824791F8)
       └─ sub_827AD9C8
            └─ sub_827AD200
                 └─ sub_8285D610   [called per streaming voice]
                      ├─ sub_8285FE48   (init ring buffer)
                      ├─ sub_82849778   (alloc/init XMA source)
                      └─ sub_8285D500   (copy name, create streaming handle)
                           └─ sub_82849A50  (create OS thread via sub_82A13110=CreateThread)
                                └─ sub_82A13110  ← XCreateThread / NtCreateThread
```

**sub_8285D610** body (file 56, line 7990):
1. Initializes a 1024-entry ring buffer (`sub_8285FE48`)
2. Calls `sub_82849778` with `r3=0` — this creates the XMA streaming source; on Xbox 360 this blocks on XMA hardware. On PC with the recomp, `sub_82849778` is an **unhooked recompiled function** that may call into XAudio2 or block waiting for a kernel event.
3. Calls `sub_8285D500` which calls `sub_82849A50` (the streaming thread launcher), which calls `sub_82A13110` — `XCreateThread` / `CreateThread` at `0x82A13110`.

**The hang likely occurs inside `sub_82849778`** or `sub_82849A50` / `sub_82A13110`:
- `sub_82849778` on Xbox 360 opens an XMA streaming context. On PC this translates to creating an XAudio2 source voice. If the XAudio2 engine hasn't been created yet (or is the wrong API), this blocks indefinitely.
- `sub_82A13110` = `CreateFileA` hook? No — from the CRT audit, `0x82A13110` is **not** a hooked CRT function. It is a native XCreateThread equivalent that needs a proper host thread-creation hook.

---

## Which Calls Are Safe to Skip vs Critical

### Safe to skip (no observable side effects if audio is stubbed):
- `sub_826225E0`, `sub_82622648`, `sub_826226B0` — environment setup, not audio-critical
- `sub_8284E1C0`, `sub_8284E830` — string path setup
- `sub_8284F310`, `sub_82477670` — sound bank loader (game still runs without sound)
- `sub_8287AC38`, `sub_8261C7C8` — DSP/reverb effects chain
- `sub_8287A6A8`, `sub_8287A408`, `sub_8287A4E0`, `sub_8287A4E8` — volume/effect params
- `sub_8299B4A8` — pan/vol apply
- `sub_8228A1E0`, `sub_825FD6B8` — radio/ambient streaming scheduler

### Critical (required for game to proceed past audio init):
- `sub_821B3510` — game's allocator; must work
- `sub_827ADB48` / `sub_827AD9C8` / `sub_827AD200` — audio engine construction; **must complete or be skipped entirely**
- `sub_8285D610` — **the hang target**; must be hooked to return immediately (stub out the ring buffer + XMA init path)
- `sub_82849778` — XMA source creation; **hook to no-op + return 0**
- `sub_82849A50` / `sub_82A13110` — thread creation; `0x82A13110` must be hooked if threads are needed

---

## Recommended Fix

Hook `sub_8285D610` (at `0x8285D610`) to be a no-op that returns the struct pointer:

```cpp
PPC_FUNC(sub_8285D610) {
    // Stub: zero-init the struct header and return immediately
    // r3 = streaming voice struct pointer (already allocated by caller)
    PPC_STORE_U32(ctx.r3.u32 + 0, 0);   // count = 0
    PPC_STORE_U16(ctx.r3.u32 + 4, 0);   // active voices = 0
    PPC_STORE_U16(ctx.r3.u32 + 6, 0);   // ring head = 0
    // return r3 unchanged
    return;
}
```

Alternatively, hook `sub_82849778` to return 0 (null XMA source) so the init loop completes without blocking.

The deeper fix: `sub_82A13110` (`0x82A13110`) needs a `CreateThread` hook — it is an unhooked `XCreateThread` analog. Once hooked, the streaming threads will launch and `sub_8285D610` will complete normally.
