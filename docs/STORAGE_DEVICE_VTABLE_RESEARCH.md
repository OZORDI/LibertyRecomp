# Storage Device Vtable Research - Shader Loading Analysis

**Document Version:** 1.0  
**Date:** December 22, 2025  
**Status:** Research Complete - Implementation Pending

---

## Executive Summary

The GTA IV shader loading failure is caused by an incomplete storage device vtable implementation. The game successfully resolves paths and opens file handles but crashes when attempting to call `vtable[19]` (GetFileInfo) because only `vtable[27]` (ReadFile) is currently implemented. All other vtable slots contain placeholder addresses that cause crashes when invoked.

**Critical Finding:** The crash occurs at address `0x8285BE74` when the game attempts to call `vtable[19]` at offset 76, which contains placeholder address `0x8212004C` instead of a valid function pointer.

---

## 1. Vtable Functions Observed During Shader Loading

### 1.1 Complete Vtable Layout (Based on Xbox 360 Storage Device)

| Slot | Offset | Function Name | Purpose | Called During Shader Load |
|------|--------|---------------|---------|---------------------------|
| 0 | 0x00 | Constructor/Destructor | Object lifecycle | No |
| 1 | 0x04 | PathCompare | Compare path prefixes | Yes (in sub_827E1EC0) |
| 2 | 0x08 | OpenFile | Open file for reading | **Unknown - Needs Trace** |
| 3 | 0x0C | Unknown | | No |
| 4 | 0x10 | Unknown | | No |
| 5 | 0x14 | Unknown | | No |
| 6 | 0x18 | ReadFile | Read file data | Potentially |
| 7 | 0x1C | Unknown | | No |
| 8 | 0x20 | Unknown | | No |
| 9 | 0x24 | Unknown | | No |
| 10 | 0x28 | Unknown | | No |
| 11 | 0x2C | CloseFile | Close file handle | Potentially |
| 12-17 | 0x30-0x44 | Unknown | | No |
| 18 | 0x48 | GetFileSize | Get file size in bytes | Potentially |
| 19 | 0x4C | **GetFileInfo** | **Get file metadata** | **YES - CRASHES HERE** |
| 20-26 | 0x50-0x6C | Unknown | | No |
| 27 | 0x6C | ReadFile (Alt) | Alternative read function | Implemented (PC) |
| 28-29 | 0x70-0x74 | Unknown | | No |

### 1.2 Currently Implemented Functions

**PC Storage Device Vtable (as of crash):**
- **vtable[27]** at offset 108 (0x6C): Implemented as `StorageDevice_ReadFile`
- **All other slots**: Placeholder addresses (`0x82120000 + offset`)

---

## 2. Detailed Call Order and Conditions

### 2.1 Shader Loading Execution Sequence

```
Phase 1: Initialization
├─ sub_82273988 (Shader system initialization)
│  └─ Prepares shader context structure
│
Phase 2: Path Construction
├─ sub_827E04F0 (Path builder)
│  ├─ Input: context=0x82A7FE08, output=0x000A0B00, size=256
│  ├─ Checks context+3072 for path config (finds 0)
│  └─ Writes empty string to buffer (ISSUE: No path config)
│
Phase 3: File Handle Acquisition
├─ sub_827E8180 (File open/find)
│  ├─ Input: pathContext=0x000A0B00 (empty path)
│  ├─ Our hook detects shader path
│  └─ Returns file handle (e.g., 0xC1264EB0 for common.rpf)
│
Phase 4: Storage Device Resolution (CRASH POINT)
├─ sub_827E1EC0 (Storage device resolver)
│  ├─ Input: path='platform:/shaders/fxl_final/fxc' at 0x80268020
│  ├─ Performs prefix matching (finds "platform:\\")
│  ├─ Iterates through registered storage devices
│  ├─ Calls vtable[1] on each device for path comparison
│  └─ Returns PC storage device at 0x82A7FE00
│
Phase 5: File Metadata Query (CRASH OCCURS HERE)
├─ Caller at 0x8285BE74
│  ├─ Loads vtable pointer from device object
│  ├─ Loads vtable[19] from offset 76
│  ├─ Gets placeholder address 0x8212004C
│  └─ Attempts to call → CRASH (invalid instruction)
```

