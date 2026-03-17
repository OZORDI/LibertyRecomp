# LibertyRecomp — Hook Patterns

## Why `extern "C"` matters

Generated code uses `PPC_WEAK_FUNC(sub_82XXXXXX)` which expands to a C-linkage weak symbol.
A C++ override gets name-mangled and **won't replace it**. `PPC_FUNC_HOOK` adds `extern "C"` automatically.

## Macro definitions (`kernel/function.h`)

```cpp
// PPC_FUNC_HOOK: direct context manipulation — access ctx.rN, ctx.fN directly
#define PPC_FUNC_HOOK(x) extern "C" PPC_FUNC(x)
// PPC_FUNC expands to: void x(PPCContext& ctx, uint8_t* base)

// GUEST_FUNCTION_HOOK: typed shim — ArgTranslator maps C++ args/return to/from ctx
#define GUEST_FUNCTION_HOOK(subroutine, function) \
    extern "C" PPC_FUNC(subroutine) { HostToGuestFunction<function>(ctx, base); }

// GUEST_FUNCTION_STUB: no-op (returns 0)
#define GUEST_FUNCTION_STUB(subroutine) \
    extern "C" PPC_FUNC(subroutine) { }
```

## Register ABI (Xbox 360 PPC calling convention)

| Role | Registers |
|------|-----------|
| Integer args | r3 (arg0) → r10 (arg7) |
| Float/double args | f1 → f13 |
| Integer return | r3 |
| Float return | f1 |
| Caller-saved | r3–r12, f0–f13 |
| Callee-saved | r14–r31, f14–f31 |
| Stack pointer | r1 |
| Thread-local | r13 |

Accessing args in a `PPC_FUNC_HOOK`:
```cpp
PPC_FUNC_HOOK(sub_82A49C38) {
    uint32_t device  = ctx.r3.u32;
    uint32_t flags   = ctx.r4.u32;
    float    scale   = ctx.f1.f32;   // if arg is float
    // ...
    ctx.r3.u64 = result;             // set return value
}
```

## Typed hook with GUEST_FUNCTION_HOOK

The host function signature maps r3..r10 → typed args and converts pointers via `base + guest_addr`:
```cpp
// Host function — regular C++ types, pointer args auto-translated
uint32_t GTAIV_CreateTexture(uint32_t device, uint32_t width, uint32_t height,
                              uint32_t levels, uint32_t usage, uint32_t format,
                              uint32_t pool, uint32_t* outTexture) {
    // device, width etc are raw guest uint32 values
    // outTexture is already translated: base + guest_ptr
    GuestTexture* tex = new GuestTexture(width, height, format);
    *outTexture = g_memory.MapVirtual(tex);
    return 0; // D3D_OK
}

// Wire it up — goes in gpu/video.cpp or a hook registration function
GUEST_FUNCTION_HOOK(sub_82A44850, GTAIV_CreateTexture);
```

## Reading struct fields from guest memory

```cpp
PPC_FUNC_HOOK(sub_8285E710) {
    uint32_t slot    = ctx.r31.u32;
    uint32_t remaining = PPC_LOAD_U32(slot + 28);   // slot->remaining_bytes
    uint32_t buf_ptr   = PPC_LOAD_U32(slot + 8);    // slot->buffer
    PPC_STORE_U32(slot + 28, 0);                     // slot->remaining = 0
}
```

## Calling a guest function from host code

```cpp
// GuestToHostFunction<ReturnType>(guest_func_ptr, args...)
uint32_t result = GuestToHostFunction<uint32_t>(sub_82172BE8, event_ptr);

// With a registered PPCFunc pointer
PPCFunc* fn = rex::Runtime::instance()->processor()->GetFunction(0x82172BE8);
uint32_t result = GuestToHostFunction<uint32_t>(fn, event_ptr);
```

## Indirect (vtable) call pattern

```cpp
PPC_FUNC_HOOK(sub_8285AD08) {
    // vtable call: *(*(r3) + offset)(r3, ...)
    uint32_t obj    = ctx.r3.u32;
    uint32_t vtable = PPC_LOAD_U32(obj);            // vtable ptr
    uint32_t fn_ptr = PPC_LOAD_U32(vtable + 120);   // vtable[30] = slot 30 * 4
    ctx.ctr.u64 = fn_ptr;
    PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
}
```

## Common hook locations

| Function | File | Purpose |
|----------|------|---------|
| `sub_82A49C38` | `gpu/video.cpp` | CreateDevice hook |
| `sub_82A467D8` | `gpu/video.cpp` | Present/VdSwap hook |
| `sub_82A44850` | `gpu/video.cpp` | CreateTexture |
| `sub_82A3A890` | `gpu/video.cpp` | SetVDecl |
| `sub_82A44B78` | `gpu/video.cpp` | SetTexture |
| `sub_82A50F28` | `gpu/video.cpp` | GpuMemAlloc stub |
