# True Root Cause Research - Stop the Bypasses
## Systematic Analysis of PAC Authentication Failures

**Date**: December 23, 2025  
**Status**: Stopping reactive bypasses, conducting proper research  
**Critical Realization**: Adding bypasses is treating symptoms, not root cause

---

## The Pattern of Failure

### Crash Sequence
1. **First crash**: `sub_821DB1E0` at `0xd73f0b51f2e09f11` → Added bypass
2. **Second crash**: `sub_8221B7A0` at `0xb8132880b0132880` → About to add bypass
3. **Next crash**: Likely another function with another garbage pointer

### The Problem
**This is whack-a-mole**. Each bypass reveals another crash. The crashes are all PAC authentication failures with garbage function pointers. This indicates a **systemic issue**, not isolated failures.

---

## What the Crashes Tell Us

### Crash Address Analysis

**Crash 1**: `0xd73f0b51f2e09f11`
- Pattern: Random-looking garbage
- Location: `sub_821DB1E0` trying to call through vtable

**Crash 2**: `0xb8132880b0132880`  
- Pattern: **b8 13 28 80 b0 13 28 80** (repeating bytes!)
- Location: `sub_8221B7A0` trying to call `sub_82990830` (memcpy)
- **Key insight**: This is NOT random - it's a specific pattern

### The Pattern Significance

`0xb8132880b0132880` breaks down as:
- `b8 13 28 80` repeated twice
- This looks like **uninitialized memory with a fill pattern**
- OR **corrupted pointer where bits got duplicated**
- OR **reading from wrong memory location that has this pattern**

---

## What sub_8221B7A0 Actually Does

From `ppc_recomp.7.cpp:54135-54201`:

```cpp
// String duplication function (like strdup)
1. Check if input string pointer is NULL (line 54152-54155)
2. Scan string to find length (lines 54160-54168)
3. Allocate memory for copy (line 54179: bl sub_8218BE28)
4. Copy string to new memory (line 54186: bl sub_82990830) ← CRASH HERE
5. Return pointer to duplicated string
```

**The crash**: Trying to call `sub_82990830` (memcpy), but getting garbage function pointer `0xb8132880b0132880`

---

## Why Is the Function Pointer Garbage?

### Hypothesis 1: Function Not Mapped
❌ **Disproven**: `sub_82990830` exists in `ppc_recomp.78.cpp:35429` and is in function mapping table

### Hypothesis 2: Corrupted Function Table
**Possible**: The function lookup table might be corrupted or not initialized

### Hypothesis 3: Wrong Call Mechanism
**Possible**: The recompiler might be using wrong calling convention

### Hypothesis 4: Memory Corruption
**Most Likely**: Something earlier in initialization corrupted the code region or function table

---

## The Real Question

**Why are function pointers becoming garbage?**

This is NOT about:
- ❌ Missing config files
- ❌ Uninitialized vtables  
- ❌ Individual function failures

This IS about:
- ✅ **Corrupted function lookup mechanism**
- ✅ **Corrupted code memory region**
- ✅ **Wrong memory initialization**
- ✅ **Recompiler issue**

---

## What Needs to Be Researched

### 1. How are PPC function calls translated?

When PPC code does `bl 0x82990830`, how does the recompiler:
1. Look up the host function pointer?
2. Validate the address?
3. Make the actual call?

### 2. Where is the function mapping table?

The mapping table at runtime:
- Where is it stored in memory?
- How is it initialized?
- Could it be corrupted?

### 3. What is the pattern 0xb8132880b0132880?

This specific pattern must mean something:
- Is it a fill pattern from allocator?
- Is it reading from wrong memory offset?
- Is it a corrupted pointer calculation?

### 4. Why does it work initially then fail?

The game initializes successfully through:
- `sub_82120000` ✅
- `sub_82120EE8` ✅  
- `sub_821DE390` ✅ (after bypassing sub_821DB1E0)
- `sub_82120FB8` starts ✅
- `sub_8221D880` (subsystem #3) ❌ CRASHES

What changes between early init and subsystem #3 that causes function pointers to become garbage?

---

## Hypothesis: Code Region Corruption

### Theory

The PPC code region (0x82000000-0x83200000) might be getting corrupted by:
1. Stream pool initialization writing to wrong address
2. Memory allocator overwriting code region
3. Vtable initialization stomping on code
4. Buffer overflow from earlier operations

### Evidence

The crash pattern `0xb8132880b0132880` suggests:
- Reading from address that has pattern fill
- The pattern `0x80` repeating is suspicious
- `0x80` is common in:
  - Uninitialized heap (some allocators use 0x80808080)
  - Stack canaries
  - Memory barriers

### Test

Check if addresses 0x82A80A00-0x82A80A24 (our null stream vtable region) overlap with or corrupt the code region where function pointers are stored.

---

## Action Plan (Research, Not Bypasses)

### Step 1: Understand Function Call Mechanism
- Read recompiler code for how `bl` instructions are translated
- Find where function pointers are stored
- Understand lookup mechanism

### Step 2: Trace Memory Writes
- Log all writes to code region (0x82000000-0x83200000)
- Identify if our defensive initialization is corrupting anything
- Check if stream pool (0x82000000-0x82020000) overlaps with critical structures

### Step 3: Validate Addresses
- Check if 0x82A80A00 (null stream vtable) is in valid range
- Check if 0x82A13D40 (no-op function) is in valid range
- Verify these don't overlap with function mapping table

### Step 4: Find the Pattern Source
- Search codebase for 0x80808080 or similar patterns
- Identify what allocator or initializer uses this pattern
- Trace where 0xb8132880b0132880 could come from

---

## Critical Realization

**The defensive vtable initialization at 0x82A80A00-0x82A80A24 might be corrupting something critical.**

From earlier code:
```cpp
// Initialize the vtable at 0x82A80A00
for (int i = 0; i < 128; i += 4) {
    PPC_STORE_U32(0x82A80A00 + i, 0x82A13D40);  // Writing to code region!
}
```

**Question**: Is 0x82A80A00 in the code region? If so, we're overwriting actual game code with our vtable initialization!

---

## Next Steps (NO BYPASSES)

1. **Verify address ranges**: Check if our defensive initialization overlaps with code
2. **Remove defensive init temporarily**: Test if crashes stop
3. **Find correct address range**: Identify safe memory region for vtables
4. **Fix at source**: Initialize vtables in correct location, not in code region

---

**End of Research - Awaiting Investigation Results**
