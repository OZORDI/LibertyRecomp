# Save System Infrastructure Analysis

## Overview

GTA IV's save system spans three layers: (1) PPC guest-side state machines that orchestrate save/load operations, (2) LibertyRecomp hooks that patch Xbox 360-specific behavior for the recomp environment, and (3) host-side XAM content management APIs that bridge Xbox 360 content containers to native filesystem paths.

The key revelation from this analysis is that **sub_822438B0 is a dedicated SAVE state machine** (states 0-7) that handles the full save lifecycle: initiate, enumerate content, write data, validate, finalize. It is distinct from sub_82242910 which is the scene/content loading state machine (states 0-14).

---

## Architecture Diagram

```
                    +---------------------------+
                    |      main.cpp             |
                    |  SaveSystem::Initialize() |
                    +-------------+-------------+
                                  |
                    +-------------v-------------+
                    |     save_system.cpp        |
                    | - GetSaveDirectory()       |
                    | - RegisterSaveSlot()       |
                    | - XamRootCreate("SaveData")|
                    +-------------+-------------+
                                  |
        +-------------------------+-------------------------+
        |                                                   |
+-------v--------+                                +--------v--------+
| save_hooks.cpp |                                |    xam.cpp      |
| PPC overrides  |                                | XamContentCreateEx |
| (8 functions)  |                                | XamContentClose    |
+-------+--------+                                | XamContentEnumerator|
        |                                         | XamEnumerate       |
        |                                         +--------+---------+
        |                                                  |
+-------v--------------------------------------------------v--------+
|                    PPC Guest State Machines                        |
|  sub_822438B0 (SAVE SM, 8 states)                                 |
|  sub_82242910 (SCENE/LOAD SM, 15 states)                          |
|  sub_821E6508 (boot-time save gate, 17 states)                    |
+-------------------------------------------------------------------+
```

---

## 1. save_hooks.cpp -- PPC Function Hooks

