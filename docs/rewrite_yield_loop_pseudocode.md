# Yield Loop Rewrite Pseudocode — sub_82212F38 and Callees

**Generated**: 2026-03-28
**Purpose**: Complete pseudocode for every function in the audio device yield loop, with exact memory addresses, for writing a host-side replacement that skips the XAudio async wait.

---

## Quick Reference: Why It Hangs

`sub_82212F38` spins waiting for `struct[2016]` to change from `1` (connecting) to `0` (connected). That write is supposed to come from an Xbox 360 async kernel callback triggered by `sub_8220FDB8` → `sub_82A11EB8`. On PC/macOS no Xbox audio device exists, so the callback never fires and the loop runs forever at 100ms intervals.

**Surgical fix**: Hook `sub_82212EC0` and write `struct[2016] = 0` on the first call.

---

## 1. sub_82212F38 — Audio Init + Yield Loop

**File**: `gta4_recomp.5.cpp` lines 56270–~56700
**Stack frame**: 4064 bytes (`stwu r1,-4064(r1)`)
**Key saved register**: `r26` = audio manager base = `0x82BC9BD8`

### Complete Pseudocode

```c
void sub_82212F38(PPCContext& ctx, uint8_t* base) {
    // --- PREAMBLE: audio manager lock ---
    r26 = 0x82BC9BD8;               // lis r11,-32067; addi r26,r11,-25640
    sub_82849860(*(r26+24));        // acquire audio lock
    r22 = 0;                        // iteration counter
    r20 = 60000;                    // timeout threshold (ori r20,r20,60000)

    // --- INIT: create audio format object ---
    sub_82862D28(stack+112, 9, data_at_82A03D78, 56);  // r29 = result (format type)
    sub_82862918(stack+112);
    *(stack[1344]+16) = 1024;       // set buffer size
    *(stack[1344]+12) = stack+320;  // set buffer ptr
    sub_82211A88(stack+1872);       // init device struct at stack+1872

    r30 = 0;                        // r30 = retry counter

    // --- ABORT EARLY CHECK ---
    if (byte[r26+10] != 0)          // 0x82BC9BE2 abort flag
        goto loc_82213478;          // skip all audio init

    // (constants set up: r16,r17,r18,r19,r23,r14,r15 = various audio config ptrs)

loc_82213010:
    // outer retry loop
    sub_828497D8(*(r26+20));        // wait on secondary lock at r26+20

    if (byte[r26+10] != 0)          // 0x82BC9BE2 abort flag
        goto loc_8221346C;

    if (r29 < 0)                    // format init failed
        goto loc_8221346C;

    sub_8285FF50(stack+88, r14);    // string format device name

    if (*(r26+16) == 0)             // no device registered yet
        { sub_8285FFA0(stack+88); goto loc_8221346C; }

    sub_822BF360(r23);              // notify device available

    r24 = *(r26+16);                // device object ptr
    *(r26+16) = 0;                  // clear slot
    sub_828530D0();                 // get timestamp → r31
    r31 |= 1;                       // ensure non-zero

    // --- TIMEOUT / RETRY GATE ---
    if ((r31 - r21) < r20 && r21 == 0)
        goto loc_82213090;
    // else goto loc_82213114 (skip yield loop)

loc_82213090:
    // --- PRE-LOOP: check abort + device presence + state ---
    if (byte[stack+3896] & 0xFFFFFF80) goto loc_822130E8;   // abort flag in struct[2024]
    r3 = sub_82212DE8(stack+1872);  // device presence check
    if (r3 == 0) goto loc_822130E8; // device not found → skip loop
    if (*(stack+3888) != 1)         // stack[3888] = struct[2016] = connection state
        goto loc_822130E8;          // state not 'connecting' → skip loop

    // ===== YIELD LOOP =====
loc_822130C0:
    if (byte[r26+10] != 0)          // 0x82BC9BE2 global abort
        goto loc_822130E8;
    sub_82212EC0(stack+1872);       // attempt state transition (THE KEY CALL)
    sub_82849918(r3=100);           // sleep 100ms
    if (*(stack+3888) == 1)         // struct[2016] still connecting?
        goto loc_822130C0;          // YES → loop again
    // NO → fall through to exit
    // ===== END YIELD LOOP =====

loc_822130E8:
    // post-loop: adjust retry backoff
    if (r30 > 4) {
        r20 = min(r20 * 2, 0x74300);  // double timeout up to cap 479232
    }
    r30++;                          // increment retry counter
    r21 = r31;                      // update last-seen timestamp

loc_82213114:
    if (byte[r26+10] != 0)
        { sub_8285FFA0(stack+88); goto loc_8221346C; }

    // --- AUDIO THREAD LAUNCH (if abort flag not set) ---
loc_8221312C:
    if (byte[stack+3896] & 0xFFFFFF80)  // struct[2024] abort bits
        goto loc_822131EC;
    // ...thread creation, audio streaming init...

loc_8221346C / loc_82213478:
    // cleanup + return
    sub_82849860_unlock(...);
    return;
}
```

