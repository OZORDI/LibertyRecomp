# Audio Device Initialization Chain — Complete Pseudocode

**Parent**: `sub_82478AF8` — called during Phase 2/4/13 of audio subsystem init.
**Sources**: `gta4_recomp.50.cpp` (sub_827AD200/9C8/DB48), `gta4_recomp.55.cpp` (sub_8284D220), `gta4_recomp.63.cpp` (sub_8294BD68/BE20/BE28/BEA0/sub_8293EA08)

---

## 1. sub_8294BD68 — Audio Manager Constructor (176-byte struct)

**Signature**: `void sub_8294BD68(_DWORD *a1)`
**Source**: `gta4_recomp.63.cpp` lines 59168–59269

```
sub_8294BD68(r3=audio_mgr_ptr):
  r31 = r3   // save 'this'

  sub_8293EA08(r31)           // init base ring-buffer struct

  r10 = 0x820A29B8            // vtable (lis -32246, addi +10680)
  r9  = -1
  r8  = 80
  r7  = 10240
  r6  = 5
  r9_inner = 4
  r11 = 0
  r10_inner = -1

  // Load float constant from 0x82000A34
  f0 = *(float*)0x82000A34

  [r31 + 0]   = 0x820A29B8    // vtable pointer
  [r31 + 64]  = 0xFFFFFFFF    // flags = -1
  [r31 + 68]  = 0             // u8 flag
  [r31 + 80]  = (float4){0,0,0,0}  // VMX zeros (stvx128)
  [r31 + 96]  = f0            // float const (distance init)
  [r31 + 100] = 10240         // u32
  [r31 + 104] = 5             // u32
  [r31 + 108] = 4             // u32
  [r31 + 112] = 0
  [r31 + 116] = 0
  [r31 + 120] = 0
  [r31 + 124] = 0
  [r31 + 128] = 0             // sub-device array ptr (null at init)
  [r31 + 132] = 0
  [r31 + 136] = 0             // u16
  [r31 + 138] = 0             // u16
  [r31 + 140] = 0
  [r31 + 144] = 0xFFFF        // u16 = -1
  [r31 + 148] = 0
  [r31 + 152] = 0
  [r31 + 156] = 0
  return r3 = r31
```

---

## 2. sub_8293EA08 — Audio Device Base Init (ring-buffer + zero fields)

**Signature**: `void sub_8293EA08(_DWORD *a1)`
**Source**: `gta4_recomp.63.cpp` lines 26719–26775

```
sub_8293EA08(r3=base_ptr):
  r31 = r3

  r11 = 0x820A2624            // vtable (lis -32246, addi +9764)
  [r31 + 0] = 0x820A2624      // vtable pointer

  sub_8285FE48(r31 + 4)       // init 1024-entry ring buffer at offset +4

  r11 = 0
  [r31 + 36] = 0              // u8
  [r31 + 38] = 0              // u16
  [r31 + 40] = 0              // u16
  [r31 + 42] = 0              // u16
  [r31 + 44] = 0              // u16
  [r31 + 46] = 0              // u16
  [r31 + 48] = 0              // u16
  [r31 + 52] = 0              // u32
  return
```

---

## 3. sub_8294BE20 — Write Voice Count

**Signature**: `void sub_8294BE20(_DWORD *mgr, uint16_t count)`
**Source**: `gta4_recomp.63.cpp` line 59273–59279

```
sub_8294BE20(r3=mgr, r4=voice_count):
  [r3 + 38] = r4 (u16)        // write voice count to offset +38
  return
```

Called from `sub_82478AF8` with r4=6000 (cast to u16 = 0x1770).

---

## 4. sub_8294BE28 — Set Listener Position / Distance Scale

**Signature**: `void sub_8294BE28(_DWORD *mgr, float *pos_a, float *pos_b)`
**Source**: `gta4_recomp.63.cpp` lines 59283–59359

```
sub_8294BE28(r3=mgr, r4=pos_ptr_a, r5=pos_ptr_b):
  v13 = lvx128(r4)            // load float4 from pos_a (16-byte aligned)
  v12 = lvx128(r5)            // load float4 from pos_b
  v13 = v12 + v13             // vaddfp: sum of positions

  // Load scale constant from global addr (lis -32080, addi +29184 = 0x827E7200-area)
  v0 = lvx128(scale_addr)
  v0 = v13 * v0               // vmulfp: scaled position

  // Store float4 result to mgr+80
  stvx128(v0, r3 + 80)        // listener position float4

  // Scalar float work: compute min of (pos_b.xyz - pos_a.xyz) components
  f13 = pos_b[0] - pos_a[0]
  f0  = pos_b[1] - pos_a[1]
  f12 = pos_b[2] - pos_a[2]
  f10 = fsel(f13-f0, f13, f0) // max(x_diff, y_diff)
  f0  = fsel(f10-f12, f10, f12) // max of above, z_diff
  f0  = frsp(f0)               // round to single

  // Load multiplier from 0x82000A34+832 (lfs f0,3444(lis_-32256)) = float const
  f0 = f0 * float_const

  [r3 + 96] = f0              // distance scale result
  return
```

