# TLS Allocator Context Save/Restore During Streaming Activation

## Summary

The TLS allocator save/restore mechanism (sub_828470E0 / sub_82847120) is gated by
bit 17 of the streaming coordinator flags at `coordinator+40`. This bit is **never
set** at runtime -- all coordinator init paths write 0 to the flags field. The
save/restore is therefore always skipped, and TLS[1676] corruption via this path
is not the root cause of allocator fallback storms.

## Functions Analyzed

| Function | Role | File:Line |
|-|-|
| sub_828470E0 | Save TLS allocator context | gta4_recomp.55.cpp:2436 |
| sub_82847120 | Restore TLS allocator context | gta4_recomp.55.cpp:2475 |
| sub_82852A50 | Streaming resource lookup (inner) | gta4_recomp.55.cpp:30531 |
| sub_82852D18 | Streaming resource processor | gta4_recomp.55.cpp:30962 |
| sub_82852DD0 | OpenAndProcess (outer wrapper) | gta4_recomp.55.cpp:31076 |
| sub_827C2420 | Streaming activation entry point | gta4_recomp.50.cpp:57394 |
| sub_8286C238 | Vtable[2] dispatch target (streaming) | gta4_recomp.56.cpp:43805 |
| sub_82851918 | Streaming coordinator update/flush | gta4_recomp.55.cpp:27973 |

## sub_828470E0 (Save) Semantics

```
tls_base = MEM[r13+0]
active   = TLS[tls_base+1676]    // current allocator context
backup   = TLS[tls_base+1680]    // default allocator context
if (active == backup):
    TLS[tls_base+1668] += 1      // nesting counter; already on default
else:
    TLS[tls_base+1676] = backup   // switch active to default
    TLS[tls_base+1672] = active   // stash old active
```

## sub_82847120 (Restore) Semantics

```
tls_base = MEM[r13+0]
nesting  = TLS[tls_base+1668]
if (nesting != 0):
    TLS[tls_base+1668] -= 1      // decrement nesting
else:
    saved = TLS[tls_base+1672]
    TLS[tls_base+1672] = 0       // clear stash
    TLS[tls_base+1676] = saved   // restore active from stash
```

Purpose: bracket code that must use the **default** heap allocator, saving and
restoring whatever custom allocator was active. Uses a nesting counter for
recursive save regions.

## Bit 17 Gating Analysis

Every streaming function reads the coordinator flags the same way:

```
lis  r11, -31970               // r11 = 0x83020000
lwz  r11, 21996(r11)           // r11 = MEM[0x831E55EC] → coordinator ptr
lwz  r11, 40(r11)              // r11 = flags word at coordinator+40
rlwinm rN, r11, 15, 31, 31    // rN = (flags >> 17) & 1
cmplwi cr6, rN, 0
beq  cr6, skip_save            // skip if bit 17 == 0
bl   sub_828470E0              // save TLS only if bit 17 == 1
```

### Coordinator Flags Initialization

All coordinator init paths (sub_827C2498, and 9+ other init functions in
gta4_recomp.50.cpp) follow the same pattern:

```
stw  r8, 88(r1)     // r8 = 0 → high word of 64-bit pair
stw  r10, 92(r1)    // r10 = function pointer → low word
ld   r10, 88(r1)    // load 64-bit: {0, fn_ptr}
std  r10, 40(r11)   // store to coordinator: offset+40=0, offset+44=fn_ptr
```

The high word at coordinator+40 (the flags) is always **0**.

### Runtime Modification of Flags

sub_82851918 is the only function that modifies coordinator flags at runtime.
It manipulates bits 11, 12, 13 (masks 0x800, 0x1000, 0x2000) via XOR operations
for streaming update/flush control. **Bit 17 (0x20000) is never set.**

No function across the generated code writes 0x20000 to the coordinator flags.

## Call Chain: sub_827C2420 → sub_82852DD0 → sub_82852D18 → sub_82852A50

```
sub_827C2420 (streaming activation)
  ├── sub_8284F310 (start streaming mgr)
  ├── vtable[1] dispatch on streaming object
  └── sub_82852DD0 (OpenAndProcess)
        ├── sub_8284F468 (find/alloc resource)
        └── sub_82852D18 (process resource)
              ├── sub_82852A50 (get resource ptr)
              │     ├── [SKIPPED] sub_828470E0 (save TLS)     ← bit17=0
              │     ├── sub_82852300 (hash lookup)
              │     ├── sub_82851DF0 (node lookup)
              │     ├── vtable[3] dispatch → allocate resource
              │     ├── vtable[1] dispatch → configure resource
              │     ├── sub_8284FC98 (finalize)
              │     ├── vtable[0] dispatch → cleanup
              │     └── [SKIPPED] sub_82847120 (restore TLS)  ← bit17=0
              │
              ├── [SKIPPED] sub_828470E0 (save TLS)           ← bit17=0
              ├── sub_8284FA58 (release resource)
              ├── sub_821B3560 (free)
              └── [SKIPPED] sub_82847120 (restore TLS)        ← bit17=0
```

## sub_8286C238 and TLS

sub_8286C238 does **not** access r13 (TLS base), TLS[1676], or TLS[1680]
directly. It makes 5+ indirect vtable calls that could transitively invoke
sub_8218BE28 (the game's malloc). sub_8218BE28 **reads** TLS[1676] to get
the allocator context but does not **write** it. This means:

- If TLS[1676] is valid: allocations proceed normally
- If TLS[1676] is 0/uninitialized: sub_8218BE28 falls back to host page allocator

## Conclusion: TLS[1676] Is Not Corrupted By This Path

Since bit 17 is never set, the save/restore is always skipped. Since it is
always skipped symmetrically (both save AND restore are skipped), TLS[1676]
is never modified by the streaming path. The value entering sub_827C2420
is the same value present when it returns.

The "ALLOC FALLBACK storm" (sub_825BF8A8 particle emitter registration) is
not caused by TLS corruption during streaming. Instead, it occurs because:

1. The particle init loop runs on a thread where TLS[1676] was **never
   initialized** (the TLS heap context was never set for that thread), or
2. The game's heap TLS setup (which writes to TLS[1676] and TLS[1680]) has
   not yet run on that thread at the point the particle system initializes.

### Potential Fix Directions

1. **Hook sub_8218BE28**: If TLS[1676] is null, substitute a valid allocator
   context rather than falling through to host page allocation.
2. **Hook thread creation**: Ensure every guest thread has TLS[1676/1680]
   initialized with a valid heap context before any guest code executes.
3. **Force bit 17**: Write 0x20000 to coordinator flags to enable the
   save/restore mechanism, so streaming always uses the default allocator.
   This is unnecessary since TLS isn't being corrupted, but it matches
   what the game was likely designed to do on Xbox 360 with multiple heaps.