### Memory Addresses Read/Written by sub_82212F38

| Address | Op | Description |
|-|-|-|
| `0x82BC9BE2` (`r26+10`) | read | Global abort byte — non-zero exits loop |
| `0x82BC9BEC` (`r26+20`) | read | Secondary lock ptr (passed to sub_828497D8) |
| `0x82BC9BF0` (`r26+24`) | read | Primary lock ptr (passed to sub_82849860) |
| `0x82BC9BE8` (`r26+16`) | read/write | Device object ptr slot (read then cleared) |
| `stack+1872` | read/write | Audio device struct base |
| `stack+3888` = `struct+2016` | read | Connection state: `1`=connecting, `0`=connected |
| `stack+3896` = `struct+2024` | read | Ready/abort bitfield |
| `stack+84` | read/write | Retry counter `r30` |

---

## 2. sub_82212DE8 — Device Presence Check

**File**: `gta4_recomp.5.cpp` lines 56071–56192
**Input**: `r3` = audio device struct ptr (stack+1872)
**Output**: `r3` = 1 (device found), 0 (not found)

### Complete Pseudocode

```c
int sub_82212DE8(audio_struct* s) {
    s->field_288 = 0;                    // clear endpoint count
    s->field_2024 &= 0x7F;              // clear bit 7 of ready bitfield (clrlwi mask)

    // Load device object pointer
    uint32_t* dev_slot = (uint32_t*)0x82BCB724;
    uint32_t dev_ptr = dev_slot[1];      // *(0x82BCB728)
    if (dev_ptr == 0) return 0;          // no device object
    if ((dev_ptr & 0xFF) == 0) return 0; // low byte zero (null check)

    // Check device name string non-empty
    // r3 = 0x82BCB798 (addi r3,r11,-18536 where lis r11,-32067)
    uint32_t name_len = 0;
    if (!sub_8285DC40(0x82BCB798, &name_len)) return 0;  // name empty

    // Device found
    s->field_288 = 1;                    // mark endpoint present
    s->field_4 = sub_82A34FD8(dev_ptr); // get device handle (via inet_addr thunk)
    s->field_2 = (uint16_t)name_len;    // store name length
    uint32_t sub_result = sub_82211B38(s);   // notify subscribers
    // update ready bitfield bits 17-24 (rlwimi r10,r11,7,17,24)
    s->field_2024 = (s->field_2024 & 0xFFFF807F) |
                    ((__builtin_rotateleft32(sub_result, 7)) & 0x7F80);
    return 1;
}
```

### Memory Addresses

| Address | Op | Description |
|-|-|-|
| `0x82BCB724` | read | Base of audio device descriptor struct |
| `0x82BCB728` (`+4`) | read | Audio device object pointer |
| `0x82BCB798` | read | Ptr to audio device descriptor (passed to sub_8285DC40) |
| `0x82BCB79C` (`+4`) | read | Audio device name string pointer (read inside sub_8285DC40) |
| `struct+2` | write | Device name length (u16) |
| `struct+4` | write | Device handle from sub_82A34FD8 |
| `struct+288` | write | Endpoint count (set to 0 then 1) |
| `struct+2024` | read/write | Ready bitfield (bits 17-24 updated) |

---

## 3. sub_8285DC40 — Device Name String Check

