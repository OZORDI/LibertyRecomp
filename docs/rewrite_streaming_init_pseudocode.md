# Streaming Init Pseudocode — Full Line-by-Line Analysis

**Generated**: 2026-03-28
**Source**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.56.cpp` (lines 7833–8692),
             `gta4_recomp.55.cpp` (lines 8749–8916)

---

## Global Addresses (Python-verified)

| Symbol | Guest Address | Description |
|-|-|-|
| streaming_manager | 0x831AB9B4 | StreamingManager singleton (36 bytes visible) |
| streaming_lock | 0x831AB9B0 | Mutex/lock protecting manager (4 bytes before mgr) |
| thread_array_base | 0x831AB9D8 | Array of streaming thread slots (256 × 160 bytes) |
| channel_flags_word | 0x82B08210 | Global flags word read by sub_8285D948 |
| threadpool_base | 0x83192980 | Thread-pool struct used by sub_82849A50 |
| fmt_string_addr | 0x82085E4C | Thread name format string |
| name_string_addr | 0x82085E54 | Thread display name string |

---

## Streaming Channel Struct Layout (1080 bytes total)

Size confirmed by `li r3, 1080` in sub_8285D948.

| Offset | Size | Field | Set by |
|-|-|-|-|
| +0 | u32 | tasks_ptr (heap ptr to task array) | sub_8285D488 |
| +4 | u16 | num_tasks (task count) | sub_8285D610 |
| +6 | u16 | hw_thread_flags (bitmask) | sub_8285D610 |
| +8 | — | data section start (r29 = self+8) | — |
| +16 | u32 | channel_thread_handle | sub_8285D610 via sub_82849A50 |
| +188 | u32 | channel_callback_ptr | sub_8285D610 arg4 |
| +1032 | ~40 | CriticalSection | sub_8285FE48(self+1032) |
| +1064 | u32 | = 0 | sub_8285D610 |
| +1068 | u32 | = 0 | sub_8285D610 |
| +1072 | u32 | = 0 (semaphore? from sub_82849778) | sub_8285D610 |
| +1076 | u32 | = 0 | sub_8285D610 |

### Task Slot Layout (192 bytes each, array at tasks_ptr)

| Offset | Size | Field | Set by |
|-|-|-|-|
| +0..15 | varies | task header / type info | sub_8285D500 internals |
| +16 | u32 | thread_handle | sub_82849A50 return |
| +20 | u32 | ready_flag = 0 | sub_8285D500 |
| +24 | u32 | semaphore_handle (init count=0) | sub_821B3510 |
| +28 | u32 | secondary_semaphore | sub_8285D500 |
| +188 | u32 | callback_fn_ptr | sub_8285D500 arg4 |

### StreamingManager Struct (at 0x831AB9B4)

| Offset | Size | Field |
|-|-|-|
| +0..28 | u32[8] | channel_ptr_array (up to 8 channel ptrs, indexed by count) |
| +32 | u32 | num_channels (incremented each sub_8285D948 call) |

### ThreadPool Struct (at 0x83192980)

| Offset | Size | Field |
|-|-|-|
| +0..703 | varies | thread data |
| +704 | u32 | thread_count |
| +708 | u32 | thread_list_head (linked list) |
| +716 | u32 | ready_flag (cleared then set after thread creation) |

---

## Function: sub_8285D9D8 — Outer Setup / Channel Registrar

**Source**: `gta4_recomp.56.cpp` lines 8545–8692
**Signature**: `void sub_8285D9D8(uint32_t stream_id, uint32_t context, uint32_t name_ptr, uint32_t priority, uint32_t* ready_out)`

### Pseudocode

```
sub_8285D9D8(r3=stream_id, r4=context, r5=name_ptr, r6=priority, r7=ready_out):
  lock_ptr    = 0x831AB9B0        // streaming_lock
  mgr         = 0x831AB9B4        // streaming_manager
  thread_arr  = 0x831AB9D8        // thread_array_base

  // Acquire lock
  sub_821D5F70(lock_ptr)          // WRITE: acquires streaming_lock

  // Check if any channel slots already registered
  READ  mgr+32 -> num_channels    // READ u32 @ 0x831AB9B6 (mgr+32)
  IF num_channels == 0:
    // First-time init: create default streaming channel
    // Args: r3=0x82085E54(name), r4=0xFFFF, r5=0x8000, r6=0, r7=0xFFFFFFFE(-2),
    //       r8=READ(0x82B08210 channel_flags)
    sub_8285D948(r3=alloc_ptr@0x82085E54, r4=0xFFFF, r5=0x8000,
                 r6=0, r7=-2, r8=READ_U32(0x82B08210))
                                  // creates streaming channel, stores ptr at mgr[0]

  // Find empty slot in thread_array (256 slots, 160 bytes each)
  loop_idx = 0
  ptr = thread_arr + 4            // +4 is the "in-use" marker
  loop:
    val = READ_U32(ptr)           // READ u32 @ ptr
    IF val == 0: goto found_slot  // slot is free
    ptr += 160                    // next slot
    loop_idx++
    IF ptr < thread_arr + 0xA004: goto loop
    // No free slot found — fall through
    WRITE_U32(ready_out, 0)       // WRITE u32 @ r30 (arg5) = 0
    lwsync                        // memory barrier
    return 0

