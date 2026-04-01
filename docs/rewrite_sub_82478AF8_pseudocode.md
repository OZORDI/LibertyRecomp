# Pseudocode Reconstruction: sub_82478AF8

**Source**: `gta4_recomp.20.cpp` lines 83744–85184 (1440 lines)
**Stack frame**: 1024 bytes (stwu r1,-1024)
**Callee-saved regs used**: r23–r31, f30–f31

---

## Global Variable Map (computed)

| Address | Name | Type |
|-|-|-|
| `0x831CC944` | g_audioMgrPtr | u32* (pointer to 176-byte struct) |
| `0x82FF541C` | g_audioFlags | u32 (bit 0=bufSize configured, bit 1=voiceCount configured) |
| `0x82FF5418` | g_audioBufferAddr | u32 (r26+r27+128) |
| `0x82FF5414` | g_maxVoices | u32 (set to 168) |
| `0x82FF5400` | g_altCBList | u32 ptr (ptr+4 = first entry) |
| `0x82FF5364` | g_xaudioObj | u32 (XAudio2 object) |
| `0x82FF5360` | g_xaudioPtr | u32* (pointer to 192-byte block) |
| `0x82FF536C` | g_voiceArrayBase | u32* (176-byte * numVoices) |
| `0x82FF5368` | g_voicePtrBlock | u32* (u32[numVoices] pointer array) |
| `0x82FF5374` | g_initialized | u32 (0/1 from malloc result of 112-byte block) |
| `0x82FF5378` | g_shutdownFlag | u32 (set to 0 at end) |
| `0x82FF537C` | g_altCBPtr4 | u32 |
| `0x831CC904` | g_audioEnginePtr | u32* (vtable dispatched) |
| `0x831CDA65` | g_audioFlag1 | u8 (set to r25=1) |
| `0x831CDA47` | g_audioFlag2 | u8 (set to r23=0) |
| `0x82FF534C` | g_audioModulePtr | u32* (loaded from lis(-32001)+21348) |

---

## Complete Pseudocode

