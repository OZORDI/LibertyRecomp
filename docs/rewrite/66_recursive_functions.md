# 66: Recursive Function Patterns That Could Consume 5.8MB of Stack

## Executive Summary

The GTA IV recomp contains **166 directly recursive functions** and **1,739 indirect recursion
pairs** (A calls B, B calls A). The most dangerous for stack overflow is **sub_82343EF0**, which
has a 3,840-byte PPC stack frame AND both direct recursion and mutual recursion with
sub_82344368. At ~3,968 bytes per PPC frame + ~128 bytes native overhead per call, only
**~1,500 recursive iterations** would consume 5.8MB of guest stack -- matching the observed
fault at 0x705D0000.

The deepest static (non-recursive) call chain is 1,548 hops deep, but only consumes ~74KB
of native stack and ~16KB of PPC stack -- nowhere near 5.8MB. The overflow must come from
**unbounded recursion**, most likely in collision/spatial-partition tree traversal (0x8233-0x8235
address range).

---

## 1. Direct Recursion: 166 Functions

Found by scanning all 38,041 recompiled function bodies for self-calls:

### Top offenders by PPC stack frame size (recursive + frame > 256B):

| Function | PPC Frame | Likely Subsystem | Risk |
|----------|-----------|-----------------|------|
| sub_82343EF0 | 3,840B | Collision/spatial tree | **CRITICAL** |
| sub_827C89F0 | 1,312B | Unknown | HIGH |
| sub_82389050 | 784B | Unknown | MEDIUM |
| sub_8251E3E8 | 624B | Unknown | MEDIUM |
| sub_8231C5A8 | 560B | Streaming/resource | MEDIUM |
| sub_82A110A8 | 496B | CRT/runtime | LOW |
| sub_82656C60 | 496B | Unknown | MEDIUM |
| sub_828DEB68 | 464B | Unknown | MEDIUM |
| sub_8251FE00 | 464B | Unknown | MEDIUM |
| sub_823D26E0 | 464B | Unknown | MEDIUM |

Total: 41 recursive functions with frames > 256 bytes.

### Recursion depth needed to overflow 5.8MB:

```
sub_82343EF0 (3840B PPC + ~128B native):  5.8MB / 3968B = ~1,494 iterations
sub_827C89F0 (1312B PPC + ~96B native):   5.8MB / 1408B = ~4,219 iterations
sub_82389050 (784B PPC + ~64B native):    5.8MB / 848B  = ~7,000 iterations
```

### World loading range (0x8220-0x8226) -- 7 recursive functions:

- sub_8221DEE0 (160B frame)
- sub_8221E290 (176B frame)
- sub_822323E0 (320B frame)
- sub_82232A50 (320B frame)
- sub_822364B8 (frame size varies)
- sub_82249F88 (frame size varies)
- sub_82263530 (frame size varies)

These are in the world loading path but have moderate frame sizes. At 320B per iteration,
~18,000 recursions would be needed -- possible but less likely than the 0x8234 range.

---

## 2. Indirect Recursion: 1,739 Pairs

### Highest-risk mutual recursion pairs by combined PPC frame size:

| Function A | Frame A | Function B | Frame B | Combined/Cycle |
|-----------|---------|-----------|---------|---------------|
| sub_82212EC0 | 96B | sub_82212F38 | 4,064B | 4,160B |
| sub_82343EF0 | 3,840B | sub_82344368 | 128B | 3,968B |
| sub_822077D8 | 464B | sub_82208408 | 560B | 1,024B |
| sub_8220BC68 | 448B | sub_8220C148 | 624B | 1,072B |
| sub_82219490 | 480B | sub_822195D8 | 544B | 1,024B |

**sub_82212F38** is notable: 4,064-byte PPC frame in the world loading range (0x8221),
engaged in mutual recursion with sub_82212EC0. This function has 27 unique callees
and 27 labels (complex control flow). At 4,160B per cycle, only ~1,427 cycles would
overflow 5.8MB.

### 3-Cycles with large combined frames (71 found):

| Cycle | Combined Frame |
|-------|---------------|
| sub_826E2BD8 -> sub_826E2CC8 -> sub_826E2D60 | 6,640B |
| sub_826CA390 -> sub_826CA440 -> sub_826CA5C0 | 5,232B |
| sub_822A5268 -> sub_822A56D0 -> sub_822A5990 | 4,960B |
| sub_823BDBB0 -> sub_823BE4B8 -> sub_823BE7D0 | 5,200B |
| sub_829C0100 -> sub_829C0200 -> sub_829C06A0 | 4,096B |

