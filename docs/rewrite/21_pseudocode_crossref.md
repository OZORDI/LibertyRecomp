# 21 - Hex-Rays Pseudocode Cross-Reference

Cross-reference of the Hex-Rays decompiler output (`default (1).xex.c`, 61MB,
30,600 decompiled functions) against the generated recomp code analyzed in
docs 01-11. The pseudocode is from a **different binary version** than the
recomp target (TU8/v8).

---

## 1. Binary Version Mismatch

**Critical finding**: The pseudocode file and the recomp binary are from
different builds of GTA IV.

| Property | Pseudocode | Recomp (TU8) |
|---|---|---|
| Function count | 30,600 decompiled (32,828 total) | 38,268 recompiled |
| Address range | 0x82120000 - 0x82A138C8 | 0x820C0000 - 0x82A2xxxx+ |
| Data segment | 0x82B8xxxx - 0x82FDxxxx | 0x82BFxxxx - 0x831xxxxx |
| State machine state var | `dword_82B94544` | `0x82BF9848` |
| Error code var | `dword_82A243C4` | `0x82A9546C` |
| USER_STATE init | `-1` (declared `int dword_82A22D04 = -1;`) | `0` (per memory docs) |

The pseudocode is likely from an **earlier title update or base game** build.
The state machine functions exist at entirely different addresses, but the
overall architecture is structurally identical. Some low-address functions
(e.g., `sub_82121E80`, `sub_8214B640`) share the same address across both
builds, while the 0x821Exxxx / 0x8224xxxx region shifted significantly.

---

## 2. Function Address Mapping

| Recomp Function | Recomp Addr | Pseudocode Equiv | PC Addr | Confirmed By |
|---|---|---|---|---|
| sub_82242910 (scene creation SM) | 0x82242910 | sub_821E4D60 | 0x821E4D60 | Switch on `dword_82B94544`, 15 states, same flow |
| sub_822438B0 (outer SM) | 0x822438B0 | sub_821E5D00 | 0x821E5D00 | SAVE signature check, 8 states, error code 33 |
| sub_8223DAA0 (readiness check) | 0x8223DAA0 | sub_821E0E88 | 0x821E0E88 | XamUserGetSigninState pattern |
| sub_8223DB20 (controller check) | 0x8223DB20 | sub_821E0F08 | 0x821E0F08 | XNotifyGetNext(0xA) pattern |
| sub_82240B78 (platform check) | 0x82240B78 | sub_821E3BA0 | 0x821E3BA0 | XNotifyGetNext(0xB) pattern |
| sub_822422E0 (game start) | 0x822422E0 | sub_821E4730 | 0x821E4730 | Sets `dword_82B94530=1`, episode selection |
| sub_82240F80 (async I/O) | 0x82240F80 | sub_821E3FA8 | 0x821E3FA8 | BeginGetCreator/BeginLoad save I/O |
| sub_822417B0 (scene dispatch) | 0x822417B0 | sub_821E4500 | 0x821E4500 | Error codes 10, 33, 34 pattern |
| sub_82242218 (scene name) | 0x82242218 | sub_821E4668 | 0x821E4668 | Scene name builder with slot iteration |
| sub_82240B08 (save slot scan) | 0x82240B08 | sub_821E0840 | 0x821E0840 | "SGTA4%02d" format string, 75-slot limit |
| sub_82142230 (frontend SM) | 0x82142230 | sub_82121E80 | 0x82121E80 | **Same address** -- main loop, calls scene SM |
| sub_8214B640 (ready-signal reset) | 0x8214B640 | sub_8214B640 | 0x8214B640 | **Same address, DIFFERENT FUNCTION** (see below) |
| sub_8214C8C8 (ready-counter) | 0x8214C8C8 | NOT FOUND | - | Not in pseudocode range |
| sub_82254FE0 (ready-signal writer) | 0x82254FE0 | NOT FOUND | - | Not in pseudocode range |

### sub_82121E80 -- Same Address, Simplified in Pseudocode

The main frontend state machine `sub_82121E80` is at the **same address** in
both versions. In the pseudocode, its structure is:

