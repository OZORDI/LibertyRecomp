# Resource Array Corruption - Root Cause Analysis

## Problem Statement

The resource array at `Global[0x8214B4B4]` has a corrupted count value of 32010 (0x7D0A) instead of the expected 0, causing `sub_82273988` to loop excessively and never complete.

## Execution Trace

### 1. Initialization in sub_82120EE8 (Core Engine Init)

```assembly
# Line 2414-2435
lwz r11,29876(r31)      # Check if resource manager exists
cmplwi cr6,r11,0
bne cr6,loc_82120F30    # Skip if already initialized

# Allocate resource manager (944 bytes)
li r3,944
bl sub_8218BE28         # Allocate
mr r30,r3               # r30 = allocated pointer

# Initialize the structure
bl sub_821207B0         # Sets count=0, base=0

# Store pointer globally
stw r30,29876(r31)      # Global[0x8214B4B4] = r30
```

**After this:** Resource manager structure at r30 has count=0 (correct)

### 2. What sub_821207B0 Does

```assembly
# Line 1334-1349
li r10,0                # r10 = 0
stw r10,0(r3)           # structure[0] = 0 (base pointer)
sth r10,4(r3)           # structure[4] = 0 (count - 16-bit)
sth r10,6(r3)           # structure[6] = 0
```

**Result:** Count field at offset +4 is explicitly set to 0

### 3. Memory Address Analysis

**Resource Manager Pointer:** Stored at `0x8214B4B4` (Global[0x82144000 + 29876])
**Allocated Structure:** At address returned by `sub_8218BE28(944)`

**Structure Layout:**
```
+0:   Base pointer (32-bit)
+4:   Count (16-bit)
+6:   Another count (16-bit)
+8-943: String data and other fields
```

### 4. The Corruption: 0x7D0A (32010)

**Hypothesis 1: Memory Overlap**
- 0x7D0A = 32010 decimal
- 0x7D00 = 32000 decimal (close to common.rpf TOC offset 0x800 = 2048)
- Could be reading wrong memory location

**Hypothesis 2: Byte Order Issue**
- 0x7D0A big-endian = 0x0A7D little-endian = 2685
- Still too high, but different value

**Hypothesis 3: Uninitialized Memory**
- The allocated 944 bytes might not be zeroed
- Count field reads garbage data

**Hypothesis 4: Pointer Confusion**
- sub_82273988 might be reading from wrong address
- Instead of reading from the allocated structure, reading from somewhere else

## Investigation: Where Does sub_82273988 Read the Count?

```assembly
# Line 26133-26135 in sub_82273988
lis r22,-31980          # r22 = 0x82144000
lwz r8,29876(r22)       # r8 = Global[0x8214B4B4] (resource manager pointer)
lhz r9,4(r8)            # r9 = [r8+4] (count)
```

This reads:
1. Global pointer at 0x8214B4B4
2. Dereferences to get structure pointer
3. Reads 16-bit value at structure+4

**The issue:** If the pointer at 0x8214B4B4 is wrong, or if the structure at that pointer is corrupted, we get wrong count.

## Root Cause Determination

Let me check what `sub_8218BE28` does - does it zero the allocated memory?

Looking at the allocation pattern, `sub_8218BE28` is a memory allocator. If it doesn't zero memory, the count field would contain garbage.

**Most Likely Root Cause:** The allocator doesn't zero memory, and the count field at offset +4 contains leftover data (0x7D0A) from a previous allocation.

## Solution Brainstorming

### Solution 1: Ensure sub_821207B0 Properly Zeros the Count
**Approach:** Hook `sub_821207B0` to explicitly zero the count field after initialization
**Pros:** Fixes at source, prevents corruption
**Cons:** Might not catch all cases if count is modified elsewhere

### Solution 2: Hook sub_8218BE28 to Zero Allocated Memory
**Approach:** Ensure all allocations are zeroed
**Pros:** Prevents all uninitialized memory issues
**Cons:** Performance overhead, might break assumptions about non-zeroed memory

### Solution 3: Fix in sub_82273988 Before Reading
**Approach:** Validate and fix count before loops (current approach)
**Pros:** Catches corruption regardless of source
**Cons:** Doesn't fix root cause, bandaid solution

### Solution 4: Initialize Resource Array Properly in sub_82120EE8
**Approach:** After calling sub_821207B0, explicitly verify count=0
**Pros:** Ensures correct state before any other code runs
**Cons:** Still doesn't prevent later corruption

## Recommended Solution: Hybrid Approach

**Step 1:** Hook `sub_821207B0` to ensure count is zeroed
**Step 2:** Add validation in `sub_82273988` as safety net
**Step 3:** Ensure sentinel values are present

This provides defense in depth.

## Implementation Plan

Will implement after confirming this analysis is correct.