```c
void sub_82478AF8(void) {
    // ── Phase 1: Environment subsystem init ──────────────────────────────
    sub_826225E0();   // 0x82478B10 — init subsys A (no args)
    sub_82622648();   // 0x82478B14 — init subsys B (no args)
    sub_826226B0();   // 0x82478B18 — init subsys C (no args)

    // ── Phase 2: Allocate 176-byte audio manager struct ──────────────────
    void* audioMgr = sub_821B3510(176);  // malloc(176)
    if (audioMgr != null)
        audioMgr = sub_8294BD68(audioMgr);  // construct XAudio manager
    // else audioMgr stays null (r30 = r23 = 0)
    g_audioMgrPtr[0x831CC944] = audioMgr;  // store globally

    // ── Phase 3: Voice/buffer sizing ─────────────────────────────────────
    // r10 = g_audioFlags[0x82FF541C]
    // r27 = *[ptr1+8]   (buf size component 1, from lis(-32001)+21384)
    // r26 = *[ptr2+8]   (buf size component 2, from lis(-32005)+1792)
    if (!(g_audioFlags & 1)) {
        // buffer addr not yet configured:
        g_audioBufferAddr[0x82FF5418] = r26 + r27 + 128;
        g_audioFlags |= 1;
    }
    if (!(g_audioFlags & 2)) {
        // voice count not yet configured:
        g_maxVoices[0x82FF5414] = 168;
        g_audioFlags |= 2;
    }

    // ── Phase 4: Position and distance setup ─────────────────────────────
    // Build two 3-float vectors on stack (stack+288, stack+320)
    // f0 = float constants from data segment
    stack[320] = stack[324] = float_const_A;   // 0.0?
    stack[328] = float_const_B;
    stack[288] = stack[292] = float_const_C;   // some default
    stack[296] = float_const_D;
    sub_8294BE28(audioMgr, &stack[288], &stack[320]);  // SetListenerPos
    sub_8294BE20(audioMgr, 6000);                       // SetMaxDistance(6000)

    // ── Phase 5: Load audio backend name and set on manager ──────────────
    g_xaudioBufAddr_u16[audioMgr+40] = g_audioBufferAddr[0x82FF5418] & 0xFFFF;
    sub_8294BEA0(audioMgr, 1500);   // SetRolloffFactor(1500)

    // r28 = lis(-32254) + (-16240) = 0x8201C090  ("default" string constant)
    // r31 = g_altCBList+4 (ptr) if non-null, else r28 (fallback to "default")
    char* backendName = (*g_altCBList[0x82FF5400+4] != null)
                         ? *g_altCBList[0x82FF5400+4]
                         : (char*)0x8201C090;  // "default"

    // ── Phase 6: Classify audio backend ──────────────────────────────────
    // stricmp(backendName, strings at 0x8201C088, 0x8201C07C, 0x8201C074, 0x8201C06C, 0x8201C068)
    int backendType;
    if      (stricmp(backendName, str_0x8201C088) == 0) backendType = 2;
    else if (stricmp(backendName, str_0x8201C07C) == 0) backendType = 1;
    else if (stricmp(backendName, str_0x8201C074) == 0) backendType = 1;
    else if (stricmp(backendName, str_0x8201C090) == 0) backendType = 0;
    else if (stricmp(backendName, str_0x8201C06C) == 0) backendType = 0;
    else                                                 backendType = 1;  // r25=1 default
    audioMgr[152] = backendType;   // store backend enum

    // ── Phase 7: Finalize audio manager + allocate XAudio object ─────────
    sub_8294E208(audioMgr);         // configure audio manager (no other params)
    g_audioModulePtr[0x82FF5360] = audioMgr;

    void* xaudioObj = sub_821B3510(192);  // malloc(192)
    if (xaudioObj != null)
        xaudioObj = sub_82956718(xaudioObj);  // construct mastering voice object
    // else xaudioObj = 0
    g_xaudioPtr[0x82FF5364] = xaudioObj;   // GLOBAL: XAudio object pointer

    // ── Phase 8: Audio graph / voice setup ────────────────────────────────
    sub_82953088();              // audio graph init
    sub_82954618(xaudioObj, 16); // set polyphony/channel count to 16

    // r11_buf = r26+r27 (buf_base)
    // r5 = g_maxVoices (168), r6 = 2097152, r7 = r11_buf*4 (buf_size_words)
    // r4 = audioMgr
    // stfiwx: compute float from r30 (buf_size), store at vtable+72
    sub_82955838(xaudioObj, audioMgr, str_0x8209CBB0, 0, 0, 0);  // CreateSourceVoices
    // NOTE: r10 = vtable[3].fn is called before sub_82955838 to get r3 arg

    // ── Phase 9: Register 6 audio source descriptors ─────────────────────
    // Each call to sub_8284D220(dest, src, format_id, 0, 0, 4)
    // copies a 16-byte struct from stack+NNN, format tag stored in advance
    // Descriptors registered at stack+464, 400, 384, 352, 432, 368, 416
    // Format tags (r9 values): various XAudio format codes
    sub_8284D220(&stack[464], r31_ptr, 0,    0, 0, 4);  // desc slot 0
    sub_8284D220(&stack[400], r31_ptr, tag1, 0, 0, 4);  // desc slot 1
    sub_8284D220(&stack[384], r31_ptr, tag2, 0, 0, 4);  // desc slot 2
    sub_8284D220(&stack[352], r31_ptr, tag3, 0, 0, 4);  // desc slot 3
    sub_8284D220(&stack[432], r31_ptr, tag4, 0, 0, 4);  // desc slot 4
    sub_8284D220(&stack[368], r31_ptr, tag5, 0, 0, 4);  // desc slot 5
    sub_8284D220(&stack[416], r31_ptr, tag6, 0, 0, 4);  // desc slot 6

    // ── Phase 10: Get 3 voice format descriptors ──────────────────────────
    // sub_82478A80 takes a stack desc ptr, returns an allocated voice format
    void* voiceFmt0 = sub_82478A80(&stack[640]);   // r29
    void* voiceFmt1 = sub_82478A80(&stack[624]);   // r28
    void* voiceFmt2 = sub_82478A80(&stack[656]);   // r31

    // ── Phase 11: Build voice config structs on stack ─────────────────────
    stack[231] = 1;  // r25 flag byte
    strncpy(&stack[208], voiceFmt0, 16);  // sub_82A00DC0
    strncpy(&stack[192], voiceFmt1, 16);
    strncpy(&stack[160], &stack[560], 16);  // with stack[180]=r30, stack[190]=r30 (u16)
    strncpy(&stack[128], &stack[592], 16);  // with stack[148]=0
    strncpy(&stack[112], &stack[608], 16);
    strncpy(&stack[96],  &stack[576], 16);

    // ── Phase 12: Create voice block ─────────────────────────────────────
    // r27 = numVoices (from g_maxVoices calculation earlier; = sub_821B3510(1024) result)
    void* voiceBlock = sub_827ADB48(r27, r4, r5, r6, r7, r8, r9, r10, r11, &stack[88]);
    g_voicePtrs[0x82FF5368] = voiceBlock;  // store voice pointer block

    // ── Phase 13: Allocate numVoices * 176 byte voice struct array ────────
    // Only if malloc(1024) returned non-null (from sub_821B3510(1024)):
    //   numVoices (r30) = r26+r27 (reused from buf computation)
    void* voiceArray;
    if (numVoices > 0x5F87D857U /* overflow sentinel */) {
        // alloc size would overflow: allocSize = -1 (sentinel)
        voiceArray = sub_821B3510(-1);   // will return null
    } else {
        // alloc = numVoices * 176 + 16
        voiceArray = sub_821B3510(numVoices * 176 + 16);
    }
    if (voiceArray != null) {
        voiceArray[0] = numVoices;
        void* base = voiceArray + 16;   // r27 = voiceArray+16
        for (int i = 0; i < numVoices; i++)
            sub_8261FBA0(base + i * 176);  // voice struct constructor
        g_voiceArrayBase[0x82FF536C] = base;  // store array start
    } else {
        g_voiceArrayBase[0x82FF536C] = null;
    }

    // ── Phase 14: Allocate voice pointer lookup array ─────────────────────
    // r3 = numVoices * 4 (clamp: if numVoices > 0x3FFFFFFF sentinel -> -1)
    void* voicePtrLookup = sub_821B3510(numVoices * 4);
    g_xaudioPtrs2[0x82FF5368] = voicePtrLookup;  // NOTE: same slot as voiceBlock above?
    // If numVoices > 0: fill pointer lookup array (loop writes 18 to voice[160], stores ptrs)
    // loop: for (int i = 0; i < numVoices; i++) {
    //     voiceArray[i*176 + 160] = 18;  // type flag
    //     voicePtrLookup[i] = &voiceArray[i*176];
    // }

    // ── Phase 15: Bind voice ptrs + configure pool sizes ─────────────────
    sub_827ACC98(xaudioObj, voicePtrLookup);       // bind voice pointer array
    sub_821B3560(voicePtrLookup);                   // free temp voice ptr lookup
    xaudioObj[768] = 128;
    sub_827ACCA0(xaudioObj, 8192, 200);             // pool: 200 large buffers
    sub_827ACCA0(xaudioObj, 4096, 300);             // pool: 300 medium buffers
    sub_827ACCA0(xaudioObj, 2048, 500);             // pool: 500 small buffers

    // ── Phase 16: Audio streaming subsystem init ──────────────────────────
    sub_821B5038((char*)0x8201BFF4);  // register streaming module name A
    sub_8287AC38();                    // init streaming graph

    // Register 2 streaming event callbacks:
    sub_8287A6A8(xaudioObj[8660], str_0x8201C02C, 6);  // event type 6
    sub_8287A6A8(xaudioObj[8660], str_0x8201C024, 4);  // event type 4

    sub_821B5038((char*)0x82FB3B0C);  // register streaming module name B
    sub_8261C7C8();                    // audio3D init

    // Set 3D audio params on xaudioObj[8660]:
    sub_8287A408(xaudioObj[8660], f_const_3444);     // set param A (float)
    sub_8287A4E0(xaudioObj[8660], f_const_17788);    // set param B (float)
    sub_8287A4E8(xaudioObj[8660], f_const_17792);    // set param C (float)

    // ── Phase 17: Audio engine vtable calls ──────────────────────────────
    // g_audioEnginePtr[0x831CC904] is an object with vtable
    // vtable[4] call (SetCategory?):
    audioEngine = *g_audioEnginePtr;
    audioEngine->vtable[4](audioEngine, str_0x8201C018, f30=float_3400);
    sub_8299B4A8(audioEngine, r4, f30);     // SetVolume(engine, ?, 0.0f?)

    // vtable[4] call again with different string:
    audioEngine->vtable[4](audioEngine, str_0x8201C00C, f30);
    sub_8299B4A8(audioEngine, r4, f30);     // SetVolume(engine, ?, 0.0f?)

    // ── Phase 18: Allocate 112-byte block (streaming/device) ─────────────
    // Clear bit 9 in flags at lis(-32078)+(-20744)
    // sub_821B3510(112) -> if non-null, sub_823B33F8(ptr) else ptr=0
    void* streamBlock = sub_821B3510(112);
    if (streamBlock != null)
        streamBlock = sub_823B33F8(streamBlock);  // construct streaming block
    g_initialized[0x82FF5374] = streamBlock;      // 0 or ptr

    // ── Phase 19: Build audio source config and call engine ──────────────
    // f30 = float const from data seg (3400), f31 = float const (2612)
    // Build 40-byte config on stack+496:
    //   [0] = f30 (float)
    //   [4] = f31, [8] = f31, [12] = 0.0, [16] = f30
    //   [20] = f31, [24] = 0.0 (from zero vector), [28] = f31
    //   [32] = f31, [36] = f30
    // Call via vtable[1] of g_audioEnginePtr[0x831CC904]:
    audioEngine->vtable[1](audioEngine, &stack[496], ...);  // InitAudioSources

    g_shutdownFlag[0x82FF5378] = 0;  // mark: not shutting down

    // ── Phase 20: Zero-initialize voice state table ───────────────────────
    // g_voiceStateTable at lis(-32001)+9056+4 (0x82FF4364 approx)
    // Zero 1024 entries * 3 u32 per entry = 12288 bytes total
    uint32_t* tbl = (uint32_t*)(g_xaudioBase + 9056 + 4);
    for (int i = 0; i < 1024; i++) {
        tbl[-1] = 0;   // entry[-4]
        tbl[0]  = 0;   // entry[0]
        tbl[1]  = 0;   // entry[+4]
        tbl += 3;
    }

    // ── Phase 21: Streaming / device thread launch ───────────────────────
    sub_8228A1E0();    // start streaming thread? (after flag stores to 0x831CDA65, 0x831CDA47)
    sub_825FD6B8();    // start RPF/streaming subsystem
    sub_82955BE0();    // start XAudio streaming thread (THIS calls sub_82212F38 internally)

    // --- sub_82955BE0 internally spawns a thread that calls sub_82212F38
    // sub_82212F38 loops waiting for audio device connection state != 1:
    //   while (*0x82BC9BE2 == 0 && state == 1) { sub_82212EC0(); sleep(100ms); }
    // On PC/macOS: state is set to 1 (connecting) by sub_82211A88 and never transitions
    // because the Xbox 360 async kernel callback (XAudio2 device arrival) never fires.

    // ── Phase 22: Audio device name string copy and stream init ──────────
    // Check g_altCBList+4 again for streaming config
    // strncpy(stack+672, "AudioDevice0" or similar, 23)  if needed
    // sub_8284F310(streamingMgr, streamFlag)
    // sub_82477670()     // streaming manager tick/start
    // *[g_streamMgr+56] = streamFlag
    // sub_827C2420(g_streamMgr)     // activate streaming
    // sub_8284E830(streamingMgr)    // finalize streaming

    // ── Phase 23: Second large-block alloc (1024-byte voice manager) ──────
    // sub_821B3510(1024) = numVoices register
    // if null: g_voicePtrs2 = 0  →  goto loc_82479208
    // else: setup 6x sub_8284D220 calls (source format descriptors)
    //        sub_82478A80 x3 (get format handles)
    //        sub_82A00DC0 x6 (strncpy 16-byte config blocks)
    //        sub_827ADB48 (create master voice block)
    //        → g_voicePtrs2[0x82FF5368] = result

    sub_822BCA90();   // LAST CALL: finalize/commit audio system
}
```

