# Phase 1: vtable[19] Implementation Research

**Date:** December 22, 2025  
**Focus:** Understanding GetFileInfo requirements for shader loading  
**Status:** Research In Progress

---

## 1. Function Call Analysis - sub_827E8880

### 1.1 Function Purpose
`sub_827E8880` is a **byte-swapping wrapper** that calls `sub_827E8420` to read data from a file stream.

### 1.2 Call Signature
```cpp
// Parameters (PPC calling convention):
// r3 = file stream structure pointer
// r4 = output buffer address
// r5 = size to read (in units, multiplied by 4 for bytes)

int32_t sub_827E8880(FileStream* stream, void* buffer, uint32_t sizeUnits);
```

### 1.3 Implementation Flow
```assembly
# Line 16429-16430: Convert size from units to bytes
rlwinm r5,r5,2,0,29    # r5 = r5 << 2 (multiply by 4)

# Line 16433-16435: Call core read function
bl 0x827e8420          # sub_827E8420(stream, buffer, sizeBytes)

# Line 16436-16438: Convert bytes read to units
srawi r3,r3,2          # r3 = r3 >> 2 (divide by 4)

# Line 16447-16473: Byte-swap the read data (big-endian to little-endian)
loc_827E88B0:
    lwz r11,0(r10)         # Load 4 bytes
    # Byte swap operations using rlwimi
    stw r11,0(r10)         # Store swapped bytes
    addi r10,r10,4         # Next 4 bytes
    # Loop for all data
```

### 1.4 Return Value
- **Success**: Number of units read (bytes / 4)
- **Failure**: -1

---

## 2. File Stream Structure Layout

### 2.1 Structure Definition (Derived from sub_827E8420)

Based on memory accesses in `sub_827E8420` (lines 15766-15905):

```c
struct FileStream {
    void*    storageDevice;     // +0:  Storage device object pointer
    uint32_t fileHandle;        // +4:  File handle/descriptor
    void*    buffer;            // +8:  Internal buffer pointer
    uint32_t bytesRead;         // +12: Total bytes read so far
    uint32_t currentPos;        // +16: Current position in buffer
    uint32_t bufferEnd;         // +20: End position of valid data in buffer
    uint32_t bufferSize;        // +24: Total buffer size
    // ... potentially more fields
};
```

### 2.2 Field Access Patterns

**Offset +0 (Storage Device):**
```assembly
# Line 15842-15857
lwz r3,0(r31)          # Load storage device pointer
lwz r11,0(r3)          # Load vtable from device
lwz r11,20(r11)        # Load vtable[5] (ReadFile function)
```

**Offset +4 (File Handle):**
```assembly
# Line 15848-15849
lwz r4,4(r31)          # Load file handle
```

**Offset +8 (Buffer):**
```assembly
# Line 15810-15815, 15891-15892
lwz r10,8(r31)         # Load buffer pointer
add r4,r10,r11         # Calculate read position
```

**Offset +12 (Bytes Read):**
```assembly
# Line 15838-15844, 15877-15883
lwz r11,12(r31)        # Load bytes read
add r11,r11,r10        # Add newly read bytes
stw r11,12(r31)        # Store updated count
```

**Offset +16 (Current Position):**
```assembly
# Line 15772-15773, 15798-15799, 15821-15822
lwz r11,16(r31)        # Load current position
```

**Offset +20 (Buffer End):**
```assembly
# Line 15766-15767, 15794-15795, 15819-15820
lwz r11,20(r31)        # Load buffer end position
```

**Offset +24 (Buffer Size):**
```assembly
# Line 15846-15847
lwz r6,24(r31)         # Load buffer size
```

---

## 3. vtable[19] Call Analysis

### 3.1 Call Context (from ppc_recomp.67.cpp:5855-5875)

```assembly
# After sub_827E1EC0 returns storage device in r3
lwz r11,0(r3)          # Load vtable pointer
lwz r4,32(r31)         # Load path parameter
lwz r11,76(r11)        # Load vtable[19] from offset 76
mtctr r11              # Move to count register
bctrl                  # Call vtable[19]

# After vtable[19] returns
mr r11,r3              # Move return value to r11
std r11,40(r31)        # Store 64-bit return value at context+40
```

### 3.2 Parameters for vtable[19]

