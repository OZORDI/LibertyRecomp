# GPU Flush/Close System — Port Analysis

## Overview

The GPU flush/close system centers on `sub_8285B088`, a "consume pending shader
registration" function. It finalizes a GPU buffer struct by optionally flushing
pending data to the Xenos GPU, then dispatching a vtable call to close/finalize
the shader slot, and finally clearing the struct fields.

On the port, `sub_8285A8B0` (the GPU buffer flush) is stubbed to a no-op because
there is no Xenos hardware. The rest of `sub_8285B088` runs as recompiled code.

---

## GPU Buffer Struct (28 bytes)

| Offset | Type | Name | Description |
|-|-|-|-|
| +0 | u32 | device_ptr | GPU device object (vtable at `*device_ptr`) |
| +4 | u32 | handle | Resource handle; set to -1 on close |
| +8 | u32 | buffer_base | Start address of GPU command/data buffer |
| +12 | u32 | cursor_offset | Current write position (advances as data is flushed) |
| +16 | u32 | pending_bytes | Bytes waiting to be submitted to GPU |
| +20 | u32 | flush_mode | 0 = initial flush path; nonzero = incremental |
| +24 | u32 | buffer_capacity | Maximum bytes the buffer can hold |

---

## Function Analysis

### sub_8285B088 — "ConsumeShaderSlot" / GPU Close

**Location**: `gta4_recomp.56.cpp` line 2343

**Pseudocode**:
```c
void sub_8285B088(GPUBufferStruct *slot) {  // r3
    // Step 1: Conditional GPU flush
    if (slot->flush_mode == 0 && slot->pending_bytes != 0) {
        sub_8285A8B0(slot);  // GPU buffer flush — STUBBED on port
    }

    // Step 2: Vtable dispatch — finalize/close
    void *device = slot->device_ptr;
    void **vtable = *(void ***)device;
    vtable[10](device, slot->handle);  // offset 40 in vtable

    // Step 3: Mark slot consumed
    slot->handle = -1;      // +4 = 0xFFFFFFFF
    slot->device_ptr = 0;   // +0 = NULL
}
```

**Port status**: SAFE as recompiled code. The only GPU-dependent path is
`sub_8285A8B0`, which is already stubbed. The vtable[10] dispatch goes to
shader slot registration functions (pure memory ops). The final field writes
are plain stores.

---

### sub_8285A8B0 — "FlushGPUBuffer" (STUBBED)

**Location**: `gta4_recomp.56.cpp` line 1170

**Pseudocode**:
```c
int sub_8285A8B0(GPUBufferStruct *slot) {  // r3
    if (slot->flush_mode == 0) {
        // Path A: Initial flush — synchronous read-all
        if (slot->pending_bytes != 0) {
            sub_82854C80(slot->device_ptr, slot->handle,
                         slot->buffer_base, slot->pending_bytes);
        }
    } else {
        // Path B: Incremental flush — partial submit
        if (slot->flush_mode != slot->pending_bytes) {
            void **vtable = *(void ***)slot->device_ptr;
            // vtable[9] (offset 36): submit partial buffer to GPU ring
            vtable[9](slot->device_ptr, slot->handle,
                      slot->buffer_base + slot->pending_bytes,
                      slot->cursor_offset - slot->pending_bytes, 0);
        }
    }

    // Fence/sync: update cursor, clear pending state
    slot->cursor_offset += slot->pending_bytes;
    slot->flush_mode = 0;
    slot->pending_bytes = 0;

    void **vtable = *(void ***)slot->device_ptr;
    // vtable[13] (offset 52): GPU fence/sync wait
    vtable[13](slot->device_ptr, slot->handle);

    return 0;  // implied by fall-through
}
```

**Port status**: MUST BE STUBBED. Contains three GPU-dependent operations:
1. `sub_82854C80` — synchronous read loop through vtable[8]
2. `vtable[9]` — partial buffer submission to Xenos command ring
3. `vtable[13]` — GPU fence wait (spin-waits on hardware completion)

All three will hang indefinitely without Xenos hardware. Currently stubbed as
empty function in `imports.cpp` line 1320.

---

### sub_82854C80 — "SynchronousReadAll" (GPU-dependent)

**Location**: `gta4_recomp.55.cpp` line 35889

**Pseudocode**:
```c
int sub_82854C80(void *device, u32 handle, u32 buf_start, u32 total_size) {
    u32 bytes_read = 0;
    while (bytes_read < total_size) {
        void **vtable = *(void ***)device;
        // vtable[8] (offset 32): read chunk from GPU buffer
        int result = vtable[8](device, handle,
                               buf_start + bytes_read,
                               total_size - bytes_read);
        if (result < 0) return 0;  // error
        bytes_read += result;
    }
    return 1;  // success
}
```

**Port status**: GPU-DEPENDENT. This is a blocking read loop that calls
vtable[8] repeatedly until all bytes are consumed. On Xbox 360, vtable[8]
reads from the Xenos GPU command buffer. Without hardware, this either hangs
(if vtable[8] returns 0) or crashes. Never called directly on the port because
`sub_8285A8B0` is stubbed.

