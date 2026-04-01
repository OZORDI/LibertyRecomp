# sub_82478AF8 — RAGE Audio Engine Init (Full Trace)

**File**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.20.cpp`, lines 83744-85183
**Called from**: sub_821FC1F8 (master init), call #30 at address 0x821FC284
**Stack frame**: 1024 bytes (`stwu r1,-1024(r1)`)
**Saved registers**: r23-r31, f30, f31 (via `__savegprlr_23`)
**Total call instances**: 80 (51 unique functions)

## Purpose

This function initializes the RAGE audio engine subsystem. It:
1. Creates and configures the audio manager object (176 bytes)
2. Creates a secondary "XAudio graph" manager (192 bytes)
3. Detects the renderer type via platform name string comparison
4. Loads 7 audio resource banks via `sub_8284D220` (rage::datResource::Load)
5. Creates and initializes the voice/streaming block (1024 bytes)
6. Allocates and constructs the viewport array (r30 * 176 + 16 bytes)
7. Configures streaming pools, 3D audio params, and a camera object
8. Zeros the 1024-entry entity tracking array (12 bytes each, 12 KB)
9. Starts audio streaming threads

## Register Convention

| Reg | Persistent Value | Meaning |
|-|-|
| r23 | 0 | Constant zero |
| r24 | 0x82FF0000 | Base for stride_value global |
| r25 | 1 (later -1) | Constant one / sentinel |
| r26 | mem_size_1 (from 0x82FB0700→+8) | Physical memory pool size 1 |
| r27 | mem_size_2 (from 0x831D5388→+8) | Physical memory pool size 2 |
| r28 | 0x8201C090 (default name string) | Default platform name ptr |
| r29 | 0x82FF0000 | Base for total_mem_size global |
| r30 | engine object ptr → later r26+r27 | Audio manager → then total memory count |
| r31 | varies per phase | Name string → manager ptrs → resource names |
| f30 | float const from 0x82000D48 | Audio param (likely 1.0f) |
| f31 | float const from 0x82000A34 | Audio param (likely 0.0f) |

## Complete Call Sequence (80 calls)

### Phase 1: Environment Init (calls 1-3)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 1 | 0x82478B10 | sub_826225E0 | none | void | Audio environment init A |
| 2 | 0x82478B14 | sub_82622648 | none | void | Audio environment init B |
| 3 | 0x82478B18 | sub_826226B0 | none | void | Audio environment init C |

### Phase 2: Audio Manager Construction (calls 4-8)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 4 | 0x82478B20 | sub_821B3510 | r3=176 | r3=ptr or NULL | operator new(176) — alloc audio mgr |
| 5 | 0x82478B30 | sub_8294BD68 | r3=alloc'd ptr | r3=constructed ptr | Audio manager constructor (if alloc succeeded) |
| 6 | 0x82478BF0 | sub_8294BE28 | r3=mgr, r4=&bbox_max(sp+288), r5=&bbox_min(sp+320) | void | Set audio bounding box |
| 7 | 0x82478BFC | sub_8294BE20 | r3=mgr, r4=6000 | void | Set max distance (6000 units) |
| 8 | 0x82478C10 | sub_8294BEA0 | r3=mgr, r4=1500 | void | Set inner distance (1500 units) |

After call 5, the engine manager pointer is stored:
- `r30` = audio manager object (176 bytes)
- `*(0x831CC944)` = r30 (first global store)
- `mgr->halfword[40]` = total_mem_size (from 0x82FF5418)

### Phase 3: Renderer Type Detection (calls 9-13)

The function reads a name override pointer from `0x82FF5404`. If non-null, uses that; otherwise falls back to the default name at `0x8201C090`.

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 9 | 0x82478C44 | _stricmp | r3=name_str, r4=0x8201C088 | r3=cmp | If match → renderer_type=2 ("DX10"?) |
| 10 | 0x82478C6C | _stricmp | r3=name_str, r4=0x8201C07C | r3=cmp | If match → renderer_type=1 ("DX9"?) |
| 11 | 0x82478C84 | _stricmp | r3=name_str, r4=0x8201C074 | r3=cmp | If match → renderer_type=1 (variant) |
| 12 | 0x82478C98 | _stricmp | r3=name_str, r4=default_name | r3=cmp | If match → renderer_type=0 |
| 13 | 0x82478CB0 | _stricmp | r3=name_str, r4=0x8201C06C | r3=cmp | If match → renderer_type=0 |

Result stored at `mgr->u32[152]` = renderer_type (0, 1, or 2).

### Phase 4: Audio Manager Finalization + XAudio Init (calls 14-19)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 14 | 0x82478CCC | sub_8294E208 | r3=mgr | void | Finalize audio manager |
| 15 | 0x82478CDC | sub_821B3510 | r3=192 | r3=ptr or NULL | operator new(192) — alloc XAudio graph obj |
| 16 | 0x82478CE8 | sub_82956718 | r3=alloc'd ptr | r3=constructed ptr | XAudio graph constructor |
| 17 | 0x82478D00 | sub_82953088 | r3=graph_obj | void | Audio graph init |
| 18 | 0x82478D0C | sub_82954618 | r3=graph_obj, r4=16 | void | Set max voices=16 |
| 19 | 0x82478D28 | sub_82955838 | r3=graph, r4=mgr, r5=stride, r6=2MB, r7=count*4 | void | Create source voices |

After call 14: `*(0x82FF5360)` = r30 (g_pEngineObject)
After call 16: `*(0x82FF5364)` = graph_obj (g_pManager1)

### Phase 5: Main Audio Resource Load (call 20)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 20 | 0x82478D7C | sub_8284D220 | r3=&sp+304, r4=0, r5=0x82236640, r6=0, r7=0 | 16-byte result at sp+304 | Load main audio description resource |

The result's field +12 is used to compute an audio timing value: `(float)(r26+r27) * float_const_at_0x82000DC8` → truncated to int → stored at `*(result+12)+72`.

### Phase 6: Audio Graph Configuration (calls 21-22)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 21 | 0x82478D98 | sub_829530B0 | r3=graph_obj, r4=ld(sp+304), r5=ld(sp+312) | void | Bind resource to graph (vtable ptr 0x821439F8 stored at sp+316) |
| 22 | 0x82478DBC | sub_82953A68 | r3=&sp+336 (vec3: f31,f31,float_const) | void | Set 3D listener position |

### Phase 7: Spatial Audio + Device Init (calls 23-28)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 23 | 0x82478DC8 | sub_8284E1C0 | r3=0x82B071E0 | void | Init spatial audio struct |
| 24 | 0x82478DDC | sub_8297B6B8 | r3=graph→+8, r4=512, r5=-1 | void | Configure voice buffer (512 samples, infinite) |
| 25 | 0x82478DE0 | sub_826BDC18 | none | void | Audio device enumeration / init |
| 26 | 0x82478DFC | VTABLE CALL | obj=*(0x831CC904), vtable[1] (offset +4), r4=0 | void | Virtual method call on audio device (init phase) |
| 27 | 0x82478E04 | sub_8296A278 | r3=1 | void | Enable audio output |
| 28 | 0x82478E18 | sub_82953AB0 | r3=1 | void | Set active voice count=1 |

### Phase 8: Audio Parameters (calls 29-30)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 29 | 0x82478E24 | sub_82953AD0 | f1=float_const_0x82000D58 | void | Set master volume or gain |
| 30 | 0x82478E34 | sub_8228A1E0 | none | void | Start audio streaming thread |

Globals written between calls:
- `*(0x831CDA65)` = 1 (g_boolFlag1 — audio initialized)
- `*(0x831CDA47)` = 0 (g_boolFlag2 — audio error cleared)

### Phase 9: Stream Init + HANG POINT (calls 31-33)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 31 | 0x82478E38 | sub_825FD6B8 | none | void | Start RPF streaming |
| 32 | 0x82478E3C | **sub_82955BE0** | none | void | **XAudio streaming init — LABELED "HANG" IN PROBES** |
| 33 | 0x82478E84 | sub_82A00DC0 | r3=sp+672, r4=0x8201C054, r5=23 | void | memcpy 23-byte path string (if no path override) |

**Call 32 (sub_82955BE0) is the known hang point.** The INIT_PROBE label is `"82478AF8 phase21 xaudio-stream-HANG"`.

### Phase 10: Resource Path Setup + Streaming Manager (calls 34-38)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 34 | 0x82478E98 | sub_8284F310 | r3=0x82B07278, r4=path_str | void | Set resource path on streaming mgr |
| 35 | 0x82478E9C | sub_82477670 | none | void | Streaming tick / poll |
| 36 | 0x82478EB0 | sub_827C2420 | r3=*(0x82B393A4) | void | Activate streaming (after storing r31 at struct+56) |
| 37 | 0x82478EB8 | sub_8284E830 | r3=0x82B07278 | void | Finalize streaming manager config |
| 38 | 0x82478EC4 | **sub_821B3510** | **r3=1024** | r3=ptr or NULL | **operator new(1024) — THE allocation** |

### Phase 11: Audio Resource Bank Loading (calls 39-52)

If the 1024-byte allocation (call 38) succeeds, 7 additional resource banks are loaded:

| # | Address | Function | Args (r4=device, r6=hash_ptr) | Notes |
|-|-|-|-|-|
| 39 | 0x82478EEC | sub_8284D220 | sp+464, device=0x83016A28, hash=0x82444808 | Audio bank 1 |
| 40 | 0x82478F4C | sub_8284D220 | sp+400, device=0x83016A28, hash=0x824F34F0 | Audio bank 2 |
| 41 | 0x82478FA4 | sub_8284D220 | sp+384, device=0x83016A28, hash=0x822BCA90 | Audio bank 3 |
| 42 | 0x82478FFC | sub_8284D220 | sp+352, device=0x83016A28, hash=0x824F3310 | Audio bank 4 |
| 43 | 0x82479054 | sub_8284D220 | sp+432, device=0x83016A28, hash=0x822BCA90 | Audio bank 5 |
| 44 | 0x824790AC | sub_8284D220 | sp+368, device=0x83016A28, hash=0x824F3320 | Audio bank 6 |
| 45 | 0x82479104 | sub_8284D220 | sp+416, device=0x83016A28, hash=0x824F3808 | Audio bank 7 |

Each sub_8284D220 call is followed by storing a vtable pointer `0x822188F8` at result+12 (the resource destructor callback).

Between resource loads, `sub_82478A80` is called 3 times to resolve resource names:

| # | Address | Function | Args | Returns |
|-|-|-|-|-|
| 46 | 0x82479140 | sub_82478A80 | r3=sp+640 | r29=name ptr 1 |
| 47 | 0x8247914C | sub_82478A80 | r3=sp+624 | r28=name ptr 2 |
| 48 | 0x82479158 | sub_82478A80 | r3=sp+656 | r31=name ptr 3 |

Then 6 memcpy calls copy 16-byte resource names into local buffers:

| # | Address | Function | Src → Dst |
|-|-|-|-|
| 49 | 0x82479170 | sub_82A00DC0 | r29 → sp+208, len=16 |
| 50 | 0x82479180 | sub_82A00DC0 | r28 → sp+192, len=16 |
| 51 | 0x82479198 | sub_82A00DC0 | sp+560 → sp+160, len=16 |
| 52 | 0x824791AC | sub_82A00DC0 | sp+592 → sp+128, len=16 |
| 53 | 0x824791BC | sub_82A00DC0 | sp+608 → sp+112, len=16 |
| 54 | 0x824791CC | sub_82A00DC0 | sp+576 → sp+96, len=16 |

### Phase 12: Voice Block Creation (call 55)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 55 | 0x824791F8 | sub_827ADB48 | r3=1024-byte-ptr, r4-r9=resource names+paths, sp+88=extra | r3=voice_block_ptr | Create voice/audio resource block |

Result stored at `*(0x82FF5368)` = g_pManager2.

If the 1024-byte alloc failed (call 38 returned NULL): `*(0x82FF5368)` = 0, skips to Phase 13.

### Phase 13: Viewport Array Allocation (calls 56-59)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 56 | 0x82479254 | sub_821B3510 | r3=r30*176+16 (or -1 if overflow) | r3=ptr | Allocate viewport array |
| 57 | 0x8247927C | sub_8261FBA0 (LOOP) | r3=viewport_ptr | void | Construct each viewport (176 bytes), loops r30 times |
| 58 | 0x82479318 | sub_827ACC98 | r3=graph_obj, r4=ptr_array | void | Bind voice ptrs to viewports |
| 59 | 0x82479320 | sub_821B3560 | r3=ptr_array | void | Free temporary pointer array |

After the loop: `*(0x82FF536C)` = viewport_base (g_pViewportArray).

The loop at `loc_824792E0` also builds a temporary pointer array: for each viewport, stores `viewport_base + i*176` at `ptr_array[i]` and sets `viewport[i].field[160] = 18`.

### Phase 14: Pool Configuration (calls 60-62)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 60 | 0x8247933C | sub_827ACCA0 | r3=graph, r4=8192, r5=200 | void | Pool config: 8KB blocks, 200 max |
| 61 | 0x8247934C | sub_827ACCA0 | r3=graph, r4=4096, r5=300 | void | Pool config: 4KB blocks, 300 max |
| 62 | 0x8247935C | sub_827ACCA0 | r3=graph, r4=2048, r5=500 | void | Pool config: 2KB blocks, 500 max |

Also sets `graph->u32[768] = 128` before the first pool config call.

### Phase 15: Streaming System Init (calls 63-72)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 63 | 0x82479368 | sub_821B5038 | r3=0x8201C034 | void | Register string label (audio?) |
| 64 | 0x8247936C | sub_8287AC38 | none | void | Init streaming graph |
| 65 | 0x82479384 | sub_8287A6A8 | r3=*(0x831C21D4), r4=0x8201C02C, r5=6 | void | Config streaming param (name, len=6) |
| 66 | 0x82479398 | sub_8287A6A8 | r3=*(0x831C21D4), r4=0x8201C024, r5=4 | void | Config streaming param (name, len=4) |
| 67 | 0x824793A4 | sub_821B5038 | r3=0x820B92FC | void | Register string label |
| 68 | 0x824793A8 | sub_8261C7C8 | none | void | Audio 3D spatial init |
| 69 | 0x824793B8 | sub_8287A408 | r3=streaming_mgr, f1=float_0x82000D74 | void | Set 3D param A |
| 70 | 0x824793C8 | sub_8287A4E0 | r3=streaming_mgr, f1=float_0x82AA457C | void | Set 3D param B |
| 71 | 0x824793D8 | sub_8287A4E8 | r3=streaming_mgr, f1=float_0x82AA4580 | void | Set 3D param C |

### Phase 16: Device String Registration via VTable (calls 72-75)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 72 | 0x824793FC | VTABLE CALL | obj=*(0x831CC904), vtable[4] (+16), r4=0x8201C018 | void | Register audio device string 1 |
| 73 | 0x82479404 | sub_8299B4A8 | f1=f30 (float_0x82000D48) | void | Set device float param 1 |
| 74 | 0x82479420 | VTABLE CALL | obj=*(0x831CC904), vtable[4] (+16), r4=0x8201C00C | void | Register audio device string 2 |
| 75 | 0x82479428 | sub_8299B4A8 | f1=f30 | void | Set device float param 2 |

### Phase 17: Camera Object + Entity Array (calls 76-80)

| # | Address | Function | Args | Returns | Notes |
|-|-|-|-|-|-|
| 76 | 0x82479440 | sub_821B3510 | r3=112 | r3=ptr or NULL | operator new(112) — alloc camera obj |
| 77 | 0x8247944C | sub_823B33F8 | r3=alloc'd ptr | r3=constructed ptr | Camera/stream block constructor |
| 78 | 0x824794AC | VTABLE CALL | obj=camera, vtable[1] (+4), r4=&sp+496(params), r5=0, r6=0 | void | Init camera with audio params |
| 79 | — | (inline loop) | Zeros 1024 entries at 0x82FF2360, 12 bytes each | void | Clear entity tracking array |
| 80 | 0x824794E8 | sub_822BCA90 | none | void | Final audio init callback |

After call 77: `*(0x82FF5374)` = camera_obj (g_pCameraObj).
After call 78: `*(0x82FF5378)` = 0 (g_initCounter).

Bit 9 of `*(0x82B1AEF8)` is cleared before call 76 (render flags adjustment).

## Globals Table

| Address | Size | Written Value | Name / Purpose |
|-|-|-|-|
| 0x82AE5F84 | u8 | 0 | g_boolFlag3 (unknown purpose) |
| 0x82B1AEF8 | u32 | clear bit 9 | g_renderFlags |
| 0x82FF2360 | 12288 | zero-filled | g_entityArray[1024] (12 bytes per entry) |
| 0x82FF5360 | u32 | engine_obj_ptr | g_pEngineObject (audio manager, 176 bytes) |
| 0x82FF5364 | u32 | graph_obj_ptr | g_pManager1 (XAudio graph, 192 bytes) |
| 0x82FF5368 | u32 | voice_block_ptr | g_pManager2 (voice block, from sub_827ADB48) |
| 0x82FF536C | u32 | viewport_array | g_pViewportArray (r30 viewports, 176 bytes each) |
| 0x82FF5374 | u32 | camera_obj_ptr | g_pCameraObj (112 bytes) |
| 0x82FF5378 | u32 | 0 | g_initCounter (zeroed at end) |
| 0x82FF5414 | u32 | 168 | g_strideValue (viewport stride) |
| 0x82FF5418 | u32 | r26+r27+128 | g_totalMemSize |
| 0x82FF541C | u32 | flags OR'd | g_initFlags (bit 0=sizes computed, bit 1=stride set) |
| 0x831CC904 | u32 | (read only) | g_pDeviceManager (vtable calls at +4, +16) |
| 0x831CC944 | u32 | engine_obj_ptr | g_pEngineObjectAlt (duplicate of 0x82FF5360) |
| 0x831CDA47 | u8 | 0 | g_boolAudioError (cleared) |
| 0x831CDA65 | u8 | 1 | g_boolAudioInit (set to true) |

## Globals Read

| Address | Purpose |
|-|-|
| 0x82FB0700 → +8 | mem_pool_size_1 (r26) |
| 0x831D5388 → +8 | mem_pool_size_2 (r27) |
| 0x82FF5404 | name override ptr (NULL = use default) |
| 0x82FF5380 | path override ptr (NULL = use default 23-char path) |
| 0x831CC904 | g_pDeviceManager (for vtable dispatch) |
| 0x831C21D4 | g_pStreamingMgr |
| 0x82B393A4 | ptr to struct receiving name at +56 |

## Struct Layouts

### AudioManager (r30, 176 bytes, alloc'd at call 4)

| Offset | Size | Field |
|-|-|-|
| +0 | 4 | vtable ptr (set by constructor sub_8294BD68) |
| +40 | 2 | total_mem_size as u16 |
| +152 | 4 | renderer_type (0=default, 1=DX9-class, 2=DX10-class) |

### XAudioGraph (*g_pManager1, 192 bytes, alloc'd at call 15)

| Offset | Size | Field |
|-|-|-|
| +0 | 4 | vtable ptr |
| +8 | 4 | sub-manager ptr (passed to sub_8297B6B8) |
| +12 | 4 | ptr to timing struct (+72 stores computed int) |
| +768 | 4 | voice limit (set to 128) |

### Viewport (176 bytes each, sub_8261FBA0 constructs)

| Offset | Size | Field |
|-|-|-|
| +0 | ... | constructor-initialized fields |
| +160 | 4 | type/state (set to 18 during init) |

### Entity Array Entry (12 bytes each, 1024 entries at 0x82FF2360)

| Offset | Size | Field |
|-|-|-|
| +0 | 4 | u32 (zeroed) |
| +4 | 4 | u32 (zeroed) |
| +8 | 4 | u32 (zeroed) |

## Indirect (VTable) Calls

There are 4 indirect calls via `PPC_CALL_INDIRECT_FUNC`:

| # | PC | Object Source | VTable Offset | r4 arg | Purpose |
|-|-|-|-|-|-|
| 1 | 0x82478DFC | *(0x831CC904) | vtable[1] (+4) | 0 | Device init |
| 2 | 0x824793FC | *(0x831CC904) | vtable[4] (+16) | 0x8201C018 (string) | Register device string 1 |
| 3 | 0x82479420 | *(0x831CC904) | vtable[4] (+16) | 0x8201C00C (string) | Register device string 2 |
| 4 | 0x824794AC | camera_obj (r3) | vtable[1] (+4) | &sp+496 (params) | Camera audio init |

## Float Constants

| Address | Used At | Likely Value | Purpose |
|-|-|-|-|
| 0x82000A34 | f31 | 0.0f | Zero init for audio params |
| 0x82000D48 | f30 | 1.0f | Unity gain / scale factor |
| 0x82000D58 | f1 for sub_82953AD0 | volume level | Master volume |
| 0x82000D74 | f1 for sub_8287A408 | distance param | 3D audio distance A |
| 0x82000DC8 | timing calc multiplier | rate constant | Audio frame rate mult |
| 0x82002510 | bbox values | coordinate | Audio bbox X/Y |
| 0x82003C04 | bbox Z | coordinate | Audio bbox Z |
| 0x82004D88 | min bbox | coordinate | Audio min bbox XY |
| 0x8201BFFC | min bbox Z | coordinate | Audio min bbox Z |
| 0x82AA457C | f1 for sub_8287A4E0 | distance param | 3D audio param B |
| 0x82AA4580 | f1 for sub_8287A4E8 | distance param | 3D audio param C |

## Critical Path Analysis: The Hang

### Known hang point: sub_82955BE0 (call 32)

From the INIT_PROBE labels in `imports.cpp`:
```
INIT_PROBE(sub_82955BE0, 3051, "82478AF8 phase21 xaudio-stream-HANG")
```

This function is called at PC `0x82478E3C`, after:
- Audio streaming thread started (sub_8228A1E0)
- RPF streaming started (sub_825FD6B8)

And before:
- Resource path setup
- Streaming manager activation
- Voice block creation

### The 1024-byte allocation at 0x82478EC4

This is call 38 (`sub_821B3510(1024)`), which occurs AFTER the hang point. The allocation itself is instrumented:

```
[TAIL-AF8] ENTER #N caller=0x82478EC4 size=1024 ...
```

If the game gets past sub_82955BE0, the 1024-byte alloc creates the buffer for `sub_827ADB48` (voice block creation). This is NOT the hang itself — it's subsequent to the hang.

### Call chain to hang

```
sub_821FC1F8 (master init)
  → sub_82478AF8 (this function)
    → sub_826225E0, sub_82622648, sub_826226B0 (env init)
    → sub_821B3510(176) → sub_8294BD68 (audio mgr ctor)
    → sub_8294BE28, sub_8294BE20, sub_8294BEA0 (config bbox/distance)
    → _stricmp x5 (renderer detection)
    → sub_8294E208 (finalize mgr)
    → sub_821B3510(192) → sub_82956718 (xaudio graph ctor)
    → sub_82953088, sub_82954618, sub_82955838 (graph config)
    → sub_8284D220 (load main resource)
    → sub_829530B0, sub_82953A68 (bind resource, set listener)
    → sub_8284E1C0 (spatial audio)
    → sub_8297B6B8 (voice buffer 512 samples)
    → sub_826BDC18 (device enum)
    → VTABLE[1] on device (init)
    → sub_8296A278(1) (enable output)
    → sub_82953AB0(1) (active voice=1)
    → sub_82953AD0 (master volume)
    → sub_8228A1E0 (start stream thread)
    → sub_825FD6B8 (start RPF stream)
    → *** sub_82955BE0 ← HANGS HERE ***