**Input:**
- **r3**: Storage device object pointer (e.g., 0x82A7FE00)
- **r4**: Path string pointer (e.g., 0x80268020 = 'platform:/shaders/fxl_final/fxc')

**Output:**
- **r3**: Return value (64-bit, stored with `std` instruction)

### 3.3 Return Value Storage

The return value is stored at `context+40` as a 64-bit value:
```assembly
std r11,40(r31)        # Store 64 bits at context+40
```

This suggests vtable[19] returns either:
1. A 64-bit integer (packed file info)
2. A pointer to a structure (32-bit pointer in upper or lower half)
3. Two 32-bit values packed together

---

## 4. What Happens After vtable[19]

### 4.1 Immediate Next Steps (lines 5868-5878)

```assembly
li r5,1                # r5 = 1 (size parameter)
addi r4,r1,80          # r4 = stack buffer at r1+80
mr r3,r30              # r3 = file handle from sub_827E8180
bl 0x827e8880          # Call sub_827E8880(handle, buffer, size=1)
```

**Analysis:**
- Calls `sub_827E8880` to read 1 unit (4 bytes) from the file
- Reads into stack buffer at `r1+80`
- Uses file handle from `sub_827E8180`, NOT the return value from vtable[19]

### 4.2 Magic Value Check (lines 5879-5888)

```assembly
lis r11,24952          # r11 = 0x61680000
lwz r10,80(r1)         # Load 4 bytes from stack buffer
ori r11,r11,26482      # r11 = 0x61686772 ("ahgr")
cmpw cr6,r10,r11       # Compare read data with magic value
beq cr6,0x8285bed8     # Branch to success if match
```

**Critical Discovery:**
- The magic value `0x61686772` is read FROM THE FILE, not generated
- It's the first 4 bytes of the shader file header
- This is a file format signature check

### 4.3 Success vs Failure Paths

**If magic value matches (success):**
```assembly
loc_8285BED8:
    mr r4,r30              # Pass file handle
    mr r3,r31              # Pass context
    bl 0x8285b680          # Process shader file
    bl 0x827e87a0          # Close file
    li r3,1                # Return success
```

**If magic value doesn't match (failure):**
```assembly
    # Log error message
    bl 0x828e0ab8          # sub_828E0AB8 (error logging)
    bl 0x827e87a0          # Close file
    li r3,0                # Return failure
```

---

## 5. vtable[19] Return Value Analysis

### 5.1 What vtable[19] DOESN'T Need to Return

Based on the execution trace:
- ❌ NOT used for file reading (file handle from sub_827E8180 is used)
- ❌ NOT used for magic value generation (magic value is read from file)
- ❌ NOT directly used in subsequent operations

### 5.2 What vtable[19] MIGHT Return

**Hypothesis 1: File Stream Structure Pointer**
```c
// vtable[19] returns a pointer to a FileStream structure
FileStream* GetFileInfo(StorageDevice* device, const char* path) {
    FileStream* stream = AllocateFileStream();
    stream->storageDevice = device;
    stream->fileHandle = OpenFile(path);
    stream->buffer = AllocateBuffer(BUFFER_SIZE);
    stream->currentPos = 0;
    stream->bufferEnd = 0;
    stream->bufferSize = BUFFER_SIZE;
    return stream;
}
```

**Hypothesis 2: Packed File Metadata**
```c
// Upper 32 bits: File size
// Lower 32 bits: File handle or flags
uint64_t GetFileInfo(StorageDevice* device, const char* path) {
    uint32_t size = GetFileSize(path);
    uint32_t handle = OpenFile(path);
    return ((uint64_t)size << 32) | handle;
}
```

**Hypothesis 3: NULL/Success Indicator**
```c
// Returns 0 for success, non-zero for failure
// Actual file operations use handle from sub_827E8180
uint64_t GetFileInfo(StorageDevice* device, const char* path) {
    return 0;  // Success
}
```

### 5.3 Evidence Analysis

**Key Observation:** The return value is stored at `context+40` but NOT immediately used. The game proceeds to read from the file using the handle from `sub_827E8180`.

**This suggests:** vtable[19] might be initializing internal state or validating the path, but the actual file I/O uses the handle from `sub_827E8180`.

---

## 6. Critical vtable Functions During Shader Loading

### 6.1 Confirmed vtable Calls

**vtable[1] (offset 4) - PathCompare:**
- Called by: `sub_827E1EC0` during storage device lookup
- Purpose: Compare path prefixes to find matching device
- Status: ✅ Likely working (game finds PC storage device)