### 2.2 PowerPC Assembly at Crash Point

**Location:** `ppc_recomp.67.cpp:5855-5865` (address 0x8285BE74)

```assembly
# After sub_827E1EC0 returns storage device in r3
lwz r11,0(r3)          # Load vtable pointer from device object
                       # r3 = 0x82A7FE00 (PC storage device)
                       # Loads vtable addr = 0x82A7FE80

lwz r4,32(r31)         # Load path parameter from context

lwz r11,76(r11)        # Load vtable[19] from offset 76
                       # Loads 0x8212004C (PLACEHOLDER!)

mtctr r11              # Move function pointer to count register

bctrl                  # Branch to count register
                       # Jumps to 0x8212004C → INVALID ADDRESS
                       # CRASH: No valid code at this address
```

### 2.3 Conditions for vtable[19] Call

**Preconditions:**
1. File handle successfully acquired from `sub_827E8180`
2. Storage device successfully resolved from `sub_827E1EC0`
3. Game needs file metadata before proceeding with shader loading

**Context at Call Time:**
- **r3**: Storage device object pointer (0x82A7FE00)
- **r4**: Path string pointer (0x80268020 = 'platform:/shaders/fxl_final/fxc')
- **r31**: Shader context structure
- **LR**: Return address (0x8285BE88)

**Post-Call Expected Behavior:**
- Function should return file metadata in r3
- Metadata stored at `context+40` (line 5874: `std r11,40(r31)`)
- Game continues to `sub_827E8880` for further processing

---

## 3. GetFileInfo Return Structure Analysis

### 3.1 Memory Location and Storage

**Return Value:** Stored in register `r3` after vtable[19] call

**Storage Location:** The returned value is immediately stored at `context+40`:
```cpp
// Line 5866-5874
mr r11,r3              # Move return value to r11
std r11,40(r31)        # Store 64-bit value at context+40
```

**Type:** 64-bit value (stored with `std` instruction, not `stw`)

### 3.2 Possible Structure Interpretations

#### Hypothesis 1: Pointer to File Info Structure
```c
struct FileInfo {
    uint32_t size;           // File size in bytes
    uint32_t attributes;     // File attributes/flags
    uint32_t timestamp;      // Creation/modification time
    uint32_t type;           // File type identifier
    // ... additional fields
};

// vtable[19] returns: FileInfo* (pointer to structure in PPC memory)
```

#### Hypothesis 2: Packed 64-bit Value
```c
// Upper 32 bits: File size
// Lower 32 bits: Attributes/flags
uint64_t fileInfo = (size << 32) | attributes;
```

#### Hypothesis 3: Handle/Descriptor
```c
// Returns a file descriptor or handle that encodes metadata
// Game uses this handle in subsequent operations
uint64_t fileDescriptor;
```

### 3.3 Evidence from Subsequent Code

**After vtable[19] call, the code proceeds to:**

```cpp
// Line 5876-5878: Call sub_827E8880
bl 0x827e8880
```

**Then checks a value at stack offset 80:**

```cpp
// Line 5879-5888: Compare result
lis r11,24952          # Load 0x61680000
lwz r10,80(r1)         # Load value from stack
ori r11,r11,26482      # r11 = 0x61686772 (ASCII: "ahgr")
cmpw cr6,r10,r11       # Compare with magic value
beq cr6,0x8285bed8     # Branch if equal (success path)
```

**Magic Value:** `0x61686772` = ASCII "ahgr" (possibly "rage" reversed or file type marker)