---

## Critical Hang Analysis

### Where sub_82212F38 is called

`sub_82212F38` is **not called directly** from `sub_82478AF8`. It is called from inside `sub_82955BE0` (called at line 84233, lr=0x82478E3C), which launches the audio streaming thread. The thread internally calls `sub_82212F38`.

### The hang mechanism

```
sub_82478AF8
  └─ sub_82955BE0()          [line 84232 / 0x82478E3C]
       └─ (spawns thread or calls directly)
            └─ sub_82212F38()
                 └─ sub_82211A88()    // sets state=1 (connecting)
                 └─ loop:
                      sub_82212EC0()  // tries to advance state
                      sleep(100ms)
                      if state == 1: loop again
                 // state is driven by XAudio2 DeviceArrival callback
                 // on Xbox 360 this fires asynchronously
                 // on PC/macOS: callback never fires → infinite spin
```

**Root cause**: `sub_82211A88` sets `struct[2016] = 1` (state = connecting). On Xbox 360 a kernel callback transitions it to 0 (connected). On PC/macOS no such callback fires, so the spin in `sub_82212F38` never exits.

**Fix target**: Either:
1. Hook `sub_82955BE0` to skip it entirely (most aggressive).
2. Hook `sub_82212F38` to return immediately (state already handled by host audio).
3. Hook `sub_82211A88` to set `struct[2016] = 0` instead of `1` (prevents spin entry).
4. Hook `sub_82212DE8` to return 0 (device not found → loop exits immediately).