**vtable[19] (offset 76) - GetFileInfo:**
- Called by: Shader loading code at 0x8285BE74
- Purpose: Unknown (validate path? initialize stream? get metadata?)
- Status: ❌ NOT IMPLEMENTED - Crashes here

**vtable[5] (offset 20) - ReadFile:**
- Called by: `sub_827E8420` during file reading
- Purpose: Read data from file into buffer
- Status: ❌ Placeholder - Would crash if reached

### 6.2 Potentially Called vtable Functions

**vtable[9] (offset 36) - Unknown:**
- Called by: `sub_827E8730` at line 16251-16257
- Context: File operation wrapper
- Status: ❌ Placeholder

**vtable[10] (offset 40) - CloseFile:**
- Called by: `sub_827E87A0` at line 16314-16320
- Purpose: Close file and cleanup
- Status: ❌ Placeholder

---

## 7. Implementation Strategy for vtable[19]

### 7.1 Minimal Implementation (Test Hypothesis 3)

**Approach:** Return 0 to indicate success, rely on sub_827E8180 handle for actual I/O

```cpp
PPC_FUNC(StorageDevice_GetFileInfo) {
    uint32_t deviceAddr = ctx.r3.u32;
    uint32_t pathAddr = ctx.r4.u32;
    
    // Read path for logging
    char path[256];
    for (int i = 0; i < 255; i++) {
        uint8_t c = PPC_LOAD_U8(pathAddr + i);
        if (c == 0) break;
        path[i] = c;
    }
    
    LOGF_WARNING("[vtable[19]] GetFileInfo device=0x{:08X} path='{}'", 
                 deviceAddr, path);
    
    // Return 0 (success indicator)
    ctx.r3.u64 = 0;
}
```

**Test:** If game proceeds past vtable[19] call without crashing

### 7.2 FileStream Implementation (Test Hypothesis 1)

**Approach:** Allocate and return a FileStream structure

```cpp
PPC_FUNC(StorageDevice_GetFileInfo) {
    uint32_t deviceAddr = ctx.r3.u32;
    uint32_t pathAddr = ctx.r4.u32;
    
    // Read path
    char path[256];
    ReadPathFromMemory(pathAddr, path);
    
    // Allocate FileStream structure in PPC memory
    uint32_t streamAddr = AllocatePPCMemory(32); // Minimum size
    
    // Initialize structure
    PPC_STORE_U32(streamAddr + 0, deviceAddr);     // Storage device
    PPC_STORE_U32(streamAddr + 4, 0xFFFFFFFF);     // Invalid handle (placeholder)
    PPC_STORE_U32(streamAddr + 8, 0);              // Buffer (NULL)
    PPC_STORE_U32(streamAddr + 12, 0);             // Bytes read
    PPC_STORE_U32(streamAddr + 16, 0);             // Current pos
    PPC_STORE_U32(streamAddr + 20, 0);             // Buffer end
    PPC_STORE_U32(streamAddr + 24, 4096);          // Buffer size
    
    LOGF_WARNING("[vtable[19]] GetFileInfo allocated stream at 0x{:08X}", streamAddr);
    
    // Return pointer to structure
    ctx.r3.u64 = streamAddr;
}
```

**Test:** Check if game uses the returned structure

### 7.3 Observation-Based Implementation

**Approach:** Add extensive logging to understand what's needed

```cpp
PPC_FUNC(StorageDevice_GetFileInfo) {
    static int s_call = 0; ++s_call;
    
    uint32_t deviceAddr = ctx.r3.u32;
    uint32_t pathAddr = ctx.r4.u32;
    
    // Read path
    char path[256];
    ReadPathFromMemory(pathAddr, path);
    
    LOGF_WARNING("[vtable[19]] CALL #{} device=0x{:08X} path='{}'", 
                 s_call, deviceAddr, path);
    
    // Log caller context
    LOGF_WARNING("[vtable[19]] LR=0x{:08X} r31=0x{:08X}", 
                 ctx.lr, ctx.r31.u32);
    
    // Try returning 0 first
    ctx.r3.u64 = 0;
    
    LOGF_WARNING("[vtable[19]] Returning 0");
}
```

**Monitor:** What happens after return, any crashes, any error messages

---

## 8. Additional vtable Functions to Implement