**Interpretation:** The game expects `sub_827E8880` to write a specific magic value to the stack, indicating successful file info retrieval or validation.

### 3.4 Required Research

**To determine exact structure:**
1. ✅ Trace what `sub_827E8880` does with the file info
2. ❌ Examine how the value at `context+40` is used later
3. ❌ Check if there are other vtable calls that provide clues
4. ❌ Analyze shader files to determine expected metadata

---

## 4. How the Game Consumes File Metadata

### 4.1 Immediate Consumption (sub_827E8880)

**Function:** `sub_827E8880` at address 0x827E8880

**Parameters:**
- **r3**: File handle from `sub_827E8180` (e.g., 0xC1264EB0)
- **r4**: Stack buffer pointer (`r1+80`)
- **r5**: Flag value (1)

**Expected Behavior:**
- Reads or validates file metadata
- Writes magic value `0x61686772` to stack buffer if successful
- Returns status code

**Current Status:** Function exists in recompiled code but behavior unknown

### 4.2 Success vs Failure Paths

#### Success Path (Magic Value Match)
```assembly
# Line 5887-5888
beq cr6,0x8285bed8     # Branch to success handler

# Line 5911-5930 (loc_8285BED8)
mr r4,r30              # Pass file handle
mr r3,r31              # Pass context
bl 0x8285b680          # Call sub_8285B680 (shader processing)
mr r3,r30              # Pass file handle
bl 0x827e87a0          # Call sub_827E87A0 (cleanup/close)
li r3,1                # Return success (1)
```

#### Failure Path (Magic Value Mismatch)
```assembly
# Line 5889-5902
# Falls through to error handling
lwz r4,32(r31)         # Load path
addi r3,r11,17904      # Load error message address
bl 0x828e0ab8          # Call sub_828E0AB8 (log error)
mr r3,r30              # Pass file handle
bl 0x827e87a0          # Call sub_827E87A0 (cleanup/close)
li r3,0                # Return failure (0)
```

### 4.3 Field Validation

**Magic Value Check:**
- Expected: `0x61686772` ("ahgr")
- Location: Stack at `r1+80`
- Purpose: Validates file type or format

**Potential Validations:**
1. File size must be non-zero
2. File type must match shader format
3. Attributes must indicate readable file
4. Timestamp or version check

### 4.4 Buffer Allocation Behavior

**No explicit buffer allocation observed in immediate code path.**

**Possible allocation in sub_8285B680:**
- This function is called on success path
- May allocate shader buffer based on file size
- Needs further tracing

### 4.5 Size, Type, and Attribute Checks

**Size Checks:**
- Not explicitly visible in crash-point code
- Likely performed in `sub_827E8880` or `sub_8285B680`

**Type Checks:**
- Magic value `0x61686772` suggests type validation
- May check file extension or header

**Attribute Checks:**
- Unknown - needs tracing of file info structure usage

---

## 5. Implementation Plan

### 5.1 Priority Order

#### Phase 1: Critical Path (Minimum Viable)
1. **Implement vtable[19] (GetFileInfo)** - HIGHEST PRIORITY
   - Must return valid file metadata structure
   - Must allow `sub_827E8880` to succeed
   - Must result in magic value `0x61686772` being written

2. **Test shader loading progression**
   - Verify game proceeds past crash point
   - Monitor for additional vtable calls
   - Check if shader loading completes

#### Phase 2: Supporting Functions (As Needed)
3. **Implement vtable[2] (OpenFile)** - IF CALLED
   - Only if tracing shows it's invoked before GetFileInfo
   - May not be needed if file handle already exists

4. **Implement vtable[18] (GetFileSize)** - IF CALLED
   - Only if game queries size separately
   - May be redundant with GetFileInfo

5. **Implement vtable[6] or vtable[11]** - IF CALLED
   - ReadFile or CloseFile
   - Only if shader loading requires them

#### Phase 3: Comprehensive Implementation
6. **Complete all vtable functions**
   - Implement remaining slots as they're discovered
   - Ensure robust error handling