---

## 5. sub_8294BEA0 — Write Secondary Distance Parameter

**Signature**: `void sub_8294BEA0(_DWORD *mgr, uint16_t param)`
**Source**: `gta4_recomp.63.cpp` lines 59363–59369

```
sub_8294BEA0(r3=mgr, r4=param):
  [r3 + 136] = r4 (u16)       // secondary distance/rolloff to offset +136
  return
```

Called from `sub_82478AF8` with r4=1500.

---

## 6. sub_8284D220 — Audio Format Descriptor Builder

**Signature**: `void sub_8284D220(void *out, void *callback, int flags, void *buf, int stride)`
**Source**: `gta4_recomp.55.cpp` lines 17061–17134

```
sub_8284D220(r3=out_struct, r4=callback_ptr, r5=flags, r6=buf, r7=stride):
  r31 = r3
  r30 = r7    // stride

  if (r4 == NULL) goto null_path

  [r31 + 8] = r4              // callback function pointer
  sub_82A00DC0(r31, r6, 16)   // memcpy 16 bytes: buf -> out_struct+0..15

  if (r30 < 8):
    // zero-pad remaining bytes: sub_829FF840(r31+r30, 0, 8-r30)
    sub_829FF840(r31 + r30, 0, 8 - r30)
  return r3 = r31

null_path:
  [r31 + 0] = r5 (flags)
  [r31 + 8] = 0
  return
```

**Purpose**: Builds a streaming audio format descriptor. With non-null callback: copies 16-byte format header and sets callback pointer at +8. With null callback: stores flags at +0, zeros callback.

---

## 7. sub_827AD200 — Sound Engine Pool Constructor (the hang path)

**Signature**: `void sub_827AD200(void *engine, ...7 descriptor args...)`
**Source**: `gta4_recomp.50.cpp` lines 6699–7171

```
sub_827AD200(r3=engine, r4..r10=7 descriptor structs):
  r31 = r3       // 'this' = engine struct
  r30 = 0

  // Store vtable precursor
  r11 = 0x82876B98   // (lis -32248, addi -29400)
  [r31 + 0] = r11    // preliminary vtable/tag

  // Init sub-struct at engine+16
  sub_827ACCE0(r31 + 16)

  // Init float fields
  f0 = *(float*)0x82A00A34   // ~0.0f from 0x82A20000+2612
  [r31 + 736] = f0   // float x4 (initial state)
  [r31 + 740] = f0
  [r31 + 744] = f0
  [r31 + 748] = f0

  // Config fields
  [r31 + 760] = 2    // u32
  [r31 + 764] = -1   // u32 (0xFFFFFFFF)
  [r31 + 768] = 1    // u32
  [r31 + 772] = -1   // u32
  [r31 + 776] = 0    // u8 x4
  [r31 + 779] = 0

  // Build first format descriptor on stack, copy to engine+780
  sub_8284D220(stack, callback=0x827ACCA8, 0, stack_buf, stride)
  // Copy 7 descriptor groups into engine at offsets +780..+908+:
  //   +780, +796, +812, +828, +844, +860, +876, +892+16=+908
  // (batched copy of 4 descriptors: r8=+780, r7=+796, r6=+812, ...)
  // Each descriptor is 16 bytes (4 floats or 4 ints)

  // r31 now reassigned = original_r31 + 892 (pointer arithmetic)
  // new r31 covers further descriptor fields

  // Global store: save engine ptr to 0x8318F1A0
  [0x8318F1A0] = r30   // (lis -31975, stw r30,-3680(r11))

  // Allocate 64-byte block
  sub_821B3510(64)
  if (result): sub_827F59C8(result, 64)
  [engine + 756] = result  // u32 ptr

  // Allocate 36-byte block
  sub_821B3510(36)
  if (result): sub_827BBCB0(result, stack[420])
  else: result = 0
  [engine + 752] = result  // u32 ptr

  // Conditional callback init
  if ([stack+471] == 0): sub_827BC040(engine, 0)

  return r3 = engine
```