**File**: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/save_hooks.cpp`

Eight hooks are defined, each overriding a weak PPC recompiled symbol via `PPC_FUNC()`:

### 1.1 sub_829A1C38 -- Content Creation Wrapper
- **Purpose**: Logging wrapper around `XamContentCreateEx` calls
- **Behavior**: Logs entry/exit with register values (r3, r4, r5), then delegates to original `__imp__sub_829A1C38`
- **Impact**: Diagnostic only; no behavioral change

### 1.2 sub_829A1CA0 -- Content Close Wrapper
- **Purpose**: Logging wrapper around `XamContentClose`
- **Behavior**: Logs the close call, then delegates to original
- **Impact**: Diagnostic only

### 1.3 sub_829A1CB8 -- Content Enumeration Wrapper
- **Purpose**: Logging wrapper around `XamContentCreateEnumerator`
- **Behavior**: Logs enumeration parameters (r3-r6), delegates to original
- **Impact**: Diagnostic only

### 1.4 sub_8297A930 -- Save Manager
- **Purpose**: Logging wrapper around the save manager orchestrator
- **Behavior**: Logs first 10 invocations with context pointer (r3), delegates to original
- **Role in pipeline**: This is called by the game's main update loop; it calls sub_829A1878 internally to perform actual save I/O

### 1.5 sub_82122CA0 -- Save System Init
- **Purpose**: Logging hook for save system initialization
- **Behavior**: Logs the three save slot contexts being created:
  - Profile Save (vtable: 0x81209104) -- player settings, stats
  - Game Save (vtable: 0x81209064) -- mission progress, world state
  - Autosave (vtable: 0x81209028) -- automatic checkpoint saves
- **Impact**: Diagnostic only

### 1.6 sub_821200D0 -- Post-Init (Critical Behavioral Hook)
- **Purpose**: Bypasses the loading screen busy-wait gate and forces kernel phase transition
- **Key fixes**:
  1. **Loop 1 bypass**: Writes `0` to `0x83137BC9` (LOADING_STEP_ADDR). On Xbox 360, a dedicated rendering thread clears this; in recomp, no such thread exists, so the busy-wait would spin forever.
  2. **Kernel phase transition**: Forces `KernelPhase::Init -> KernelPhase::Runtime` on first entry, breaking a circular dependency where runtime APIs are needed before the phase formally transitions.
  3. **Save state machine**: Previously forced state=17 + retval=3 to bypass the save SM entirely. This was **removed** because it prevented sub_8223F9F0 (XAM dialog flow) from running, which meant the readiness dword at 0x82BF9B70 was never set, trapping the front-end state machine in state 3 forever. Now delegates to `__imp__sub_821200D0` to let RexGlue's XAM subsystem run naturally.
- **Memory addresses**:
  - `0x83137BB7`: Loading in-progress flag
  - `0x83137BC9`: Loading step (non-zero = still loading)
  - `0x82B94554`: Save state machine step (0-17)
  - `0x82B946C8`: Save state machine return value

### 1.7 sub_8219F728 -- Active Player Slot Counter
- **Purpose**: Forces 1 active player so the save state machine can proceed
- **Problem solved**: On Xbox 360, XNotify sign-in events populate 4 player slots at 0x82ACBD60 (stride 188). On macOS, no events fire, so all slots remain null, and the save gate (sub_821E6508) returns 0, causing an infinite spin.
- **Fix**: Returns `1` unconditionally (user 0 active), matching RexGlue's UserProfile which hardcodes signin_state=1 for player 0

### 1.8 sub_8218C2C0 -- Loading Complete Check
- **Purpose**: Signals "loading complete" to exit Loop 2 in sub_821200D0
- **Problem solved**: Original code checks VBlank hardware (sub_82856FE0 -> sub_82878998) which doesn't exist in recomp. Always returns 0, trapping the render loop forever.
- **Fix**: Returns `1` unconditionally

### 1.9 sub_82192E00 -- Streaming Init
- **Purpose**: Clears streaming-pending flag after synchronous init
- **Problem solved**: On Xbox 360, hardware streaming completion thread clears 0x830F5820. In recomp, VFS is synchronous, so the flag is never cleared.
- **Fix**: Runs original, then writes `0` to `0x830F5820`

### 1.10 sub_827DE648 -- Streaming Completion Barrier
- **Purpose**: Belt-and-suspenders fallback for streaming barrier
- **Fix**: Returns immediately without spinning (0x830F5820 already cleared by sub_82192E00 hook)

---

## 2. save_system.cpp / save_system.h -- Host-Side Save Management

**Files**:
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/save_system.cpp`
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/save_system.h`

### API Surface

| Function | Description |
|----------|-------------|
| `Initialize()` | Called from main.cpp before game init. Creates save directory, enumerates existing saves, registers them with XAM content system |
| `GetSaveDirectory()` | Returns platform-specific saves path (macOS: `~/Library/Application Support/LibertyRecomp/saves/`) |
| `GetSaveFilePath(slot)` | Returns `saves/SGTA4XX` where XX is zero-padded slot number |
| `SaveSlotExists(slot)` | Checks if save file exists on disk |
| `EnumerateSaveFiles()` | Lists all files matching `SGTA4*` prefix in save directory |
| `RegisterSaveSlot(slot)` | Calls `XamRegisterContent()` to make save visible to PPC code |
| `ImportSaveFile(src, slot)` | Copies external save file to slot, registers it |

### Key Details

- **Max save slots**: 16 (SGTA400 through SGTA415)
- **Naming convention**: `SGTA4XX` (GTA IV standard)
- **Save types**: Profile Save (0x81209104), Game Save (0x81209064), Autosave (0x81209028)
- **Content registration**: Uses `XamMakeContent(XCONTENTTYPE_SAVEDATA, filename)` then `XamRegisterContent()`
- **Root mappings**: Creates three aliases: "SaveData", "save", "saves" all pointing to the saves directory
- **Initialization order**: Called from `main.cpp:265`, after heap init but before game entry

---

## 3. sub_822438B0 -- The SAVE State Machine (Outer SM)

**Source**: `gta4_recomp.6.cpp` line 87081
**Documentation**: `/Users/Ozordi/Downloads/LibertyRecomp/docs/rewrite/02_sub_822438B0_states.md`

This is the core save orchestrator. It runs during gameplay to handle saving.

### State Flow

| State | Name | Key Action |
|-------|------|------------|
| 0 | IDLE | Returns 0 immediately. External code sets STATE=1 to begin save |
| 1 | INITIATE | Sets save_trigger=1, field_9844=2, transitions to state 2 |
| 2 | ENUMERATE | Calls sub_82242910 (inner SM). On success -> state 3; on error 33 -> done; else -> state 7 |
| 3 | SAVE OP (read phase) | Calls **sub_82240F80(0)**. Checks "SAVE" magic header at 0x82BF394C. On success -> state 4; bad magic -> error 29 |
| 4 | VALIDATE | Calls **sub_822446E8** + **sub_8223F740(0)** (only when SUB_STATE==2). On validation failure -> error 31 |
| 5 | WRITE DATA | Calls **sub_82240F80(1)** (write phase). On success -> state 6 |
| 6 | FINALIZE | Waits for 3000-tick timer. Calls **sub_8223CAD8** (cleanup) + **sub_826CD808** (get device pointer) + sub_829DBAA8/sub_829DB688 (process save result) |
| 7 | ERROR/RETRY | Calls sub_8223CC68 (cancel timer) + sub_82242608 (retry handler) |

### Return Values

| Value | Meaning |
|-------|---------|
| 0 | Idle / save complete (state 0 or state 6 with pointer) |
| 1 | In progress (keep calling) |
| 2 | Done with reset (epilogue resets STATE=0, SUB_STATE=0) |

### Critical Functions Called

- **sub_82240F80(mode)**: The actual save I/O function. mode=0 is read/verify phase, mode=1 is write phase. Called from states 3 and 5.
- **sub_822446E8**: Post-save setup, called from state 4 when SUB_STATE==2. No arguments.
- **sub_8223F740(0)**: Save validation check, returns bool. Called from state 4 after sub_822446E8. Failure sets error_code=31.
- **sub_8223CAD8**: Cleanup/teardown. Called from state 6 finalization and the shared epilogue.
- **sub_826CD808**: Returns a pointer to the device/storage manager object. Called in state 6 to check if finalization is complete. Also used by sub_8223DAA0 (readiness check).

### Error Codes

| Code | Meaning | Set By |
|------|---------|--------|
| 29 | Invalid save header (no "SAVE" magic) | State 3 |
| 31 | Post-save validation failed | State 4 |
| 33 | Special cancellation (triggers immediate done) | External |

### Key Memory Map

| Address | Type | Description |
|---------|------|-------------|
| 0x82BF9838 | u32 | STATE -- current save state (0-7) |
| 0x82BF9834 | u32 | SUB_STATE -- secondary state |
| 0x82BF3940 | 16 bytes | Data buffer; bytes +12..+15 checked for ASCII "SAVE" |
| 0x82BF9898 | u64 | Save result (8-byte value stored in state 6) |
| 0x82A95466 | u8 | Save trigger flag |
| 0x82A9547C | u8 | Ready flag (polled in state 2) |
| 0x82A9546C | u32 | Error code |

---

## 4. sub_8223F9F0 -- Xbox Guide UI / Storage Device Selection

**Hook location**: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/imports.cpp:2035`

