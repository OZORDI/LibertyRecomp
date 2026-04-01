# Scoped Lock Trace: sub_8285FF50 / sub_8285FFA0

## Scoped Lock Object Layout (8 bytes)

| Offset | Size | Field | Description |
|-|-|-|-|
| 0x00 | 4 | recursive_count | 1 on first lock; supports recursive acquisition |
| 0x04 | 4 | cs_ptr | Guest pointer to X_RTL_CRITICAL_SECTION |

## X_RTL_CRITICAL_SECTION Layout (28 bytes, packed)

| Offset | Size | Field | Description |
|-|-|-|-|
| 0x00 | 1 | type | Always 1 (EventSynchronizationObject auto-reset) |
| 0x01 | 1 | absolute | spin_count / 256 |
| 0x02 | 1 | size | Unused |
| 0x03 | 1 | inserted | Unused |
| 0x04 | 4 | signal_state (be32) | 0 normally |
| 0x08 | 4 | wait_list_flink (be32) | Stashed with kXObjSignature ('REX\0') after first kernel wait |
| 0x0C | 4 | wait_list_blink (be32) | Stashed XObject handle after first kernel wait |
| 0x10 | 4 | lock_count (native s32) | -1 = unlocked, 0+ = locked (atomic CAS target) |
| 0x14 | 4 | recursion_count (be32) | 0 = unlocked, 1+ = times locked by owner |
| 0x18 | 4 | owning_thread (be32) | Guest PKTHREAD of lock owner, 0 if unlocked |

Source: `xboxkrnl_rtl.cpp:339-346`, verified with `static_assert_size(X_RTL_CRITICAL_SECTION, 28)`

---

## Chain 1: sub_8285FF50 (Constructor) -> RtlEnterCriticalSection

### Step 1: sub_8285FF50 (gta4_recomp.56.cpp:14208)

```
this[+0] = 1                          // recursive_count = 1
this[+4] = cs_ptr                     // save critical section pointer
if (*(uint32_t*)cs_ptr == 0) return   // guard: CS uninitialized (type==0 means bytes 0-3 all zero)
RtlEnterCriticalSection(cs_ptr)
return this                           // r3 = this for chaining
```

Guard check: `lwz r11, 0(r4)` loads the first 4 bytes of X_DISPATCH_HEADER as a 32-bit word. If the critical section was never initialized (all fields zero), the lock is skipped entirely. After `RtlInitializeCriticalSection`, `type=1` so this word is non-zero.

### Step 2: RtlEnterCriticalSection (xboxkrnl_rtl.cpp:386)

```
cur_thread = XThread::GetCurrentThread()->guest_object()  // guest PKTHREAD
spin_count = cs->header.absolute * 256

// Fast path: recursive acquisition
if (cs->owning_thread == cur_thread) {
    atomic_inc(&cs->lock_count)
    cs->recursion_count++
    return
}

// Spin loop (spin_count iterations)
while (spin_count--) {
    if (atomic_cas(-1, 0, &cs->lock_count)) {  // CAS: -1 → 0
        cs->owning_thread = cur_thread
        cs->recursion_count = 1
        return
    }
}

// Slow path: kernel wait
if (atomic_inc(&cs->lock_count) != 0) {
    xeKeWaitForSingleObject(cs_host_ptr, 8, 0, 0, nullptr)  // *** DEADLOCK POINT ***
}
cs->owning_thread = cur_thread
cs->recursion_count = 1
```

Lock semantics:
- **lock_count = -1**: unlocked (CAS target)
- **lock_count = 0**: locked, no waiters
- **lock_count > 0**: locked, N waiters queued
- **Recursive**: yes, via owning_thread check before CAS
- **Spin count**: `header.absolute * 256` iterations before kernel wait

### Step 3: xeKeWaitForSingleObject (xboxkrnl_threading.cpp:758)