```c
int sub_82121E80()
{
    sub_821E4398();           // reset state counter (same as recomp)
    for ( i = 0; ; i = v8 == 2 )
    {
        while ( 1 )
        {
            sub_827DAE18(1);  // tick
            sub_821221A8();   // frame advance
            if ( !i ) break;
            if ( i == 1 )
            {
                sub_821E47F8(dword_82A243CC, ...);  // setup save load
                i = 2;
            }
            v6 = sub_821E5D00();  // == sub_822438B0 in recomp
            if ( !v6 ) { v7 = 5; goto LABEL_15; }
            if ( v6 == 2 ) goto LABEL_9;
        }
        v8 = sub_821E6508();      // == sub_82142230-inner in recomp
        if ( v8 == 1 ) { v7 = 3; goto LABEL_15; }
        if ( v8 == 3 ) break;
    }
    // ... exit sequence: clear flags, cleanup ...
}
```

**Key insight**: The pseudocode shows sub_82121E80 is a much simpler two-phase
loop than the 10-state machine described in doc 07. Phase 1 calls
`sub_821E6508` (the save/autoload state machine) until it returns 1 or 3.
Phase 2 (when `i` becomes nonzero) calls `sub_821E5D00` (the scene creation
outer SM) until it returns 0 or 2.

---

## 3. Data Address Mapping

The state machine variables are at different addresses in each build but serve
identical roles:

| Recomp Address | Recomp Name | Pseudocode Address | Pseudocode Name |
|---|---|---|---|
| `0x82BF9848` | scene creation state | `0x82B94544` | `dword_82B94544` |
| `0x82BF9838` | outer SM state | `0x82B94534` | `dword_82B94534` |
| `0x82BF9844` | platformMode | `0x82B94540` | `dword_82B94540` |
| `0x82BF9834` | game start state | `0x82B9452C` | `dword_82B9452C` |
| `0x82BF981E` | done flag | `0x82B9451E` | `byte_82B9451E` |
| `0x82BF981F` | scene loaded flag | `0x82B9451F` | `byte_82B9451F` |
| `0x82A9546C` | error code | `0x82A243C4` | `dword_82A243C4` |
| `0x82A95466` | content byte flag | `0x82A243BE` | `byte_82A243BE` |
| `0x82A22D04` | USER_STATE | `0x82A22D04` | `dword_82A22D04` (**same addr**) |

---

## 4. Full Pseudocode for Key Functions

### 4.1 sub_821E4D60 (= sub_82242910, Scene Creation SM, 15 states)

```c
int sub_821E4D60()
{
    switch ( dword_82B94544 )
    {
    case 0:   // Check readiness
        if ( sub_821E0E88() )               // XamUserGetSigninState check
        {
            dword_82B94544 = 4;
            sub_827DD720(...);              // scene object init
            return 1;
        }
        dword_82B94544 = 1;
        return 1;

    case 1:   // Secondary checks
        if ( sub_826711E8() ) goto LABEL_9;  // XNotify check
        if ( sub_821E0E88() ) goto LABEL_10;
        if ( !sub_821E2B50(0, 0, v14) ) goto LABEL_115; // dialog check
        if ( !v14[0] ) goto LABEL_22;
        goto LABEL_9;

    // ... cases 2-3: dialog handling ...

    case 4:   // platformMode switch (KEY STATE)
        if ( sub_821E0F08() ) goto LABEL_24;   // controller disconnect -> error 33
        dword_82B946C0 = 0;
        if ( (dword_82B94540 - 3) <= 1u )      // platformMode 3 or 4
        {
            sub_821E23A0(1, &dword_82B8E778);  // CalculateSizeOfBuffer
            dword_82B946C0 = dword_82B8E77C;
        }
        if ( byte_82B9451E ) goto LABEL_39;    // done flag -> skip to state 5
        if ( sub_821E3B30() )                  // readiness gate
        {
            dword_82B94544 = 9;
            return 1;
        }
        switch ( dword_82B94540 )              // platformMode switch
        {
            case 0: case 1:  goto LABEL_35;    // accepted
            case 3:                            // base game
                if ( !byte_82B9451F ) goto LABEL_36;
                if ( !byte_82A243BE ) byte_82B9451E = 1;
                break;
            case 4:                            // DLC
                if ( byte_82A243BE ) goto LABEL_36;
                byte_82B9451E = 1;
                goto LABEL_35;
            default: goto LABEL_36;            // -> error 34
        }
        // ...

    // ... cases 5-14 handle scene loading, saving, async I/O ...

    case 13:  // Cleanup
        if ( sub_821E0F08() ) goto LABEL_24;   // error 33
        if ( sub_821E3BA0() ) goto LABEL_38;   // error 34
        if ( !sub_821E2B50(4, -dword_82B946BC, v14) ) goto LABEL_115;
        if ( v14[0] )
        {
            byte_82A243BE = 0;
            byte_82B8E9DA = 0;
            sub_821E0F78(0);
            dword_82B94544 = 0;
            return 1;
        }
        dword_82A243C4 = 6;   // error code 6
        return 2;
    }
}
```