At 6,640B per 3-cycle, only ~893 iterations would overflow 5.8MB.

---

## 3. Large Stack Frame Functions (726 total > 512B)

### Top 15 by frame size:

| Function | PPC Frame | Notes |
|----------|-----------|-------|
| sub_82364230 | 19,360B | 0x8236 range (collision/spatial) |
| sub_82368F78 | 19,328B | 0x8236 range |
| sub_82369158 | 19,296B | 0x8236 range |
| sub_828BAFD0 | 19,264B | Unknown |
| sub_82367768 | 19,200B | 0x8236 range |
| sub_82952CA8 | 19,168B | Unknown |
| sub_82952DD0 | 19,152B | Unknown |
| sub_8250B548 | 19,152B | Unknown |
| sub_82367A98 | 19,152B | 0x8236 range |
| sub_82367240 | 19,152B | 0x8236 range |
| sub_82200010 | 18,880B | **World loading entry** |
| sub_829BCAA0 | 18,496B | Unknown |
| sub_822EA918 | 18,448B | Unknown |
| sub_82195418 | 17,872B | Unknown |
| sub_822B8C40 | 17,376B | Unknown |

**sub_82200010** (18,880B frame) is at the very start of the world loading range.
If this is on the call path during init, it alone consumes 18.4KB of guest stack.

The 0x8236 range dominates the top of the list with 6 functions having ~19KB frames.
This is the same address range as the recursive collision functions, suggesting these
are related (possibly different entry points into the same spatial traversal system).

Sum of all 726 large-frame functions: 1,690,336 bytes (1.61MB) -- but these don't all
execute on the same call path.

---

## 4. Deepest Static Call Chains

### Maximum static depth by entry point:

| Entry Point | Max Depth | Notes |
|------------|-----------|-------|
| sub_82140000 | 1,172 hops | Module entry |
| sub_8218D640 | 1,149 hops | (recursive, so depth includes cycle) |
| sub_825C92D0 | 1,548 hops | **Deepest in codebase** |

### The 0x825C9 chain (deepest static path):

The globally deepest call chain starts at sub_825C92D0 (depth 1,548). These functions
are **script/AI behavior tree dispatchers**: each reads object data, tests a condition,
and dispatches to the next handler. Most have 0-byte PPC frames (leaf or tail-call-like),
with a few having 96-128B frames.

Traced path (60 hops) shows:
- Cumulative PPC stack: 5,792B (5.7KB)
- Native stack estimate: 4,880B (4.8KB)
- Total: ~10.7KB

This chain is NOT the overflow cause -- 1,548 calls at ~48B native each = ~74KB, well
within the 16MiB host stack or the 512KB-6MB guest stack.

---

## 5. Particle System Functions

**sub_825BF8A8** and **sub_825EDDA0** were NOT found as recompiled function definitions
in the generated code. They may be:
- Excluded from codegen (in the 278 excluded functions)
- Inlined by the original compiler
- Located in a different address range than expected

Since they don't appear as recompiled functions, they cannot be the direct cause of
native stack overflow through recursion.

---

## 6. Resource/RPF Traversal Functions

No recompiled functions that directly call file I/O (CreateFileA, ReadFile, etc.) are
recursive. File I/O hooks redirect to rexcrt implementations which handle files through
the VFS layer. RPF archive traversal happens at a higher level in the game code, not
through direct file system calls.

However, the streaming/resource range (0x8231-0x8240) contains **18 recursive functions**,
with sub_82343EF0 (3,840B frame) being the most dangerous. This range likely handles
resource tree traversal (RPF directory structures, scene graph nodes, collision tree nodes).

---

## 7. The Primary Suspect: sub_82343EF0

### Profile:
- **PPC stack frame**: 3,840 bytes
- **Direct recursion**: Yes (1 self-call site)
- **Mutual recursion**: Yes, with sub_82344368 (128B frame)
- **Register saves**: __savegprlr_24 (saves r24-r31) + __savevmx_124 (saves VMX/Altivec)
- **Callers**: sub_82343D80, sub_82344368
- **Callees**: 12 unique functions
- **Address range**: 0x8234 -- collision/spatial partition system (966 functions in 0x8233-0x8235)

### Why this is the most likely overflow cause:

1. **VMX register saves** indicate SIMD computation -- consistent with collision detection
   or spatial partitioning (BVH tree, k-d tree, octree)
2. **3,840B frame** = the function operates on large local arrays (likely spatial bounds,
   intersection results, candidate lists)
3. **Tree traversal recursion** has data-dependent depth: a deep spatial tree with many
   subdivisions could recurse hundreds or thousands of times