```
auto object = XObject::GetNativeObject<XObject>(kernel_state(), object_ptr);
// ^^^ THIS ACQUIRES global_critical_region ^^^
X_STATUS result = object->Wait(wait_reason, processor_mode, alertable, timeout_ptr);
return result;
```

### Step 4: GetNativeObject (xobject.cpp:365)

```
auto global_lock = rex::thread::global_critical_region::AcquireDirect();
// ^^^ ACQUIRES the singleton std::recursive_mutex ^^^

auto header = reinterpret_cast<X_DISPATCH_HEADER*>(native_ptr);

if (header->wait_list_flink == kXObjSignature) {
    // Already initialized — lookup by stashed handle
    uint32_t handle = header->wait_list_blink;
    auto object = kernel_state->object_table()->LookupObject<XObject>(handle);
    // ^^^ LookupObject ALSO acquires global_critical_region (recursive OK) ^^^
    return object;
} else {
    // First use — create XEvent (type 1 = EventSynchronizationObject)
    auto ev = new XEvent(kernel_state);
    ev->InitializeNative(native_ptr, header);
    StashHandle(header, object->handle());
    return object_ref<XObject>(object);
}
```

### Step 5: ObjectTable::LookupObject (object_table.cpp:270)

```
global_critical_region_.mutex().lock();    // recursive lock — OK since caller already holds it
uint32_t slot = GetHandleSlot(handle);
if (slot < table_capacity_) {
    object = table_[slot].object;
}
if (object) object->Retain();
global_critical_region_.mutex().unlock();
return object;
```

### Step 6: XObject::Wait (xobject.cpp:203)

```
auto wait_handle = GetWaitHandle();  // returns event_->get() (platform Event)
auto timeout_ms = ... (infinite for CS waits since timeout_ptr is nullptr)
auto result = rex::thread::Wait(wait_handle, false, timeout_ms);
// Blocks on platform event (pthread_cond / WaitForSingleObject)
```

Critical: **global_critical_region is released before Wait** because `global_lock` is a `unique_lock` that goes out of scope when `GetNativeObject` returns the `object_ref`. The Wait itself does NOT hold the global lock.

---

## Chain 2: sub_8285FFA0 (Destructor) -> RtlLeaveCriticalSection

### Step 1: sub_8285FFA0 (gta4_recomp.56.cpp:14257)

```
if (this[+0] == 0) return                // never locked
this[+0]--                                // decrement recursive count
if (this[+0] != 0) return                // still recursively held
cs_ptr = this[+4]                         // load CS pointer
if (*(uint32_t*)cs_ptr == 0) return       // guard: CS uninitialized
RtlLeaveCriticalSection(cs_ptr)
```

### Step 2: RtlLeaveCriticalSection (xboxkrnl_rtl.cpp:436)

```
assert(cs->owning_thread == cur_thread)
assert(cs->recursion_count > 0)

if (--cs->recursion_count != 0) {
    // Still recursively held
    atomic_dec(&cs->lock_count)
    return
}

// Fully releasing
cs->owning_thread = 0
if (atomic_dec(&cs->lock_count) != -1) {
    // There were waiters — wake one
    xeKeSetEvent(cs_host_ptr, 1, 0)  // *** ANOTHER global_critical_region acquisition ***
}
```

### Step 3: xeKeSetEvent (xboxkrnl_threading.cpp:409)

```
auto ev = XObject::GetNativeObject<XEvent>(kernel_state(), event_ptr);
// ^^^ ACQUIRES global_critical_region (same path as Enter) ^^^
return ev->Set(increment, !!wait);
// Set signals the platform event, waking one waiter
```

---

## The Double-Lock Issue (Deadlock Analysis)

### Lock acquisition sequence for RtlEnterCriticalSection slow path:

