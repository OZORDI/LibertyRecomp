# sub_82852D18 Cross-Reference Analysis

## Function Signature (from generated code)

```
sub_82852D18(r3=manager, r4=resource_node, r5=callback_obj, r6=data, r7=flag)
```

Inside the function:
- r5 (callback_obj) is saved to r29
- `[r29+0]` is dereferenced as a vtable pointer
- `[[r29+0]+8]` (vtable slot 2) is called via indirect dispatch (`bctrl`)
- Also calls sub_82852A50 (lock acquire), sub_82851A10, sub_8284FA58, sub_821B3560, sub_828470E0/sub_82847120 (sync pair)

## Direct Callers of sub_82852D18 (3 sites)

### 1. sub_82852DD0 (gta4_recomp.55.cpp:31125)
Wrapper function. Maps its own args to sub_82852D18:
- sub_82852D18.r5 = sub_82852DD0.r6 (callback/vtable obj)
- sub_82852D18.r4 = result of sub_8284F468 (resource lookup)
- Calls sub_8285B088 after (lock release)

### 2. sub_828EA388 (gta4_recomp.60.cpp:19391)
- r3 = global at `[0x831E55EC]` (streaming manager singleton)
- r4 = r30 (passed-in resource handle)
- **r5 = DYNAMIC** — return value of virtual call `[obj+0]+4` on r31
- r6 = r31 (the resource object)
- r7 = 1

### 3. sub_828EBCE0 (gta4_recomp.60.cpp:23190)
- r3 = r29 = global at `[0x831E55EC]` (same streaming manager)
- r4 = r30
- **r5 = DYNAMIC or GLOBAL** — either virtual call `[obj+0]+4` return, or loaded from `[0x831C62AC]`
- r6 = r31 (resource object)
- r7 = 1

## Indirect Callers via sub_82852DD0 (9 sites)

sub_82852DD0.r5 (which becomes sub_82852D18.r5) is the callback/vtable pointer.

| Caller | File:Line | r5 (callback obj) | r6 (data, maps to D18.r5) |
|-|-|-|-|
| sub_826729B8 | 37.cpp:80519 | 0x8233BFE4 | dynamic (vfunc return) |
| sub_826729B8 | 37.cpp:80542 | 0x8233BFE4 | global [0x8319FDE8] |
| sub_826729B8 | 37.cpp:80572 | 0x8233BFE4 | dynamic (vfunc return) |
| sub_826729B8 | 37.cpp:80589 | 0x8233BFE4 | global [0x8319FDE8] |
| sub_827C2420 | 50.cpp:57452 | 0x8233BFE4 | dynamic (vfunc return) |
| sub_827DC9xx | 51.cpp:42014 | 0x8237C308 | dynamic or [0x8319FE30] |
| sub_827DF9xx | 51.cpp:49492 | 0x8237C0FC | dynamic or [0x8319FE34] |
| sub_827E16xx | 51.cpp:53957 | 0x8237CD28 | stack-local buffer |
| sub_827E8Cxx | 51.cpp:72199 | 0x8237CD28 | dynamic or [0x8319FE50] |

**Note**: In the sub_82852DD0 path, the r5 arg to sub_82852DD0 maps to sub_82852D18's r5. The "r5 (callback obj)" column above is the sub_82852DD0.r5 which becomes the vtable/callback object dispatched inside sub_82852D18.

## Distinct Vtable/Callback Addresses

Four distinct callback object addresses are used:
1. **0x8233BFE4** — used by sub_826729B8 (4 calls) and sub_827C2420 (1 call). This is the streaming system's primary dispatch table.
2. **0x8237C308** — used by sub_827DC9xx (resource type A registration)
3. **0x8237C0FC** — used by sub_827DF9xx (resource type B registration)
4. **0x8237CD28** — used by sub_827E16xx and sub_827E8Cxx (resource type C/generic registration)
5. **DYNAMIC** — sub_828EA388 and sub_828EBCE0 get the callback object from a virtual method return value at runtime

## Thread Context Analysis

### Main Thread (Init) Callers
- **sub_826729B8** — called from sub_822A0968 (gta4_recomp.8.cpp:98416), a large sequential init function that registers streaming modules during startup
- **sub_827C2420** — called from sub_82478Exx (gta4_recomp.20.cpp:84298), another init-time registration path
- **sub_827DC9xx, sub_827DF9xx, sub_827E16xx, sub_827E8Cxx** — resource type registration functions in the 0x827Dxxxx-0x827Exxxx range, called during init

### Streaming Worker Thread Callers
- **sub_828EA388** — called from sub_828EA8B8, called from sub_828EA9A0, called from **sub_828C67B8** (a virtual method override with NO direct callers — dispatched via vtable from streaming worker threads)
- **sub_828EBCE0** — called from sub_828EBD60, called from sub_828EC5xx, same streaming subsystem

## Deadlock Risk Assessment

**YES — there is a deadlock risk.** sub_82852D18 is called from both:
1. Main init thread (via sub_826729B8, sub_827C2420, etc.)
2. Streaming worker threads (via sub_828C67B8 -> sub_828EA9A0 -> sub_828EA8B8 -> sub_828EA388 -> sub_82852D18)

Inside sub_82852D18:
- Calls sub_82852A50 to acquire a resource (lock/critical section)
- Calls sub_828470E0/sub_82847120 (the sync primitive pair, conditionally based on a global flag at `[0x831E55EC]+40` bit 17)
- Does an indirect vtable call via `[[r5]+8]`

If the main thread holds a lock and the streaming thread tries to acquire the same lock (or vice versa), or if the vtable dispatch function on the streaming thread tries to signal/wait on a primitive the main thread is blocking on, deadlock occurs.

The hang in the sub_827C2420 path specifically: the main init thread calls sub_827C2420 -> sub_82852DD0 -> sub_82852D18 -> sub_82852A50 (lock acquire). If a streaming worker thread is simultaneously in sub_828EA388 -> sub_82852D18 -> sub_82852A50 trying to acquire the same lock, classic deadlock.