---

## Complete Call Graph (ordered by first occurrence)

| Order | Address | Name | Args | Purpose |
|-|-|-|-|-|
| 1 | `0x826225E0` | sub_826225E0 | () | Init subsys A |
| 2 | `0x82622648` | sub_82622648 | () | Init subsys B |
| 3 | `0x826226B0` | sub_826226B0 | () | Init subsys C |
| 4 | `0x821B3510` | sub_821B3510 | (176) | malloc(176) → audioMgr |
| 5 | `0x8294BD68` | sub_8294BD68 | (audioMgr) | Construct audio manager |
| 6 | `0x8294BE28` | sub_8294BE28 | (mgr,vec,vec) | SetListenerPos |
| 7 | `0x8294BE20` | sub_8294BE20 | (mgr,6000) | SetMaxDistance |
| 8 | `0x8294BEA0` | sub_8294BEA0 | (mgr,1500) | SetRolloff |
| 9 | `0x829FFCF0` | rexcrt__stricmp | ×4 | Backend name classify |
| 10 | `0x8294E208` | sub_8294E208 | (mgr) | Finalize audio mgr |
| 11 | `0x821B3510` | sub_821B3510 | (192) | malloc(192) → xaudioObj |
| 12 | `0x82956718` | sub_82956718 | (xaudioObj) | Construct XAudio2 object |
| 13 | `0x82953088` | sub_82953088 | () | Audio graph init |
| 14 | `0x82954618` | sub_82954618 | (xObj,16) | Set channel count=16 |
| 15 | vtable[3] | indirect | (audioEngine) | Get r3 for voice setup |
| 16 | `0x8296A278` | sub_8296A278 | (1) | Set audio state flag |
| 17 | `0x82953AB0` | sub_82953AB0 | (1) | Enable audio output |
| 18 | `0x82953AD0` | sub_82953AD0 | (f1=float) | Set master volume |
| 19 | `0x82955838` | sub_82955838 | (xObj,mgr,...) | CreateSourceVoices |
| 20 | `0x8297B6B8` | sub_8297B6B8 | (xObj[8],-1,512) | Set voice pool flags |
| 21 | `0x826BDC18` | sub_826BDC18 | () | Unknown |
| 22 | vtable[1] | indirect | (audioEngine) | Register callback? |
| 23 | `0x8284D220` | sub_8284D220 | ×6 | Register audio descriptors |
| 24 | `0x82478A80` | sub_82478A80 | ×3 | Get voice format handle |
| 25 | `0x82A00DC0` | sub_82A00DC0 | ×6 | strncpy voice config |
| 26 | `0x827ADB48` | sub_827ADB48 | (numV,...) | Create voice block |
| 27 | `0x821B3510` | sub_821B3510 | (nV*176+16) | malloc voice array |
| 28 | `0x8261FBA0` | sub_8261FBA0 | ×numVoices | Voice struct ctor (176B) |
| 29 | `0x821B3510` | sub_821B3510 | (nV*4) | malloc ptr lookup |
| 30 | `0x827ACC98` | sub_827ACC98 | (xObj,ptrs) | Bind voice ptrs |
| 31 | `0x821B3560` | sub_821B3560 | (ptrs) | free voice ptr lookup |
| 32 | `0x827ACCA0` | sub_827ACCA0 | (xObj,8192,200) | Pool config large |
| 33 | `0x827ACCA0` | sub_827ACCA0 | (xObj,4096,300) | Pool config medium |
| 34 | `0x827ACCA0` | sub_827ACCA0 | (xObj,2048,500) | Pool config small |
| 35 | `0x821B5038` | sub_821B5038 | (str) | Register stream module A |
| 36 | `0x8287AC38` | sub_8287AC38 | () | Init streaming graph |
| 37 | `0x8287A6A8` | sub_8287A6A8 | ×2 | Register stream events |
| 38 | `0x821B5038` | sub_821B5038 | (str) | Register stream module B |
| 39 | `0x8261C7C8` | sub_8261C7C8 | () | Audio3D init |
| 40 | `0x8287A408` | sub_8287A408 | (obj,f1) | Set 3D param A |
| 41 | `0x8287A4E0` | sub_8287A4E0 | (obj,f1) | Set 3D param B |
| 42 | `0x8287A4E8` | sub_8287A4E8 | (obj,f1) | Set 3D param C |
| 43 | vtable[4] | indirect ×2 | (engine,str,f30) | SetCategory/SetFilter |
| 44 | `0x8299B4A8` | sub_8299B4A8 | ×2 | SetVolume |
| 45 | `0x821B3510` | sub_821B3510 | (112) | malloc streaming block |
| 46 | `0x823B33F8` | sub_823B33F8 | (ptr) | Construct streaming block |
| 47 | vtable[1] | indirect | (audioEngine,...) | InitAudioSources |
| 48 | zero-fill loop | — | 1024×3 stores | Zero voice state table |
| 49 | `0x8228A1E0` | sub_8228A1E0 | () | Start streaming thread |
| 50 | `0x825FD6B8` | sub_825FD6B8 | () | Start RPF streaming |
| 51 | **`0x82955BE0`** | sub_82955BE0 | () | **XAudio streaming thread — HANGS** |
| 52 | `0x82A00DC0` | sub_82A00DC0 | (str,src,23) | strncpy device name |
| 53 | `0x8284F310` | sub_8284F310 | (mgr,flag) | Start streaming mgr |
| 54 | `0x82477670` | sub_82477670 | () | Streaming tick |
| 55 | `0x827C2420` | sub_827C2420 | (streamMgr) | Activate streaming |
| 56 | `0x8284E830` | sub_8284E830 | (mgr) | Finalize streaming |
| 57 | `0x821B3510` | sub_821B3510 | (1024) | malloc voice mgr |
| 58 | `0x8284D220` | sub_8284D220 | ×6 | Register format descs |
| 59 | `0x82478A80` | sub_82478A80 | ×3 | Get format handles |
| 60 | `0x82A00DC0` | sub_82A00DC0 | ×6 | Copy config blocks |
| 61 | `0x827ADB48` | sub_827ADB48 | (...) | Create master voice |
| 62 | `0x822BCA90` | sub_822BCA90 | () | Finalize audio system |

