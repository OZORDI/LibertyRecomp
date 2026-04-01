# Scene Loading Dispatch: sub_82242218 and sub_822422E0

## Overview

These two functions form the scene/level loading dispatch layer in GTA IV's state machine. They sit between the high-level game state machine (sub_82142230, sub_8214B640) and the lower-level scene resource loading system. Both functions gate their work behind a shared `state_phase` variable and coordinate via several state flags.

- **sub_82242218** ("Request Scene Load") -- initiates a scene load request based on parameter sign
- **sub_822422E0** ("Game Start / Level Selection") -- selects the correct level name based on game mode (GTA4/TLAD/TBOGT) and kicks off loading

---

## Memory Addresses (All Python-verified)

| Address      | Size  | Name                     | Description |
|-------------|-------|--------------------------|-------------|
| `0x82BF981F` | byte  | `scene_loading_triggered`| **The critical byte that state 4 checks.** Set to 1 when r3 < 0 in sub_82242218, cleared to 0 by sub_822422E0 after processing. |
| `0x82BF9830` | dword | `loading_in_progress`    | Guard flag in sub_82242218. If nonzero, skips load initiation. Set to 1 after load begins. |
| `0x82BF9834` | dword | `state_phase`            | Shared state variable. 0 = idle (can accept new loads), 1 = loading started (causes both functions to return 2 = "busy"), >=2 also returns busy. |
| `0x82BF9838` | dword | `loading_started`        | Guard flag in sub_822422E0. If nonzero, skips level selection. Set to 1 after level selection completes. |
| `0x82BF9F00` | dword | `guard_ptr`              | Checked early in sub_82242218 when r3 < 0. If nonzero, skips the negative-r3 special path. |
| `0x82BF3A60` | buf   | `level_name_buf`         | Destination buffer for formatted level name string (written by sub_82240480 via sprintf). |
| `0x82BF3960` | buf   | `DLC_name_buf`           | 8-byte UTF-16 buffer for DLC prefix. "TLAD" or "TBOGT" written as uint16 shorts. |
| `0x82BF3EA0` | array | `level_entries`          | Array of 308-byte structs. Name string at offset +264 within each entry. |
| `0x82A9547C` | byte  | `use_name_copy_flag`     | If nonzero, sub_822422E0 copies from level_entries[r3] instead of using game_mode dispatch. |
| `0x82A97CF0` | dword | `scene_index`            | Scene/level index stored by callers before invoking sub_822422E0. |
| `0x82B39504` | dword | `game_mode`              | 0 = GTA IV base game, 1 = TLAD, 2 = TBOGT. Read by both sub_822422E0 and sub_82240480. |
| `0x82006688` | rodata| `scene_loading_name`     | Constant string in .rodata, source for strncpy when r3 < 0 in sub_82242218. |

---

## sub_82242218 -- Request Scene Load

**Address**: `0x82242218`
**File**: `gta4_recomp.6.cpp` line 83752
**Parameters**: `r3` = scene request parameter (sign determines behavior)
**Returns**: `r3` = 0 (accepted/skipped) or 2 (busy)

### Control Flow

```
sub_82242218(r3):
    save r31 = r3  (the request parameter)

    // --- EARLY GUARD (r3 < 0 only) ---
    if r31 < 0:
        ptr = *(uint32_t*)0x82BF9F00     // guard_ptr
        if ptr != 0:
            goto SKIP_LOAD               // someone else is handling it

    // --- STATE PHASE CHECK ---
    phase = *(uint32_t*)0x82BF9834        // state_phase
    if phase == 1 OR phase == 3:
        return 2                          // BUSY -- load already in progress
    if phase >= 1:
        return 2                          // same: any non-zero phase = busy

    // --- LOAD INITIATION (phase == 0 only) ---
    if *(uint32_t*)0x82BF9830 != 0:       // loading_in_progress
        goto SKIP_LOAD                    // already loading, return 0

    sub_8223D110()                        // reset/prepare scene loader (reads camera floats)

    *(uint8_t*)0x82BF981F = 0             // clear scene_loading_triggered FIRST

    if r31 < 0:
        // NEGATIVE PATH: direct scene name from .rodata
        *(uint8_t*)0x82BF981F = 1         // SET scene_loading_triggered = 1
        strncpy(0x82BF3950, 0x82006688, 16)  // copy hardcoded scene name
        sub_82211060()                    // fire event (type=13, subcmd=52)
    else:
        // NON-NEGATIVE PATH: use scene index
        sub_82240480(r31)                 // format level name from index + game_mode
        sub_822110A0()                    // fire event (type=14, subcmd=52)

    *(uint32_t*)0x82BF9830 = 1            // loading_in_progress = 1
    *(uint32_t*)0x82BF9834 = 1            // state_phase = 1

SKIP_LOAD:
    return 0
```

