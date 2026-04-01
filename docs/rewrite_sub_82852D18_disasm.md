# sub_82852D18 Raw PPC Disassembly Analysis

## File Offset

```
PE base:    0x82000000
Target:     0x82852D18
File offset: 0x852D18
```

## sub_82852D18 — Full Disassembly

```asm
; Prologue (save GPR via helper, allocate 0x80 stack frame)
0x82852D18:  mflr       r12
0x82852D1C:  bl         savegpr_prolog           ; 0x829FF7C4
0x82852D20:  stwu       r1, -0x80(r1)

; r3 = context, r5 = callback_obj, r6 = callback_arg
0x82852D24:  mr         r30, r3                  ; r30 = context
0x82852D28:  mr         r29, r5                  ; r29 = callback_obj (vtable ptr at [r29])
0x82852D2C:  mr         r28, r6                  ; r28 = callback_arg

; Call sub_82852A50(context, r4) — "get resource ptr"
0x82852D30:  bl         sub_82852A50             ; r3,r4 passed through from caller
0x82852D34:  mr         r27, r3                  ; r27 = resource_ptr (from sub_82852A50)

; Null check: if resource_ptr == 0, early return
0x82852D38:  cmplwi     cr6, r27, 0
0x82852D3C:  bne        cr6, 0x82852D48
0x82852D40:  addi       r1, r1, 0x80             ; tear down frame
0x82852D44:  b          restgpr_epilog           ; 0x829FF814 — return (r3 undefined)

; Load first field of resource: r31 = resource_ptr->field_0
0x82852D48:  lwz        r31, 0(r27)
0x82852D4C:  cmplwi     cr6, r31, 0
0x82852D50:  bne        cr6, 0x82852D5C
0x82852D54:  li         r30, 0                   ; result = 0 (field_0 is null)
0x82852D58:  b          0x82852D88               ; skip to cleanup

; ---- Core operation (field_0 not null) ----
; sub_82851A10(context, field_0->name_str) — "pre-vtable-call" / name lookup
0x82852D5C:  mr         r3, r30                  ; r3 = context
0x82852D60:  lwz        r4, 0(r31)               ; r4 = *field_0 (name string ptr)
0x82852D64:  bl         sub_82851A10             ; lookup by name in hash table

; Indirect call via callback_obj vtable slot 2 (offset +8)
0x82852D68:  lwz        r11, 0(r29)              ; r11 = callback_obj->vtable
0x82852D6C:  mr         r5, r28                  ; r5 = callback_arg
0x82852D70:  mr         r4, r31                  ; r4 = field_0
0x82852D74:  mr         r3, r29                  ; r3 = callback_obj (this)
0x82852D78:  lwz        r11, 8(r11)              ; r11 = vtable[2] (3rd slot)
0x82852D7C:  mtctr      r11
0x82852D80:  bctrl                               ; INDIRECT CALL: vtable[2](this, field_0, arg)

0x82852D84:  li         r30, 1                   ; result = 1 (success)

; ---- Cleanup / teardown ----
0x82852D88:  lis        r11, -0x7CE2             ; 0x831E0000
0x82852D8C:  lwz        r11, 0x55EC(r11)         ; r11 = *(0x831E55EC) — global ptr
0x82852D90:  lwz        r11, 0x28(r11)           ; r11 = global->flags
0x82852D94:  rlwinm     r31, r11, 0xF, 0x1F, 0x1F ; extract bit 17 → r31
0x82852D98:  cmplwi     cr6, r31, 0
0x82852D9C:  beq        cr6, 0x82852DA4
0x82852DA0:  bl         sub_828470E0             ; debug/profiling enter

; sub_8284FA58(resource_ptr) — teardown: iterates resource children, frees each via sub_821B3560
0x82852DA4:  mr         r3, r27
0x82852DA8:  bl         sub_8284FA58

; sub_821B3560(resource_ptr) — free: loads allocator from TLS[r13+0x68C], calls vtable[3](ptr)
0x82852DAC:  mr         r3, r27
0x82852DB0:  bl         sub_821B3560

0x82852DB4:  cmplwi     cr6, r31, 0
0x82852DB8:  beq        cr6, 0x82852DC0
0x82852DBC:  bl         sub_82847120             ; debug/profiling exit

; Return r30 (0 or 1)
0x82852DC0:  mr         r3, r30
0x82852DC4:  addi       r1, r1, 0x80
0x82852DC8:  b          restgpr_epilog           ; 0x829FF814
```

## Answers to Specific Questions

### 1. Is sub_82852A50 called first?

**Yes.** `sub_82852A50` is the very first function call at `0x82852D30`, immediately after the prologue saves registers. It receives `r3` (context) and `r4` (passed through from caller) and returns the "resource pointer" in `r3`, which is stored in `r27`.