**Comparison with recomp (doc 01)**:
- State count: 15 in both (0-14), MATCHES
- State 4 platformMode switch: identical pattern `(val-3) <= 1u` for accepting modes 3/4
- Error codes: 33 (controller disconnect), 34 (platform mismatch), 6 (default error) -- all MATCH
- The pseudocode confirms `byte_82B9451E` is the "done flag" and `byte_82B9451F` is the "scene loaded flag"

### 4.2 sub_821E5D00 (= sub_822438B0, Outer SM, 8 states)

```c
int sub_821E5D00()
{
    v0 = 0;
    switch ( dword_82B94534 )
    {
    case 0:
        return 0;                               // idle

    case 1:
        byte_82A243BE = 1;                      // content byte = 1
        dword_82B94540 = 2;                     // platformMode = 2
        byte_82B9451E = 0;                      // clear done flag
        dword_82B94544 = 0;                     // reset scene SM state
        dword_82B94534 = 2;                     // advance to state 2
        // fall through to case 2

    case 2:
        if ( !byte_82A243D0 )
        {
            byte_82B8E638 = 1;
            dword_82B8E634 = 0;
            dword_82B8E63C = 2;
        }
        v2 = sub_821E4D60();                    // call scene creation SM
        if ( v2 == 2 )
        {
            if ( dword_82A243C4 == 33 )         // error 33 -> return 1 (retry)
                goto LABEL_41;
            dword_82B94534 = 7;                 // other error -> state 7
        }
        else if ( !v2 )                         // scene SM done
        {
            // clear 16-byte buffer
            dword_82B94534 = 3;
        }
        goto LABEL_42;

    case 3:
        v5 = sub_821E3FA8(0);                   // async I/O (GetCreator)
        if ( !v5 )
        {
            dword_82B94534 = 4;
            // CHECK SAVE SIGNATURE: bytes 12-15 == "SAVE" (0x53,0x41,0x56,0x45)
            if ( byte_82B8E64C == 83 )
                v6 = byte_82B8E64D == 65 && byte_82B8E64E == 86 && byte_82B8E64F == 69;
            else
                v6 = 0;
            if ( !v6 )
            {
                nullsub_1("CGenericGameStorage::GenericLoad - expected to find SAVE ...\n");
                dword_82A243C4 = 29;            // error 29
                dword_82B94534 = 7;
            }
        }
        else if ( v5 == 2 )
        {
            if ( dword_82A243C4 != 33 )
                dword_82B94534 = 7;
            else
                goto LABEL_41;                  // error 33 retry
        }
        goto LABEL_42;

    case 4:
        dword_82B94534 = 5;
        if ( dword_82B94530 == 2 )              // game start state check
        {
            sub_821D8398();
            if ( !sub_821E28A0(0) )
            {
                dword_82A243C4 = 31;
                dword_82B94534 = 7;
            }
        }
        goto LABEL_42;

    case 5:
        v7 = sub_821E3FA8(1);                   // async I/O (BeginLoad)
        if ( !v7 )
            dword_82B94534 = 6;
        else if ( v7 == 2 )
        {
            if ( dword_82A243C4 == 33 )
                v0 = 1;                         // retry
            else
                dword_82B94534 = 7;
        }
        // fall through to LABEL_42

    LABEL_42:
        if ( !v0 ) return 1;                    // in progress
        // cleanup and return 2 (done)
        sub_821E00C0(&dword_82B8E634, 2);
        dword_82B94534 = 0;
        dword_82B94530 = 0;
        sub_821DFF58();
        return 2;

    case 6:
        if ( byte_82B8E638 && dword_82B8E63C == 2 && dword_82B8E634 < 0xBB8 )
            goto LABEL_42;                      // 0xBB8 = 3000 -- timer check!
        sub_821DFF58();
        v9 = sub_82672970();                    // get scene object
        v10 = sub_824B8758(v9);
        // store scene u64
        return 0;                               // done

    case 7:
        sub_821E00C0(&dword_82B8E634, 2);
        if ( sub_821E4B20() )
            goto LABEL_41;
        goto LABEL_42;
    }
}
```