found_slot:
  // Compute slot index → address
  // r10 = loop_idx * 4; r11 = loop_idx * 5; r11 = r11 * 32 = loop_idx * 160
  slot = thread_arr + loop_idx * 160  // = slot base

  WRITE_U32(slot+0, stream_id)    // WRITE: slot.stream_id = r27
  WRITE_U32(slot+4, context)      // WRITE: slot.context = r26
  sub_82A00DC0(slot+8, ...)       // WRITE: init slot.name (strcpy/memset)

  // Also insert into streaming manager's per-channel dispatch
  chan_slot_ptr = READ_U32(mgr + priority*4)  // READ: mgr channel ptr
  sub_8285D2C8(chan_slot_ptr+8, &slot)        // WRITE: link slot into channel

  lwsync                          // memory barrier
  WRITE_U32(ready_out, 0)         // WRITE u32 @ r30 = 0
  return slot                     // r3 = slot ptr
```

---

## Function: sub_8285D948 — Allocate + Register Streaming Channel

**Source**: `gta4_recomp.56.cpp` lines 8458–8541
**Signature**: `StreamingChannel* sub_8285D948(uint32_t alloc_ptr, uint32_t unk4, uint32_t unk5, uint32_t unk6, uint32_t unk7, uint32_t ch_flags, uint32_t unk8, uint32_t extra_flags)`

### Pseudocode

```
sub_8285D948(r3, r4, r5, r6, r7, r8=extra_flags):
  mgr = 0x831AB9B4

  READ_U32(mgr+32) -> saved_count  // READ: snapshot channel count before alloc
                                   // addr: 0x831AB9B4 + 32 = 0x831AB9D4

  // Allocate 1080-byte channel struct
  sub_821B3510(1080)               // malloc(1080) -> r3
  channel = r3
  IF channel == 0: return 0        // alloc failed

  // Load channel type flags
  READ_U32(0x82B08210) -> flags_word   // READ u32 @ 0x82B08210
  masked_flags = flags_word & extra_flags  // AND with arg r8

  // Initialize channel (see sub_8285D610)
  sub_8285D610(channel, r4, r5, r6, r7, masked_flags)
                                   // WRITES: all channel fields (see sub_8285D610 below)

  // Register in manager
  idx = saved_count
  slot_offset = idx * 4            // 4 bytes per ptr slot
  READ_U32(mgr+32) -> new_count    // READ: mgr+32 again
  new_count++
  WRITE_U32(mgr+32, new_count)     // WRITE u32 @ mgr+32 = new_count
  WRITE_U32(mgr + slot_offset, channel)  // WRITE: mgr[idx] = channel

  return old_count (r3=r24=saved_count on return path)
