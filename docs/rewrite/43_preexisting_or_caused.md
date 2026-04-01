# 43: Stack Guard Infinite Loop -- Pre-existing or Caused by Scene Hooks?

## Verdict: PRE-EXISTING, not caused by scene creation hooks

The stack guard page infinite loop at 0x705D0000 is a pre-existing bug in the
stack guard handler (`xmemory.cpp:448-461`), not a consequence of the scene
creation state machine hooks. The hooks enabled the game to progress further,
revealing a deeper bug that previously manifested as an immediate crash.

---

## 1. Three Independent Lines of Evidence

### A. Timeline: The bug predates the hooks

The stack guard handler was added in commit `b595515d` (Mar 25, 2026 15:13).
The scene creation hooks exist only as **uncommitted local changes** to
`LibertyRecomp/kernel/imports.cpp` (lines 1575-2176, adding ~600 lines to the
1739-line committed version). The committed version at HEAD (`09b50a9a`) has no
scene hooks at all -- it ends with networking/voice/session registrations.

Before the handler existed (pre-Mar 25), the same category of fault at
`0x807008FFFC` (guest `0x7008FFFC`, squarely in the stack range
`0x70000000-0x7F000000`) caused immediate process death via unhandled SIGBUS
(`liberty_crash6.log`, Mar 25 14:11). The handler converted these instant
crashes into an infinite loop by always returning `true` regardless of whether
`Protect()` succeeded.

### B. The hooks do not cause recursion or stack overflow

Every hook was audited (see doc 46 for full details):

| Hook | Behavior | Recursion Risk | Stack Impact |
|------|----------|---------------|--------------|
| sub_8223DB20 (sign-in guard) | Returns 0, no side effects | None | None |
| sub_82240B78 (storage guard) | Returns 0, no side effects | None | None |
| sub_82240B08 (device ready) | Sets 2 flags + returns 1 | None | None |
| sub_8224FFC8 (dialog accept) | Returns 1 for accept, 0 for cancel | None | None |
| sub_8284A7E8 (content create) | Pass-through diagnostic wrapper | None | None |
| sub_822417B0 (delta clamp) | Pass-through, clamps delta >= 0 | None | None |
| sub_82242910 (scene creation) | Forces platformMode=3, intercepts fast path | None | None |
| sub_822438B0 (state 6 inner) | Pass-through diagnostic wrapper | None | None |
| sub_822422E0 (state 5 start) | Pass-through, resets stale value 2->0 | None | None |
| sub_822440F8 (state 4 bypass) | Returns 2 directly (skip save device) | None | None |

Key properties:
- No hook calls itself or any function that calls back to it
- The state machine is strictly non-recursive (sub_82242910 returning 0 advances
  sub_822438B0 from state 2 to state 3; it never re-enters state 2)
- The hooks that stub functions (sub_8223DB20, sub_82240B78, sub_82240B08,
  sub_8224FFC8, sub_822440F8) replace complex Xbox-specific logic with simple
  constant returns -- they **reduce** stack depth, not increase it
- The hooks that wrap functions (sub_82242910, sub_822438B0, sub_822422E0,
  sub_822417B0, sub_8284A7E8) add only 1-2 stack frames for printf

### C. The crash occurs in a completely different subsystem

The stack guard page fault sequence (doc 41 analysis):

1. State machine completes normally: states 0->1->3->4->9->10->11->12->14
2. Outer SM advances: sub_822438B0 states 2->3->7->0 (exits with ret=2)
3. Ready signal written to 1 at 0x82BF9B70
4. Game enters main loop (YIELD counter runs to 2,306,000)
5. ~80,000 lines of MISSING-FUNC spam from vtable dispatchers (null function
   pointers at 0x828C99CC, 0x828C1F94, 0x8291E144, 0x8291E1B0, etc.)