**File**: `gta4_recomp.56.cpp` lines 8899–8957
**Input**: `r3` = ptr to device descriptor struct, `r4` = output u32* for string length
**Output**: `r3` = 1 (name non-empty), 0 (null or empty)

### Complete Pseudocode

```c
int sub_8285DC40(device_desc* desc, uint32_t* out_len) {
    char* name_ptr = (char*)desc->field_4;  // lwz r3,4(r3)
    if (name_ptr == NULL) return 0;
    if (name_ptr[0] == '\0') return 0;
    *out_len = sub_82A00B48(name_ptr);  // strlen
    return 1;
}
```

**"Ready" condition**: `*(uint32_t*)0x82BCB79C != 0` AND `*(char*)*(uint32_t*)0x82BCB79C != '\0'`

### Memory Addresses

| Address | Op | Description |
|-|-|-|
| `0x82BCB79C` (= `*0x82BCB798 + 4`) | read | Device name string pointer |
| `*(0x82BCB79C)[0]` | read | First byte of device name (null-check) |

---

## 4. sub_82212EC0 — Connection State Machine Step

**File**: `gta4_recomp.5.cpp` lines 56196–56266
**Input**: `r3` = audio device struct ptr
**Called from**: yield loop every 100ms

### Complete Pseudocode

```c
void sub_82212EC0(audio_struct* s) {
    if (s->field_2016 != 1) return;    // only act when state == 1 (connecting)

    sub_8220FDB8(s);                   // attempt XAudio2 endpoint enumeration + async connect

    if (s->field_2016 == 1) return;    // still connecting (async not done) → nothing more

    // State changed (0 = connected):
    uint32_t notify_result = 0;
    if (s->field_288 != 0)             // if endpoint was found
        notify_result = sub_82211B38(s);  // notify subscribers, get bool

    // Update ready bitfield bits 17-24
    s->field_2024 = (s->field_2024 & 0xFFFF807F) |
                    ((__builtin_rotateleft32(notify_result, 7)) & 0x7F80);
}
```

### Memory Addresses

| Address | Op | Description |
|-|-|-|
| `struct+2016` | read | Connection state: `1`=connecting, `0`=connected |
| `struct+288` | read | Endpoint count |
| `struct+2024` | read/write | Ready bitfield (bits 17-24) |

**Hook target for fix**: Write `struct+2016 = 0` before or inside this function to make the loop exit on the first iteration.

---

## 5. sub_8220FDB8 — XAudio2 Endpoint Enumeration + Async Connect

**File**: `gta4_recomp.5.cpp` lines 48914–49071
**Input**: `r3` = audio device struct ptr
**Does NOT write struct+2016** — that write comes from an async completion callback.

### Complete Pseudocode

```c
void sub_8220FDB8(audio_struct* s) {
    audio_enum_state* state = s + 292;     // addi r31,r29,292

    // Init enumerator object
    sub_829E4000(state);                   // XAudio2 IMMDeviceEnumerator init
    if (result == 0) return;               // init failed

    sub_829E4160(state);                   // enumerate devices (fills state)
    sub_829E4000(state);                   // re-init after enumeration
    if (result == 0) return;

    // Close any existing device handle
    rexcrt_CloseHandle(s->field_328);
    s->field_80_local = 0;
    s->field_328 = 0;                      // stw r28,328(r29)

    // Build device path and issue async I/O connect
    sub_822BCA90(state);                   // build Xbox audio device path
    uint32_t status = sub_82A11EB8(state, &local_buf, 0);  // async NtCreateFile/DeviceIoControl
    if (status != 0) return;               // async open failed

    int dev_count = local_buf.count;       // number of devices found
    if (dev_count <= 0) return;

    // Enumerate up to 8 audio endpoints
    // s->field_2 = &s->field_2 (endpoint array base)
    // s->field_336 = device_list start
    uint32_t* ep = s + 2;                  // addi r31,r29,2
    device_entry* dev = s + 336;           // addi r30,r29,336
    int i = 0;
    while (i < 8 && i < dev_count) {
        sub_829E4638(local_buf_88);        // get endpoint format descriptor
        sub_829E4650(local_buf_88, dev->field_0, dev+8);  // fill endpoint info

        uint16_t sample_rate = local_buf[92];
        if (sample_rate == 0) sample_rate = 2000;    // default 2000 Hz
        ep->field_0 = sample_rate;                   // sth r10,0(r31)

        uint32_t format_tag = local_buf[88];
        if (format_tag == 0) format_tag = 0x5402480A; // default WAVEFORMAT tag
        ep->field_30 = format_tag;                   // stw r11,30(r31)
        ep->field_2 = dev->field_0;                  // stw r9,2(r31)

        i++;                                         // addi r28,r28,1
        s->field_288++;                              // increment endpoint count
        ep += 36;                                    // addi r31,r31,36
        dev += 208;                                  // addi r30,r30,208
    }
    // NOTE: s->field_2016 is NOT written here.
    // It is written by the async completion callback from sub_82A11EB8.
    // On Xbox 360: kernel fires callback → writes s->field_2016 = 0
    // On PC/macOS: no Xbox audio device → callback never fires
}
```