---

## Key Memory Writes Summary

| Address | Value | When |
|-|-|-|
| `0x831CC944` | audioMgr ptr | Phase 2 |
| `0x82FF541C` | flags \|= 1 | Phase 3 (first time) |
| `0x82FF5418` | r26+r27+128 | Phase 3 (first time) |
| `0x82FF541C` | flags \|= 2 | Phase 3 (second time) |
| `0x82FF5414` | 168 | Phase 3 (second time) |
| `audioMgr+40` | buf_addr (u16) | Phase 4 |
| `audioMgr+152` | backend enum (0-2) | Phase 6 |
| `0x82FF5360` | audioMgr ptr | Phase 7 |
| `0x82FF5364` | xaudioObj ptr | Phase 7 |
| `0x82FF536C` | voiceArray base | Phase 13 |
| `0x82FF5368` | voicePtrLookup | Phase 14 |
| `xaudioObj+768` | 128 | Phase 15 |
| `0x82FF5374` | streamBlock ptr | Phase 18 |
| `0x831CDA65` | 1 (r25) | Before sub_8228A1E0 |
| `0x831CDA47` | 0 (r23) | Before sub_8228A1E0 |
| `0x82FF5378` | 0 (shutdown=false) | Phase 20 |
| voice state table | 0x0 × 1024×3 | Phase 20 |
