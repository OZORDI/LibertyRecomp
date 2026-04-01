# sub_827C2420 — Stream Manager Audio Device Registration

Called from sub_82478AF8 (audio init) at address 0x82478EAC.

## Call Site in sub_82478AF8 (gta4_recomp.20.cpp:84288-84298)

### r31 — Audio Device Name String

r31 is set in two stages:

1. **Line 84236**: `r31 = r1 + 672` (stack buffer, default)
2. **Line 84240-84246**: Check `*(0x82FF5380)` — if non-null, `r31 = *(0x82FF5380)` (override)

If r31 stays as the stack buffer, it was populated by `sub_82A00DC0` (strncpy-like, 23 chars from string at `0x8201C054`). The string is an audio backend name, previously compared via `_stricmp` against constants at `0x8201C088`, `0x8201C07C`, `0x8201C074`, `0x8201C06C` to select audio mode (setting `r30->field_152` to 0 or 2).

### Store to streamMgr->field_56

```
r10 = *(0x82B393A4)         // stream manager singleton
stw r31, 56(r10)            // streamMgr->audioDeviceName = r31
r3 = *(0x82B393A4)          // same ptr passed as arg to sub_827C2420
```

### r3 — Stream Manager Ptr

`r3 = *(0x82B393A4)` — the global RAGE audio stream manager singleton.

## Inside sub_827C2420 (gta4_recomp.50.cpp:57394-57462)

### Register Setup

| Register | Value | Meaning |
|-|-|
| r31 | r3 (arg) | Stream manager ptr from `*(0x82B393A4)` |
| r30 | `0x82B07278` | Critical section (lis -32080 + 29304) |
| r4 | `*(r31 + 56)` | Audio device name string (just stored by caller) |

### Execution Flow

1. **Enter critical section**: `sub_8284F310(r3=0x82B07278, r4=audioDeviceName)`
   - sub_8284F310 saves r4 as r29, checks if `*(r29) == '$'` (ASCII 36)
   - Reads `*(mutex + 3072)` for lock state
   - This is a named critical section enter

2. **Load globals**:
   - `r10 = *(r31 + 0)` — vtable of stream manager
   - `r29 = *(0x831E55EC)` — global (event/callback dispatcher)

3. **Virtual call**: `vtable[1](streamMgr)` — `*(*(r31) + 4)` dispatched via `bctrl`
   - This is the second entry in the stream manager vtable
   - Returns a value used as r6 (likely a stream count or capability flags)

4. **Register event callback**: `sub_82852DD0` with:

| Arg | Register | Value |
|-|-|-|
| r3 | r3 | `*(0x831E55EC)` — event dispatcher |
| r4 | r4 | String at `0x8207A57C` — event/callback name |
| r5 | r5 | String at `0x820BBFE4` — secondary identifier |
| r6 | r6 | vtable[1] result |
| r7 | r7 | Stream manager ptr (r31) |
| r8 | r8 | 1 (enable flag) |

   sub_82852DD0 internally:
   - Calls `sub_8284F468(r3=0x82B07278, r4=eventName, r5=secondaryId, r6=0, r7=1)` to allocate/find a callback slot
   - If slot found, calls `sub_82852D18(dispatcher, slot, vtableResult, streamMgr, 1)` to register
   - Calls `sub_8285B088` to release the slot

5. **Leave critical section**: `sub_8284E830(r3=0x82B07278)`

## Key Globals

| Address | Role |
|-|-|
| `0x82B393A4` | Stream manager singleton ptr |
| `0x82B07278` | Audio critical section / named mutex |
| `0x831E55EC` | Event/callback dispatcher ptr |
| `0x82FF5380` | Optional audio device name override ptr |
| `0x82FF5404` | Earlier audio backend override ptr |

## What *(r31 + 56) Contains

`streamMgr->field_56` is the audio device name string, written at line 84292 (`stw r31, 56(r10)`) immediately before the sub_827C2420 call. It is either:
- A stack-local string buffer (`r1 + 672`) containing a 23-char audio backend name
- An override pointer from global `*(0x82FF5380)`

sub_827C2420 reads it back as `lwz r4, 56(r31)` and passes it to the critical section enter function, which uses it as the mutex/section name. The `'$'` prefix check suggests RAGE uses `$`-prefixed names for system-level critical sections.

## Pseudo-C

```c
void sub_827C2420(StreamManager* mgr) {
    CriticalSection* cs = (CriticalSection*)0x82B07278;
    const char* deviceName = mgr->audioDeviceName;  // field_56

    EnterNamedCriticalSection(cs, deviceName);       // sub_8284F310

    EventDispatcher* dispatcher = *(EventDispatcher**)0x831E55EC;
    uint32_t vtableResult = mgr->vtable[1](mgr);    // virtual call

    RegisterStreamCallback(                          // sub_82852DD0
        dispatcher,
        "callbackName",     // 0x8207A57C
        "secondaryId",      // 0x820BBFE4
        vtableResult,
        mgr,
        1                   // enable
    );

    LeaveNamedCriticalSection(cs);                   // sub_8284E830
}
```
