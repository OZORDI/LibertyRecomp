# Operator New Hang at 0x82478EC4 (1024-byte alloc in sub_82478AF8)

## Symptom

sub_82478AF8 (engine init) calls `sub_821B3510(1024)` at return address 0x82478EC4.
Earlier allocations from the same function (176 bytes at 0x82478B20, 192 bytes at 0x82478CDC) succeed.
The 1024-byte allocation hangs.

## Full Allocation Path

### Layer 1: sub_821B3510 (operator new)

**Location**: gta4_recomp.2.cpp line 83062

```
TLS_base = PPC_LOAD_U32(r13 + 0)
allocator = PPC_LOAD_U32(TLS_base + 1676)   // rage::sysMemAllocator*
vtable = PPC_LOAD_U32(allocator + 0)
fn_alloc = PPC_LOAD_U32(vtable + 8)         // vtable[2] = Allocate()
tail-call fn_alloc(allocator, size=1024, align=16, flags=0)
```

Pure dispatcher. No locking, no branching, no failure path. If it hangs, the vtable[2] target is the culprit.

### Layer 2: Concrete Allocator (vtable[2] target)

The vtable[2] pointer resolves at runtime to the concrete allocator's `Allocate` virtual method. The RAGE heap allocator implementation lives in the 0x829E1xxx range:

| Function | Role |
|-|-|
| sub_829E12D0 | Core alloc: acquires lock, walks free-list, splits blocks |
| sub_829E1558 | Defragmenting alloc: retries with compaction |
| sub_829E2748 | Defrag wait: **yield loop** (spins calling KeDelayExecutionThread) |
| sub_829E2258 | Bin alloc: small-object slab allocator path |

### Layer 3: Locking (sub_8285FF50 / sub_8285FFA0)

RAGE's scoped lock wrapper. Located in gta4_recomp.56.cpp line 14206.

```
sub_8285FF50(lock_guard, critical_section):
  if critical_section->initialized != 0:
    RtlEnterCriticalSection(critical_section)
```

`RtlEnterCriticalSection` (rexglue xboxkrnl_rtl.cpp line 386):
1. If already owned by current thread: increment recursion_count, return
2. Spin loop (spin_count * 256 iterations): atomic CAS on lock_count
3. If spin fails: `xeKeWaitForSingleObject(cs, ...)` -- **blocking wait**

### Layer 4: sub_82849918 (Yield/Sleep)

Chain: `sub_82849918` -> `sub_82A12B60` -> `sub_82A1A200` -> `KeDelayExecutionThread`

sub_82A1A200 converts milliseconds to 100ns units (multiply by -10000), then calls
`KeDelayExecutionThread(KernelMode=1, alertable=r4, timeout)`.
Called with r3=1 (1ms delay).

## Three Blocking Scenarios

### Scenario A: Critical Section Deadlock

sub_829E12D0 acquires a critical section via sub_8285FF50 before walking the free-list.
If another thread holds this lock (e.g., a worker thread doing a free/realloc), and that
thread is itself blocked waiting on something the main thread owns, classic deadlock.

**Relevant code**: The allocator uses `critical_section` at `allocator+124` (sub_829E1598
line: `addi r26,r30,124`) and `allocator+1292` (line: `addi r24,r30,1292`). Two locks per
allocator object.

### Scenario B: Defrag Wait Loop (sub_829E2748)

If the allocator is out of contiguous memory for the 1024-byte request:

```
loop:
  sub_82849860(...)           // try to compact
  if byte[request+16] & 0x80 == 0:
    sub_82849918(1)           // KeDelayExecutionThread(1ms)
    goto loop                 // spin forever waiting for memory
```

The loop checks `byte[request+16] & 0xFFFFFF80`. If this flag is never set (because
the defragmenter thread is not running or no memory is being freed), this loops forever.
On Xbox 360 with 6 hardware threads, the defrag thread gets cycles. In the recomp with
potentially different thread scheduling, it may starve.

### Scenario C: Fallback Allocator Hook Bypass

The hook on sub_8218BE28 (imports.cpp line 901) catches calls where TLS[1676] is invalid
and routes to SystemHeapAlloc. However, **sub_821B3510 does NOT call sub_8218BE28**.
sub_821B3510 does a direct vtable dispatch: `PPC_CALL_INDIRECT_FUNC(vtable[2])`.

If TLS[1676] points to a valid allocator object but that allocator's internal state is
corrupt (heap metadata, free-list pointers), the vtable[2] target will execute but may:
- Loop infinitely in free-list traversal (circular linked list from double-free)
- Block on critical section owned by a dead/stuck thread
- Enter defrag yield loop that never resolves

The sub_8218BE28 fallback hook **never fires** in this path because TLS[1676] IS valid.

## Hook Coverage Gap

| Allocation entry point | Hook coverage | Fallback |
|-|-|-|
| sub_8218BE28 (malloc) | Hooked: checks TLS[1676], fallback to SystemHeapAlloc | Yes |
| sub_8218BE50 (aligned malloc) | Hooked: same with alignment | Yes |
| sub_821B3510 (operator new) | Hooked: logging only | **No fallback** |
| sub_82847248 (rage::alloc) | Not hooked | No |

sub_821B3510's hook (imports.cpp line 1790) only logs; it always falls through to
`__imp__sub_821B3510` which does the vtable dispatch. If the vtable[2] target hangs,
there is no recovery.

## Recommended Investigation

1. **Add diagnostic to sub_821B3510 hook**: Before calling `__imp__sub_821B3510`,
   read TLS[1676] and dump `allocator`, `vtable`, `vtable[2]` to identify which
   concrete allocator implementation is being dispatched to.

2. **Add timeout wrapper**: If `__imp__sub_821B3510` doesn't return within N seconds,
   log the thread's PPC call stack and force-return a SystemHeapAlloc fallback.

3. **Hook the concrete allocator**: Once vtable[2] target is identified (likely in
   0x829E1xxx range), hook it to detect the defrag yield loop or critical section wait.

4. **Check for double-free corruption**: The existing sub_82848B68 double-free detector
   (imports.cpp line 1013) may have logged corruption that caused a circular free-list.

## Key Files

| File | Content |
|-|-|
| `gta4_recomp.2.cpp:83062` | sub_821B3510 (operator new vtable dispatch) |
| `gta4_recomp.67.cpp:1-400` | sub_829E12D0 (RAGE heap alloc with critical section) |
| `gta4_recomp.67.cpp:3034` | sub_829E2748 (defrag yield loop with KeDelayExecutionThread) |
| `gta4_recomp.55.cpp:8563` | sub_82849918 (Sleep: chains to KeDelayExecutionThread) |
| `gta4_recomp.56.cpp:14206` | sub_8285FF50 (scoped lock: calls RtlEnterCriticalSection) |
| `imports.cpp:901` | sub_8218BE28 hook (fallback allocator, NOT reached by operator new) |
| `imports.cpp:1790` | sub_821B3510 hook (logging only, no fallback) |
| `xboxkrnl_rtl.cpp:386` | RtlEnterCriticalSection (spin then KeWait) |
