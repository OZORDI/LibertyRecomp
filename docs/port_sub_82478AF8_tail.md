# sub_82478AF8 Tail Section Analysis

Source: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.20.cpp` lines 84287-85184

This documents every call after `sub_82477670` (streaming tick, phase 22) through the function epilogue. The earlier phases (1-22) work. The hang is in **sub_827C2420** which calls **sub_82852DD0** (OpenAndProcess).

## Key Global Addresses

| Symbol | Address | Description |
|-|-|
| g_streamingMgrPtr | 0x82B393A4 | Pointer to streaming manager object |
| g_streamingCS | 0x82B07278 | Critical section for streaming |
| g_threadMgr | 0x831E55EC | Thread manager pointer |
| g_voiceDesc | 0x83016A28 | Voice descriptor global |
| g_voiceBlock | 0x82FF5368 | Stores audVoiceBlock* |
| g_voiceList | 0x82FF536C | Stores audVoice array pointer |
| g_voiceExtra | 0x82FF5374 | Stores stream block |
| g_initDone | 0x82FF5378 | Init-complete flag (set to 0 at end) |
| g_zeroArray | 0x82FF2360 | 1024-entry x 12-byte zeroed array |
| g_audioOutputDev | *(0x831C21D4) | AudioOutputDevice singleton |
| g_byteFlag | 0x82AE5F84 | Byte flag (set to 0 = r23) |
| g_flagWord | 0x82B1AEF8 | Word flag (bit 9 cleared) |

## Pseudocode

```c
// === TAIL OF sub_82478AF8 (after streaming tick) ===

// --- STEP T1: Store frame count, activate streaming ---
streaming_mgr = *(uint32_t*)0x82B393A4;
streaming_mgr->field_56 = r31;          // store frame count
sub_827C2420(streaming_mgr);            // *** HANG POINT ***

// --- STEP T2: Release streaming lock ---
sub_8284E830(r29);                      // r29->refcount-- (at offset 3072)

// --- STEP T3: Allocate voice block buffer (1024 bytes) ---
void* voice_buf = sub_821B3510(1024);   // HeapAlloc(1024)
r30 += 64;                              // advance voice cursor

if (voice_buf == NULL) goto skip_voice_setup;

// --- STEP T4: Build 7 format descriptors via sub_8284D220 ---
// Each sub_8284D220(out, name_ptr, callback_ptr, extra, size=4)
// builds a 16-byte "format descriptor" struct:
//   [0..3] = callback_ptr  (or NULL for default)
//   [4..7] = reserved
//   [8..11] = name_ptr     (string pointer)
//   [12..15] = extra/vtable

// Descriptor 1 @ sp+464: name=0x82165060, callback=NULL, vtable=0x82444808
sub_8284D220(&sp[464], NULL, 0x82165060, 0, 0);
sp[476] = 0x82444808;   // patch vtable into descriptor

// Copy descriptor 1 to sp+560
memcpy16(&sp[560], &sp[464]);

// Descriptor 2 @ sp+400: name=g_voiceDesc(0x83016A28), callback=0x824F34F0
//   extra=sp+244 containing 0x822BCA90 (NOP function)
sub_8284D220(&sp[400], g_voiceDesc, 0, &sp[244], 4);
sp[244] = 0x822BCA90;

// Copy descriptor 2 to sp+592
memcpy16(&sp[592], &sp[400]);

// Descriptor 3 @ sp+384: name=g_voiceDesc, callback=0x822BCA90
//   extra=sp+260 containing 0x822BCA90
sub_8284D220(&sp[384], g_voiceDesc, 0, &sp[260], 4);
sp[412] = 0x822188F8;   // override vtable
sp[260] = 0x822BCA90;

// Copy descriptor 3 to sp+608
memcpy16(&sp[608], &sp[384]);

// Descriptor 4 @ sp+352: name=g_voiceDesc, callback=0x824F3310
//   extra=sp+256 containing 0x824F3310
sub_8284D220(&sp[352], g_voiceDesc, 0, &sp[256], 4);
sp[396] = 0x822188F8;
sp[256] = 0x824F3310;