sub_82852A50 itself:
- Calls `sub_82852300` (resource lookup by name, iterates a hash table)
- Calls `sub_82851DF0` (hash-table node lookup — `divwu`-based hash, walks linked list via `+0x38` chain)
- Makes 3 indirect calls (bctrl) through vtable slots at offsets `+0xC`, `+0x4`, and `+0x0`
- Returns a reference-counted object (calls `sub_8284FC98` for addref)

### 2. Is there a bctrl (indirect call) after sub_82851A10?

**Yes.** At `0x82852D80` there is a `bctrl` — an indirect call through `callback_obj->vtable[2]` (vtable slot at offset `+8`). The setup is:
```
r11 = *(r29)          ; load vtable ptr from callback_obj
r11 = *(r11 + 8)      ; vtable[2]
ctr = r11
bctrl                  ; call vtable[2](callback_obj, field_0, callback_arg)
```
This is the **only** `bctrl` in `sub_82852D18` itself.

### 3. Any loops (backward branches)?

**No backward branches in sub_82852D18.** All conditional branches jump forward. The function is strictly linear: lookup → null-check → name-resolve → vtable-call → cleanup → return.

However, **sub_8284FA58** (called at teardown) has a loop:
```
0x8284FA7C:  lwz  r11, 4(r30)          ; array base
0x8284FA80:  lwzx r3, r11, r31         ; array[i]
0x8284FA84:  bl   sub_821B3560          ; free(array[i])
0x8284FA88:  lhz  r11, 8(r30)          ; count
0x8284FA8C:  addi r29, r29, 1          ; i++
0x8284FA90:  addi r31, r31, 4          ; offset += 4
0x8284FA94:  cmpw cr6, r29, r11
0x8284FA98:  blt  cr6, 0x8284FA7C      ; ← LOOP back if i < count
```
This iterates over child resources, freeing each one.

### 4. Any calls to sync primitives?

**No.** Neither `sub_82852D18` nor any of its direct callees reference `KeWaitForSingleObject`, `KeSetEvent`, `KeResetEvent`, `KeInitializeEvent`, `NtCreateEvent`, `ExCreateThread`, or `KeResumeThread`. The function is purely synchronous.

## Call Graph Summary

```
sub_82852D18(context, name, callback_obj, callback_arg)
  |
  +-- sub_82852A50(context, name) → resource_ptr
  |     +-- sub_82852300(name, ...) — resource lookup (hash table iteration)
  |     |     +-- sub_8285AD08 — string compare (4-byte prefix check)
  |     |     +-- sub_828D0608 — secondary lookup
  |     |     +-- sub_82851C40 — hash iteration advance
  |     |     +-- bctrl — vtable match callback
  |     +-- sub_82851DF0(context, hash_key) — hash-table node find
  |     |     (divwu hash, linked list walk via +0x38 chain)
  |     +-- bctrl — vtable[3] (create from name)
  |     +-- bctrl — vtable[1] (init with flags)
  |     +-- bctrl — vtable[0] (destructor/release)
  |     +-- sub_8284FC98 — addref
  |
  +-- sub_82851A10(context, name_str) — name-to-object lookup
  |     +-- sub_82912948 — hash table search (+0x18 from context)
  |     (returns object ptr or 0)
  |
  +-- bctrl — callback_obj->vtable[2](this, field_0, callback_arg)
  |
  +-- sub_8284FA58(resource_ptr) — teardown children
  |     +-- sub_821B3560(child) — free each child (loop)
  |     +-- sub_82863628 — finalize main node
  |     +-- sub_821B3560(main_node)
  |     +-- sub_821B3560(array_ptr) — free backing array
  |
  +-- sub_821B3560(resource_ptr) — free resource
        (loads allocator from TLS[r13+0x68C], tail-calls vtable[3])
```

## Key Observations

1. **sub_82852D18 is a "use and discard" operation**: it looks up a resource by name via `sub_82852A50`, optionally invokes a callback on it, then unconditionally tears it down (`sub_8284FA58` + `sub_821B3560`). The resource is freed regardless of whether the callback succeeded.

2. **The single bctrl at 0x82852D80** dispatches through `callback_obj->vtable[2]`. This is where the actual "operation" happens (per the INIT_PROBE label "82852DD0 operation"). If this vtable slot points to a GPU-related function (e.g., shader compilation via the Xenos command buffer), it would block in the recompiled environment.

3. **No sync primitives, no loops, no blocking waits** in `sub_82852D18` itself. Any blocking would come from the indirect call at `0x82852D80` or from inside `sub_82852A50`'s vtable dispatches.

4. **sub_821B3560 is the allocator free path**: reads `TLS[r13 + 0x68C]` to get the heap allocator context, then tail-calls through its vtable slot 3 (the dealloc method). This matches the existing `sub_8218BE28` analysis (the malloc equivalent reads the same TLS offset).

5. **Debug/profiling guards** at `0x82852DA0`/`0x82852DBC` (`sub_828470E0`/`sub_82847120`) are conditional on bit 17 of `*(*(0x831E55EC) + 0x28)` — a global profiling-enabled flag. These are the same guards seen in `sub_82852A50`.
