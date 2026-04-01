# sub_82478AF8 Hang: 1024-byte Allocation at 0x82478EC4

## Call Sequence Leading to Hang

```
sub_82478AF8 (engine init)
  -> sub_826225E0, sub_82622648, sub_826226B0  (early init triple)
  -> sub_821B3510(176) -> sub_8294BD68(ptr) -> r30 = constructed object
  -> ... ~500 lines of streaming/resource setup ...
  -> sub_827C2420(streaming_ctx)               (streaming activation)
  -> sub_8284E830(r29=0x82B07278)              (streaming path finalize)
  -> sub_821B3510(1024)                        <<< HANG POINT (0x82478EC4)
```

## Register State at Alloc Call

| Reg | Value | Source |
|-|-|-|
| r29 | 0x82B07278 | `lis -32080; addi r29,r11,29304` at line 84278 -- streaming resource path object |
| r30 | object_ptr + 64 | Originally `sub_8294BD68(alloc(176))` result; `addi r30,r30,64` at line 84307 right before call. This is an offset into the 176-byte object allocated at function start. |
| r31 | stack+672 OR global ptr | Set to `r1+672` (stack local string buffer) at line 84237; conditionally overwritten from global `[0x82FF537C+4]` at line 84246 if that global is non-null. Used as a path string for resource loading. |

r27 receives the allocation result (`mr r27,r3` at line 84311).

## What the 1024 Bytes Are For

The 1024-byte allocation is the **streaming module instance** (likely `CStreaming` or an internal RAGE streaming manager). After a successful alloc:

1. **Six** `sub_8284D220` calls construct file path strings on the stack (addresses like `0x82C80060`, `0x82C4B4F0`, `0x82C43470`, `0x82C43510`, `0x82C83808`) -- these are resource archive/RPF path descriptors.
2. **Three** `sub_82478A80` calls hash the constructed paths into 16-byte identifiers.
3. A large struct is assembled on the stack (offsets 96-231) from the hashed paths.
4. `sub_827ADB48` is called with r3=r27 (the 1024-byte block) plus all the hashed path data as r4-r10 -- this is the **streaming module initialization** function that sets up internal tables, file handles, and resource registries.
5. The return value is stored into global `g_streamingModule` at `0x82FF5368`.

After that, execution reaches `loc_82479218` which allocates a **second pool**: `r30 * 176 + 16` bytes (an array of 176-byte streaming resource entries), initializes each with `sub_8261FBA0`, and stores the result into `g_streamingArray` at `0x82FF536C`.

## Failure Path (alloc returns 0 -> loc_82479208)

If the 1024-byte alloc fails (`cmplwi cr6,r27,0; beq cr6,loc_82479208`):

1. Stores 0 (r23) into `g_streamingModule` at `0x82FF5368`.
2. Falls through to `loc_82479218` which proceeds to allocate the streaming entry array anyway.
3. The game will crash later when anything dereferences the null streaming module pointer.

**There is no error handling, no log, no graceful fallback.** A null g_streamingModule is a guaranteed crash.

## Is This Allocation Necessary?

**Yes, absolutely critical.** This 1024-byte block is the core streaming module that manages all resource loading (RPF archives, textures, models, audio). Without it:

- `g_streamingModule` is null
- All streaming requests will dereference null
- The second allocation at `loc_82479250` still proceeds but its entries point nowhere

## Why It Hangs

The hang is in `sub_821B3510` itself (operator new). This is the game's `operator new` which calls the game's malloc (`sub_8218BE28`), which reads TLS at `r13+1676` for heap allocator context. If TLS heap context is null, it falls back to the host page allocator.

Possible causes:
1. **Heap TLS not initialized** for the thread running engine init -- same root cause as the particle alloc storm documented in MEMORY.md.
2. **Heap exhaustion** -- prior allocations consumed available memory.
3. **Deadlock in the allocator** -- if the fallback page allocator uses a lock that is already held.

The hang is NOT in what comes after -- the alloc call itself never returns.

## Post-Alloc Flow (if it succeeded)

After the streaming module + streaming array are set up, the function continues with:
- Another alloc for a pointer array (`r30 * 4` bytes) iterated to set type=18 on each streaming entry
- `sub_827ACC98` -- registers the pointer array with the streaming module
- `sub_827ACCA0` called 3x with bucket sizes (8192/200, 4096/300, 2048/500) -- pool allocator init for streaming buffers
- Then more world initialization continues (the remaining ~900 lines of the 1440-line function)