**Global write**: `0x8318F1A0` ← engine pointer

---

## 8. sub_827AD9C8 — Sound Engine Constructor (mid-level)

**Signature**: `void *sub_827AD9C8(void *engine, ...args...)`
**Source**: `gta4_recomp.50.cpp` lines 7821–7964

```
sub_827AD9C8(r3=engine, r4=arg1, r5=arg2, r6..r10=5 more args):
  r31=engine, r30=r4, r29=r5, r28=r6, r27=r7, r26=r8, r25=r9, r24=r10

  // Copy 6 descriptor structs from caller's args into local stack
  // 6 × sub_82A00DC0(stack_dst, src, 16)
  sub_82A00DC0(stack+208, stack+528, 16)   // desc 1
  sub_82A00DC0(stack+192, stack+512, 16)   // desc 2
  sub_82A00DC0(stack+160, stack+480, 16)   // desc 3 (with header fix)
  sub_82A00DC0(stack+128, stack+448, 16)   // desc 4 (with header fix)
  sub_82A00DC0(stack+112, stack+432, 16)   // desc 5
  sub_82A00DC0(stack+ 96, stack+416, 16)   // desc 6

  // Call the deep constructor
  sub_827AD200(engine, r30, r29, r28, r27, r26, r25, r24, stack[88])

  // Set vtable
  r11 = 0x82078D38   // (lis -32248, addi -29384)
  [r31 + 0] = r11    // vtable pointer

  // Launch streaming thread
  sub_8279A6D0(engine + 992, r4=20)   // r3=struct+992, r4=20 (thread count?)

  return r3 = engine
```

---

## 9. sub_827ADB48 — Sound Engine Outer Constructor (top-level hang entry)

**Signature**: `void *sub_827ADB48(void *engine, ...args...)`
**Source**: `gta4_recomp.50.cpp` lines 8045–8179

```
sub_827ADB48(r3=engine, r4=arg1, r5=arg2, r6..r10=5 more):
  r31=engine, r30=r4, r29=r5, r28=r6, r27=r7, r26=r8, r25=r9, r24=r10

  // Copy 6 descriptor structs (same pattern as sub_827AD9C8)
  sub_82A00DC0(stack+208, stack+512, 16)
  sub_82A00DC0(stack+192, stack+496, 16)
  sub_82A00DC0(stack+160, stack+464, 16)   // with header fix
  sub_82A00DC0(stack+128, stack+432, 16)   // with header fix
  sub_82A00DC0(stack+112, stack+416, 16)
  sub_82A00DC0(stack+ 96, stack+400, 16)

  // Call mid-level constructor
  sub_827AD9C8(engine, r30, r29, r28, r27, r26, r25, r24, stack[88])

  // Set vtable (outer)
  r11 = 0x82078D48   // (lis -32248, addi -29368)
  [r31 + 0] = r11    // vtable pointer

  return r3 = engine
```

---

## Audio Device Struct Layout (176-byte audio manager)

Based on writes in `sub_8294BD68` + `sub_8293EA08` + parameter setters:

| Offset | Size | Init value | Set by | Meaning |
|-|-|-|-|-|
| 0 | 4 | `0x820A29B8` | `sub_8294BD68` | Outer vtable |
| 0 | 4 | `0x820A2624` | `sub_8293EA08` (first) | Base vtable (overwritten by BD68) |
| 4 | ~32 | ring buffer | `sub_8285FE48` | 1024-entry ring buffer struct |
| 36 | 1 | 0 | `sub_8293EA08` | u8 flag |
| 38 | 2 | 0 | `sub_8293EA08` | u16, overwritten by `sub_8294BE20` with voice_count |
| 40 | 2 | 0 | `sub_8293EA08` | u16 |
| 42 | 2 | 0 | `sub_8293EA08` | u16 |
| 44 | 2 | 0 | `sub_8293EA08` | u16 |
| 46 | 2 | 0 | `sub_8293EA08` | u16 |
| 48 | 2 | 0 | `sub_8293EA08` | u16 |
| 52 | 4 | 0 | `sub_8293EA08` | u32 |
| 64 | 4 | 0xFFFFFFFF | `sub_8294BD68` | flags = -1 |
| 68 | 1 | 0 | `sub_8294BD68` | u8 |
| 80 | 16 | {0,0,0,0} | `sub_8294BD68` (vmx) / `sub_8294BE28` | float4 listener position |
| 96 | 4 | float_const | `sub_8294BD68` / `sub_8294BE28` | distance scale float |
| 100 | 4 | 10240 | `sub_8294BD68` | u32 config |
| 104 | 4 | 5 | `sub_8294BD68` | u32 config |
| 108 | 4 | 4 | `sub_8294BD68` | u32 config |
| 112 | 4 | 0 | `sub_8294BD68` | u32 |
| 116 | 4 | 0 | `sub_8294BD68` | u32 |
| 120 | 4 | 0 | `sub_8294BD68` | u32 |
| 124 | 4 | 0 | `sub_8294BD68` | u32 |
| 128 | 4 | 0 | `sub_8294BD68` | ptr: sub-device array |
| 132 | 4 | 0 | `sub_8294BD68` | u32 |
| 136 | 2 | 0 → 1500 | `sub_8294BD68` / `sub_8294BEA0` | secondary distance param (u16) |
| 138 | 2 | 0 | `sub_8294BD68` | u16 |
| 140 | 4 | 0 | `sub_8294BD68` | u32 |
| 144 | 2 | 0xFFFF | `sub_8294BD68` | u16 = -1 |
| 148 | 4 | 0 | `sub_8294BD68` | u32 |
| 152 | 4 | 0 | `sub_8294BD68` | u32 |
| 156 | 4 | 0 | `sub_8294BD68` | u32 |