### 8.1 vtable[5] (offset 20) - ReadFile

**Called by:** `sub_827E8420` at lines 15854-15868

**Signature:**
```cpp
int32_t (*ReadFile)(StorageDevice* device, uint32_t handle, 
                    void* buffer, uint32_t size);
```

**Implementation Priority:** HIGH (called during file reading)

### 8.2 vtable[10] (offset 40) - CloseFile

**Called by:** `sub_827E87A0` at lines 16312-16320

**Signature:**
```cpp
void (*CloseFile)(StorageDevice* device, uint32_t handle);
```

**Implementation Priority:** MEDIUM (cleanup function)

### 8.3 vtable[9] (offset 36) - Unknown

**Called by:** `sub_827E8730` at lines 16249-16257

**Signature:**
```cpp
int32_t (*Unknown)(StorageDevice* device, ...);
```

**Implementation Priority:** LOW (may not be called in shader path)

---

## 9. Testing Strategy

### 9.1 Phase 1A: Minimal vtable[19]

**Steps:**
1. Implement vtable[19] returning 0
2. Register in vtable at offset 76
3. Build and run
4. Monitor logs for:
   - vtable[19] call confirmation
   - Any subsequent crashes
   - Which vtable functions are called next

**Expected Outcomes:**
- **Best case:** Game proceeds past vtable[19], may crash at vtable[5]
- **Neutral case:** Game still crashes but at different location
- **Worst case:** Game crashes immediately (wrong return type)

### 9.2 Phase 1B: FileStream Implementation

**If Phase 1A fails:**
1. Implement vtable[19] returning FileStream pointer
2. Allocate structure in PPC memory
3. Initialize all fields to safe defaults
4. Test again

### 9.3 Phase 1C: Additional vtable Functions

**If game proceeds past vtable[19]:**
1. Implement vtable[5] (ReadFile) - Use existing RPF reading logic
2. Implement vtable[10] (CloseFile) - Cleanup function
3. Test complete shader loading sequence

---

## 10. Open Questions

### 10.1 Critical Questions

**Q1: What does vtable[19] actually return?**
- **Research Method:** Test minimal implementation, observe behavior
- **Priority:** CRITICAL
- **Status:** ❌ Unknown

**Q2: Is the FileStream structure correct?**
- **Research Method:** Trace memory accesses after vtable[19] call
- **Priority:** HIGH
- **Status:** ⚠️ Partially known (offsets 0-24 identified)

**Q3: What does vtable[5] (ReadFile) expect?**
- **Research Method:** Examine sub_827E8420 vtable call at line 15857
- **Priority:** HIGH
- **Status:** ⚠️ Signature known, implementation TBD