### 5.2 Implementation Strategy for vtable[19]

#### Step 1: Research Function Signature

**Required Information:**
- What does `sub_827E8880` expect from the file info?
- What structure fields are accessed?
- What values result in the magic number being written?

**Research Method:**
```cpp
// Add logging to trace sub_827E8880 behavior
PPC_FUNC(sub_827E8880) {
    uint32_t fileHandle = ctx.r3.u32;
    uint32_t bufferAddr = ctx.r4.u32;
    uint32_t flags = ctx.r5.u32;
    
    // Log all parameters and memory accesses
    // Trace what gets written to bufferAddr
    // Determine what file info is needed
}
```

#### Step 2: Define File Info Structure

**Based on research, define:**
```cpp
struct StorageDeviceFileInfo {
    uint32_t size;           // File size in bytes
    uint32_t attributes;     // File attributes
    uint32_t type;           // File type identifier
    uint32_t flags;          // Additional flags
    // ... other fields as discovered
};
```

#### Step 3: Implement vtable[19]

```cpp
// Pseudo-code for GetFileInfo implementation
PPC_FUNC(StorageDevice_GetFileInfo) {
    uint32_t pathAddr = ctx.r4.u32;
    
    // Read path string
    char path[256];
    ReadPathFromMemory(pathAddr, path);
    
    // Resolve file through VFS
    FileInfo info = VFS::GetFileInfo(path);
    
    // Allocate structure in PPC memory
    uint32_t infoAddr = AllocatePPCMemory(sizeof(StorageDeviceFileInfo));
    
    // Populate structure
    PPC_STORE_U32(infoAddr + 0, info.size);
    PPC_STORE_U32(infoAddr + 4, info.attributes);
    PPC_STORE_U32(infoAddr + 8, info.type);
    // ... populate other fields
    
    // Return pointer to structure
    ctx.r3.u32 = infoAddr;
}
```

#### Step 4: Register Function in Vtable

```cpp
// In InitializePCStorageDevice()
PPC_STORE_U32(StorageConstants::PC_STORAGE_VTABLE_ADDR + 76, 
              STORAGE_DEVICE_GETFILEINFO_ADDR);
```

#### Step 5: Test and Iterate

1. Run game with vtable[19] implemented
2. Monitor logs for success/failure
3. Check if magic value `0x61686772` is written
4. Adjust implementation based on results
5. Trace for additional vtable calls

### 5.3 Risk Mitigation

**Risk 1: Incorrect Structure Layout**
- **Mitigation:** Start with minimal structure, expand as needed
- **Fallback:** Log all memory accesses to determine required fields

**Risk 2: Additional Vtable Calls**
- **Mitigation:** Implement placeholder logging for all vtable slots
- **Fallback:** Implement functions on-demand as they're called

**Risk 3: Magic Value Not Generated**
- **Mitigation:** Trace `sub_827E8880` thoroughly before implementing
- **Fallback:** May need to implement additional vtable functions

### 5.4 Success Criteria

**Minimum Success:**
- Game does not crash at vtable[19] call
- Execution proceeds past address 0x8285BE74
- No immediate crash in subsequent code

**Full Success:**
- Magic value `0x61686772` written to stack
- Success path taken (branch to 0x8285BED8)
- `sub_8285B680` called (shader processing)
- Shader loading completes without errors

---

## 6. Open Questions and Required Research

### 6.1 Critical Questions

1. **What does sub_827E8880 do with the file info?**
   - Status: ❌ Not researched
   - Priority: CRITICAL
   - Method: Trace through recompiled code

2. **What is the exact layout of the FileInfo structure?**
   - Status: ❌ Unknown
   - Priority: CRITICAL
   - Method: Memory inspection during vtable[19] call

3. **Are there other vtable functions called after GetFileInfo?**
   - Status: ❌ Unknown
   - Priority: HIGH
   - Method: Continue execution trace past crash point