// Copy descriptor 4 to sp+576
memcpy16(&sp[576], &sp[352]);

// Descriptor 5 @ sp+432: name=g_voiceDesc, callback=0x822BCA90
//   extra=sp+248 containing 0x822BCA90
sub_8284D220(&sp[432], g_voiceDesc, 0, &sp[248], 4);
sp[364] = 0x822188F8;
sp[248] = 0x822BCA90;

// Copy descriptor 5 to sp+448
memcpy16(&sp[448], &sp[432]);

// Descriptor 6 @ sp+368: name=g_voiceDesc, callback=0x824F3320
//   extra=sp+252 containing 0x824F3320
sub_8284D220(&sp[368], g_voiceDesc, 0, &sp[252], 4);
sp[444] = 0x822188F8;
sp[252] = 0x824F3320;

// Copy descriptor 6 to sp+272
memcpy16(&sp[272], &sp[368]);

// Descriptor 7 @ sp+416: name=g_voiceDesc, callback=0x824F3808
//   extra=sp+240 containing 0x824F3808
sub_8284D220(&sp[416], g_voiceDesc, 0, &sp[240], 4);
sp[380] = 0x822188F8;
sp[240] = 0x824F3808;

// Copy descriptor 7 to sp+480
memcpy16(&sp[480], &sp[416]);

// --- STEP T5: Create 3 default format descriptors via sub_82478A80 ---
// sub_82478A80(out) builds a descriptor with callback=0x822BCA90 (NOP)
//   and vtable=0x82444808
r29 = sub_82478A80(&sp[640]);   // format desc A
r28 = sub_82478A80(&sp[624]);   // format desc B
r31 = sub_82478A80(&sp[656]);   // format desc C

// --- STEP T6: Copy format descriptors into voice routing arrays ---
sp[231] = r25;   // byte flag from earlier
memmove(&sp[208], r29, 16);     // from desc A
memmove(&sp[192], r28, 16);     // from desc B
sp[180] = r30;                   // voice cursor
sp[190] = (uint16_t)r30;
memmove(&sp[160], &sp[560], 16);  // from desc 1 copy
sp[148] = r23;                   // (0 = initial value)
memmove(&sp[128], &sp[592], 16);  // from desc 2 copy
memmove(&sp[112], &sp[608], 16);  // from desc 3 copy
memmove(&sp[96],  &sp[576], 16);  // from desc 4 copy

// --- STEP T7: Create audVoiceBlock ---
// Load all descriptor pairs into registers
sp[88] = *(uint64_t*)&sp[448];   // stack pass
sub_827ADB48(voice_buf,          // r3 = 1024-byte buffer
             *(u64*)r31,         // r4,r5 = desc C (16 bytes as 2x u64)
             *(u64*)&sp[480],    // r6,r7 = desc 7 copy
             *(u64*)&sp[272],    // r8,r9 = desc 6 copy
             *(u64*)&sp[448]);   // r10 = desc 5 copy (+ sp[88])

// Store voice block handle
*(uint32_t*)0x82FF5368 = return_value;   // g_voiceBlock
goto voice_done;

skip_voice_setup:
    *(uint32_t*)0x82FF5368 = 0;  // g_voiceBlock = NULL (r23=0)

voice_done:

// --- STEP T8: Frequency/sample rate check + voice array allocation ---
*(uint8_t*)0x82AE5F84 = 0;      // clear byte flag

// Overflow check: r30 * 176 must not overflow
if (r30 > 0x01745A97 /*24,403,607*/) {
    alloc_size = -1;  // force failure
} else {
    alloc_size = r30 * 176 + 16;
}

void* voice_array = sub_821B3510(alloc_size);  // HeapAlloc
if (voice_array == NULL) {
    *(uint32_t*)0x82FF536C = 0;  // g_voiceList = NULL
} else {
    *(uint32_t*)(voice_array) = r30;   // store count at header
    voice_list = voice_array + 16;     // skip header

    // Initialize each voice (176 bytes each)
    for (int i = r30-1; i >= 0; i--) {
        sub_8261FBA0(&voice_list[i]);  // audVoice::Init
    }
    *(uint32_t*)0x82FF536C = voice_list;  // g_voiceList
}

