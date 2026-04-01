# Allocator vtable dispatch chain (operator new → TLS[1676] → vtable[2])

## Summary

`sub_821B3510` (operator new) and `sub_8218BE28` (RAGE malloc) are **identical thunks**:

```
lwz  r11, 0(r13)        ; TLS base
li   r10, 0x68C          ; offset 1676
mr   r4, r3              ; r4 = size
li   r6, 0               ; flags = 0
li   r5, 0x10            ; alignment = 16
lwzx r3, r10, r11        ; r3 = TLS[1676] = allocator object
lwz  r11, 0(r3)          ; vtable pointer
lwz  r11, 8(r11)         ; vtable[2] = Allocate
mtctr r11
bctr                      ; tail-call Allocate(allocator, size, 16, 0)
```

Related thunks:
- `sub_8218BE50`: same but r5=r4 (explicit alignment arg)
- `sub_8218BE78`: Free — calls vtable[3] (offset +12)
- `sub_828474B8`: calls vtable[13] (offset +52) for allocator info

## TLS[1676] initialization

**sub_82849940** sets TLS[1676] during thread startup:
1. Takes a 44-byte thread descriptor struct as arg
2. Copies to stack: `memcpy(stack+80, arg, 44)`
3. Stores `stack[88]` (descriptor+8 = allocator ptr) to both TLS[1676] and TLS[1680]
4. Calls `stack[80]` (descriptor+0) as a function pointer (thread entrypoint)

The TLS allocator stack uses three slots:
- TLS[1668] = push/pop refcount
- TLS[1672] = saved (previous) allocator
- TLS[1676] = current allocator (used by operator new)
- TLS[1680] = target/default allocator

Push (sub_828470E0 / sub_827D85E0) swaps TLS[1672]/TLS[1676] with TLS[1680].
Pop (sub_82847120 / sub_827D8620) restores from TLS[1672] when refcount hits 0.

## Allocator vtable: rage::sysMemMultiAllocator

Constructed by **sub_829E7510** at 0x829E753C: `stw r10, 0(r3)` where r10 = **0x820A6074**.

Vtable at 0x820A6074 (20 entries):

|Idx|Offset|Address|Role|
|-|-|-|-|
|0|+0|0x829E7558|destructor/reset|
|1|+4|0x826F26B8|stub (return 0)|
|2|+8|0x829E7338|**Allocate(this, size, align, flags)**|
|3|+12|0x829E75B8|**Free(this, ptr)**|
|4|+16|0x828E0AB8|nop (blr)|
|5|+20|0x828E0AB8|nop|
|6|+24|0x829E76E8|resize/realloc|
|7|+28|0x829E73D8|GetSize|
|8|+32|0x829E7420|GetTotalSize|
|9|+36|0x829E7470|GetUsedSize|
|10|+40|0x826E9E00|stub (return 1)|
|11|+44|0x828E0AB8|nop|
|12|+48|0x826F26B8|stub (return 0)|
|13|+52|0x828E0AB8|nop (called by sub_828474B8)|
|14|+56|0x828E0AB8|nop|
|15|+60|0x826F26B8|stub (return 0)|
|16|+64|0x826F26B8|stub (return 0)|
|17|+68|0x828E0AB8|nop|
|18|+72|0x829E74C0|ValidateHeap|
|19|+76|0x8278DDF8|unknown|

## vtable[2] = sub_829E7338 (pool/bucket Allocate)

```
sub_829E7338(this, size, align, flags):
  bucket_count = this->field_04
  for i = 0..bucket_count-1:
    threshold = this->size_thresholds[i]  // at this+0x28 + i*4
    if size > threshold: continue
    break
  if i >= bucket_count: return NULL       // size too large for any pool

  alloc_count = this->alloc_counts[i]     // at this+0x50+i (byte)
  capacity    = this->capacities[i]       // at this+0x48+i (byte)
  if alloc_count == capacity: return NULL  // pool full

  this->alloc_counts[i]++
  base   = this->pool_bases[i]            // at this+8+i*4
  stride = this->slot_sizes[i]            // at this+0x28+count+i*4
  slot   = capacity - alloc_count + alloc_counts[i] - 1  // computed index
  return base + slot * stride
```

**No blocking operations**: no locks, no waits, no syscalls, no function calls.
Pure arithmetic — O(n) bucket scan where n = number of size classes (typically 4-8).

## Fallback when pool returns NULL

When sub_829E7338 returns NULL (size too large or pool full), the caller chain differs:

- **sub_8218BE28 / sub_821B3510**: tail-call to vtable[2], so NULL propagates directly to caller. No fallback.
- **sub_8218BF20** (internal alloc with packed flags): calls sub_828B0068 or sub_828AFF60, which load the **global heap objects** at 0x8312B7D0/0x8312B7D8 and call THEIR vtable[2]. This is a separate dispatch path not reached from operator new.

The hook at `imports.cpp:901` catches NULL/broken TLS[1676] in sub_8218BE28 and routes to RexGlue SystemHeapAlloc as a safety net.

## Global heap objects

Registered by sub_828B01A0 (sysMemInit), called from sub_82893060:

|Global|Address|Role|
|-|-|-|
|physical heap|0x8312B7D0|Primary allocator for physical memory|
|physical sub-alloc|0x8312B7D4|Pool allocator backed by physical heap|
|virtual heap|0x8312B7D8|Primary allocator for virtual memory|
|virtual sub-alloc|0x8312B7DC|Pool allocator backed by virtual heap|

The physical/virtual heaps have vtable at **0x82000970** (set by sub_8218C74C).
Their vtable[2] = 0x828572F8 -> sub_82856D48 (timing/profiling wrapper around the real allocator).

## sub_8218BF98 — NOT the allocator vtable

The address 0x8218BF98 stores **switch table entries** for sub_8218BF20 (internal alloc with packed flags). The entries set alignment before a common dispatch body. This is populated by vtable_prepopulate.h but is NOT a C++ virtual function table.

## sub_8218BE28 — is it the vtable[2] target?

**No.** sub_8218BE28 is the outer dispatcher that CALLS vtable[2]. The actual vtable[2] target is sub_829E7338 (pool allocator) for sub-allocator objects, or sub_82856D48 (profiled heap allocator) for the primary heap objects.

## Blocking operations in the allocation chain

For the normal path (TLS[1676] → sub_829E7338):
- **Zero blocking operations.** Pure register/memory arithmetic.

For the fallback path (sub_8218BF20 → sub_828B0068/sub_828AFF60 → heap vtable[2]):
- sub_82856D48 reads `mftb` (time base) for profiling — non-blocking.
- Calls sub_828E0AB8 (critical section acquire/release) — **potential blocking point** if contended.

For the RexGlue fallback (hook in imports.cpp):
- Calls `SystemHeapAlloc` — host-side allocation, not guest-blocking.