**Note**: The struct is 176 bytes total (allocated by `sub_821B3510(176)` in `sub_82478AF8` Phase 2).

---

## Sound Engine Struct Key Fields (sub_827AD200 / sub_827ADB48)

| Offset | Written by | Value | Meaning |
|-|-|-|-|
| 0 | `sub_827ADB48` | `0x82078D48` | Outer vtable |
| 0 | `sub_827AD9C8` | `0x82078D38` | Mid vtable (overwritten by ADB48) |
| 0 | `sub_827AD200` | `0x82876B98` | Initial tag (overwritten) |
| 16 | `sub_827AD200` → `sub_827ACCE0` | sub-struct | Nested sub-struct init |
| 736 | `sub_827AD200` | float_const | f32 field x1 |
| 740 | `sub_827AD200` | float_const | f32 field x2 |
| 744 | `sub_827AD200` | float_const | f32 field x3 |
| 748 | `sub_827AD200` | float_const | f32 field x4 |
| 752 | `sub_827AD200` | alloc ptr | 36-byte block via `sub_827BBCB0` |
| 756 | `sub_827AD200` | alloc ptr | 64-byte block via `sub_827F59C8` |
| 760 | `sub_827AD200` | 2 | u32 config |
| 764 | `sub_827AD200` | -1 | u32 flag |
| 768 | `sub_827AD200` | 1 | u32 flag |
| 772 | `sub_827AD200` | -1 | u32 flag |
| 776-779 | `sub_827AD200` | 0 | u8 x4 |
| 780+ | `sub_827AD200` | descriptors | 7 × 16-byte format descriptor array |
| 992 | (used as arg) | thread ctx | Passed to `sub_8279A6D0` as thread launch target |

**Global written**: `0x8318F1A0` ← engine struct pointer (saved by `sub_827AD200`)

---

## Hang Analysis Summary

```
sub_82478AF8
  └─ sub_827ADB48   (gta4_recomp.50.cpp:8045)
       └─ sub_827AD9C8  (line 7821)
            ├─ sub_827AD200  (line 6699)
            │    ├─ sub_827ACCE0   — sub-struct init
            │    ├─ sub_8284D220   — format descriptor builder
            │    ├─ sub_821B3510   — allocator (64 bytes)
            │    ├─ sub_827F59C8   — 64-byte block init
            │    ├─ sub_821B3510   — allocator (36 bytes)
            │    ├─ sub_827BBCB0   — 36-byte block init
            │    └─ sub_827BC040   — optional callback init
            └─ sub_8279A6D0  (line 7954) ← LAUNCHES STREAMING THREAD (r4=20)
```

`sub_8279A6D0` is called with `r3 = engine + 992`, `r4 = 20`. This is the streaming I/O thread launcher. On Xbox 360 this calls into the XMA/XAudio kernel thread creation path. In LibertyRecomp the likely hang point is `sub_8279A6D0` attempting to create a native thread via an unhooked `XCreateThread` analog, or `sub_827BC040` / `sub_827BBCB0` blocking on an audio kernel event.

**Recommended hook target**: `sub_8279A6D0` (at `0x8279A6D0`) — stub to no-op to skip thread launch. Alternatively hook `sub_827ADB48` entirely and return a zeroed struct.