// --- STEP T9: Allocate pointer array for voices ---
// Overflow check: r30 * 4
if (r30 > 0x3FFFFFFF) {
    ptr_alloc = -1;
} else {
    ptr_alloc = r30 * 4;
}

void* ptr_array = sub_821B3510(ptr_alloc);  // HeapAlloc
if (r30 > 0) {
    // Fill pointer array: each entry points to voice_list[i]
    // Also set voice_list[i].field_160 = 18 (category/type)
    uint32_t offset = 0;
    for (uint32_t i = r30; i > 0; i--) {
        void* voice = g_voiceList + offset;
        voice->field_160 = 18;
        ptr_array[r30-i] = voice;
        offset += 176;
    }
}

// --- STEP T10: Bind voice block to mixer and configure pools ---
sub_827ACC98(g_voiceBlock, ptr_array);  // voice->f752 -> sub_827BC040 (mixer bind)
sub_821B3560(ptr_array);                // HeapFree(ptr_array)

// Configure 3D audio pools on voice block
g_voiceBlock->field_768 = 128;
sub_827ACCA0(g_voiceBlock, 8192, 200);   // pool A: 8192 size, 200 limit
sub_827ACCA0(g_voiceBlock, 4096, 300);   // pool B: 4096 size, 300 limit
sub_827ACCA0(g_voiceBlock, 2048, 500);   // pool C: 2048 size, 500 limit

// --- STEP T11: Set working directory for audio ---
sub_821B5038("0x8201C034");   // SetCurrentDir (audio assets path)

// --- STEP T12: Create AudioOutputDevice singleton ---
sub_8287AC38();  // alloc 48 bytes + construct -> store at *(0x831C21D4)

// --- STEP T13: Register audio file sources ---
g_audioDev = *(uint32_t*)0x831C21D4;
sub_8287A6A8(g_audioDev, "0x8201C02C", 6);  // register source A (priority 6)
sub_8287A6A8(g_audioDev, "0x8201C024", 4);  // register source B (priority 4)

// --- STEP T14: Set working directory for audio data ---
sub_821B5038("0x820B92FC");   // SetCurrentDir (audio data path)

// --- STEP T15: Init 3D audio system ---
sub_8261C7C8();  // audio3D init (builds path string, opens device)

// --- STEP T16: Configure 3D audio parameters ---
g_audioDev = *(uint32_t*)0x831C21D4;
sub_8287A408(g_audioDev, f1=*(float*)0x82000D74);  // setDistanceFactor
sub_8287A4E0(g_audioDev, f1=*(float*)0x82AA457C);  // setDopplerFactor
sub_8287A4E8(g_audioDev, f1=*(float*)0x82AA4580);  // setRolloffFactor

// --- STEP T17: Create 2 audio listeners via vtable dispatch ---
audio_obj1 = *(uint32_t*)(r26 - 14076);
audio_obj1->vtable[4](audio_obj1, "0x8201C018");  // createListener A
sub_8299B4A8(audio_obj1, f30);                      // setVolume(f30)

audio_obj2 = *(uint32_t*)(r26 - 14076);
audio_obj2->vtable[4](audio_obj2, "0x8201C00C");  // createListener B
sub_8299B4A8(audio_obj2, f30);                      // setVolume(f30)

// --- STEP T18: Clear hardware flag, allocate stream block ---
*(uint32_t*)0x82B1AEF8 &= ~0x200;  // clear bit 9

void* stream_block = sub_821B3510(112);  // HeapAlloc(112)
if (stream_block != NULL) {
    stream_obj = sub_823B33F8(stream_block);  // construct stream block
} else {
    stream_obj = NULL;
}

// --- STEP T19: Store stream block + configure voice parameters ---
*(uint32_t*)0x82FF5374 = stream_obj;  // g_voiceExtra

// Build two 3D audio parameter structs on stack (sp+496..sp+540)
// with f30/f31 float values, zero vector
stream_obj->vtable[1](stream_obj, &sp[496], 0, 0);  // configure stream

