# sub_82852D18 Hang Trace

## Call Chain

```
sub_82478AF8 (audio init)
  -> sub_827C2420 (audio resource load)
    -> sub_82852DD0 (resource manager load wrapper)
      -> sub_8284F468 (find resource entry by name, iterates data sources)
      -> sub_82852D18 (THE HANG SITE - resource load core)
```

## sub_82852D18 (gta4_recomp.55.cpp:30962) Internal Flow

| Step | Line | Call | Purpose |
|-|-|-|-|
| 1 | 30982 | sub_82852A50 | Resource container lookup (returns ptr or NULL) |
| 2 | 30996 | load r31=*r27 | Dereference resource entry -> first field |
| 3 | 31012 | sub_82851A10 | Name lookup: strip "__" prefix, call sub_82912948 (hash map find) |
| 4 | 31027 | vtable[2] on r29 | **Indirect call: *(*(r29)+8)** -- the resource load callback |
| 5 | 31045 | sub_828470E0 | TLS thread-affinity enter (no blocking) |
| 6 | 31051 | sub_8284FA58 | Cleanup: free resource entries in loop via sub_821B3560 |
| 7 | 31056 | sub_821B3560 | Free via TLS allocator vtable[3] |
| 8 | 31063 | sub_82847120 | TLS thread-affinity exit (no blocking) |

## sub_82852A50 (gta4_recomp.55.cpp:30531) Internal Flow

| Step | Line | Call | Purpose |
|-|-|-|-|
| 1 | 30561 | sub_828470E0 | TLS thread-affinity enter |
| 2 | 30573 | sub_82852300 | Hash table iteration: calls sub_8285AD08 (stream read), sub_828D0608 (iterator) |
| 3 | 30586 | sub_82851DF0 | Hash map lookup by key (pure data, no blocking) |
| 4 | 30603 | vtable[3] on (r28+16) | Container match/compare callback |
| 5 | 30620 | vtable[1] on r31 | Post-lookup initialization |
| 6 | 30625 | sub_8284FC98 | Allocate 20-byte descriptor, call sub_8284D220 (name copy) |
| 7 | 30640 | vtable[0] on r31 | Release/destructor on lookup result |
| 8 | 30647 | sub_82847120 | TLS thread-affinity exit |

## Blocking Analysis

### Direct blocking calls: NONE found in static chain
None of sub_82849790, sub_828497D8, sub_82849820, NtWaitForSingleObjectEx, KeWaitForSingleObject, or RtlEnterCriticalSection are called directly from any function in the sub_82852D18 call tree.

### Indirect blocking via stream I/O (sub_8285AD08)

The sub_82852300 -> sub_8285AD08 path reads resource data from a stream:

```
sub_8285AD08 (stream read, gta4_recomp.56.cpp:1820)
  -> sub_8285A8B0 (stream refill, gta4_recomp.56.cpp:1170)
    -> vtable[9] at offset 36: positioned read (seek + read)
    -> sub_82854C80 (synchronous read-all loop, gta4_recomp.55.cpp:35889)
      -> vtable[8] at offset 32: raw Read() in a LOOP until all bytes consumed
    -> vtable[13] at offset 52: buffer refill/pump
```

**sub_82854C80 is a synchronous loop** that calls vtable[8] (Read) repeatedly until `bytes_read >= requested`. If vtable[8] returns 0 (EOF/error) or never returns, this loop hangs.

### Indirect blocking via vtable[2] at line 31027

The vtable call at line 31027 is `*(*(r29)+8)` where r29 = callback object at **0x820BBFE4** (static data, set in sub_827C2420). This is the resource load callback -- it receives (callback_obj, resource_entry, audio_obj_ptr) and performs the actual resource loading. This vtable method likely calls into the RPF/streaming layer which could block on:
- File I/O completion events
- Async read operations that never complete
- Semaphore waits for I/O worker threads

## Root Cause Candidates

1. **sub_82854C80 read loop** (most likely): vtable[8] on a stream object returns 0 bytes (no progress), causing an infinite loop. This would happen if the underlying RPF file handle is invalid or the VFS returns 0 bytes without error.

2. **vtable[2] callback at line 31027**: If the resource load callback blocks waiting for an I/O completion event that was never signaled (e.g., OVERLAPPED hEvent issue), this would hang.

3. **sub_8285A8B0 vtable[13]**: The buffer pump/refill method (offset 52) could block on an I/O semaphore waiting for data from an async reader thread that is not running.

## Key Addresses

| Address | Identity |
|-|-|
| 0x831E55EC | Global resource manager pointer |
| 0x82907278 | Resource lock/critical section |
| 0x820BBFE4 | Callback vtable object (audio resource loader) |
| 0x8207A57C | Resource name string |
| sub_828470E0 | TLS thread-affinity enter (non-blocking) |
| sub_82847120 | TLS thread-affinity exit (non-blocking) |
| sub_82A13040 | WaitForSingleObject wrapper (NOT in this chain) |
| sub_82A1A450 | NtWaitForSingleObjectEx caller (NOT in this chain) |

## Semaphore/Event Functions

| Function | Behavior | In sub_82852D18 chain? |
|-|-|-|
| sub_82849790 | NtWaitForSingleObjectEx(handle, timeout=0) -- non-blocking poll | No |
| sub_828497D8 | NtWaitForSingleObjectEx(handle, timeout=INFINITE) -- **BLOCKING** | No |
| sub_82849820 | NtWaitForSingleObjectEx(handle, timeout=caller) -- varies | No |
| sub_82A13040 | Sets alertable=0, calls sub_82A1A450 | No |
| sub_82A1A430 | Timeout converter: -1 -> NULL(infinite), else ms*-10000 | No |