4. **Mutual recursion** with sub_82344368 means the recursion pattern alternates between
   a "traverse" function (82343EF0, 3840B) and a "process node" function (82344368, 128B),
   totaling ~3,968B per level of the tree
5. At **1,494 iterations**, the guest stack is exhausted (5.8MB)

### The recursion chain:

```
sub_82343D80 (176B)     -- initial caller (scene traversal entry?)
  -> sub_82343EF0 (3840B)  -- recursive tree traversal (SIMD-heavy)
       -> sub_82343EF0 (3840B)  -- direct self-recursion (depth-first traversal)
       -> sub_82344368 (128B)   -- node processor
            -> sub_82343EF0 (3840B)  -- recurse into subtree
            -> sub_82343D80 (176B)   -- may re-enter from another path
```

### Context around the recursive call:

```cpp
// Conditional recursion - only if r6 (child node pointer?) is non-null:
if (ctx.cr6.eq) goto loc_82343F7C;  // skip recursion if null
// Set up parameters: r8=r27, r7=r29, r5=r24, r4=r28, r3=r25
sub_82343EF0(ctx, base);            // recurse into child
```

This matches tree traversal: check if child exists, if so recurse into it.

---

## 8. Other Candidates

### sub_82212F38 (4,064B frame, mutual recursion with sub_82212EC0):
- In world loading range (0x8221)
- 27 unique callees, 27 labels (complex control flow)
- 44 total call sites
- Mutual recursion creates 4,160B per cycle
- Could overflow in ~1,427 cycles

### sub_827C89F0 (1,312B frame, direct recursion):
- Unknown subsystem
- At 1,408B per iteration (PPC + native), would need ~4,219 iterations

### 3-cycle at 0x826E (6,640B per cycle):
- sub_826E2BD8 (4,304B) -> sub_826E2CC8 (96B) -> sub_826E2D60 (2,240B)
- Only ~893 cycles needed to overflow
- But 3-cycles are less likely to occur unbounded in practice

---

## 9. Could It Be a Tight Loop Without Returning?

**No.** In the recomp architecture, each PPC function call is a real C++ function call.
There is no mechanism for a "tight loop" to consume stack without returning -- each
`sub_XXX(ctx, base)` pushes a native frame that persists until the callee returns.

However, the PPC stack and native stack grow **independently**:
- **PPC stack** (ctx.r1): writes to emulated memory via `PPC_STORE_U32(base + ea, ...)`
- **Native stack**: grows with each C++ function call

The guest stack guard page fault occurs when `PPC_STORE_U32` writes to a guest address
past the guard page (0x705D0000). The native stack could overflow independently if the
host stack (8MiB for guest threads, 16MiB for rex threads) is exhausted by deep recursion.

### Stack budgets:

| Stack | Size | Usage at depth 1500 |
|-------|------|---------------------|
| Guest PPC stack | ~5.8MB (from XEX header + guard pages) | 1500 * 3840B = 5.6MB (OVERFLOW) |
| Host native stack (guest_thread) | 8MiB | 1500 * ~128B = 188KB (safe) |
| Host native stack (rex XThread) | 16MiB | 1500 * ~128B = 188KB (safe) |

The guest PPC stack overflows first because PPC frame sizes (3,840B) are ~30x larger
than native frame overhead (~128B). The host stack is safe at these depths.

---

## 10. Conclusions

1. **The overflow is caused by recursive PPC functions exhausting the guest stack**, not
   the host native stack. The 5.8MB guest stack is consumed by PPC `stwu r1,-3840(r1)`
   instructions in recursive collision/spatial-tree traversal.

2. **sub_82343EF0 is the primary suspect**: 3,840B PPC frame, direct + mutual recursion,
   SIMD-heavy (VMX saves), in the collision subsystem. Only ~1,500 recursive iterations
   would overflow the stack.

3. **The fix options are**:
   - Increase the guest stack allocation (set `XEX_HEADER_DEFAULT_STACK_SIZE` to 16MB+)
   - Implement stack guard page expansion (grow the guest stack when the guard page is hit)
   - Convert the deepest recursive functions to iterative traversal (requires understanding
     the data structures)

4. **Static call depth alone cannot cause this**: the deepest non-recursive chain is 1,548
   hops but only consumes ~75KB of stack. Recursion is required.

5. **The 0x829D and 0x8290-0x8291 recursive functions** (11 + 9 = 20 functions with small
   frames) are likely linked-list or tree utilities in the CRT/runtime. They are less
   dangerous due to smaller frames (96-352B) but could contribute if called from within
   the main recursive chain.