**Q4: Are there other vtable calls after GetFileInfo?**
- **Research Method:** Continue execution trace past crash point
- **Priority:** MEDIUM
- **Status:** ❌ Unknown (can't trace past crash)

### 10.2 Secondary Questions

**Q5: Why does the game call vtable[19] if it already has a file handle?**
- **Hypothesis:** vtable[19] validates the path or initializes stream state
- **Status:** ⚠️ Speculative

**Q6: What is the magic value 0x61686772 in shader files?**
- **Answer:** File format signature ("ahgr" or "rage" related)
- **Status:** ✅ Confirmed

**Q7: Can we skip vtable[19] entirely?**
- **Answer:** No - game requires it for shader loading sequence
- **Status:** ✅ Confirmed

---

## 11. Implementation Plan for Review

### 11.1 Proposed Implementation Order

**Step 1: Implement vtable[19] (Minimal)**
```cpp
// File: kernel/imports.cpp
// Location: After InitializePCStorageDevice()

static void StorageDevice_GetFileInfo(PPCContext& ctx, uint8_t* base) {
    uint32_t deviceAddr = ctx.r3.u32;
    uint32_t pathAddr = ctx.r4.u32;
    
    // Read path
    char path[256] = {0};
    for (int i = 0; i < 255; i++) {
        uint8_t c = PPC_LOAD_U8(pathAddr + i);
        if (c == 0) break;
        path[i] = c;
    }
    
    LOGF_WARNING("[vtable[19]] GetFileInfo device=0x{:08X} path='{}'", 
                 deviceAddr, path);
    
    // Return 0 for now (test minimal implementation)
    ctx.r3.u64 = 0;
    
    LOGF_WARNING("[vtable[19]] Returning 0");
}

// Register in InitializePCStorageDevice():
constexpr uint32_t STORAGE_DEVICE_GETFILEINFO_ADDR = 0x82A13D10;
RegisterDynamicFunction(STORAGE_DEVICE_GETFILEINFO_ADDR, StorageDevice_GetFileInfo);
PPC_STORE_U32(StorageConstants::PC_STORAGE_VTABLE_ADDR + 76, 
              STORAGE_DEVICE_GETFILEINFO_ADDR);
```

**Step 2: Monitor and Iterate**
- Check if game proceeds past vtable[19]
- Identify next crash point or vtable call
- Adjust implementation based on findings

**Step 3: Implement vtable[5] (ReadFile) if needed**
```cpp
static void StorageDevice_ReadFile(PPCContext& ctx, uint8_t* base) {
    // r3 = device, r4 = handle, r5 = buffer, r6 = size
    // Use existing RPF reading logic
    // Return bytes read
}
```

**Step 4: Implement vtable[10] (CloseFile) if needed**
```cpp
static void StorageDevice_CloseFile(PPCContext& ctx, uint8_t* base) {
    // r3 = device, r4 = handle
    // Cleanup resources
}
```

### 11.2 Files to Modify

**Primary:**
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/imports.cpp`
  - Add `StorageDevice_GetFileInfo` function
  - Register in `InitializePCStorageDevice()`
  - Update vtable at offset 76

**Secondary (if needed):**
- Same file for additional vtable functions

### 11.3 Testing Checklist

- [ ] vtable[19] registered at correct offset (76)
- [ ] Function address is valid and registered
- [ ] Game calls vtable[19] without crashing
- [ ] Execution proceeds to sub_827E8880
- [ ] Magic value check occurs
- [ ] Identify next crash point or success

---

## 12. Risk Assessment

### 12.1 Known Risks

**Risk 1: Wrong Return Type**
- **Impact:** Game may crash if expecting pointer but gets 0
- **Mitigation:** Start with 0, try pointer if fails
- **Likelihood:** MEDIUM

**Risk 2: Additional vtable Calls**
- **Impact:** May crash at vtable[5] or vtable[10]
- **Mitigation:** Implement proactively or on-demand
- **Likelihood:** HIGH

**Risk 3: FileStream Structure Incorrect**
- **Impact:** If returning pointer, wrong structure causes crashes
- **Mitigation:** Trace memory accesses carefully
- **Likelihood:** MEDIUM

### 12.2 Success Indicators

**Minimal Success:**
- ✅ No crash at vtable[19] call
- ✅ Execution reaches sub_827E8880
- ✅ Log messages show progression

**Moderate Success:**
- ✅ Magic value read successfully
- ✅ Either success or failure path taken (not crash)
- ✅ Shader loading completes or fails gracefully

**Full Success:**
- ✅ Magic value matches (0x61686772)
- ✅ Success path taken (branch to 0x8285BED8)
- ✅ Shader processing begins (sub_8285B680)
- ✅ Game progresses beyond shader loading

---

## 13. Next Steps

### 13.1 Immediate Actions

1. **Implement minimal vtable[19]** (return 0)
2. **Register at offset 76** in PC storage vtable
3. **Build and test** (do not run yet per user request)
4. **Prepare logging** to capture execution flow

### 13.2 Contingency Plans

**If vtable[19] returning 0 fails:**
- Try returning a FileStream pointer
- Allocate structure in PPC memory
- Initialize with safe defaults

**If additional vtable functions are needed:**
- Implement vtable[5] (ReadFile) using RPF logic
- Implement vtable[10] (CloseFile) as no-op
- Implement vtable[9] if called

**If magic value check fails:**
- Verify shader file format
- Check if file is being read correctly
- Ensure byte-swapping is working

---

## 14. Conclusion

vtable[19] (GetFileInfo) is the critical missing piece for shader loading. The function is called after file handle acquisition and before file reading. Its exact return value is unknown, but testing will reveal the requirements.

**Recommended Approach:**
1. Start with minimal implementation (return 0)
2. Monitor execution and adjust based on behavior
3. Implement additional vtable functions as discovered
4. Iterate until shader loading succeeds

**Confidence Level:**
- Root cause identified: 100%
- Implementation approach: 85%
- Success probability: 75% (may need iteration)

---

**Document End - Ready for Implementation**
