# LibertyRecomp — Crash Debugging

## Run & capture logs

```bash
"./out/build/macos-release/LibertyRecomp/Liberty Recompiled.app/Contents/MacOS/Liberty Recompiled" \
  > /tmp/liberty_run.log 2>&1

# Watch live
tail -f /tmp/liberty_run.log
```

Stdout = game `printf`. Stderr = crash handler + RexGlue diagnostics.

## Guest address from fault address

The crash handler prints `Fault address: 0xHHHHHHHH` and `Memory base: 0xHHHHHHHH`.

```
guest_addr = fault_addr - g_memory.base
```

If `guest_addr` is in `0x82000000–0x83200000` → it's a generated code address.
Find the function: `grep -n "loc_82XXXXXX\|sub_82XXXXXX" glue/.../generated/*.cpp`

If outside that range → host code crash (check the `Symbol:` line in the dump).

## ARM64 register dump (in the crash handler)

```
x0 = PPCContext*  (ctx argument to every generated function)
x1 = base         (g_memory.base — should match Memory base line)
```

If `x1 == g_memory.base`, the crash is inside a generated PPC function.
Read `x0` as a `PPCContext` to get guest register state: `r0-r31`, `lr`, `ctr`.

## Common crash signatures

| Log pattern | Cause | Fix |
|-------------|-------|-----|
| `MISSING-FUNC indirect call to 00000000` | Null vtable slot or unresolved function pointer | Benign if in global ctor phase (before first xstart log). Otherwise add a hook. |
| `[SetVDecl] skip — invalid type at 0xffff` | D3D device init sentinel value | Pass through `declAddr < 0x10000` without type-checking. |
| `[SetTexture] skip` | Texture not registered in `s_textureHandleMap` | Register under both physical and virtual addr in `sub_82A55DC0` hook. |
| `ObReferenceObjectByHandle type mismatch` | Real kernel type ptr (< 0x80000000) vs Xenia magic | Accept both ranges in `xboxkrnl_ob.cpp`. |
| Infinite loop at 100% CPU in `sub_8286F078` | r7=0 from read-vtable → decompressor gets zero bytes → loops | Fix the read path / DMA buffer. |
| Crash at `sub_8285E710` | Streaming device returning 0 bytes (slot pos == device capacity) | Check OVERLAPPED hEvent signaling in `kernel/crt/file.cpp`. |

## Guest tick frequency (loading hangs)

If the game's loading-screen tasks never advance (stuck at state=0):
```cpp
// MUST be set after rex::Runtime::Setup() and before LaunchModule()
rex::chrono::Clock::set_guest_tick_frequency(50000000ULL); // 50 MHz = Xbox 360 timebase
```
Without this, `PPC_QUERY_TIMEBASE()` returns host-CPU ticks (~1-4 GHz) and the
game's `1/50M` scaling constant returns ~0 ms, freezing every time-gated task.

## Adding a diagnostic print in generated code

In a `PPC_FUNC_IMPL` body:
```cpp
fprintf(stderr, "[DIAG] sub_82XXXXXX: r3=0x%08X r4=0x%08X\n",
        ctx.r3.u32, ctx.r4.u32);
fflush(stderr);
```

**Always remove diagnostic prints before committing** — ~400 fprintf calls were
the source of a major slowdown in the project's history.

## Checking function table registration

```cpp
auto* p = rex::Runtime::instance()->processor();
PPCFunc* fn = p->GetFunction(0x82A11290); // should be non-null after Setup()
fprintf(stderr, "[DIAG] GetFunction=%p HasFT=%d\n", fn, (int)p->HasFunctionTable());
```
