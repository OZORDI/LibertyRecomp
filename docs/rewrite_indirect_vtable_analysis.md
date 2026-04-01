# Indirect Vtable Call Analysis: sub_82852D18 (gta4_recomp.55.cpp:31027)

## 1. PPC_CALL_INDIRECT_FUNC Definition

Defined in `glue/rexglue-sdk-main/include/rex/ppc/context.h:127-140`.

**Recompiled mode** (when PPC_CONFIG_H_INCLUDED):
- Checks if address is in `[PPC_CODE_BASE, PPC_CODE_BASE + PPC_CODE_SIZE)` = `[0x82140000, 0x82A8635C)`
- If in range: looks up function table via `PPC_LOOKUP_FUNC(base, addr)`
- If function table entry is non-null: calls `_icf_fn(ctx, base)`
- Otherwise: prints `[MISSING-FUNC] indirect call to %08X (in_range=%d) from %08X` to stderr and **returns silently**

**Library mode** (fallback): `__builtin_debugtrap()` (hard trap).

## 2. Behavior When Address Is NOT in the Function Table

**Does not crash, hang, or abort.** It prints a diagnostic to stderr via fprintf + fflush and silently returns. The caller continues with whatever garbage was left in `ctx.r3` (return value register). This can cause downstream issues (null pointer dereference, wrong control flow, infinite loops) but the indirect call itself never blocks.

## 3. Tracing r29 (The Vtable Object)

Call chain from `sub_827C2420` (gta4_recomp.50.cpp:57418-57452):

| Step | Code | Effect |
|-|-|-|
| 1 | `r10 = PPC_LOAD_U32(r31 + 0)` | Load vtable ptr from r31 object |
| 2 | `r11 = PPC_LOAD_U32(r10 + 4)` | Load vtable[1] |
| 3 | `PPC_CALL_INDIRECT_FUNC(r11)` | Call vtable[1]; return value in r3 |
| 4 | `r6 = r3` (return value) | Flows into sub_82852DD0 as arg r6 |
| 5 | sub_82852DD0: `r29 = r6` | Stored in r29 |
| 6 | sub_82852DD0 calls sub_82852D18 with `r5 = r29` | Passes dynamic object |
| 7 | sub_82852D18: `r29 = r5` | The vtable object |

**r29 in sub_82852D18 is the return value of a prior vtable[1] dispatch on the caller's object. It is DYNAMIC and not statically resolvable.**

Static arguments computed via Python:
- `r5` to sub_82852DD0 = `lis(-32244) + (-16412)` = `(-2113142784 + -16412) & 0xFFFFFFFF` = **0x820BBFE4** (data section, likely type name string)
- `r4` to sub_82852DD0 = `lis(-32248) + (-23172)` = `(-2113404928 + -23172) & 0xFFFFFFFF` = **0x8207A57C** (data section, likely function ptr/string)

Both are below PPC_CODE_BASE (0x82140000) -- they are .rdata/.data pointers, not function addresses.

## 4. What Does vtable[2] Resolve To?

**Cannot be statically determined.** The vtable pointer is loaded from the first dword of a dynamically-returned object. The runtime type of that object determines the vtable, and the vtable[2] entry determines the target function.

However, the target MUST be in `[0x82140000, 0x82A8635C)` AND have a valid function table entry for the call to succeed. If not, PPC_CALL_INDIRECT_FUNC prints MISSING-FUNC and returns.

## 5. Is There a Function at the vtable[2] Address?

Not determinable statically. Grepping for `0x820BBFE4` and `0x8207A57C` in the generated code yielded no results -- confirming these are data pointers, not function addresses.

The vtable itself lives at whatever address the first dword of the returned object points to. If the XEX image data is properly loaded, the vtable entries should contain valid code section addresses. The key question is whether:
1. The vtable is in the prepopulated set -- **it is NOT** (gta4_vtables.txt and vtable_prepopulate.h contain no entries in the streaming/resource class range)
2. The vtable data exists in the raw XEX image -- depends on whether PPC_STORE_U32 or the image loader placed the correct values in guest memory

## 6. Existing Instrumentation

`imports.cpp` already has probes on this exact call chain:
- `INIT_PROBE(sub_82852DD0, 30553, "827C2420 thread-launch-dispatch")`
- `INIT_PROBE(sub_82852D18, 30555, "82852DD0 operation")`
- `INIT_PROBE(sub_82851A10, 30557, "82852D18 pre-vtable-call")`

## 7. Conclusions

| Question | Answer |
|-|-|
| What is PPC_CALL_INDIRECT_FUNC? | Macro that looks up function in table by `(addr - CODE_BASE) * 2`, calls it if found |
| What happens if address is missing? | Prints `[MISSING-FUNC]` to stderr, **silently returns** (no crash/hang) |
| Is r29 statically resolvable? | **No** -- it is the return value of a prior vtable[1] dispatch |
| Is vtable[2] in the function table? | Unknown at static analysis time; depends on runtime object type |
| Can this be the hang point? | **Not directly** -- PPC_CALL_INDIRECT_FUNC never hangs. But if the call silently fails and returns garbage in r3, downstream code may enter an infinite loop or deadlock waiting for a result that never comes |
| Are streaming vtables prepopulated? | **No** -- neither gta4_vtables.txt nor the manual entries in memory.cpp cover this class hierarchy |

### Root Cause Hypothesis

If this vtable dispatch silently fails (MISSING-FUNC), sub_82852D18 continues and returns `r30=1` (success). The caller `sub_82852DD0` returns this "success" to `sub_827C2420`, which proceeds as if the streaming resource was successfully activated. The actual operation (whatever vtable[2] was supposed to do) never happened, potentially leaving a streaming resource in an incomplete state that downstream code spins on forever.

**To confirm: check stderr logs for `[MISSING-FUNC] indirect call to XXXXXXXX` with `from 82852D84` (the LR value set at line 31026).**