### Key Observations

1. **r3 < 0** sets `scene_loading_triggered` (0x82BF981F) to 1. This is the byte that state 4 in the parent state machine checks to decide whether to proceed.
2. **r3 >= 0** uses `sub_82240480` to format a level name string based on the scene index and current game_mode, then stores it in `level_name_buf` (0x82BF3A60).
3. The function is idempotent when `loading_in_progress` is already 1 -- it returns 0 without doing anything.
4. The state_phase check returns 2 ("busy") for any non-zero phase, preventing re-entrant loading.
5. Two different event notifiers are fired depending on path: sub_82211060 (event 13) for negative, sub_822110A0 (event 14) for non-negative.

---

## sub_822422E0 -- Game Start / Level Selection

**Address**: `0x822422E0`
**File**: `gta4_recomp.6.cpp` line 83871
**Parameters**: `r3` = scene/level index
**Returns**: `r3` = 0 (accepted/skipped) or 2 (busy)

### Control Flow

```
sub_822422E0(r3):
    // --- STATE PHASE CHECK (identical to sub_82242218) ---
    phase = *(uint32_t*)0x82BF9834
    if phase == 1 OR phase == 3:
        return 2                          // BUSY
    if phase >= 1:
        return 2

    // --- LOAD GUARD ---
    if *(uint32_t*)0x82BF9838 != 0:       // loading_started
        goto DONE                         // already started, return 0

    // --- LEVEL NAME SELECTION ---
    if *(uint8_t*)0x82A9547C != 0:        // use_name_copy_flag
        // COPY FROM LEVEL ENTRIES ARRAY
        src = 0x82BF3EA0 + r3 * 308 + 264   // level_entries[r3].name (offset +264)
        dst = 0x82BF3A60                     // level_name_buf
        // byte-by-byte copy loop until null terminator
        copy src -> dst
    else:
        // GAME MODE DISPATCH
        if r3 < 0:
            mode = *(uint32_t*)0x82B39504    // game_mode
            if mode == 1:
                r3 = 13                      // TLAD level index
            elif mode == 2:
                r3 = 14                      // TBOGT level index
            else:
                r3 = 12                      // GTA IV default level index

        sub_82240480(r3)                     // format level name from index

    // --- FINALIZE ---
    *(uint8_t*)0x82BF981F = 0             // CLEAR scene_loading_triggered
    *(uint32_t*)0x82BF9838 = 1            // loading_started = 1
    *(uint32_t*)0x82BF9834 = 2            // state_phase = 2
    sub_822110E0()                        // fire event (type=15, subcmd=52)

DONE:
    return 0
```

### Key Observations

1. **Game mode dispatch** (when `use_name_copy_flag` == 0 and r3 < 0): translates `game_mode` to a hardcoded level index:
   - 0 (GTA IV) -> index 12
   - 1 (TLAD) -> index 13
   - 2 (TBOGT) -> index 14
2. **Level entries copy** (when `use_name_copy_flag` != 0): copies the name string from `level_entries[r3]` at offset 264 within the 308-byte struct directly into `level_name_buf`.
3. **Clears `scene_loading_triggered`** (0x82BF981F = 0) -- this is the complement of sub_82242218 which sets it.
4. **Advances state_phase to 2** (vs sub_82242218 which sets it to 1), establishing the loading pipeline progression: 0 -> 1 (requested) -> 2 (level selected).
5. Fires event type 15 (vs 13/14 in sub_82242218).

---

## Sub-function Summary

| Function       | Address      | Role |
|----------------|-------------|------|
| `sub_8223D110`  | `0x8223D110` | Scene loader reset/prepare. Reads camera position floats. Called at start of sub_82242218 load path. |
| `sub_82240480`  | `0x82240480` | Level name formatter. Takes scene index in r3, reads `game_mode` from 0x82B39504, calls sprintf (sub_82A00108) to format name into `level_name_buf` (0x82BF3A60). For TLAD (mode 1): writes "TLAD" to DLC_name_buf. |
| `sub_82211060`  | `0x82211060` | Event notifier: fires event type 13, subcmd 52 via sub_825EC320 + sub_82210C70. Used for negative-r3 (scene loading name) path. |
| `sub_822110A0`  | `0x822110A0` | Event notifier: fires event type 14, subcmd 52. Used for non-negative-r3 (indexed scene) path. |
| `sub_822110E0`  | `0x822110E0` | Event notifier: fires event type 15, subcmd 52. Used by sub_822422E0 after level selection. |
| `sub_825EC320`  | `0x825EC320` | Low-level event constructor/init (called by all three notifiers). |
| `sub_82210C70`  | `0x82210C70` | Low-level event dispatch (called by all three notifiers with subcmd 52). |