This is the "XAM dialog flow" function. It is called by the inner state machine (sub_82242910) in states 1, 2, 5, 7, and 13 with different mode arguments:
- Mode 0: Initial content check
- Mode 1: Alternate content check
- Mode 2: Show storage device selector UI
- Mode 3: Re-enumerate content after device selection
- Mode 4: Error recovery content re-enumeration

Internally, sub_8223F9F0 calls **sub_82254FE0** which writes `1` to `0x82BF9B70`, the readiness dword. This is critical -- if sub_8223F9F0 never runs (e.g., because the save SM was bypassed), the front-end state machine (sub_82142230) loops forever in state 3 waiting for that readiness signal.

The imports.cpp hook is diagnostic only -- logs entry/exit and delegates to the original.

---

## 5. Additional Save-Related Hooks in imports.cpp

**File**: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/imports.cpp`

| Hook | Purpose |
|------|---------|
| `sub_8223CAD8` (line 1963) | Diagnostic: logs save cleanup/teardown calls |
| `sub_82254FE0` (line 1992) | Diagnostic: logs the critical readiness signal write to 0x82BF9B70 |
| `sub_8224FA38` (line 2007) | Diagnostic: logs readiness reset (-1 written to 0x82BF9B70) |
| `sub_8214C8C8` (line 2020) | Diagnostic: logs readiness counter increments toward 4 |
| `sub_8223F9F0` (line 2035) | Diagnostic: logs XAM dialog flow calls |
| `sub_82219AC0` (line 1979) | Diagnostic: logs player struct checks during state 3 |

---

## 6. XAM Content API Implementation (LibertyRecomp's xam.cpp)

**File**: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/xam.cpp`