```

---

## Function: sub_8285D610 — Initialize Streaming Channel Struct

**Source**: `gta4_recomp.56.cpp` lines 7990–8267
**Signature**: `void sub_8285D610(StreamingChannel* self, uint32_t unk4, uint32_t name_idx, uint32_t unk6, uint32_t unk7, uint32_t hw_thread_mask)`

### Pseudocode

```
sub_8285D610(r3=self, r4=unk4, r5=name_idx, r6=unk6, r7=unk7, r8=hw_thread_mask):
  r29 = self + 8       // data section
  r25 = unk4 (name_idx)
  r24 = name_idx
  r23 = unk6
  r22 = unk7
  r27 = hw_thread_mask

  WRITE_U32(self+0, 0)             // tasks_ptr = null
  WRITE_U16(self+4, 0)             // num_tasks = 0
  WRITE_U16(self+6, 0)             // hw_thread_flags = 0

  sub_8285FE48(r29+1024)           // init CriticalSection @ self+1032
  sub_82849778(0)                  // init semaphore (count=0) -> r3=semaphore_handle

  // Store zeroes around the semaphore area
  WRITE_U32(r29+1064, 0)           // self+1072 = 0
  WRITE_U32(r29+1060, 0)           // self+1068 = 0
  WRITE_U32(r29+1068, r3)          // self+1076 = semaphore_handle (sub_82849778 result)
  WRITE_U32(r29+1056, 0)           // self+1064 = 0

  // Decode hw_thread_mask bits into task_types[] array on stack (r1+96)
  task_count = 0
  IF hw_thread_mask & 0x01: stack[task_count++] = 1   // HW thread 1
  IF hw_thread_mask & 0x02: stack[task_count++] = 2   // HW thread 2
  IF hw_thread_mask & 0x04: stack[task_count++] = 3
  IF hw_thread_mask & 0x08: stack[task_count++] = 4
  IF hw_thread_mask & 0x10: stack[task_count++] = 5
  IF hw_thread_mask & 0x20: stack[task_count++] = 6
  IF hw_thread_mask & 0x40: stack[task_count++] = 7
  IF hw_thread_mask & 0x80: stack[task_count++] = 8   // HW thread 8

  // Read existing hw_thread_flags to check if this is first time
  READ_U16(self+6) -> existing_flags
  IF existing_flags == 0:         // first init
    IF task_count > 0:
      sub_8285D488(self, task_count)  // malloc(task_count * 192) + store at self+0
    ELSE:
      WRITE_U32(self+0, 0)
  WRITE_U16(self+4, task_count)   // num_tasks = task_count

  IF task_count == 0: goto done

  // Create one thread per task type
  fmt_str = 0x82085E4C
  k = 0
  loop:
    hw_thread_id = stack[k]       // READ u32 @ r1+96+k*4

    // Format thread name: snprintf(r1+80, 16, fmt_str, name_idx, hw_thread_id)
    sub_82158E08(r3=r1+80, r4=16, r5=fmt_str, r6=name_idx, r7=hw_thread_id)
                                  // WRITE: local name buffer @ r1+80

    // Initialize task slot k
    slot = self+0 (tasks_ptr) + k*192
    sub_8285D500(slot, self, r1+80, hw_thread_id, self, unk7, unk6, callback?)
                                  // WRITES: slot fields (see sub_8285D500 below)

    k++
    IF k < task_count: goto loop

done:
  return self
```

---

## Function: sub_8285D500 — Initialize Task Slot + Create Thread

**Source**: `gta4_recomp.56.cpp` lines 7835–7986
**Signature**: `void sub_8285D500(slot, channel, name_buf, hw_thread_id, channel2, unk5, priority, callback)`

### Pseudocode

```
sub_8285D500(r3=slot, r4=channel, r5=name_buf, r6=hw_thread_id,
             r7=channel2, r8=unk8, r9=callback):
  r31 = slot    // task slot base

  // Copy name (unrolled strcpy into slot header area)
  // Copies from r10 (name_buf) into r11 (slot name area) 5 chars per iter
  strcpy_unrolled(slot_name_dst, name_buf)
  WRITE_U8(dst_end, 0)             // null-terminate

  // Create semaphore
  sub_821B3510(r3=unk8_count)      // CREATE_SEMAPHORE(count=unk8)
  WRITE_U32(slot+28, r3)           // semaphore_handle at slot+28
  WRITE_U32(slot+20, 0)            // ready_flag = 0
  WRITE_U32(slot+188, channel)     // callback_or_owner = r4 (channel ptr)

  // Create thread
  // r3 = 0x8285D368 (thread entry point name string)
  // r4 = slot (this)
  // r5 = name_buf (r27)
  // r6 = priority (r26)
  // r7 = slot (r31)
  // r8 = 1 (CREATE_SUSPENDED?)
  // r9 = callback (r28)
  sub_82849A50(0x8285D368, slot, name_buf, priority, slot, 1, callback, hw_thread_id)
  WRITE_U32(slot+16, r3)           // thread_handle = XCreateThread result
  return
