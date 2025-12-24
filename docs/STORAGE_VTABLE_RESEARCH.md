# Storage vtable[27] Research - Complete Analysis

## Executive Summary

The game uses a **validation token pattern** where `vtable[27]` must echo back a magic value passed by the caller. The current implementation returns file size instead of this validation code, causing the comparison to fail.

---

## Research Question 1: What value should be written to [r5] output pointer?

### Answer: File size or resource handle

**Evidence from PPC code:**

`sub_8249BE88` at line 37427-37428:
```cpp
// lwz r4,80(r1)              // Load value from [r1+80] (the output pointer)
ctx.r4.u64 = PPC_LOAD_U32(ctx.r1.u32 + 80);
// bl 0x8249bdc8             // Pass it as r4 to sub_8249BDC8
sub_8249BDC8(ctx, base);
```

**What sub_8249BDC8 does with it:**

`sub_8249BDC8` at line 37262-37287:
```cpp
// mr r29,r4                  // Save the output value
ctx.r29.u64 = ctx.r4.u64;

// Call sub_827EDB10 with r5=r29 (the output value)
mr r5,r29
sub_827EDB10(ctx, base);
```

The output value is used as a parameter for further file operations. Based on the context, this is likely:
- **File size** (for resource loading)
- **File handle** (for streaming)
- **Resource descriptor** (for lookup)

**Current implementation** (line 7291-7293 in `imports.cpp`):
```cpp
if (outputAddr != 0) {
    PPC_STORE_U32(outputAddr, static_cast<uint32_t>(fileSize));
}
```

This part is **CORRECT** - writing file size to the output pointer.

---

## Research Question 2: Is "7" always constant or does it vary?

### Answer: It varies per resource type/operation

**Evidence from grep search:**

Found multiple instances of `li r6,` with different values:
- `li r6,7` - Most common (fonts, textures, resources)
- `li r6,12` - Different resource type
- `li r6,13` - Different resource type  
- `li r6,14` - Different resource type
- `li r6,0` - Many instances (likely "no validation" operations)
- `li r6,1` - Some instances

**Pattern Analysis:**

In `sub_82205438` (line 89305-89329):
```cpp
// First attempt with r6=7
li r6,7
bl 0x8249be88
sub_8249BE88(ctx, base);

// If that fails, fallback with r6=7 again
li r6,7
bl 0x82205390
sub_82205390(ctx, base);
```

The value "7" appears to be:
- **Resource type identifier** or **operation code**
- **Path component count** (e.g., "platform:/textures/fonts" has components)
- **Validation token** specific to this operation

**Conclusion:** The validation code varies based on:
1. Resource type being accessed
2. Operation being performed
3. Caller's context

For the "platform:/textures/fonts" resource, the code is **always 7**.

---

## Research Question 3: What does sub_8249BDC8 do with the output value?

### Answer: Uses it for resource loading/streaming operations

**Complete trace of sub_8249BDC8:**

```
sub_8249BDC8(r3=context, r4=output_value):
    Line 37262-37263: Save output value to r29
        r29 = r4 (the value from vtable[27] output)
    
    Line 37272-37274: Call sub_8249BC80
        sub_8249BC80(r31)  // Some initialization
    
    Line 37277-37287: Call sub_827EDB10 with output value
        r3 = r28 (some context)
        r4 = stack buffer (r1+80)
        r5 = r29 (THE OUTPUT VALUE)
        sub_827EDB10(ctx, base)
    
    Line 37288-37293: Check return value
        if (result != 0) goto error path
    
    Line 37295-37307: Loop with sub_8249BAE0
        Calls sub_8249BAE0 up to 1000 times
        Uses the output value throughout
    
    Line 37321-37335: Retry sub_827EDB10
        Continues using r29 (output value)
    
    Line 37337-37348: Error path
        Calls sub_827ED798 cleanup
        Returns 1 (failure)
    
    Line 37349-37356: Success path
        Returns 0 (success)
```

**Purpose:** `sub_8249BDC8` appears to be a **resource loader** that:
1. Takes the file size/handle from vtable[27]
2. Performs streaming/loading operations
3. Retries up to 1000 times if needed
4. Returns success/failure

---

## Root Cause Analysis

### The Bug

**Current implementation** (line 7296 in `imports.cpp`):
```cpp
ctx.r3.s64 = static_cast<int64_t>(fileSize);  // Returns 35631 for fonts.xtd
```