**Comparison with recomp (doc 02)**:
- State count: 8 (0-7), MATCHES
- State 1 writes: content byte=1, platformMode=2, done flag=0, scene state=0 -- all MATCH
- State 2: calls scene SM, checks error 33 for retry -- MATCHES
- State 3: SAVE signature check at bytes 12-15 (0x53,0x41,0x56,0x45) -- MATCHES exactly
- State 6: timer comparison against 3000 (0xBB8) -- MATCHES doc 07 description
- Error code pattern: 33 triggers retry, other codes go to state 7 -- MATCHES

### 4.3 sub_821E0E88 (= sub_8223DAA0, Readiness/Sign-in Check)

```c
int sub_821E0E88()
{
    dword_82B8EA90 = -1;                        // reset player index
    if ( !sub_82672970() )                      // get player manager
        return 0;
    v0 = sub_82672970();
    v1 = sub_8296C148(v0);                      // get sign-in index
    dword_82B8EA90 = v1;
    if ( v1 >= 0 && XamUserGetSigninState(v1) == eXamUserSigninState_NotSignedIn )
    {
        dword_82B8EA90 = -1;
        return 0;
    }
    return 1;
}
```

**Comparison with recomp (doc 03)**:
- The pseudocode reveals sub_8223DAA0 is a simple sign-in state check
- Returns 0 if no player manager or player is not signed in
- Returns 1 if a signed-in player exists
- In the recomp, this function is hooked to always return 1 (bypass XAM)

### 4.4 sub_821E0F08 (= sub_8223DB20, Controller Disconnect Check)

```c
int sub_821E0F08()
{
    v2 = 0;
    if ( !XNotifyGetNext(dword_82B8E630, 0xA, &v2, &v3) )
        return 0;                               // no notification
    if ( dword_82B8EA90 == -1 )
        return 0;                               // no player
    if ( sub_821E0E88() == 0 )                  // player still signed in?
        return 1;                               // DISCONNECTED (triggers error 33)
    return 0;
}
```

**Key insight**: This function checks XNotify event ID 0xA (controller
disconnect). If a disconnect notification fires AND the player is no longer
signed in, it returns 1. The recomp hooks this to always return 0 (no
disconnects on PC).

### 4.5 sub_821E3BA0 (= sub_82240B78, Platform Mode Change Check)

```c
int sub_821E3BA0()
{
    if ( !XNotifyGetNext(dword_82B8E630, 0xB, &v1, &v2)
        || !sub_821E0E88()
        || !byte_82B8E777
        || sub_821E3B30() )
        return 0;
    byte_82B8E776 = 1;
    byte_82A243BF = 1;
    return 1;                                   // triggers error 34
}
```

**Key insight**: Checks XNotify event ID 0xB (storage device change / platform
mode change). Returns 1 to trigger error 34 in the scene creation SM. Recomp
hooks this to always return 0.

### 4.6 sub_821E4730 (= sub_822422E0, Game Start / Episode Selection)

```c
int sub_821E4730(signed int a1)
{
    if ( a1 >= 0 || dword_82BEC640 )
    {
        if ( dword_82B94530 )                   // already started
            return 2;
        if ( !dword_82B9452C )                  // not yet initiated
        {
            sub_821E0550();                     // init
            byte_82B9451F = 0;                  // clear scene loaded flag
            if ( a1 >= 0 )
            {
                sub_821E35D0(a1, ...);          // select save slot
                sub_82206120();                 // load level
            }
            else
            {
                byte_82B9451F = 1;              // auto-load flag
                sub_8298FEF0(byte_82B8E650, "SG_HDG_AUTO", 0x10u);
                sub_822060E0();                 // auto-load
            }
            dword_82B9452C = 1;
            dword_82B94530 = 1;
        }
    }
    return 0;
}
```

**Comparison with recomp (doc 05)**:
- The pseudocode reveals two paths: manual save slot (a1 >= 0) or auto-load (a1 < 0)
- Auto-load writes "SG_HDG_AUTO" header and sets byte_82B9451F = 1
- Manual load calls sub_821E35D0 with the slot index
- Both paths set the state vars to 1 (initiated)
- The recomp hooks this to return specific values to skip the save system

### 4.7 sub_821E3FA8 (= sub_82240F80, Async Save I/O)