```

---

## Function: sub_82849A50 — XCreateThread Wrapper

**Source**: `gta4_recomp.55.cpp` lines 8751–8916
**Signature**: `HANDLE sub_82849A50(name_str, context, display_name, stack_size, ctx2, suspended, callback, priority)`

### Pseudocode

```
sub_82849A50(r3=name_str, r4=context, r5=display_name, r6=stack_size,
             r7=ctx2, r8=suspended, r9=callback, r24=priority):
  threadpool = 0x83192980

  IF stack_size < 16384: stack_size = 16384  // enforce minimum stack

  sub_821D5F70(threadpool+716)     // acquire threadpool.ready_flag lock

  // Try to create thread
  sub_828499E8(threadpool, 0)      // alloc thread slot -> r31=thread_ptr
  WRITE_U32(threadpool+716, 0)     // WRITE: clear ready_flag

  IF thread_ptr == 0: return -1    // alloc failed

  // Fill thread struct
  WRITE_U32(thread_ptr+0, context) // thread.context = r4
  WRITE_U32(thread_ptr+4, ctx2)    // thread.ctx2 = r5(display_name) -> stored at +4
  READ_U32(r13+1676) -> allocator  // READ: TLS[1676] = current allocator
  WRITE_U32(thread_ptr+8, allocator)  // thread.allocator_ctx

  // Platform XCreateThread call
  sub_82A13110(r3=0, r4=stack_size, r5=name_ptr, r6=thread_ptr, r7=4,
               r8=r1+80, r9=allocator_ctx)
                                   // WRITES: creates native OS thread
  IF result == 0: goto thread_alloc_failed
  // On success:
  thread_handle = result
  WRITE_U32(thread_ptr+0, READ_U32(threadpool+708))  // prepend to linked list
  WRITE_U32(threadpool+708, thread_ptr)               // new head
  READ_U32(threadpool+704) -> cnt
  WRITE_U32(threadpool+704, cnt+1)                   // increment thread count
  return -1  // (error return — thread handle is in thread_ptr+0)

thread_success:
  sub_82A11478(thread_handle, priority)  // SetThreadPriority
  sub_82A114F8(thread_handle, 1)         // ResumeThread
  sub_82A11580(thread_handle, hw_affinity) // SetThreadAffinityMask
  IF suspended: sub_82A13120(thread_handle) // extra suspend
  return thread_handle
```

---

## Synchronization Protocol

### Who creates the semaphore
- `sub_8285D610` calls `sub_82849778(0)` → creates a semaphore with initial count 0.
  Result stored at `channel+1076` (= `r29+1068`).
- Per-task semaphores: `sub_821B3510(count)` called inside `sub_8285D500`, stored at `slot+28`.

### Who waits
- Worker threads (created by sub_82849A50) wait on `slot+28` semaphore to receive work items.
- The outer ready flag at `slot+20` is set to 0 at creation; the thread loop checks it.

### Who signals
- Game code (streaming dispatcher) increments the semaphore to wake a worker.
- `sub_8285D9D8` writes 0 to `*ready_out` (arg5 / r30) after slot registration — the caller
  polls this value until the thread signals it ready (sets it non-zero via `lwsync`).

### Call chain for full setup
```
sub_8285D9D8(stream_id, ctx, name, prio, ready_out)
  └─ sub_821D5F70(streaming_lock)       // lock
  └─ [if first] sub_8285D948(...)       // create default channel
  └─ find free slot in thread_array
  └─ sub_82A00DC0(slot+8, ...)          // name
  └─ sub_8285D2C8(chan+8, &slot)        // link into channel dispatch

sub_8285D948(...)
  └─ sub_821B3510(1080)                 // malloc channel
  └─ sub_8285D610(channel, ...)         // init channel
  └─ mgr[count++] = channel             // register

sub_8285D610(channel, ...)
  └─ sub_8285FE48(channel+1032)         // init critical section
  └─ sub_82849778(0)                    // create channel semaphore
  └─ sub_8285D488(channel, task_count)  // alloc task array
  └─ loop: sub_82158E08(buf, fmt, ...)  // format thread name
         sub_8285D500(slot, ...)        // init task + create thread

sub_8285D500(slot, ...)
  └─ sub_821B3510(count)                // create task semaphore
  └─ sub_82849A50(...)                  // XCreateThread → thread_handle @ slot+16
```

---

## Key Addresses for Hook Targeting

| Function | Address | Purpose |
|-|-|-|
| sub_8285D9D8 | 0x8285D9D8 | Outer setup, call with ready_out ptr |
| sub_8285D948 | 0x8285D948 | Alloc + register 1080-byte channel |
| sub_8285D610 | 0x8285D610 | Zero-init channel, create threads |
| sub_8285D500 | 0x8285D500 | Per-task slot init + XCreateThread |
| sub_82849A50 | 0x82849A50 | XCreateThread wrapper |
| sub_8285FE48 | 0x8285FE48 | CriticalSection init |
| sub_82849778 | 0x82849778 | Semaphore create (count=0) |
| sub_821B3510 | 0x821B3510 | malloc / create semaphore |
| sub_8285D488 | 0x8285D488 | Task array allocator |