---

## Callers

### sub_82242218 Callers

1. **sub_82258CB0** (gta4_recomp.7.cpp line 38030) -- Loading orchestrator state machine
   - Switch on return value of sub_82243C50 (case 2 = state 3 in the switch):
   - `r3 = *(uint32_t*)(r1+80)` loaded from stack (initialized to -1 at line 38095)
   - This is the primary caller that triggers the negative-r3 path
   - After calling sub_82242218, it reads 0x82BFA110 and calls sub_8229C140

2. **sub_825D32F0** (gta4_recomp.32.cpp line 34484) -- Thin wrapper / trampoline
   - Sets `r3 = -1` and tail-calls sub_82242218
   - This is a convenience function for triggering scene_loading_triggered = 1

### sub_822422E0 Callers

1. **sub_82142230** (gta4_recomp.0.cpp line 5305) -- Main game state machine
   - At label `loc_821425D0` (state transition):
   - `r3 = *(uint32_t*)(r17 + 21624)` -- reads scene index from game context (0x82A95478 if r17 = 0x82A90000)
   - After return, sets state r29 = 6, then calls sub_822438B0 to check loading status

2. **sub_8214B640** (gta4_recomp.0.cpp line 27784) -- Secondary game state machine
   - At label `loc_8214C4F4`:
   - `r3 = *(uint32_t*)(0x82A97CF0)` -- reads scene index from scene_index global
   - After return, calls sub_82252D08 for fade/transition, then sub_822438B0 for status

---

## State Machine Integration

The loading pipeline flows through these states:

```
state_phase (0x82BF9834):
  0 = IDLE          -- both functions can accept new work
  1 = REQUESTED     -- sub_82242218 has initiated a load request
  2 = LEVEL_SELECTED -- sub_822422E0 has selected the level name
  3 = (checked but not set by these functions -- set elsewhere, treated as BUSY)
```

### Typical call sequence:

```
1. Parent state machine calls sub_82242218(scene_index)
   - r3 < 0: sets scene_loading_triggered=1, copies hardcoded name, phase -> 1
   - r3 >= 0: formats level name from index, phase -> 1

2. Parent state machine calls sub_822422E0(scene_index)
   - Sees phase=1, returns 2 (BUSY) -- parent loops

3. Something external resets phase to 0 (via sub_822438B0 or similar)

4. Parent calls sub_822422E0(scene_index) again
   - Phase=0, selects level name based on game_mode or level_entries
   - Clears scene_loading_triggered, phase -> 2
```

### The scene_loading_triggered byte (0x82BF981F):

- **Set to 1** by sub_82242218 when r3 < 0 (e.g., from sub_825D32F0 wrapper or sub_82258CB0 with stack arg = -1)
- **Cleared to 0** by sub_822422E0 after it finishes level selection
- **Also cleared to 0** as first action in sub_82242218 before the sign check (always clears before potentially re-setting)
- **State 4 checks this byte** to decide whether to proceed with the load

---

## The strncpy at line 83839

```c
strncpy(0x82BF3950, 0x82006688, 16);
```

This copies a 16-byte scene loading name from read-only data at `0x82006688` into a buffer at `0x82BF3950`. This is a separate buffer from `level_name_buf` (0x82BF3A60) -- it appears to be a short "scene loading identifier" or tag that is only written in the negative-r3 path. The 16-byte limit suggests it is a fixed-size name field (likely null-terminated within those 16 bytes). This name is written just before firing event type 13 via sub_82211060, so it serves as a parameter or label for the scene load request event.

---

## Hook Strategy Notes

For writing replacement hooks:

1. **sub_82242218** can be hooked to intercept all scene load requests. The sign of r3 determines the behavior:
   - r3 < 0: "reload current scene" or "load default scene" path
   - r3 >= 0: "load specific scene by index" path

2. **sub_822422E0** can be hooked to control level selection. The game_mode -> level index mapping (12/13/14) is the key dispatch for base game vs DLC content.

3. Both functions share `state_phase` (0x82BF9834) as a mutual exclusion mechanism. Any hook must maintain this protocol or the parent state machines will deadlock waiting for phase transitions.

4. The `scene_loading_triggered` byte (0x82BF981F) is the critical handoff signal that state 4 in the parent state machine polls. A hook that needs to trigger scene loading must set this byte.