### Memory Addresses

| Address | Op | Description |
|-|-|-|
| `struct+288` | read/write | Endpoint count (incremented per endpoint found) |
| `struct+292` | read/write | XAudio2 enumerator state object |
| `struct+328` | read/write | Existing device handle (closed then zeroed) |
| `struct+336` | read | Device list start |
| `struct+2016` | NOT written | Written only by async callback (never fires on PC) |

---

## 6. sub_82849918 — Sleep Trampoline

**File**: `gta4_recomp.55.cpp` lines 8563–8568
**Input**: `r3` = milliseconds to sleep (called with `r3=100`)

### Complete Pseudocode

```c
void sub_82849918(uint32_t ms) {
    // tail-call: just forwards to sub_82A12B60 with r3 unchanged
    sub_82A12B60(ms);
}

void sub_82A12B60(uint32_t ms) {
    // r4 = 0 (alertable=false)
    sub_82A1A200(ms, /*alertable=*/0);
}

void sub_82A1A200(uint32_t ms, uint8_t alertable) {
    int64_t interval;
    if (ms == (uint32_t)-1) {
        interval = 0x80000000_00000000LL;  // infinite wait
    } else {
        interval = (int64_t)(uint64_t)ms * -10000LL;  // relative 100ns units
        // ms=100 → interval = -1,000,000 (= 100ms)
    }
    do {
        status = KeDelayExecutionThread(/*mode=*/1, alertable, &interval);
    } while (alertable && status == 257);  // STATUS_ALERTED=0x101 → retry if alertable
    // returns: 192 (STATUS_USER_APC) if APC delivered, else 0
}
```

**Sleep duration**: `r3=100` → `100 * -10000 = -1,000,000` (100ns units) = **exactly 100 milliseconds**.

---

## 7. sub_82A34FD8 — Device Handle via inet_addr Thunk

**File**: `gta4_recomp.70.cpp` lines 46579–46584
**Input**: `r3` = audio device object pointer
**Output**: `r3` = device handle (u32)

### Complete Pseudocode

```c
uint32_t sub_82A34FD8(uint32_t device_ptr) {
    // tail-call to NetDll_inet_addr (0x82A755B4)
    // On Xbox 360: this function at 0x82A34FD8 is the XAudio2 device handle getter
    // In host codegen: mapped to inet_addr thunk which parses the device name
    // as a dotted-decimal string → returns a u32 handle token
    return NetDll_inet_addr((const char*)device_ptr);
}
```

**Notes**:
- Called from `sub_82212DE8` with `r3 = audio device object ptr`
- Result stored at `struct+4` (device handle field)
- On Xbox 360, function at `0x82A34FD8` is an XAudio2 device accessor; host codegen registered `NetDll_inet_addr` at that slot
- The device "handle" stored at `struct+4` is used downstream but is NOT the connection state guard — hooking sub_82212EC0 does not require this to return a valid value

---

## 8. Complete Loop Condition Chain

