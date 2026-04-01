# sub_8286C238 -- rage::datResourceInfo Visitor (pgStreamable tree traversal)

## Identity

- **Address**: 0x8286C238
- **File**: `gta4_recomp.56.cpp` line 43805
- **Stack frame**: 416 bytes
- **Return value**: void
- **Class**: `rage::datResourceInfo` / `rage::datResource` visitor infrastructure (pgStreamable system)

## Vtable Membership

| Vtable address | Index | Context |
|-|-|
| 0x82087930 | [3] | pgStreamable vtable (strings: "[Broken pgStreamable]", "[Invalid pgStreamable]") |
| 0x82087A70 | [3] | Visitor vtable (string "Visitor" at 0x82087A90 follows immediately) |
| 0x8212D3E0 | method table entry | datResource method dispatch table (30+ methods in 0x8286xxxx range) |

**Vtable 1** (0x82087930): `[0]=0x820FE780 (dtor), [1]=0x8286C828, [2]=0x8286C750, [3]=sub_8286C238`

**Vtable 2** (0x82087A70): `[0]=0x820FE810 (dtor), [1]=0x8286D348, [2]=0x822BCA90 (NOP), [3]=sub_8286C238`

**Note**: In both known vtables, sub_8286C238 is at index **3** (offset 0xC), not index 2. The diagnostic hook reported vtable[2] dispatch resolving here, which implies either a different vtable was active at runtime, or the hook uses 0-based counting from the first virtual method (skipping the destructor at [0]).

## RTTI / Class Names

No RTTI Complete Object Locator found at vtable[-1] for either vtable (Xbox 360 may have stripped RTTI). Related RTTI strings found in PE:

- `.?AVpgBase@rage@@` at 0x82B2881C
- `.?AVpgStreamableRefBase@rage@@` at 0x82AADE9C
- `.?AVparInstanceVisitor@rage@@` at 0x82AF96E8
- `.?AVparBuildTreeVisitor@rage@@` at 0x82B07E98
- `.?AVparInitVisitor@rage@@` at 0x82B07EC0

## Step-by-Step Behavior

1. **Prologue**: Saves GPRs r14-r31, allocates 416-byte stack frame. Captures `this` (r3->r24), arg2 (r4->r30), arg3 (r5->r26). Loads global ptr at `[0x831E55EC]` (game engine config/state).

2. **Version check** (optional debug path): Reads halfword at `this+28`, masks to 10 bits. If non-zero, calls `sub_822BCA90` (NOP debug trace), then reads a global flag bit. If flag set, calls `sub_828540B8` (version comparator) comparing `this+28` vs `arg2+16`. If result == 3 (versions equal), skips debug logging. Otherwise logs version mismatch via `sub_822BCA90` with format string at 0x820878D8.

3. **Pre-visit dispatch**: Calls `sub_8286E1E0` (recursive tree visitor) passing `(this, arg3, fmt_string_0x8207C4FC, arg2)`. This walks the resource tree firing pre-visit callbacks through indirect vtable calls.

4. **Array build**: Calls `sub_8286BF20` which takes `(stack_buf+128, this, arg3)` and builds two parallel arrays from a linked list at `arg2+28`. Stores node pointers and associated data pointers at `obj+0` / `obj+68` with incrementing counters.

5. **Dirty flag clear**: If global flag bit 3 is set and the linked list head is non-null, walks the list clearing bit 2 (mask `& 0xFB`) in each node's byte at offset +4.

6. **Main traversal loop** (loc_8286C3B0 -- loc_8286C5C8): Iterates backward through the array built in step 4. For each entry:
   - Reads a resource descriptor at `entry+24` (halfword count field)
   - Inner loop iterates over sub-entries, loading resource object pointers
   - **Vtable dispatch [1]** (getName): Calls object's vtable[1] to get name string
   - **Name comparison**: Uses `_stricmp` (rexcrt) or byte-by-byte compare depending on global flag
   - If name mismatch: calls `sub_82863248` (linked list advance) and loops
   - If name matches:
     - **Vtable dispatch [2]** (getSize): Calls object's vtable[2] for size/offset
     - Calls `sub_8286C098` (observer dispatch) with computed offset + format string 0x82087898
     - **Vtable dispatch [8]**: Calls object's vtable[8] with `(obj, node, offset)`
     - Calls `sub_8286C098` again with format string 0x820878A0
     - Sets dirty bit (OR 0x4 into node byte at +4) if flag bit 3 set
   - After match processing: calls `sub_82863248` to advance linked list
   - If no match found and global flag bit 4 set: calls getName again via vtable, logs via `sub_822BCA90` with format string 0x820878A8
   - If global flag bit 8 set: initializes a small struct via `sub_8286D398`, then calls `sub_82872850` (conditional visitor)

7. **Post-dirty-flag report** (loc_8286C5E4): If flag bit 3 set, walks remaining unvisited nodes. For any node without dirty bit 2, logs name + identifier via `sub_822BCA90` with format string 0x82087864.