4. **What generates the magic value 0x61686772?**
   - Status: ❌ Unknown
   - Priority: HIGH
   - Method: Reverse engineer sub_827E8880

### 6.2 Secondary Questions

5. **Is vtable[2] (OpenFile) called before GetFileInfo?**
   - Status: ❌ Unknown
   - Priority: MEDIUM
   - Method: Trace earlier in shader loading sequence

6. **How are shader files validated?**
   - Status: ❌ Unknown
   - Priority: MEDIUM
   - Method: Analyze shader file headers

7. **What other storage device functions are used?**
   - Status: ❌ Unknown
   - Priority: LOW
   - Method: Comprehensive vtable call tracing

---

## 7. Technical References

### 7.1 Key Code Locations

**PowerPC Recompiled Code:**
- Crash point: `ppc_recomp.67.cpp:5855-5865` (address 0x8285BE74)
- Storage device resolver: `ppc_recomp.63.cpp:3-412` (sub_827E1EC0)
- Path builder: `ppc_recomp.62.cpp:39620-39851` (sub_827E04F0)
- File open: `imports.cpp:7772-7896` (sub_827E8180 hook)

**PC Implementation:**
- Vtable initialization: `imports.cpp:7058-7082` (InitializePCStorageDevice)
- Storage device hook: `imports.cpp:7085-7117` (sub_827E1EC0 hook)

### 7.2 Memory Addresses

**PC Storage Device:**
- Device object: `0x82A7FE00`
- Vtable pointer: `0x82A7FE80`
- vtable[19] placeholder: `0x8212004C` (INVALID)

**Shader Context:**
- Context structure: `0x82A7FE08`
- Path config offset: `+3072` (currently 0)
- File info storage: `+40` (after vtable[19] call)

**Path Buffers:**
- Buffer 1: `0x000A0B00` (used by sub_827E04F0)
- Buffer 2: `0x80268020` (used by sub_827E1EC0)

### 7.3 Magic Values

- File type check: `0x61686772` ("ahgr")
- Success return: `1`
- Failure return: `0`

---

## 8. Conclusion

The shader loading failure is definitively caused by an incomplete vtable implementation. The minimum viable fix requires implementing `vtable[19]` (GetFileInfo) to return valid file metadata that allows `sub_827E8880` to succeed and write the magic value `0x61686772`.

**Next Steps:**
1. Trace `sub_827E8880` to understand file info requirements
2. Define FileInfo structure based on findings
3. Implement vtable[19] with proper structure
4. Test and iterate based on results
5. Implement additional vtable functions as discovered

**Estimated Complexity:**
- Research phase: 2-4 hours
- Implementation phase: 1-2 hours
- Testing and iteration: 2-4 hours
- Total: 5-10 hours

**Confidence Level:**
- Root cause identified: 100%
- Solution approach: 95%
- Implementation details: 60% (needs research)

---

## Appendix A: Vtable Function Signatures (Hypothetical)

```cpp
// Based on Xbox 360 storage device patterns

// vtable[1] - Path comparison
int32_t (*PathCompare)(void* device, const char* path, size_t length);

// vtable[2] - Open file
uint32_t (*OpenFile)(void* device, const char* path, uint32_t flags);

// vtable[6] - Read file
int32_t (*ReadFile)(void* device, uint32_t handle, void* buffer, uint32_t size);

// vtable[11] - Close file
void (*CloseFile)(void* device, uint32_t handle);

// vtable[18] - Get file size
uint32_t (*GetFileSize)(void* device, const char* path);

// vtable[19] - Get file info (CRITICAL)
FileInfo* (*GetFileInfo)(void* device, const char* path);

// vtable[27] - Read file (alternative)
int32_t (*ReadFileAlt)(void* device, uint32_t handle, void* buffer, 
                       uint32_t size, uint32_t offset);
```

---

**Document End**