```
sub_82212F38 loops WHILE ALL of:
  1. *(r26+10) == 0          (0x82BC9BE2 global abort byte == 0)
  2. sub_82212DE8() == 1     (device found: *0x82BCB728 != 0 AND *(*0x82BCB79C) != '\0')
  3. struct+2016 == 1        (connection state == 'connecting')

Each iteration (100ms):
  sub_82212EC0(struct):
    → sub_8220FDB8(struct):
        → sub_829E4000/4160: enumerate XAudio2 devices (stub, finds nothing on PC)
        → sub_82A11EB8: issue async I/O to Xbox audio device (no-op on PC)
        [async callback would write struct+2016 = 0 → exits loop]
    re-reads struct+2016 → still 1 → returns without updating bitfield
  sub_82849918(100) → KeDelayExecutionThread(-1000000, alertable=0) → 100ms sleep
  re-checks struct+2016 == 1 → still 1 → loops again
```

---

## 9. Recommended Hook

```cpp
// In LibertyRecomp/kernel/ (e.g. audio_hooks.cpp):
PPC_FUNC_HOOK(__imp__sub_82212EC0) {
    // r3 = audio device struct pointer
    // Write state = 0 (connected) to exit the yield loop immediately
    PPC_STORE_U32(ctx.r3.u32 + 2016, 0);
    // Optionally mark 1 endpoint found so downstream init proceeds:
    PPC_STORE_U32(ctx.r3.u32 + 288, 1);
    return;
}
```

**Why this works**: The yield loop in `sub_82212F38` reads `stack+3888` (= `struct+2016`) and exits when it is not `1`. Writing `0` on the first call to `sub_82212EC0` causes the loop to exit after one 100ms sleep.

**What NOT to stub**: Do not stub `sub_82212F38` entirely — it sets up audio format params, creates the audio streaming pipeline, and launches the audio thread before reaching the yield loop.

---

## 10. Memory Address Summary

| Address | Size | Description | Ready value |
|-|-|-|-|
| `0x82BC9BD8` | base | Audio manager object (`r26`) | — |
| `0x82BC9BE2` | u8 | Global abort flag (`r26+10`) | `0` |
| `0x82BC9BE8` | u32 | Audio device object slot (`r26+16`) | non-zero |
| `0x82BC9BEC` | u32 | Secondary lock ptr (`r26+20`) | — |
| `0x82BC9BF0` | u32 | Primary lock ptr (`r26+24`) | — |
| `0x82BCB724` | base | Audio device descriptor struct | — |
| `0x82BCB728` | u32 | Audio device object pointer | non-zero |
| `0x82BCB798` | ptr | Device descriptor (passed to sub_8285DC40) | — |
| `0x82BCB79C` | u32 | Device name string pointer | non-null, non-empty |
| `struct+2` | u16 | Device name length | — |
| `struct+4` | u32 | Device handle (from sub_82A34FD8) | — |
| `struct+288` | u32 | Endpoint count | ≥1 |
| `struct+292` | data | XAudio2 enumerator state | — |
| `struct+328` | u32 | Async device handle | — |
| `struct+2016` | u32 | **Connection state** — **THE LOCK** | `0` (connected) |
| `struct+2024` | u8 | Ready bitfield (bits 17-24) | non-zero |

`struct` = stack-allocated device struct at `r1+1872` within `sub_82212F38`.

---

## 11. File Locations (Verified)

| File | Lines | Content |
|-|-|-|
| `gta4_recomp.5.cpp` | 56270–~56700 | `sub_82212F38` (audio init + yield loop) |
| `gta4_recomp.5.cpp` | 56071–56192 | `sub_82212DE8` (device presence check) |
| `gta4_recomp.5.cpp` | 56196–56266 | `sub_82212EC0` (connection state step) |
| `gta4_recomp.5.cpp` | 48914–49071 | `sub_8220FDB8` (XAudio2 endpoint enumerate + async connect) |
| `gta4_recomp.55.cpp` | 8563–8568 | `sub_82849918` (sleep trampoline, 100ms) |
| `gta4_recomp.69.cpp` | 36747–36753 | `sub_82A12B60` (sets alertable=0, calls sleep) |
| `gta4_recomp.69.cpp` | 50254–50335 | `sub_82A1A200` (KeDelayExecutionThread, 100ms) |
| `gta4_recomp.56.cpp` | 8899–8957 | `sub_8285DC40` (device name non-empty check) |
| `gta4_recomp.70.cpp` | 46579–46584 | `sub_82A34FD8` (device handle via inet_addr thunk) |

All paths relative to `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/`.