8. **Post-visit dispatch**: Calls `sub_8286C180` (recursive cleanup dispatcher) with `(this, arg3, fmt_string_0x8207C4F0)`.

9. **Epilogue**: Restores stack and GPRs r14-r31, returns.

## Direct Sub-calls

| Function | Purpose | Blocking? |
|-|-|-|
| sub_822BCA90 | NOP / debug trace stub (just `blr`) | No |
| sub_828540B8 | Version comparator: compares two packed 32-bit version fields. Returns 0-4 | No |
| sub_8286E1E0 | Recursive tree visitor: walks child nodes (offset +8), fires callbacks through vtable indirect calls and sub_82A01900 (bsearch) | No |
| sub_8286BF20 | Array builder: walks linked list (offset +8/+12/+24), stores to dual arrays at obj+0/+68 | No |
| sub_82863248 | Linked list advance: `node = node->next(+24)`; returns 1 if reached sentinel | No |
| sub_8286C098 | Recursive observer dispatch: calls sub_8284E060 (key hash?), sub_82A01900 (bsearch), then indirect callback. Recurses via child pointer at obj+8 | No |
| sub_8286D398 | Small struct init: stores vtable ptr, 0xFFFF halfword, two zero bytes | No |
| sub_82872850 | Conditional visitor: calls vtable[16] on object, then sub_8286D5F0 if bit test passes | No |
| sub_8286C180 | Recursive cleanup: same pattern as sub_8286C098 but with 3 args instead of 6. Recurses via child at obj+8 | No |
| rexcrt__stricmp | Case-insensitive string compare (hooked CRT at 0x829FFCF0) | No |

## Indirect Vtable Calls

| Location (return addr) | Vtable offset | Likely method | Object |
|-|-|-|-|
| 0x8286C414 | vtable[1] | getName() | resource entry (r30) |
| 0x8286C4A0 | vtable[2] | getSize() / getOffset() | resource entry (r30) |
| 0x8286C4DC | vtable[8] | visit/apply callback | resource entry (r30) |
| 0x8286C560 | vtable[1] | getName() (debug log path) | resource entry (r30) |

These are all dispatches on the **resource entry** objects being visited, NOT on `this`.

## GPU / D3D Hardware

**None**. No calls to 0x82A4xxxx (D3D device layer). No Xenos register writes. No PM4 ring buffer access. No D3D device creation/submission.

## Sync Primitives

**None**. No semaphores, events, mutexes, critical sections, or blocking waits. All sub-calls are pure computation or memory traversal.

## Hang Risk Assessment

The function itself has **no blocking primitives**. Potential hang sources:

1. **Linked list corruption**: The inner loop uses `sub_82863248` to advance through a linked list. If the list is circular (node->next(+24) never reaches the sentinel stored at stack+84), the inner loop at `loc_8286C3E4` will spin forever.

2. **Recursive depth**: `sub_8286C098`, `sub_8286C180`, and `sub_8286E1E0` are all recursive (recurse via child pointer at obj+8). A deeply nested or circular resource tree could cause stack overflow.

3. **Indirect call targets**: The four vtable dispatches call unknown functions on resource entry objects. If any of those targets block (e.g., accessing GPU state, waiting on I/O), the function would appear to hang.

4. **The array loop count** comes from `stack+192` (set by `sub_8286BF20`). If this value is very large due to a corrupt linked list, the outer loop runs excessively.

**Most likely hang cause**: One of the indirect vtable calls (getName, getSize, or the vtable[8] apply method) on a resource entry object either accesses GPU state or encounters a null/corrupt vtable pointer, causing a fault or infinite dispatch loop.

## Global State Dependency

The function reads `[0x831E55EC]->ptr->offset_40` and extracts individual flag bits (bits 3, 4, 5, 8, 15, 16) to control debug logging, dirty-flag tracking, and visitor dispatch paths. This is likely a `rage::datResourceInfo` or engine configuration singleton.

## Key Constants (computed via Python)

| Expression | Value | Usage |
|-|-|-|
| lis r28,-31970 | 0x831E0000 | Base of global data segment |
| 0x831E0000 + 21996 | 0x831E55EC | Engine config singleton pointer |
| lis r11,-32248 | 0x82080000 | Base of format string region (.rdata) |
| 0x82080000 - 15108 | 0x8207C4FC | Pre-visit format string |
| 0x82080000 - 15120 | 0x8207C4F0 | Post-visit format string |
| 0x82080000 + 30936 | 0x820878D8 | Version mismatch format string |
| 0x82080000 - 10388 | 0x8207D76C | Default vtable for small struct init |
| 0x82080000 + 22636 | 0x8208586C | Debug visitor format |
| 0x82080000 + 30888 | 0x820878A8 | Post-match format string |
| 0x82080000 + 30880 | 0x820878A0 | Apply-result format string |
| 0x82080000 + 30872 | 0x82087898 | Pre-apply format string |
| 0x82080000 + 30820 | 0x82087864 | Unvisited-node warning format |
| 0x82080000 + 31028 | 0x82087934 | sub_8286C650 format string |