1. Thread atomically increments `lock_count` (detects contention)
2. Calls `xeKeWaitForSingleObject(cs_as_event_ptr, ...)`
3. Inside: `GetNativeObject()` acquires **global_critical_region** (std::recursive_mutex)
4. Inside GetNativeObject: `ObjectTable::LookupObject()` acquires **global_critical_region** again (recursive, OK)
5. GetNativeObject returns, **releasing global_critical_region**
6. `object->Wait()` blocks on the platform event (does NOT hold global_critical_region)

### Lock acquisition sequence for RtlLeaveCriticalSection with waiters:

1. Atomically decrements `lock_count`, detects it wasn't -1 (waiters exist)
2. Calls `xeKeSetEvent(cs_as_event_ptr, 1, 0)`
3. Inside: `GetNativeObject()` acquires **global_critical_region**
4. `ev->Set()` signals the platform event
5. Returns, releasing **global_critical_region**

### Why this is NOT a classic deadlock between two mutexes:

The global_critical_region is a **std::recursive_mutex** (see `mutex.cpp:16`). It can be acquired multiple times by the same thread. The acquisition in `GetNativeObject` and `LookupObject` is nested-safe.

### The REAL deadlock risk: global_critical_region contention

`global_critical_region` is the **single most contended lock** in the runtime — 124 acquisition sites across 24 files:
- `xmemory.cpp`: 21 sites (memory allocation)
- `kernel_state.cpp`: 17 sites
- `object_table.cpp`: 13 sites
- `shared_memory.cpp`: 10 sites (GPU)
- `content_manager.cpp`: 7 sites
- `module.cpp`: 6 sites
- `virtual_file_system.cpp`: 5 sites
- `processor.cpp`: 5 sites
- Plus threading, audio, texture cache, etc.

**Every** RtlEnterCriticalSection slow path and **every** RtlLeaveCriticalSection with waiters must acquire global_critical_region. If any other thread is holding global_critical_region for a long operation (memory allocation, VFS operation, GPU shared memory), the critical section enter/leave will stall behind it.

### Scenario: Priority inversion via global_critical_region

```
Thread A: holds guest CS_X, wants to leave → needs global_critical_region for SetEvent
Thread B: holds global_critical_region (inside xmemory allocation), tries to enter guest CS_X
          → spins, then slow path → needs global_critical_region (already holds it, recursive OK)
          → calls Wait() → releases global_critical_region → blocks on event
Thread A: can now acquire global_critical_region → signals event → Thread B wakes
```

This works correctly due to recursive_mutex. But:

```
Thread A: holds guest CS_X, wants to leave → needs global_critical_region
Thread B: holds global_critical_region (long VFS operation), does NOT need CS_X
Thread A: BLOCKED waiting for global_critical_region
Thread C: needs CS_X → spins → slow path → needs global_critical_region → BLOCKED
```

Result: **convoy effect**. All threads needing any guest critical section are serialized behind whoever holds global_critical_region, even if they use completely independent critical sections.

### First-time initialization amplification

On first use of any critical section's slow path, `GetNativeObject` must:
1. Acquire global_critical_region
2. `new XEvent(kernel_state)` — heap allocation under the lock
3. `InitializeNative()` — more work under the lock
4. `StashHandle()` — write back to dispatch header
5. Release global_critical_region

This is especially dangerous during game startup when many threads initialize their critical sections simultaneously.

---

## Summary

The scoped lock (sub_8285FF50/sub_8285FFA0) is a simple 8-byte RAII wrapper over RtlEnterCriticalSection / RtlLeaveCriticalSection. The critical section itself is a 28-byte Xbox 360 `X_RTL_CRITICAL_SECTION` with an embedded `X_DISPATCH_HEADER` that doubles as an auto-reset event for waiter notification.

The core issue is that the kernel wait/signal path funnels through `GetNativeObject`, which requires the **global_critical_region** — a single `std::recursive_mutex` shared by 124 call sites across memory, VFS, GPU, audio, and object management. This creates a convoy effect where independent critical sections become coupled through the global lock.
