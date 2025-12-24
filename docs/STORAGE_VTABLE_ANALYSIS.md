# Storage vtable[27] Deep Analysis

## Executive Summary

Traced execution from sub_82205438 down to vtable[27] call. Game is stuck because execution stops after sub_827DB2A8 returns - never reaches sub_8249BE88. The vtable[27] validation token pattern was fully documented but is not the root blocker.

## Complete Execution Trace

### Level 1: sub_821DE390 (Resource Init)
```
Location: ppc_recomp.5.cpp:55140
Status: STUCK - enters but never exits

Calls:
  Line 55158-55160: sub_82204770 ✓ (completes)
  Line 55165-55167: sub_82124EF0 ✓ (completes)  
  Line 55174-55176: sub_82205438 ✗ (never returns)
```

### Level 2: sub_82205438 (Resource Lookup)
```
Location: ppc_recomp.6.cpp:89256
Status: STUCK - enters but never exits

Flow:
  Line 89268-89299: Setup and data structure access
  Line 89300-89302: sub_827DB2A8() ✓ (completes)
  Line 89303-89319: Should call sub_8249BE88 ✗ (never reached)
  
Evidence: Logs show "sub_827DB2A8 EXIT" but execution stops
```

### Level 3: sub_827DB2A8 (Unknown Function)
```
Location: ppc_recomp.62.cpp:26810
Status: COMPLETES (logs show ENTER/EXIT)

Internal calls:
  Line 26846-26848: sub_827F1428 (if condition met)
  Line 26863-26865: sub_827DAC90 (if condition met)
  Line 26883-26885: sub_827F1478 ← LAST CALL BEFORE RETURN
  
Returns to caller at line 26890-26891
```

## The Validation Token Pattern

### Discovery
Through PPC code analysis, found that sub_8249BE88 uses validation tokens:

```
Caller (sub_82205438):
  li r6,7              // Set validation token
  bl sub_8249BE88      // Call with token in r6
  
sub_8249BE88:
  mr r31,r6            // Save token (7) to r31
  bl sub_827EDED0      // Format path
  bl sub_827E1EC0      // Get storage device
  [vtable call]        // Call device->vtable[27]
  cmpw cr6,r3,r31      // Compare return with saved token (7)
  bne cr6,failure      // If not equal, fail
  bl sub_8249BDC8      // Success continuation
  return 0             // Success
```

### Validation Codes Found
- 7: Textures, fonts, common resources
- 12, 13, 14: Different resource categories
- 0: No validation
- 1: Simple validation
- 72, 74, 77: Specialized operations
- 752, 784: Large buffer operations

### The Challenge
vtable[27] must return the validation code but doesn't receive it as a parameter. It's stored in r31 by the caller before the vtable call.

## Root Blocker

**Game is NOT stuck on vtable[27] validation** - it never reaches that code.

**Actual blocker:** Execution stops after sub_827DB2A8 returns at line 89302. Should continue to line 89303 but doesn't.

**Possible causes:**
1. Thread blocked waiting for event/semaphore
2. sub_827F1478 (last call in sub_827DB2A8) blocks
3. Some initialization incomplete
4. Context switch to another thread that's blocked

**Evidence:**
- sub_827DB2A8 logs "EXIT" 
- No crash or error
- Other threads continue looping
- Main thread appears frozen
- vtable[27] never called (no VTABLE27 log despite adding logging)

## UnleashedRecomp Lesson

They avoid this complexity by hooking at file API level:
```cpp
GUEST_FUNCTION_HOOK(sub_82BD4668, XCreateFileA);
GUEST_FUNCTION_HOOK(sub_82BD4478, XReadFile);
```

Not at storage device vtable level. This bypasses:
- Validation token complexity
- vtable registration
- Platform-specific storage logic

## Next Steps (Research Only)

1. Trace sub_827F1478 - does it block?
2. Check what sub_827DB2A8 initializes
3. Identify missing initialization preventing forward progress
4. Find GTA IV's file API functions to hook properly

## Status

Research complete. No patches applied. Root cause: execution blocked after sub_827DB2A8, not vtable validation.