6. 42 calls to address 0x000F4000 from 0x821911C4 (sub_821910D0's vtable call)
7. Stack guard pages begin firing at 0x70000000, walk to 0x705D0000
8. Infinite loop: BaseHeap::Protect fails (uncommitted), handler returns true

The gap between the state machine completing (line ~1157) and the first stack
guard fault (line 81823) spans ~80,000 log lines of MISSING-FUNC vtable
dispatch failures. This is the world loading / render dispatch code running
after scene creation -- a completely separate subsystem from the state machine.

---

## 2. The MISSING-FUNC Call to 0x000F4000 from sub_821910D0

### What sub_821910D0 does

`sub_821910D0` (at `gta4_recomp.2.cpp:1886-2085`) is a **render dispatch
coordinator**. Its structure:

1. Enters a critical section (RtlEnterCriticalSection)
2. Checks if a render object list exists at `r30+304`
3. If yes: signals an event (KeSetEvent), then waits on multiple objects
   (KeWaitForMultipleObjects with 2 objects, timeout = 1, alertable = 3)
4. If no: calls `sub_8218FFB0` then `sub_82191228`
5. **At 0x821911B0-0x821911C4**: reads a vtable pointer:
   ```
   lwz r3, 64(r30)     // r3 = obj->field_64  (the "render object")
   lwz r11, 0(r3)      // r11 = vtable ptr = *(r3)
   lwz r11, 68(r11)    // r11 = vtable[17] (17th virtual function)
   mtctr r11            // indirect call target
   bctrl               // CALL -- lr set to 0x821911C4
   ```
6. If the return value (r3) >= 0, increments an atomic counter

### Why it calls 0x000F4000

The call chain is:
- `r30` = a global object loaded from address `-2095251456 + 21484` = `0x82C0539C`
- `r30 + 64` = the "render object" pointer
- `*(renderObj)` = vtable base
- `*(vtable + 68)` = the 17th virtual function

Address `0x000F4000` is far below the guest code range (0x82000000+). This means
the vtable at `*(renderObj)` contains `0x000F4000` at offset 68. This is a
**corrupted or uninitialized vtable entry** -- not a legitimate function address.

### Relationship to hooks

None of the scene creation hooks touch the render object at `0x82C0539C + 21484`
or any vtable pointers. The sub_82191228 / sub_8218FFB0 helper functions are not
hooked. The render dispatch system is entirely outside the scope of the scene
creation state machine.

The render system's corrupted vtable is most likely caused by one of:
1. An unimplemented import that was supposed to populate the vtable
2. A missing render system initialization step (GPU/video subsystem)
3. Recompiled code that reads the vtable before the render object is fully
   constructed (race condition between the main thread and GPU thread)

---

## 3. Is Thread t41614048 Running the Scene Creation SM?

**Yes, but that is irrelevant.** Thread t41614048 is the Main XThread (doc 42).
It runs the entire game: initialization, state machine, main loop, and render
dispatch. The sequence is:

1. Main XThread runs the scene creation state machine (lines 1008-1157)
2. State machine COMPLETES (line 1153: sub_822438B0 #13 ret=2 state=7->0)
3. Main XThread continues into the main game loop
4. Main game loop calls render dispatch (sub_821910D0 via the main update path)
5. Render dispatch hits the corrupted vtable (0x000F4000)
6. MISSING-FUNC handler absorbs the bad call (prints warning, returns)
7. Eventually the render dispatch loop or another path consumes stack space
8. Stack grows past the last committed page into uncommitted territory
9. Stack guard handler fails, infinite loop begins

The scene creation hooks ran on this thread and completed successfully long
before the stack guard fault. The thread continued executing game code.

---

## 4. Could the Hooks Skip Initialization That Later Code Depends On?

### sub_822440F8 bypass (state 4 inner -- the most aggressive hook)

This hook returns 2 directly, completely skipping the save device selection
state machine (7 states: controller scan, content enumeration, content loading,
save slot enumeration/read/write). This is the most aggressive modification.

**Could skipping save initialization affect the render system?** No. The save
device selection controls:
- Player save slot assignment
- Content enumeration handles (XamContent APIs)
- Profile data loading

None of these affect render object vtables. The render system (sub_821910D0)
reads from a global at `0x82C0539C + 21484` which is populated by the GPU/video
initialization path (VdInitializeEngines, VdSetGraphicsInterruptCallback, etc.),
not by the save system.

### sub_82240B08 (device readiness -- sets flags prematurely)

Sets `0x82BF3A77 = 1` and `0x82BF3CDA = 1` without actually running
XamContentGetDeviceData. These flags have exactly 1 reader each:
- `0x82BF3A77`: read only inside sub_82240B78 (which we stub to return 0)
- `0x82BF3CDA`: read only inside sub_8223DB90's flow (cleanup, not render)

Neither affects the render dispatch path.

### sub_82242910 (platformMode=3 fix)

Forces platformMode to 3, which routes states 10-14 through the "online
single-player with save device" path. This affects only sub_82242910's internal
state transitions and the save file validation call (sub_8223F790). It does not
touch any render system globals.

---

## 5. Summary: Causal Chain

```
Scene hooks --> state machine completes --> game enters main loop
                                                  |
                                                  v
                                      render dispatch (sub_821910D0)
                                                  |
                                                  v
                                      corrupted vtable: call 0x000F4000
                                                  |
                                                  v
                                      MISSING-FUNC absorbs call (no-op)
                                                  |
                                                  v
                                      repeated vtable dispatch attempts
                                      (80K+ null/bad indirect calls)
                                                  |
                                                  v
                                      stack grows past committed pages
                                                  |
                                                  v
                                      BaseHeap::Protect fails on 0x705D0000
                                                  |
                                                  v
                                      handler returns true --> INFINITE LOOP
```

The hooks enabled the game to reach further than before. Without them, the game
was stuck in the state machine (oscillating ready signal, error 34, etc.). The
stack guard fault was always going to happen once the game reached the main loop
and tried to dispatch render calls through uninitialized vtables.

The root cause is two bugs working together:
1. **Uninitialized vtable** at render dispatch object (0x000F4000 at vtable+68)
2. **Stack guard handler ignores Protect() failure** (xmemory.cpp:457-460)

The first bug causes unbounded stack growth. The second bug converts what should
be a clean crash into an infinite loop.

---

## Key Files

| File | Role |
|------|------|
| `LibertyRecomp/kernel/imports.cpp` (lines 1575-2176) | Scene creation hooks (uncommitted) |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` (lines 448-461) | Stack guard handler (the infinite loop bug) |
| `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.2.cpp` (lines 1886-2085) | sub_821910D0 (render dispatch with vtable call at 0x821911C4) |
| `docs/rewrite/41_log_context_before_crash.md` | Log analysis showing 80K-line gap between SM and fault |
| `docs/rewrite/42_thread_identity.md` | Thread t41614048 = Main XThread |
| `docs/rewrite/46_cascading_failures_audit.md` | Full hook risk audit |