```

### Why it hangs

sub_82955BE0 is the XAudio streaming initialization. On Xbox 360, this would set up the XMA decoder hardware and DMA streaming channels. In the recompiled environment, this likely:

1. Waits for an XAudio2 callback that never fires (no real hardware)
2. Spins on a synchronization primitive (event/semaphore) waiting for the audio device
3. Attempts to communicate with the audio thread started by sub_8228A1E0, which itself may be stuck

The audio device vtable call at 0x82478DFC (call 26) initializes the device BEFORE sub_82955BE0. If the device init sets up a callback/event mechanism that the streaming init expects to be signaled, and the recompiled audio backend doesn't fire those signals, sub_82955BE0 will hang forever.

## Memory Layout Summary

```
0x82FF2360  ┌─ g_entityArray[1024] (12 bytes each, 12 KB)
0x82FF5360  ├─ g_pEngineObject (AudioManager*, 176 bytes)
0x82FF5364  ├─ g_pManager1 (XAudioGraph*, 192 bytes)
0x82FF5368  ├─ g_pManager2 (VoiceBlock*, from sub_827ADB48)
0x82FF536C  ├─ g_pViewportArray (Viewport[r30], 176 bytes each)
0x82FF5374  ├─ g_pCameraObj (CameraAudio*, 112 bytes)
0x82FF5378  ├─ g_initCounter (u32, 0)
0x82FF5380  ├─ path override ptr
0x82FF5404  ├─ name override ptr
0x82FF5414  ├─ g_strideValue (168)
0x82FF5418  ├─ g_totalMemSize (r26+r27+128)
0x82FF541C  └─ g_initFlags (bit field)
```

All addresses in the 0x82FF53xx range are the RAGE audio engine's global state table.