```c
int sub_821E3FA8(char a1)
{
    // a1=0: GetCreator (verify save file), a1=1: BeginLoad (load save data)
    if ( !dword_82B946B4 )  // state 0: initiate
    {
        if ( sub_821E0F08() ) goto error_33;
        if ( sub_821E3BA0() ) goto error_34;
        v4 = sub_827DCDA0(byte_830F54F8, dword_82B8EA90);
        if ( !sub_827DD598(byte_830F54F8, dword_82B8EA90, 1, v4, byte_82B8E760) )
        {
            nullsub_1("CGenericGameStorage::DoLoad - BeginGetCreator failed\n");
            dword_82A243C4 = 23;
            return 2;
        }
        dword_82B946B4 = 1;
    }
    // state 1: poll completion
    if ( sub_827DD5C8(byte_830F54F8, dword_82B8EA90, v8) )
    {
        dword_82B946B4 = 0;
        sub_827DD5F0(...);
        // check for errors: 24, 25, 26
        if ( a1 ) { /* read save data */ }
        else { /* read 16-byte header -> buffer at 82B8E640 */ }
        return 0;  // done
    }
    return 1;  // still in progress
}
```

**Key insight**: The async I/O function is a 2-state machine (initiate/poll).
It uses Xbox content management APIs (`sub_827DD598` = BeginGetCreator,
`sub_827DD5C8` = poll completion). The recomp handles this by completing
synchronously via rexcrt file hooks.

### 4.8 sub_821E0840 (= sub_82240B08, Save Slot Scanner)

```c
int sub_821E0840(...)
{
    if ( dword_82B8EA94 > 75 )       // max 75 save slots
    {
        dword_82A243C4 = 42;         // error 42: too many saves
        return 0;
    }
    // format string "SGTA4%02d" for save file names
    // iterate through existing slots, compare names
    // populate slot array at 82B8EA98 (77-entry * stride)
    // ...
}
```

**Key insight**: The save slot scanner builds a list of "SGTA4XX" named files
(up to 75 slots). Error code 42 means the slot limit was exceeded. The
pseudocode confirms the 75-slot limit and the "SGTA4" prefix pattern.

---

## 5. Discrepancies and New Insights

### 5.1 sub_8214B640 is NOT a Ready-Signal Resetter

**CRITICAL**: In the pseudocode, `sub_8214B640` at the SAME address is:

```c
int sub_8214B640()
{
    result = sub_8218BE28(28);      // malloc(28)
    if ( result )
    {
        result = sub_8214C150(result);  // CPedFactoryNY pool constructor
        dword_83137C5C = result;       // store global pool ptr
    }
    else
        dword_83137C5C = 0;
    return result;
}
```

This is a **CPedFactoryNY pool allocator** that creates a 28-byte control
struct, then allocates a 92160-byte pool (384 entries x 240 bytes) and a
240-byte index array. The function was identified in the recomp docs as the
"ready-signal resetter" -- this naming may be incorrect for the TU8 build, or
the function at 0x8214B640 was replaced/relocated by a title update.

**Either way**: the pseudocode version of `sub_8214B640` has nothing to do with
XAM readiness. It is a ped (pedestrian) factory pool initializer. This means
the recomp's function at 0x8214B640 in TU8 is a completely different function
that was patched into the same address by a title update.

### 5.2 USER_STATE Initial Value

The pseudocode declares `dword_82A22D04 = -1` (weak symbol, initialized to
-1). The recomp memory docs state USER_STATE starts at 0. This discrepancy
means either:
- The TU8 build changed the initial value from -1 to 0, or
- The recomp docs incorrectly identified the initial value

The pseudocode's `sub_821E6508` (auto-load SM) checks `if (dword_82A22D04 == -1)`
to trigger a 4-controller scan for signed-in users. If the value is NOT -1
(e.g., set to a specific player index), it skips the scan and assumes 1 player.

### 5.3 Class Names Revealed by Debug Strings

The pseudocode reveals RAGE engine class names through debug print statements:

| Function (Pseudocode) | Class.Method | Role |
|---|---|---|
| sub_821E5D00 | CGenericGameStorage::GenericLoad | Outer SM (save loading) |
| sub_821E4D60 | CGenericGameStorage (inner SM) | Scene creation SM |
| sub_821E23A0 | CGenericGameStorage::CalculateSizeOfBuffer | Save buffer allocation |
| sub_821E3FA8 | CGenericGameStorage::DoLoad | Async I/O (BeginGetCreator) |
| sub_821E0840 | CGenericGameStorage::ScanMemoryCard | Save slot scanner |
| sub_821E6508 | CGenericGameStorage::AutoLoadAtStartOfGame | Auto-load SM |
| sub_8214B640 | CPedFactoryNY (pool init) | Ped factory allocator |

