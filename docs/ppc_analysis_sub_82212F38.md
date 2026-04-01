# PPC Analysis: sub_82212F38 — Audio Device Yield Loop

**Generated**: 2026-03-28
**Source file**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.5.cpp` lines 56270–~56700

---

## Purpose

`sub_82212F38` is the audio subsystem initialization function. It enumerates available audio endpoints, connects to the default device, and **spins in a yield loop** waiting for the connection state to transition from `1` (connecting) to `0` (connected). On Xbox 360 the transition is driven by an async kernel callback; on PC/macOS that callback never fires.

---

## The Yield Loop — Exact Structure

**Loop entry label**: `loc_822130C0` (line 56487)
**Sleep call site**: line 56501 (`sub_82849918(ctx, base)` with `r3=100`)

```
// Preamble checks before entering loop (run once):
if (stack[3896] & 0xFFFFFF80) goto exit_loop;  // abort flag
if (sub_82212DE8(struct_at_stack+1872) == 0) goto exit_loop;  // device not found
if (stack[3888] != 1) goto exit_loop;            // state not 'connecting'

loc_822130C0:  // <-- loop back target
  if (byte[r26+10] != 0) goto exit_loop;         // global abort flag at 0x82BC9BE2
  sub_82212EC0(struct_at_stack+1872);             // attempt state transition
  sub_82849918(r3=100);                           // sleep 100ms
  if (stack[3888] == 1) goto loc_822130C0;        // loop while still connecting
```

**Loop exits when**: `stack[3888]` (= `struct[2016]` = audio connection state) changes from `1` to any other value.

**Loop continues while**: `struct[2016] == 1` AND global abort byte at `0x82BC9BE2 == 0`.

---

## Call Chain Analysis

### sub_82212DE8 — Device presence check

**Location**: `gta4_recomp.5.cpp` line 56071
**Takes**: pointer to audio struct (stack+1872)

Pseudocode:
```
struct[288] = 0;                          // clear connected flag
ptr = load_u32(0x82BCB728);               // load audio device object pointer
if (ptr == null) return 0;               // no device object
if ((ptr & 0xFF) == 0) return 0;          // null low byte
sub_8285DC40(0x82BCB798, &stack[80]);     // check device name string
if (sub_8285DC40 returned 0) return 0;   // name empty = not ready
struct[4] = sub_82A34FD8(ptr);           // store device handle
struct[2] = stack[80];                    // store name length
sub_82211B38(struct);                    // notify subscribers
struct[2024] |= (result << 7);           // set ready bit
return 1;
```

**Returns 1** (device found) if `*0x82BCB728 != null` AND device name string at `*0x82BCB79C` is non-empty.

### sub_8285DC40 — Audio device name string check

**Location**: `gta4_recomp.56.cpp` line 8899
**Takes**: `r3` = pointer to device descriptor struct, `r4` = output u32 pointer

```
r3 = load_u32(r3 + 4);                  // r3 = *0x82BCB79C (device name string ptr)
if (r3 == null) return 0;               // no string pointer
if (byte[r3] == 0) return 0;            // empty string
*r4 = strlen(r3);                       // store string length
return 1;
```

**Key memory addresses**:

| Address | Content | Ready value | Not-ready value |
|-|-|-|-|
| `0x82BCB728` | u32: audio device object pointer | non-zero | 0 / null |
| `0x82BCB79C` | u32: pointer to device name string | ptr to non-empty string | null or ptr to `""` |

**"Ready"** = `*0x82BCB728 != 0` AND `*(char*)*0x82BCB79C != '\0'`
**"Not ready"** = either pointer is null or string is empty

### sub_82212EC0 — Connection state machine step

**Location**: `gta4_recomp.5.cpp` line 56196
**Takes**: pointer to audio struct

```
if (struct[2016] != 1) return;          // only act when connecting
sub_8220FDB8(struct);                   // attempt actual connection
if (struct[2016] == 1) return;          // if still connecting, nothing more
if (struct[288] != 0)                   // if device flagged
    result = sub_82211B38(struct);      // notify subscribers