**What the game expects** (line 37421-37424 in `ppc_recomp.26.cpp`):
```cpp
// cmpw cr6,r3,r31    // Compare vtable[27] return with r31 (which is 7)
ctx.cr6.compare<int32_t>(ctx.r3.s32, ctx.r31.s32, ctx.xer);
// bne cr6,0x8249bf04 // If NOT equal, goto failure
if (!ctx.cr6.eq) goto loc_8249BF04;
```

**Result:** `35631 != 7` → Comparison fails → Function returns 1 (error) → Game stuck

### The Pattern

This is a **validation token echo pattern**:
1. Caller sets `r6 = 7` (validation token)
2. `sub_8249BE88` saves it to `r31`
3. Calls `vtable[27]` to perform operation
4. **Expects vtable[27] to return the same value (7)**
5. Compares return with saved token
6. If match → success path
7. If no match → failure path

### Why This Pattern Exists

This is a **sanity check mechanism**:
- Ensures the vtable call actually executed
- Validates the operation completed correctly
- Prevents using stale/invalid data
- Common pattern in Xbox 360 SDK for async operations

---

## The Fix (Understanding Only - No Implementation)

### What Should Happen

**vtable[27] (StorageDevice_ReadFile) should:**

```cpp
// Pseudo-code (NOT actual implementation)
static void StorageDevice_ReadFile(PPCContext& ctx, uint8_t* base) {
    uint32_t devicePtr = ctx.r3.u32;
    uint32_t pathAddr = ctx.r4.u32;
    uint32_t outputAddr = ctx.r5.u32;
    
    // The validation token is NOT passed directly to vtable[27]
    // It's stored in r31 by sub_8249BE88 before the call
    // We need to determine it from context or always return a fixed value
    
    // Process the file...
    uint64_t fileSize = GetFileSize(path);
    
    // Write file size to output pointer (CORRECT - already doing this)
    if (outputAddr != 0) {
        PPC_STORE_U32(outputAddr, static_cast<uint32_t>(fileSize));
    }
    
    // Return the validation token (NOT file size)
    // For "platform:/textures/fonts", this should be 7
    ctx.r3.s64 = 7;  // Echo back the validation token
}
```

### The Challenge

**Problem:** vtable[27] doesn't receive the validation token as a parameter. It's stored in `r31` by the caller before the vtable call.

**Possible solutions:**
1. **Always return 7** - Works for this specific case but breaks other resources
2. **Parse the path** - Determine validation code from path structure
3. **Lookup table** - Map paths to validation codes
4. **Hook sub_8249BE88 instead** - Intercept at a higher level where we have access to r6

### Recommended Approach

Based on UnleashedRecomp patterns, the proper fix is:
- **Don't reimplement vtable[27]** - Let original PPC code handle the logic
- **Hook the file system layer below** - Provide proper file handles/sizes
- **Ensure VFS returns correct paths** - The real issue might be path resolution

---

## Additional Findings

### Other Resource Types

From grep results, other validation codes found:
- **12, 13, 14** - Different resource categories
- **0** - No validation (direct operations)
- **1** - Simple validation
- **72, 74, 77** - Specialized operations
- **752, 784** - Large buffer operations

### Path String Location

Line 89307-89308 in `sub_82205438`:
```cpp
// addi r31,r11,29352
ctx.r31.s64 = ctx.r11.s64 + 29352;
```

The string `"platform:/textures/fonts"` is stored at address `(r11 + 29352)` where `r11 = -32256 = 0x82200000`.

Actual address: `0x82200000 + 29352 = 0x82207298`

### Fallback Behavior

If `sub_8249BE88` fails (returns non-zero), `sub_82205438` calls `sub_82205390` as a fallback with the same parameters. This suggests there are multiple ways to load resources, and the vtable method is just one approach.

---

## Conclusion

The game is stuck because:
1. ✅ VFS path resolution works (file found)
2. ✅ File size is written to output pointer correctly
3. ❌ **vtable[27] returns file size (35631) instead of validation code (7)**
4. ❌ Comparison fails: `35631 != 7`
5. ❌ Function returns error, game cannot proceed

The proper fix requires understanding how to determine the correct validation code to return, which likely depends on the operation type or path being accessed.

---

*Research completed: December 22, 2025*
*Status: Root cause identified, no implementation performed*