LibertyRecomp implements its own XAM content APIs (separate from RexGlue's content_manager.cpp). These are the functions called by the PPC wrappers (sub_829A1C38, etc.):

### XamContentCreateEx
- Handles `CREATE_ALWAYS` and `OPEN_EXISTING` modes
- For `XCONTENTTYPE_SAVEDATA`: uses `GetSavePath(true)` as root
- For `XCONTENTTYPE_DLC`: uses `GetGamePath() / "dlc"` as root
- Creates directories on demand, registers content in `gContentRegistry`
- Maps root name via `XamRootCreate(szRootName, root)`

### XamContentClose
- Removes root mapping from `gRootMap`
- Completes overlapped operation if provided

### XamContentCreateEnumerator
- Only supports user 0; returns ERROR_NO_SUCH_USER otherwise
- Creates `XamEnumerator` from `gContentRegistry[dwContentType - 1]`
- Returns buffer size = `sizeof(_XCONTENT_DATA) * cItem`

### XamEnumerate
- Iterates through enumerator, copies content data to buffer
- Returns ERROR_NO_MORE_FILES when exhausted

### XamContentGetDeviceData
- Returns a dummy HDD device with 1GB total/free space
- Device name: "GTA4"

### Content Registry Structure
```cpp
std::array<xxHashMap<XHOSTCONTENT_DATA>, 3> gContentRegistry{};
// Index 0 = XCONTENTTYPE_SAVEDATA (type 1)
// Index 1 = XCONTENTTYPE_DLC (type 2)
// Index 2 = XCONTENTTYPE_??? (type 3)
```

---

## 7. RexGlue's Content Manager (Parallel Implementation)

**File**: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/src/system/xam/content_manager.cpp`

RexGlue provides a **second, independent** content management system ported from Xenia. It is NOT directly called by LibertyRecomp's save hooks; rather it exists for the RexGlue-side XAM kernel exports.

### Key Differences from LibertyRecomp's xam.cpp

| Feature | LibertyRecomp xam.cpp | RexGlue content_manager.cpp |
|---------|----------------------|----------------------------|
| Storage | `xxHashMap<XHOSTCONTENT_DATA>` in-memory | Filesystem directories under `root_path/title_id/content_type/` |
| VFS integration | Manual root map (`gRootMap`) | Creates `HostPathDevice` per package, registers in VFS |
| Package tracking | None (stateless) | `open_packages_` map with `ContentPackage` objects |
| Thread safety | None | `global_critical_region_` mutex |
| Cleanup | Just erases root map entry | `CloseOpenedFilesFromContent()` releases file handles |

### RexGlue Content Manager API

| Method | Description |
|--------|-------------|
| `CreateContent(root, data)` | Creates directory at `root_path/title_id/content_type/file_name/`, registers VFS device |
| `OpenContent(root, data)` | Opens existing package directory, registers VFS device |
| `CloseContent(root)` | Unregisters VFS device and symbolic link, releases file handles |
| `ListContent(device_id, type)` | Lists directories under `root_path/title_id/content_type/` |
| `ContentExists(data)` | Checks if package directory exists on disk |
| `DeleteContent(data)` | Removes package directory tree |
| `Get/SetContentThumbnail` | Reads/writes `__thumbnail.png` in package directory |
| `ResolveGameUserContentPath()` | Returns `root_path/title_id/profile/user_name/` |

### RexGlue's XAM Kernel Exports (xam_content.cpp)

**File**: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/src/kernel/xam/xam_content.cpp`

These are the PPC-callable kernel exports that delegate to the ContentManager:
- `XamContentCreateEnumerator_entry`: Creates `XStaticEnumerator<XCONTENT_DATA>`, populates from `content_manager()->ListContent()`
- `xeXamContentCreate`: Full create/open logic with CREATE_NEW, CREATE_ALWAYS, OPEN_EXISTING, OPEN_ALWAYS modes
- `XamContentGetLicenseMask_entry`: Returns license mask from cvar

---

## 8. Dual Implementation Problem

There are **two parallel content management systems** active:

1. **LibertyRecomp's xam.cpp**: Used by the PPC save wrapper functions (sub_829A1C38 -> XamContentCreateEx, sub_829A1CA0 -> XamContentClose, sub_829A1CB8 -> XamContentCreateEnumerator)
2. **RexGlue's content_manager.cpp + xam_content.cpp**: Used by RexGlue's kernel export table for PPC `sc` (system call) instructions

Which one handles a given call depends on whether the PPC code calls the function via a direct `bl` (branch-link, handled by recompiled wrappers -> LibertyRecomp's xam.cpp) or via a kernel import dispatch (handled by RexGlue's export table -> content_manager.cpp).

The save wrappers (sub_829A1C38, sub_829A1CA0, sub_829A1CB8) use direct `bl` calls, so they go through **LibertyRecomp's xam.cpp**. The XAM dialog flow (sub_8223F9F0) may use kernel imports internally, which would go through **RexGlue's exports**.

---

## 9. Save Operation Lifecycle (End-to-End)

### Boot-Time Initialization

1. `main.cpp` calls `SaveSystem::Initialize()` -- creates `~/Library/Application Support/LibertyRecomp/saves/`, registers existing `SGTA4XX` files with content system, creates "SaveData"/"save"/"saves" root mappings

2. `sub_821200D0` hook fires -- clears loading gate (BC9=0), forces KernelPhase::Runtime, delegates to original which runs:
   - sub_821E6508 (boot save gate, 17-state SM)
   - sub_8219F728 hook returns 1 (active player), allowing the gate to proceed
   - sub_8223F9F0 runs through XAM dialog flow, calling sub_82254FE0 which sets readiness dword 0x82BF9B70=1

3. sub_82122CA0 runs -- initializes 3 save slot contexts (Profile, Game, Autosave)

### Runtime Save Operation

1. Game triggers save -> sets 0x82BF9838 (STATE) to 1

2. **State 1 (INITIATE)**: Sets save trigger, transitions to state 2

3. **State 2 (ENUMERATE)**: Calls sub_82242910 (inner SM) which calls sub_8223F9F0 for content enumeration -> eventually calls XamContentCreateEnumerator -> XamEnumerate -> returns list of SGTA4XX saves

4. **State 3 (SAVE OP read)**: Calls sub_82240F80(0) -- reads/verifies save data, checks "SAVE" magic header

5. **State 4 (VALIDATE)**: If SUB_STATE==2, calls sub_822446E8 (post-save setup) + sub_8223F740(0) (validation)

6. **State 5 (WRITE DATA)**: Calls sub_82240F80(1) -- writes save data to disk via XamContentCreateEx (CREATE_ALWAYS mode) -> creates save directory, writes through rexcrt file hooks (CreateFileA, WriteFile, CloseHandle)

7. **State 6 (FINALIZE)**: Waits for 3000-tick timer, calls sub_8223CAD8 (cleanup) + sub_826CD808 (device check) + save result processing

8. **Epilogue**: Resets STATE=0, SUB_STATE=0, calls sub_8223CAD8 cleanup

---

## 10. Key Findings

### What Works
- Save directory creation and enumeration via SaveSystem
- XAM content APIs (create, close, enumerate) in xam.cpp
- Boot-time save gate traversal via sub_8219F728 hook (player count = 1)
- Readiness signaling via sub_8223F9F0 -> sub_82254FE0 -> 0x82BF9B70
- Loading gate bypass via sub_821200D0 hook

### Potential Issues
1. **Dual content management**: Two independent systems (LibertyRecomp xam.cpp vs RexGlue content_manager.cpp) could get out of sync if both handle the same content type
2. **No thread safety** in LibertyRecomp's xam.cpp content registry (`gContentRegistry` is unprotected)
3. **sub_8297A930 hook is diagnostic only** -- the save manager runs unmodified, which means any Xbox 360-specific behavior inside it (async I/O, hardware timers) could cause issues
4. **3000-tick timer in state 6**: This is a game-time timer, not wall-clock. If the game's tick system isn't running properly, finalization could stall
5. **sub_826CD808 (device pointer)**: Returns a storage device manager pointer. In the readiness check (sub_8223DAA0), if this returns 0, the system reports "not ready". The function's behavior in the recomp environment is unverified

### Functions Still Running As Recompiled PPC (No Hooks)
- sub_82240F80 -- actual save read/write I/O
- sub_822446E8 -- post-save validation setup
- sub_8223F740 -- save validation check
- sub_826CD808 -- device/storage manager pointer getter
- sub_829DBAA8 / sub_829DB688 -- save result processing
- sub_82242608 -- error retry handler