These names confirm the entire state machine cluster is part of the
**CGenericGameStorage** class in the RAGE engine.

### 5.4 sub_82121E80 Main Loop is Simpler Than Expected

The pseudocode shows sub_82121E80 is a simple two-phase loop, not the complex
10-state machine described in doc 07/12. The two phases are:

1. **Phase 1** (`i=0`): Calls `sub_821E6508` (AutoLoadAtStartOfGame) each
   frame until it returns 1 (success -> enter phase 2) or 3 (multiplayer ->
   exit).

2. **Phase 2** (`i=1,2`): Calls `sub_821E47F8` once (setup), then calls
   `sub_821E5D00` (GenericLoad) each frame until it returns 0 (done) or 2
   (error).

The doc 07 description of "r29 state machine with states 0-9" may describe
a more complex version in TU8 that was refactored from this simpler loop.

### 5.5 Error Code Semantics Confirmed

The pseudocode confirms all error codes from doc 06:

| Code | Pseudocode Context | Doc 06 Name |
|---|---|---|
| 6 | State 13 default error | Normal error |
| 7 | State 6 BeginSave failure | Scene load error |
| 8 | State 8 dialog check fail | Save check error |
| 9 | State 8 unknown status | Save status error |
| 10 | sub_821E4500 scene dispatch fail | Scene dispatch error |
| 14 | State 9 BeginEnumerate fail | Enumeration error |
| 15 | State 10 dialog result=1 | Dialog error |
| 16 | State 10 dialog unknown | Dialog status error |
| 23 | sub_821E3FA8 BeginGetCreator fail | I/O init error |
| 24 | sub_821E3FA8 check fail 1 | I/O check error 1 |
| 25 | sub_821E3FA8 check fail 2 | I/O check error 2 |
| 26 | sub_821E3FA8 empty buffer | I/O empty error |
| 29 | SAVE signature missing | Invalid save file |
| 31 | State 4 sub_821E28A0 fail | Buffer init error |
| 33 | sub_821E0F08 controller disconnect | Controller error |
| 34 | sub_821E3BA0 platform change | Platform error |
| 42 | sub_821E0840 slot count > 75 | Too many saves |
| 50 | State 14 save load fail | Save load error |

### 5.6 Timer Value 3000 (0xBB8) Confirmed

In sub_821E5D00 state 6:
```c
if ( byte_82B8E638 && dword_82B8E63C == 2 && dword_82B8E634 < 0xBB8 )
    goto LABEL_42;  // keep waiting
```

This confirms the 3000-tick timer guard before finalizing the scene. After the
timer expires, the scene object is stored and the function returns 0 (done).

### 5.7 "SGTA4" Save File Naming Convention

The save slot scanner formats filenames as `"SGTA4%02d"` (e.g., SGTA400,
SGTA401, ..., SGTA474). The 75-slot limit is hardcoded. This is consistent
with the Xbox 360 content management system which stores save files in
per-title content directories.

---

## 6. Summary: Agreement Between Pseudocode and Recomp

| Aspect | Agreement? | Notes |
|---|---|---|
| State machine structure (15 states) | YES | Identical flow, same transitions |
| Outer SM structure (8 states) | YES | Same state count, same logic |
| Error code values | YES | All codes 6-50 match |
| SAVE signature check | YES | Bytes 12-15, 0x53/0x41/0x56/0x45 |
| platformMode switch | YES | `(val-3) <= 1u` pattern in state 4 |
| Timer threshold 3000 | YES | 0xBB8 in state 6 |
| sub_8214B640 role | **NO** | Pseudocode: CPedFactory pool alloc; Recomp docs: ready-signal reset |
| USER_STATE initial value | **DISAGREE** | Pseudocode: -1; Recomp docs: 0 |
| Main loop complexity | **SIMPLER** | Pseudocode: 2-phase loop; Docs: 10-state machine |
| XAM readiness system | NOT FOUND | sub_82254FE0, sub_8214C8C8 not in pseudocode |

The core game logic (CGenericGameStorage class) is structurally identical
between the pseudocode build and the TU8 recomp binary. The title update added
functions (like the XAM readiness system) and reorganized addresses, but the
state machine architecture is preserved.

---

*Generated 2026-03-27 from `/Users/Ozordi/Downloads/default (1).xex.c` (Hex-Rays
9.1.0, 30,600 functions) cross-referenced against docs 01-11 analysis of
`gta4_recomp.*.cpp` (38,268 recompiled functions, TU8/v8 build).*
