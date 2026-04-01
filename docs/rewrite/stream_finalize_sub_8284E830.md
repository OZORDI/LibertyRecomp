# sub_8284E830 — Streaming Path Stack Pop

## Identity

- **Address**: 0x8284E830
- **Size**: 3 instructions (12 bytes)
- **Object**: Path Manager at 0x82B07278
- **Field**: offset 0xC00 (3072) = path stack depth counter
- **Counter address**: 0x82B07E78 (when called with r3=0x82B07278)

## Disassembly

```
lwz  r11, 3072(r3)    ; load counter
addi r11, r11, -1     ; decrement
stw  r11, 3072(r3)    ; store back
blr
```

Equivalent C: `this->pathStackDepth--;`

## Path Manager Layout (0x82B07278)

| Offset|Field|Purpose|
|-|-|-|
| 0xC00 (3072) | pathStackDepth | Current number of pushed paths |
| 0xC04 (3076) | deviceCount | Number of registered VFS devices |
| 0xC08 (3080) | defaultDeviceIdx | Default device for path resolution |
| (n+4)*256 | pathSlots[n] | 256-byte path string buffers |

## Paired Function: sub_8284F310 (Path Push)

sub_8284F310(pathMgr, pathStr) does:

1. Calls sub_8284E690(pathStr) to check if path is relative (no leading `/`, `\`, or `:`)
2. If relative and counter > 0: copies current path prefix from slot `(counter+3)*256`
3. If relative and counter == 0: loads a default path from a global string
4. Appends the new path component into the prefix buffer (stack frame local)
5. Calls sub_82211840(localBuf, pathStr, 256) to join/normalize the path
6. Copies result into slot `(counter+4)*256` via sub_8284EAF0 (strncpy + backslash-to-slash)
7. Increments counter: `pathMgr->pathStackDepth++`

sub_8284EAF0 additionally converts `\` to `/` in the stored path.

## Call Chain in sub_82478AF8 (Engine Init)

At address 0x82478E84:

```
1. sub_8284F310(0x82B07278, pathStr)      ; push path, counter N -> N+1
2. sub_82477670()                          ; create streaming ctx (912-byte alloc)
                                           ;   stores ctx ptr at 0x82B493A4
3. sub_827C2420(streaming_ctx)             ; streaming activation
   |-> sub_8284F310(0x82B07278, ctx+56)   ;   push ctx path, counter N+1 -> N+2
   |-> vtable[1](streaming_ctx)           ;   virtual method call
   |-> sub_82852DD0(...)                  ;   device registration
   |   |-> sub_8284F468(pathMgr, ...)     ;     iterate devices, try open
   |   |   |-> sub_8284F0C0(...)          ;       build full path from stack
   |   |   |-> sub_8285AA68(path, flag)   ;       attempt device open (vtable call)
   |   |-> sub_82852D18(...)              ;     register device binding
   |   |   |-> sub_82852A50(path)         ;       device lookup
   |   |   |-> sub_82851A10(...)          ;       set device
   |   |   |-> vtable[8](dev, ...)        ;       device init callback
   |   |   |-> sub_828470E0()             ;       [conditional] heap context push
   |   |   |-> sub_8284FA58() + free      ;       cleanup old binding
   |   |   |-> sub_82847120()             ;       [conditional] heap context pop
   |   |-> sub_8285B088(dev)              ;     release device ref
   |-> sub_8284E830(0x82B07278)           ;   pop path, counter N+2 -> N+1
4. sub_8284E830(0x82B07278)                ; pop path, counter N+1 -> N
5. sub_821B3510(1024)                      ; 1024-byte alloc <-- SOMETIMES HANGS
```

## Lock/Critical Section Analysis

**sub_8284E830**: No locks. Pure data write, no branches, no calls.

**sub_827C2420**: No direct locks. Sub-call sub_82852D18 has conditional heap context push/pop via sub_828470E0/sub_82847120. These are BALANCED: both guarded by the same condition (bit 17 of `*(*(0x831E55EC) + 40)`).

**sub_82852DD0**: No direct lock manipulation. Delegates to sub_82852D18.

**sub_82852D18 heap context**:
- Push (sub_828470E0): saves TLS[1676] to TLS[1672], sets TLS[1676] = TLS[1680]
- Pop (sub_82847120): restores TLS[1676] from TLS[1672]
- Both conditional on same flag — always balanced

**Conclusion**: No lock is held on exit from this call sequence.

## Streaming State on Entry vs Exit

| Property | Before | After |
|-|-|-|
| Path stack depth | N | N (balanced) |
| Streaming ctx ptr at 0x82B493A4 | may be null | points to new 912-byte object |
| Streaming ctx vtable | - | set to 0x820FC054 |
| Streaming ctx +56 | - | path string pointer |
| Device binding | not registered | registered via sub_82852DD0 |
| TLS heap context [1676] | allocator A | allocator A (unchanged) |

## Relationship to the 1024-Byte Alloc Hang

sub_8284E830 **cannot** cause the hang. It is a trivial decrement with no side effects beyond the counter.

sub_827C2420 **cannot** leave corrupted heap context — all push/pop operations are balanced.

The 1024-byte alloc (sub_821B3510) works as follows:
```
r11 = *(r13+0)         ; TLS thread block
r3  = *(r11+1676)      ; TLS heap allocator context
vtable = *(r3)
call *(vtable+8)(r3, 1024, 16, 0)   ; allocator->Alloc(size, align, flags)
```

Possible hang causes in sub_821B3510:
1. **TLS[1676] is null** — vtable read from address 0x0 causes fault or infinite retry
2. **TLS[1676] points to wrong allocator** — allocator's internal lock contention with another thread
3. **Allocator itself is locked** — another thread holds the allocator's mutex and is blocked
4. **Allocator memory exhaustion** — internal retry loop waiting for memory to be freed

None of these relate to sub_8284E830 or the path stack.

## What a Native Rewrite Needs

For sub_8284E830:
```cpp
// PathManager::PopPath()
void PathManager_PopPath(PathManager* mgr) {
    mgr->pathStackDepth--;
}
```

For the surrounding call sequence, the native rewrite must ensure:
1. The path stack push/pop is balanced (same as original)
2. The streaming context object is properly initialized before sub_827C2420
3. The VFS device registration in sub_82852DD0 succeeds (device must exist)
4. TLS heap allocator context is valid before the 1024-byte alloc
5. The heap context push/pop in sub_82852D18 remains balanced

The hang investigation should focus on:
- **TLS[1676] state** at the point of the 1024-byte alloc
- Whether the calling thread's TLS heap is properly initialized
- Whether sub_82477670's 912-byte alloc (which happens earlier) succeeded, as failure zeros out the streaming ctx pointer at 0x82B493A4