*(uint32_t*)0x82FF5378 = 0;  // g_initDone = 0 (r23)

// --- STEP T20: Zero-fill global voice state array ---
// 1024 entries * 12 bytes = 12288 bytes at 0x82FF2360
ptr = 0x82FF2360 + 4;
for (int i = 1024; i > 0; i--) {
    *(uint32_t*)(ptr - 4) = 0;   // field 0
    *(uint32_t*)(ptr + 0) = 0;   // field 1
    *(uint32_t*)(ptr + 4) = 0;   // field 2
    ptr += 12;
}

// --- STEP T21: Sync/finalize ---
sub_822BCA90();  // NOP (just blr) - placeholder for GPU sync

// --- EPILOGUE ---
// Restore f30, f31, GPRs r23-r31, return
```

## Detailed Function Reference

### sub_827C2420 -- ActivateStreaming (HANG POINT)

**Location**: `gta4_recomp.50.cpp:57394`
**Args**: r3 = streaming manager object (from *0x82B393A4)
**Flow**:
1. `sub_8284F310(CS=0x82B07278, mgr->field_56)` -- AcquireLock + copy resource path string (up to 255 chars) into stack buffer. Checks first byte != '$', calls sub_8284E690 for validation. Increments refcount at CS+3072.
2. `mgr->vtable[1](mgr)` -- Virtual call to get resource name/handle. Result -> r6.
3. `sub_82852DD0(threadMgr=*0x831E55EC, path=0x8207A57C, name=0x820BBFE4, r6=vtable_result, r7=mgr, r8=1)` -- **THIS IS THE HANG**. OpenAndProcess: calls sub_8284F468 to find a free resource slot, then sub_82852D18 to process it (which does a vtable[2] dispatch that can block on I/O), then sub_8285B088 to flush.
4. `sub_8284E830(CS)` -- ReleaseLock: decrements *(CS+3072).

**Why it hangs**: sub_82852DD0 -> sub_8284F468 searches for a free slot in the streaming resource table (up to 3076 slots). sub_82852D18 then does a vtable[2] dispatch on the found resource. This vtable dispatch resolves to an Xbox 360 XAudio2/DirectSound streaming callback that blocks waiting for hardware I/O completion that never arrives on macOS.

**Porting status**: NEEDS NATIVE REWRITE. The vtable dispatch at the bottom of sub_82852D18 targets Xbox audio hardware. The resource slot finding (sub_8284F468) and lock management (sub_8284F310/sub_8284E830) are pure game logic and can stay recompiled.

### sub_8284D220 -- FormatDescriptorInit

**Location**: `gta4_recomp.55.cpp:17061`
**Args**: r3=output(16 bytes), r4=name_ptr(or NULL), r5=callback, r6=extra, r7=size
**What**: If r4 != NULL, copies `size` bytes from r6 to r3, stores r4 at r3+8, zero-pads if size < 8. If r4 == NULL, stores r5 at r3+0, zeroes r3+8.
**Touches GPU**: No. Pure data structure initialization.
**Porting**: SAFE -- stays as recompiled.

### sub_82478A80 -- DefaultFormatDescriptor

**Location**: `gta4_recomp.20.cpp:83674`
**Args**: r3 = output buffer (16 bytes)
**What**: Calls sub_8284D220 with callback=0x822BCA90 (NOP function), then patches vtable to 0x82444808. Returns pointer to the output.
**Touches GPU**: No.
**Porting**: SAFE.

### sub_827ADB48 -- audVoiceBlock::Create

**Location**: `gta4_recomp.50.cpp:8045`
**Args**: r3=buffer, r4-r5=format pair 1, r6-r7=format pair 2, r8-r9=format pair 3, r10=format 4, sp+88=format 5
**What**: Copies 6 format descriptors via memmove, passes to sub_827AD9C8 (inner constructor), sets vtable to 0x82078D48. The inner constructor (sub_827AD9C8) does the same -- copies descriptors plus extra fields like sample rate and channel count.
**Touches GPU**: No. Pure voice routing setup.
**Porting**: SAFE.

### sub_822BCA90 -- SyncFinalize (NOP)

**Location**: `gta4_recomp.9.cpp:60535`
**What**: Just `blr` (return). Originally a GPU sync barrier or render-thread fence that was compiled out for the Xbox 360 build.
**Porting**: SAFE (already a no-op).

### sub_8284E830 -- ReleaseLock (refcount--)

**Location**: `gta4_recomp.55.cpp:20574`
**Args**: r3 = critical section object
**What**: Decrements *(r3 + 3072) by 1.
**Porting**: SAFE.

### sub_821B3510 -- HeapAlloc

**Location**: `gta4_recomp.2.cpp:83062`
**Args**: r3 = size
**What**: Reads TLS[r13+1676] for allocator, calls vtable[2](allocator, size, align=16, flags=0). Game's malloc equivalent.
**Porting**: SAFE (already hooked via rexcrt heap).

### sub_821B3560 -- HeapFree

**Location**: `gta4_recomp.2.cpp:83116`
**Args**: r3 = ptr
**What**: If ptr != NULL, calls TLS allocator vtable[3](allocator, ptr). Game's free equivalent.
**Porting**: SAFE.

### sub_82A00DC0 -- memmove

**What**: Copies `r5` bytes from `r4` to `r3`. Used extensively to copy 16-byte format descriptors.
**Porting**: SAFE.

### sub_8261FBA0 -- audVoice::Init

**Location**: `gta4_recomp.36.cpp:18485`
**Args**: r3 = voice struct (176 bytes)
**What**: Calls sub_827BA840 (base init), sets vtable at +0 to 0x8201FC0C, vtable at +80 to 0x8201FBE8, field_160=13, field_164=0, field_12=0.
**Touches GPU**: No. Pure game logic struct init.
**Porting**: SAFE.

### sub_827ACC98 -- VoiceBlock::BindMixer

**Location**: `gta4_recomp.50.cpp:5834`
**Args**: r3 = voice block, r4 = ptr_array
**What**: Reads voice_block->field_752, tail-calls sub_827BC040 which allocates a 20-byte mixer node and links voices to it.
**Touches GPU**: No.
**Porting**: SAFE.

### sub_827ACCA0 -- VoiceBlock::ConfigurePool

**Location**: `gta4_recomp.50.cpp:5845`
**Args**: r3 = voice block, r4 = pool_size, r5 = limit
**What**: Reads voice_block->field_756, tail-calls sub_827F5860 which creates a voice pool with the given size/limit parameters. Involves buffer alignment to 16 bytes.
**Touches GPU**: No (pure memory pool management).
**Porting**: SAFE.

### sub_8287AC38 -- AudioOutputDevice::Create

**Location**: `gta4_recomp.57.cpp:11986`
**What**: Allocates 48 bytes, calls sub_8287A878 (constructor), stores result at *(0x831C21D4).
**Touches GPU**: The constructor (sub_8287A878) likely initializes XAudio2/DirectSound output.
**Porting**: NEEDS REVIEW. If sub_8287A878 touches Xbox audio hardware, this needs a native rewrite. However, it may just create an abstraction layer struct that gets populated later.

### sub_8287A6A8 -- AudioOutputDevice::RegisterSource

**Location**: `gta4_recomp.57.cpp:11162`
**Args**: r3 = device, r4 = source_path_string, r5 = priority
**What**: Uses sub_8284F468 to find a streaming resource slot, reads 4 bytes from it (sub_8285AD08), allocates memory for the source data, copies it, and registers it with the device.
**Touches GPU**: No (file I/O only).
**Porting**: SAFE -- uses streaming system which is already hooked.

### sub_821B5038 -- SetWorkingDirectory

**Location**: `gta4_recomp.2.cpp:87184`
**Args**: r3 = path string
**What**: Parses path (checks for '/', '\\', ':'), sets a global working directory flag. Pure file system path management.
**Porting**: SAFE.

### sub_8261C7C8 -- Audio3D::Init

**Location**: `gta4_recomp.36.cpp:10649`
**What**: Concatenates two strings (from 0x8201BC30 and 0x8201FB98), checks total length < 128, builds a combined path. Uses this path to initialize 3D audio subsystem.
**Touches GPU**: No (string manipulation + file path setup).
**Porting**: SAFE.

### sub_8287A408 -- SetDistanceFactor

**Location**: `gta4_recomp.57.cpp:10721`
**What**: Trampoline to sub_8287BE88. Sets distance factor for 3D audio spatialization.
**Touches GPU**: Likely writes to XAudio2 3D settings.
**Porting**: NEEDS REVIEW for audio backend.

### sub_8287A4E0 -- SetDopplerFactor

**What**: Trampoline to sub_8287C038. Same category as above.
**Porting**: NEEDS REVIEW.

### sub_8287A4E8 -- SetRolloffFactor

**What**: Trampoline to sub_8287C060.
**Porting**: NEEDS REVIEW.

### sub_8299B4A8 -- SetVolume

**Location**: `gta4_recomp.65.cpp:51991`
**Args**: r3 = audio object, f1 = volume (float)
**What**: Stores f1 at *(r3 + 12). Simple field write.
**Porting**: SAFE.

### sub_823B33F8 -- ConstructStreamBlock

**Location**: `gta4_recomp.15.cpp:3123`
**Args**: r3 = 112-byte buffer
**What**: Sets vtable to 0x82014B74, initializes float fields at +16..+28 to 0.0, various bitfield manipulations for audio format (sample rate = 3073, channels, etc). Sets up streaming decode params.
**Touches GPU**: No. Pure struct init.
**Porting**: SAFE.

### Vtable Dispatch Calls (Lines 85024-85053)

Two vtable[4] calls on an audio listener object loaded from `*(r26 - 14076)`. These create named audio listener channels. The vtable target is read from `obj->vtable[4]` (offset 16 in vtable). Each is followed by sub_8299B4A8 to set volume.

**Porting**: NEEDS REVIEW -- the vtable[4] target likely creates Xbox audio submix voices.

## Porting Summary

### SAFE (stays as recompiled code)
- sub_8284D220 (format descriptor init)
- sub_82478A80 (default format descriptor)
- sub_8284E830 (refcount--)
- sub_821B3510 / sub_821B3560 (heap alloc/free)
- sub_82A00DC0 (memmove)
- sub_827ADB48 (voice block create)
- sub_827AD9C8 (voice block inner constructor)
- sub_822BCA90 (NOP)
- sub_8261FBA0 (voice init)
- sub_827ACC98 (mixer bind)
- sub_827ACCA0 (pool config)
- sub_821B5038 (set working directory)
- sub_8261C7C8 (audio3D path init)
- sub_8299B4A8 (set volume -- simple store)
- sub_823B33F8 (stream block construct)
- sub_8287A6A8 (register source -- file I/O)
- All memmove/memcpy chains
- The voice array allocation + init loop
- The pointer array + category assignment loop
- The 1024-entry zero-fill loop

### NEEDS NATIVE REWRITE
- **sub_827C2420** -- specifically the sub_82852DD0 call inside it. The lock management (sub_8284F310, sub_8284E830) is fine, but sub_82852DD0 dispatches to a vtable[2] callback that targets Xbox streaming hardware.
- **sub_8287AC38** (AudioOutputDevice constructor) -- if sub_8287A878 touches XAudio2.
- **sub_8287A408 / sub_8287A4E0 / sub_8287A4E8** (3D audio parameter setters) -- if targets write to XAudio2 3D listener state.
- **Vtable[4] dispatch** (lines 85024-85053) -- audio listener creation, likely XAudio2 submix voices.

### ROOT CAUSE OF HANG
The hang is in `sub_827C2420` -> `sub_82852DD0` -> `sub_82852D18` -> vtable[2] dispatch. The vtable[2] target is a streaming resource processor that on Xbox 360 would submit I/O requests and wait for DMA completion. On macOS, this I/O completion never signals because the XAudio2 streaming backend doesn't exist. The fix is to hook sub_827C2420 or sub_82852DD0 to either:
1. Skip the streaming activation entirely (if audio can be deferred), or
2. Provide a native streaming backend that completes immediately.