**Potential blocker**: If any other code path calls `sub_82854C80` directly
(not through `sub_8285A8B0`), it would hang. No such callers were found in
the current codebase — it is only called from `sub_8285A8B0`.

---

### sub_8285F428 — "RegisterShaderSlotWithOffset"

**Location**: `gta4_recomp.56.cpp` line 12418

**Pseudocode**:
```c
int sub_8285F428(void *context, u32 slot_id, u8 is_write) {
    // Add base offset from context+36 to slot_id
    slot_id += *(u32 *)(context + 36);
    return sub_8285EC98(context, slot_id, is_write);
}
```

**Port status**: SAFE. Pure arithmetic wrapper, then tail-calls `sub_8285EC98`.

---

### sub_8285EC98 — "RegisterShaderSlot"

**Location**: `gta4_recomp.56.cpp` line 11297

**Pseudocode**:
```c
int sub_8285EC98(void *context, u32 slot_id, u8 is_write) {
    if (is_write == 0) return -1;  // read-only request — skip

    // Look up shader handle via sub_8285E500 (path-based shader cache search)
    void *shader = sub_8285E500(context, slot_id);
    if (shader == NULL) return -1;

    // Check shader flags: bit 30 of *(shader+12) must be set
    u32 flags = *(u32 *)(shader + 12);
    if (!(flags & 0x40000000)) return -1;

    // Optional debug logging (if context+1128 byte is set)
    if (*(u8 *)(context + 1128)) {
        sub_822BCA90(...);  // debug print
    }

    // Find empty slot in array of 16 (68-byte entries at context+40)
    int slot_index = -1;
    for (int i = 0; i < 16; i++) {
        if (*(u32 *)(context + 40 + i * 68) == 0) {
            slot_index = i;
            break;
        }
    }
    if (slot_index < 0) {
        sub_822BCA90(...);  // "all slots full" error
        return -1;
    }

    // Initialize the slot
    u32 *slot = (u32 *)(context + 40 + slot_index * 68);
    slot[0] = 0;  // clear
    slot[1] = 0;
    slot[2] = 0;
    memset(slot + 3, 0, 56);  // zero event struct

    // Check shader flags for async load requirement
    u32 needs_async = (flags & 0x40000000) && !(flags & 0x80000000) ? 1 : 0;
    if (needs_async) {
        sub_828470E0(...);  // acquire lock
        int err = sub_8286EDE0(slot + 3, ...);  // async load shader data
        if (err != 0) {
            // error: log and return -1
            sub_82847120(...);  // release lock
            return -1;
        }
        sub_82847120(...);  // release lock
    }

    // Commit: store shader handle into slot
    slot[0] = shader;
    slot[1] = 0;  // clear status
    return slot_index;
}
```

**Port status**: SAFE. Pure memory operations — scans a 16-entry slot array,
zeros a slot, optionally loads shader data through the async file system.
No GPU hardware dependencies. The `sub_8286EDE0` call uses the file I/O
system (already hooked via rexcrt CreateFileA etc.), not GPU commands.

---

## Vtable Slot Summary

| Slot | Offset | Called From | Operation | GPU-Dependent? |
|-|-|-|-|-|
| vtable[8] | 32 | sub_82854C80 | Read chunk from GPU buffer | YES |
| vtable[9] | 36 | sub_8285A8B0 | Submit partial buffer to command ring | YES |
| vtable[10] | 40 | sub_8285B088 | Finalize/close shader resource | NO (memory cleanup) |
| vtable[13] | 52 | sub_8285A8B0 | GPU fence — wait for processing | YES |

---

## Port Recommendations

### Keep as recompiled code (no hook needed)

| Function | Reason |
|-|-|
| sub_8285B088 | Safe with sub_8285A8B0 stubbed; does memory cleanup + vtable[10] |
| sub_8285EC98 | Pure memory: shader slot array scan + allocation |
| sub_8285F428 | Thin wrapper: adds offset, tail-calls sub_8285EC98 |
| sub_82852B78 | ShaderBind: cache lookup + path resolution (pass-through hook OK) |
| sub_82854C80 | Never reached (only caller is stubbed sub_8285A8B0) |

### MUST be replaced with native hooks

| Function | Reason | Current Hook |
|-|-|-|
| sub_8285A8B0 | GPU buffer flush — dispatches vtable[9] + vtable[13] | Empty stub (imports.cpp:1320) |
| sub_82852FB0 | ShaderFinalise — calls sub_8285B088 chain for Xenos compile | Returns 1 (imports.cpp) |
| sub_82299500 | Renderer subsystem init — entire Xenos pipeline setup | Returns 1 (imports.cpp) |

### Current hook strategy is correct

The existing approach in `imports.cpp` is sound:
1. `sub_8285A8B0` is stubbed (no GPU flush)
2. `sub_8285B088` passes through to recompiled code (safe because step 1)
3. `sub_82852FB0` returns 1 (shader finalize is handled by the port's own cache)
4. `sub_82299500` returns 1 (renderer init replaced by port's D3D/Metal backend)

The only risk is if future code paths call `sub_8285A8B0` or `sub_82854C80`
outside the current call chain. Both are already hooked, so this is handled.