struct[2024] bit[7..14] = result;       // set ready bitfield
```

**Critical**: `sub_82212EC0` only transitions `struct[2016]` out of `1` if `sub_8220FDB8` modifies it. `sub_8220FDB8` is the actual XAudio2 endpoint enumeration + async connect function.

### sub_8220FDB8 — XAudio2 endpoint connect

**Location**: `gta4_recomp.5.cpp` line 48914

- Uses `sub_829E4000`/`sub_829E4160` (XAudio2 `IMMDeviceEnumerator` equivalent)
- Closes any existing handle at `struct[328]` via `rexcrt_CloseHandle`
- Calls `sub_82A11EB8` (likely async `NtCreateFile`/`DeviceIoControl`)
- Enumerates up to 8 audio endpoints, incrementing `struct[288]` per endpoint
- **Does NOT write `struct[2016]`** — that write is expected from an async completion callback
- On Xbox 360: kernel audio device responds to async I/O, callback writes `struct[2016] = 0`
- On PC/macOS: no Xbox audio device exists, async callback never fires

### sub_82849918 — Sleep 100ms

**Location**: `gta4_recomp.55.cpp` line 8563
**Chain**: `sub_82849918` → `sub_82A12B60` (sets `r4=0`) → `sub_82A1A200(r3=100, r4=0)`

`sub_82A1A200` computes interval `= r3 * -10000 = -1,000,000` (stored as 64-bit 100ns-unit relative timeout), then calls `KeDelayExecutionThread`. This is a **100 millisecond sleep**.

---

## Complete Condition Chain

```
sub_82212F38 loops while:
  struct[2016] == 1                          (connection state = connecting)
    set by: XAudio2 async I/O initiation
    cleared by: async completion callback writing struct[2016] = 0
      which depends on: Xbox 360 kernel audio device being present

  → sub_82212EC0 tries to advance state:
      → sub_8220FDB8 enumerates endpoints and issues async I/O
          → sub_82A11EB8 (async device connect) fires callback on Xbox 360
          → callback writes struct[2016] = 0 → loop exits

  AND global abort byte 0x82BC9BE2 == 0
```

---

## Why It Never Exits on PC/macOS

`sub_8220FDB8` initiates an async I/O request to what would be an Xbox 360 kernel audio device. On PC/macOS:

1. `sub_829E4000` / `sub_829E4160` (XAudio2 device enumeration stubs) return no valid device handle.
2. `sub_82A11EB8` is called but the target device path does not exist on a PC file system.
3. No completion callback ever writes `struct[2016] = 0`.
4. `sub_82212EC0` re-reads `struct[2016]`, finds it still `== 1`, and returns without advancing state.
5. The loop sleeps 100ms and retries indefinitely.

---

## Minimal Fix

**Hook `sub_82212EC0`** — this is the most surgical point. It is called every 100ms from the loop; writing `struct[2016] = 0` tells the loop the connection is "done" on the first call.

```cpp
// In LibertyRecomp/kernel/imports.cpp or audio_hooks.cpp:
PPC_FUNC_HOOK(__imp__sub_82212EC0) {
    // r3 = audio struct pointer
    // Write state=0 (connected) immediately to exit the yield loop
    PPC_STORE_U32(ctx.r3.u32 + 2016, 0);
    // Optionally also set the connection count to signal 1 endpoint found:
    // PPC_STORE_U32(ctx.r3.u32 + 288, 1);
    return;
}
```

**Alternative**: Hook `sub_8220FDB8` and write `r3+2016 = 0` before returning. This is equivalent but acts deeper in the call chain.

**Do NOT stub `sub_82212F38` entirely** — the function also sets up audio format parameters, creates the audio thread, and initializes the streaming pipeline before the yield loop is reached.

---

## Memory Address Summary

| Address | Size | Description |
|-|-|-|
| `0x82BC9BD8` | base | Audio manager object base (`r26`) |
| `0x82BC9BE2` | u8 | Global abort flag (`r26+10`) — non-zero breaks the loop |
| `0x82BCB728` | u32 | Audio device object pointer |
| `0x82BCB79C` | u32 | Audio device name string pointer (via struct at `0x82BCB798`) |
| `struct+2016` | u32 | Per-device connection state: `1`=connecting, `0`=connected |
| `struct+288` | u32 | Connected/endpoint count flag |
| `struct+328` | u32 | Device handle (closed/reopened by `sub_8220FDB8`) |
| `struct+2024` | u8 | Ready bitfield (bits 7–14 set by `sub_82212EC0`) |

`struct` = stack-allocated audio device struct at `r1+1872` within `sub_82212F38`.

---

## File Locations

| File | Lines | Content |
|-|-|-|
| `gta4_recomp.5.cpp` | 56270–~56700 | `sub_82212F38` (audio init + yield loop) |
| `gta4_recomp.5.cpp` | 56071–56192 | `sub_82212DE8` (device presence check) |
| `gta4_recomp.5.cpp` | 56196–56265 | `sub_82212EC0` (connection state step) |
| `gta4_recomp.5.cpp` | 48914–49071 | `sub_8220FDB8` (XAudio2 endpoint connect) |
| `gta4_recomp.55.cpp` | 8563–8568 | `sub_82849918` (sleep trampoline) |
| `gta4_recomp.55.cpp` | 36747–36753 | `sub_82A12B60` (sets r4=0, calls sleep) |
| `gta4_recomp.69.cpp` | 50254–50335 | `sub_82A1A200` (actual 100ms KeDelayExecutionThread) |
| `gta4_recomp.56.cpp` | 8899–8957 | `sub_8285DC40` (device name non-empty check) |

All paths relative to `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/`.
