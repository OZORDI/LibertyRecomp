// =============================================================================
// Shader Factory Chain - Full Decompilation + Device Guard Bypass
// =============================================================================
//
// Complete decompilation of the RAGE Xbox 360 shader loading pipeline.
// Every function in the chain has been analyzed from PPC recomp scaffolds
// and Hex-Rays pseudocode to produce this documented reference and the
// critical hook implementations.
//
// ===== PIPELINE OVERVIEW =====
//
//   sub_828CD290  grmShaderFactoryStandard::Preload (vtable[7])
//       Reads preload.list from shader directory, iterates each line,
//       strips .fx/.fx2 extensions, calls sub_828C73F0 per shader name.
//       Fallback: loads "embedded:/rage_im" twice if list is missing.
//
//   sub_828C73F0  LoadShaderByName
//       Finds free slot in g_ShaderTable[128] at 0x831C30B8.
//       Allocates 112-byte grmShaderFx, sets name hash, builds VFS path
//       (handles "embedded:" prefix + "dcl/" directory). Calls sub_828CAA60
//       to open and parse the FXC. On success calls sub_828C6E48 to build
//       the UI parameter list. On failure cleans up and returns -1.
//
//   sub_828CAA60  OpenCompiledShader
//       Resets effect state via sub_828CA8F8. Builds path with "fxc" ext
//       in "fxl_final" directory. Opens fiStream, queries file size via
//       device vtable[19]. Reads 4-byte magic. If 0x61786772 ("axgr")
//       dispatches to sub_828CA318. Returns 1 on success, 0 on failure.
//
//   sub_828CA318  ParseCompiledEffect
//       The core FXC parser. Reads from stream in order:
//       1. Pixel shader count + entries (sub_828C8108 per entry)
//       2. Vertex shader count + entries (sub_828C8020 input layout +
//          bytecode read + sub_82A42BA8 CreateVertexShader if device != 0)
//       3. Technique count + entries (sub_828C91D0 per entry, then
//          cross-references VS/PS indices against technique name hashes)
//       4. Parameter count + entries (sub_828C85A0 alloc + sub_828C91D0)
//       5. Pass/annotation count + entries (sub_828CA248 + sub_828CA188)
//       6. Final fixup: sub_828C9660 resolves technique shader refs
//
//   sub_828C0B48  grcDevice::Setup
//       Creates D3D device (sub_82A416B8), render targets, initial render
//       state, two vertex declaration tables (3-element and 4-element),
//       loads built-in shaders via sub_828CAA60, resolves technique indices
//       for draw/drawblit/drawskinned/unlit_draw/unlit_drawskinned,
//       finds DiffuseTex parameter, allocates command buffer.
//
// ===== HELPER FUNCTIONS (analyzed, run as recompiled PPC) =====
//
//   sub_828C8108  ReadPixelShaderEntry: input layout + bytecode + CreatePS
//   sub_828C8020  ReadShaderInputLayout: count + 8-byte entries from stream
//   sub_828C8898  FindTechniqueByName: hash scan at stride 48, returns 1-based
//   sub_828C9728  FindPassByName: hash scan at stride 16, returns 1-based
//   sub_828C85A0  AllocateTechniqueArray: count*48, init defaults + 0xFFFF sentinels
//   sub_828CA248  AllocateAnnotationArray: count*16, zero init
//   sub_828CA188  ReadPassData: name + sub-pass array (32B each) from stream
//   sub_828C91D0  ReadTechniqueData: type/name/annotations/params from stream
//   sub_828C9660  ResolveTechniqueShaderRefs: resolves VS/PS indices per sub-pass
//   sub_828CA8F8  ResetEffectState: frees all arrays, zeros control fields
//   sub_828C73A0  InitShaderFxObject: zeros name, inits effect arrays
//   sub_828C8210  ReleaseShaderParams: releases ref-counted param objects
//   sub_828C8630  BuildShaderParamBlock: allocates param data block
//   sub_82A42BA8  CreateVertexShader: alloc descriptor + bytecode copy
//   sub_82A42CB8  CreatePixelShader: alloc descriptor + bytecode copy + PS init
//
// ===== ROOT CAUSE (device guard regression) =====
//
//   On PC recomp, sub_82A416B8 (D3D device creation) returns failure or is
//   stubbed, leaving 0x831C22A4 == 0. Both sub_828CA318 and sub_828C8108
//   skip CreateShaderFromBytecode calls when the device pointer is NULL.
//   The shader objects get slot+8 = 0 (no GPU resource), causing downstream
//   rendering to fail silently.
//
//   Fix: Hook sub_828CA318 and sub_828C8108 to set a sentinel value at
//   0x831C22A4 before the original function runs, then restore after.
//   The Create*Shader functions have their own internal callback gates
//   (0x831CF6B0/0x831CF6B4) that gracefully handle the no-GPU case.
//
// =============================================================================

#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>

// =============================================================================
// Guest Memory Addresses
// =============================================================================

namespace ShaderFactory {

// Graphics device pointer (rage::grcDevice*)
// Written by sub_828C0B48 after D3D device creation (sub_82A416B8).
// Read as a guard by sub_828CA318 and sub_828C8108 before shader creation.
constexpr uint32_t DEVICE_PTR              = 0x831C22A4;

// Copy of device ptr stored during setup
constexpr uint32_t DEVICE_PTR_COPY         = 0x831C2294;

// Global technique table: 48 bytes per entry
constexpr uint32_t TECHNIQUE_TABLE_BASE    = 0x831C3DF0;
constexpr uint32_t TECHNIQUE_TABLE_COUNT   = 0x831C4FF0;

// Shader table: 128 grmShaderFx* slots
constexpr uint32_t SHADER_TABLE            = 0x831C30B8;
constexpr uint32_t SHADER_TABLE_END        = 0x831C32B8;

// Built-in shader factory "this" (used for drawblit etc.)
constexpr uint32_t BUILTIN_SHADER_FX       = 0x831C25A8;

// Named technique/pass indices resolved by sub_828C0B48
constexpr uint32_t TECH_DRAW               = 0x831C23E0;
constexpr uint32_t TECH_UNLIT_DRAW         = 0x831C23BC;
constexpr uint32_t TECH_DRAWSKINNED        = 0x831C23FC;
constexpr uint32_t TECH_UNLIT_DRAWSKINNED  = 0x831C23C4;
constexpr uint32_t TECH_DRAWBLIT           = 0x831C2404;
constexpr uint32_t PARAM_DIFFUSETEX        = 0x831C2290;

// GPU shader creation callback gates
constexpr uint32_t VS_CREATE_CALLBACK      = 0x831CF6B0;
constexpr uint32_t PS_CREATE_CALLBACK      = 0x831CF6B4;

// VFS root device
constexpr uint32_t VFS_ROOT                = 0x82B07278;

// Device guard bypass sentinel
constexpr uint32_t DEVICE_SENTINEL         = 0xDEAD0001;

// FXC magic: "axgr" in big-endian
constexpr uint32_t FXC_MAGIC_AXGR          = 0x61786772;

} // namespace ShaderFactory

// =============================================================================
// sub_828CA318 - Parse Compiled Effect (DEVICE GUARD BYPASS)
// =============================================================================
//
// The core FXC parser. 1392 bytes of PPC code that reads the entire compiled
// effect binary from a stream. The parsing order is:
//
//   1. Read pixel shader count (sub_8285AFA0)
//      Allocate array: count * 12 bytes (sub_821B3510)
//      Store at this+16, capacity at this+22
//      For each pixel shader:
//        - Increment count at this+20
//        - Call sub_828C8108(entry, stream) which:
//          a) Reads input layout via sub_828C8020
//          b) Reads srcSize + dstSize (u16 each)
//          c) Allocates bytecode buffer (sub_821B3510)
//          d) If srcSize != dstSize: endian swap (sub_82A585A8)
//             else: direct read (sub_8285AD08)
//          e) If *(0x831C22A4) != 0: entry[2] = sub_82A42CB8(buffer)
//          f) Free temp buffer (sub_821B3560)
//
//   2. Read vertex shader count (sub_8285AFA0)
//      Allocate array: count * 12 bytes
//      Store at this+24, capacity at this+30
//      For each vertex shader:
//        - Increment count at this+28
//        - Read input layout via sub_828C8020
//        - Read srcSize (u16) and dstSize (u16) via sub_8285B0F8
//        - If dstSize == 0: entry[2] = 0 (skip)
//        - Else:
//          a) Allocate bytecode buffer (srcSize bytes)
//          b) Read bytecode from stream (sub_8285AD08)
//          c) entry[2] = 0
//          d) If *(0x831C22A4) != 0: entry[2] = sub_82A42BA8(buffer)
//          e) Free bytecode buffer
//
//   3. Read technique count (sub_8285AFA0)
//      For each technique:
//        - Increment global counter at 0x831C4FF0
//        - Get entry ptr: base + 48 * globalIndex (in 0x831C3DF0 table)
//        - Read technique data: sub_828C91D0(entry, stream)
//        - Set VS/PS index sentinels to 0xFFFF
//        - Cross-reference: scan pixel shader array (this+16..+20) and
//          vertex shader array (this+24..+28) to find matching name hashes
//          and store indices at entry+28 (VS) and entry+30 (PS)
//        - Dedup: if a previous technique has the same name hash and
//          has sentinel indices, copy the resolved indices to it and
//          decrement the global counter (merge duplicate techniques)
//
//   4. Read parameter count (sub_8285AFA0)
//      Allocate array: sub_828C85A0(this+8, count) -> count * 48 bytes
//      Store at this+8, capacity at this+14
//      For each parameter:
//        - Increment count at this+12
//        - Read technique data: sub_828C91D0(entry, stream)
//      Then post-process: for each param with type==6 (sampler):
//        - Build "VS"/"PS"/"Size" suffix names and find technique indices
//          via sub_828C8898, store at entry+32/+33/+34
//
//   5. Read pass/annotation count (sub_8285AFA0)
//      Allocate array: sub_828CA248(this, count) -> count * 16 bytes
//      Store at this+0 (?), capacity at (?)
//      For each pass:
//        - Read pass data: sub_828CA188(entry, stream)
//
//   6. Final fixup: sub_828C9660(this) resolves all technique shader refs
//
// Our hook ensures the device pointer is non-zero so shader objects are
// created by sub_82A42BA8 and sub_82A42CB8. The original PPC handles all
// the complex parsing, array management, endian swapping, and technique
// cross-referencing correctly.
//
// Args:
//   r3 = effect data pointer (grmShaderFx + 8)
//   r4 = fiStream* (positioned after the 4-byte magic)
//
PPC_FUNC_IMPL(__imp__sub_828CA318);
PPC_FUNC_HOOK(sub_828CA318)
{
    using namespace ShaderFactory;

    uint32_t savedDevicePtr = PPC_LOAD_U32(DEVICE_PTR);

    if (savedDevicePtr == 0)
    {
        PPC_STORE_U32(DEVICE_PTR, DEVICE_SENTINEL);
        LOG_INFO("shader_factory: Set device guard sentinel for FXC parsing");
    }

    __imp__sub_828CA318(ctx, base);

    if (savedDevicePtr == 0)
    {
        PPC_STORE_U32(DEVICE_PTR, savedDevicePtr);
    }
}

// =============================================================================
// sub_828C8108 - Read Pixel Shader Entry (DEVICE GUARD BYPASS)
// =============================================================================
//
// Called by sub_828CA318 in a loop for each pixel shader entry.
// Decompiled logic:
//
//   void ReadPixelShaderEntry(uint32_t* entry, fiStream* stream) {
//       ReadShaderInputLayout(entry, stream);     // sub_828C8020
//       uint16_t srcSize, dstSize;
//       stream->ReadU16(&srcSize, 1);             // sub_8285B0F8
//       stream->ReadU16(&dstSize, 1);
//       void* buf = rage::Alloc(srcSize);         // sub_821B3510
//       if (srcSize == dstSize) {
//           stream->ReadBytes(buf, srcSize);      // sub_8285AD08
//       } else {
//           // Need endian swap: alloca temp, read, swap
//           void* tmp = alloca(dstSize);           // sub_82A020D4
//           stream->ReadBytes(tmp, dstSize);
//           EndianSwap(buf, srcSize, tmp, dstSize); // sub_82A585A8
//       }
//       entry[2] = 0;
//       if (*(uint32_t*)0x831C22A4 != 0)
//           entry[2] = CreatePixelShader(buf);    // sub_82A42CB8
//       rage::Free(buf);                          // sub_821B3560
//   }
//
// The standalone hook is a safety net in case sub_828C8108 is called
// from outside sub_828CA318's already-guarded scope.
//
PPC_FUNC_IMPL(__imp__sub_828C8108);
PPC_FUNC_HOOK(sub_828C8108)
{
    using namespace ShaderFactory;

    uint32_t savedDevicePtr = PPC_LOAD_U32(DEVICE_PTR);

    if (savedDevicePtr == 0)
    {
        PPC_STORE_U32(DEVICE_PTR, DEVICE_SENTINEL);
    }

    __imp__sub_828C8108(ctx, base);

    if (savedDevicePtr == 0)
    {
        PPC_STORE_U32(DEVICE_PTR, savedDevicePtr);
    }
}

// =============================================================================
// sub_828CAA60 - Open and Parse Compiled Shader File
// =============================================================================
//
// Decompiled logic:
//
//   bool OpenCompiledShader(grmShaderFx* fx, const char* name, int flags) {
//       ResetEffectState(fx);                     // sub_828CA8F8
//       if (name != nullptr) {
//           char path[256];
//           BuildPath(VFS_ROOT, path, 256, name, "fxc", flags);
//           if (!AppendExtension(path, 256, "fxl_final"))
//               return false;
//           datString::Set(&fx->path, path);      // at fx+32
//           fx->nameHash = atStringHash(ToLower(name), 0); // at fx+36
//       }
//       fiStream* stream = fiDevice::Open(fx->path, READ);
//       if (!stream) return false;
//       fiDevice* dev = fiDevice::GetDevice(fx->path, READ);
//       fx->fileSize = dev->GetFileSize(fx->path); // vtable[19]
//       uint32_t magic;
//       stream->ReadU32(&magic, 1);               // sub_8285B168
//       if (magic != 0x61786772) {                // "axgr"
//           printf("Old version of rage effect");
//           stream->Close();
//           return false;
//       }
//       ParseCompiledEffect(fx, stream);          // sub_828CA318
//       stream->Close();
//       return true;
//   }
//
// We hook for diagnostics and delegate to the original PPC, which handles
// the VFS path building, stream I/O, and magic validation correctly.
//
// NOTE: sub_828CAA60 hook REMOVED from this file.
// The authoritative hook is in shader_collection_patches.cpp which has the
// safety-net path push logic. Having it in both files caused a duplicate
// symbol conflict where the linker could pick this diagnostic-only version
// over the functional one.

// =============================================================================
// sub_828CD290 - grmShaderFactoryStandard::Preload (vtable slot[7])
// =============================================================================
//
// Decompiled logic:
//
//   void Preload(grmShaderFactoryStandard* self, const char* dirPath) {
//       fiPackfile::SetPath(VFS_ROOT, dirPath);
//       fiStream* list = fiPackfile::Open(VFS_ROOT, "preload", "list", 0, 1);
//       if (!list) {
//           // Fallback: load embedded default shaders
//           LoadShaderByName("embedded:/rage_im", 0);
//           LoadShaderByName("embedded:/rage_im", 0);
//           fiPackfile::Close(VFS_ROOT);
//           return;
//       }
//       printf("Preloading shaders from '%s'...", dirPath);
//       int rawParam = fiPackfile::GetRawPtr(VFS_ROOT, "preload", "list");
//       fiTokenizer tok(dirPath, list);
//       char line[128];
//       while (tok.ReadLine(line, 128)) {
//           char* dot = strrchr(line, '.');
//           if (!dot) continue;
//           if (_stricmp(dot, ".shadert") == 0) continue;  // unsupported
//           if (_stricmp(dot, ".fx") == 0 || _stricmp(dot, ".fx2") == 0) {
//               *dot = '\0';  // strip extension
//               LoadShaderByName(line, rawParam);           // sub_828C73F0
//           }
//       }
//       list->Close();
//       fiPackfile::Close(VFS_ROOT);
//   }
//
PPC_FUNC_IMPL(__imp__sub_828CD290);
PPC_FUNC_HOOK(sub_828CD290)
{
    LOG_INFO("shader_factory: grmShaderFactoryStandard::Preload entered");

    __imp__sub_828CD290(ctx, base);

    LOG_INFO("shader_factory: grmShaderFactoryStandard::Preload complete");
}

// =============================================================================
// sub_828C0B48 - Graphics Device + Shader Factory Setup
// =============================================================================
//
// Decompiled logic (1336 bytes):
//
//   void grcDevice_Setup(int param) {
//       // 1. Allocator init
//       g_allocResult = AllocatorInit(param);          // 0x831C2540
//
//       // 2. Read config
//       if (GetConfigValue(g_configNode, &tmp))
//           g_configVal = tmp;                          // 0x82B0B444
//
//       // 3. Query video mode
//       XVIDEO_MODE videoMode;
//       XGetVideoMode(&videoMode);
//       if (g_configFlag) ReadConfigPair(g_node1, &g_resolution);
//       ReadConfigPair(&g_widthNode,  &g_displayWidth);  // 0x82B0B454
//       ReadConfigPair(&g_heightNode, &g_displayHeight);  // 0x82B0B458
//
//       // 4. Clamp resolution
//       if (g_displayWidth == 1) {
//           g_displayWidth = min(videoMode.dwDisplayWidth, 1280);
//       }
//       if (g_displayHeight == 1) {
//           g_displayHeight = min(videoMode.dwDisplayHeight, 720);
//       }
//       g_renderWidth  = g_displayWidth;   // 0x82B0B45C
//       g_renderHeight = g_displayHeight;  // 0x82B0B460
//       g_flag1 = g_flag2 = 0;
//       g_initFlag = 1;
//
//       // 5. Pre-device init
//       sub_828C0160(g_val1, g_val2);
//       sub_828BF4F8();
//
//       // 6. Build device creation flags
//       int flags = (g_aaEnabled != 0) | 4;
//       if (g_msaaConfig) flags |= 8;
//
//       // 7. Create D3D device
//       if (!g_skipDeviceCreate) {
//           int hr = D3DDevice_Create(0, g_adapter, g_window, flags,
//                                     &g_presentParams, &g_device);
//           g_deviceFailed = (hr < 0) ? 1 : 0;
//       }
//       bool ok = !g_deviceFailed;
//       g_deviceCopy = g_device;            // 0x831C2294
//       g_deviceOk = ok;
//
//       // 8. Create secondary device for shadows
//       D3DDevice_Create(0, 2, 0, 8, 0, &g_shadowDevice);
//       g_shadowOk = ok;
//
//       // 9. Render targets + state
//       CreateRenderTargets();
//       SetRenderState(g_device, 0, g_defaultState);
//       sub_82A3C2B8(g_device, g_stateBlock);
//       SetSwapCallback(g_device, SwapCallback);
//
//       // 10. Vertex declarations (two tables)
//       if (!(g_vdeclFlags & 1)) {
//           g_vdeclFlags |= 1;
//           // Initialize 3-element vertex decl table (7 entries, stride 56)
//           // at 0x831C26C0..0x831C2710
//       }
//       if (!(g_vdeclFlags & 2)) {
//           g_vdeclFlags |= 2;
//           // Initialize 4-element vertex decl table (7 entries)
//           // at 0x831C2650..0x831C26BC
//       }
//       g_vdecl3 = CreateVertexDeclaration(&table3, 3); // 0x831C23CC
//       g_vdecl4 = CreateVertexDeclaration(&table4, 4); // 0x831C2298
//
//       // 11. Load built-in shaders
//       OpenCompiledShader(&g_builtinFx, g_shaderPath, 0); // 0x831C25A8
//       g_techDraw          = FindPassByName(&g_builtinFx, "draw", 1);
//       g_techUnlitDraw     = FindPassByName(&g_builtinFx, "unlit_draw", 1);
//       g_techDrawSkinned   = FindPassByName(&g_builtinFx, "drawskinned", 1);
//       g_techUnlitSkinned  = FindPassByName(&g_builtinFx, "unlit_drawskinned", 1);
//       g_techDrawBlit      = FindPassByName(&g_builtinFx, "drawblit", 1);
//       g_paramDiffuseTex   = FindTechByName(&g_builtinFx, "DiffuseTex", 1);
//
//       // 12. Final init
//       sub_828BE8A8();
//       g_cmdBuffer = rage::Alloc(16 * (g_cmdBufSize + 256));
//       ++g_heapRefCount;
//       void* eventObj = rage::Alloc(128);
//       if (eventObj) {
//           HANDLE evt = CreateEvent(g_eventConfig, -1, 0, 1028);
//           g_renderEvent = sub_82A581B0(eventObj, g_eventConfig, evt,
//                                        g_cmdBufSize, g_cmdBuffer, 0);
//       }
//       --g_heapRefCount;
//   }
//
PPC_FUNC_IMPL(__imp__sub_828C0B48);
PPC_FUNC_HOOK(sub_828C0B48)
{
    using namespace ShaderFactory;

    LOG_INFO("shader_factory: grcDevice::Setup entered");

    uint32_t devBefore = PPC_LOAD_U32(DEVICE_PTR);
    LOGF_INFO("shader_factory: Device ptr BEFORE setup: 0x{:08X}", devBefore);

    __imp__sub_828C0B48(ctx, base);

    uint32_t devAfter = PPC_LOAD_U32(DEVICE_PTR);
    LOGF_INFO("shader_factory: Device ptr AFTER setup: 0x{:08X}", devAfter);

    if (devAfter == 0)
    {
        LOG_WARNING("shader_factory: Device creation FAILED - device ptr still NULL");
    }
    else
    {
        uint32_t techDraw    = PPC_LOAD_U32(TECH_DRAW);
        uint32_t techBlit    = PPC_LOAD_U32(TECH_DRAWBLIT);
        uint32_t techSkinned = PPC_LOAD_U32(TECH_DRAWSKINNED);
        uint32_t paramDiff   = PPC_LOAD_U32(PARAM_DIFFUSETEX);
        LOGF_INFO("shader_factory: Techniques resolved - draw:{} blit:{} skinned:{} DiffuseTex:{}",
                  techDraw, techBlit, techSkinned, paramDiff);
    }
}

// =============================================================================
// Data Structure Reference
// =============================================================================
//
// grmShaderFx Object (112 bytes, allocated by sub_828C73F0):
//   +0:   name ptr (datString)
//   +4:   padding
//   +8:   === effect data start (passed to sub_828CAA60/sub_828CA318) ===
//   +8+0:   pixel shader array ptr
//   +8+4:   PS write cursor (u16) / PS capacity (u16)
//   +8+8:   vertex decl data (freed by sub_828CA028)
//   +8+16:  vertex shader array ptr
//   +8+20:  VS write cursor (u16) / VS capacity (u16)
//   +8+24:  VS input layout array ptr
//   +8+28:  VS write cursor (u16) / VS capacity (u16)
//   +8+32:  resolved path (datString) -- set by sub_828CAA60
//   +8+36:  name hash (atStringHash)  -- set by sub_828CAA60
//   +8+40:  file size (uint64_t)      -- set by sub_828CAA60
//   +8+48:  shader params             -- freed by sub_828C8210
//   +96:  UI param array ptr          -- built by sub_828C6E48
//   +100: UI param write cursor (u16) / UI param capacity (u16)
//   +104: shader group hash (int32_t, -1 = unset)
//
// Pixel/Vertex Shader Array Entry (12 bytes):
//   +0:  input layout count (int32_t)
//   +4:  input layout data ptr (uint32_t) -- 8 bytes per input
//   +8:  GPU shader handle (uint32_t) -- CreateVS/CreatePS result
//
// Input Layout Sub-Entry (8 bytes, array at entry+4):
//   +0:  semantic type (uint8_t)
//   +1:  semantic index (uint8_t)
//   +2:  format/flags (uint16_t)
//   +4:  name hash (uint32_t)
//
// Technique Table Entry (48 bytes, global table at 0x831C3DF0):
//   +0:   type (uint8_t)
//   +1:   element count (uint8_t)
//   +2:   sub-type (uint8_t)
//   +3:   param count (uint8_t)
//   +4:   name string ptr (datString)
//   +8:   annotation string ptr (datString)
//   +12:  name hash (atStringHash)
//   +16:  annotation hash (atStringHash)
//   +20:  param array ptr
//   +24:  value data ptr
//   +28:  VS technique index (uint16_t, 0xFFFF = unset)
//   +30:  PS technique index (uint16_t, 0xFFFF = unset)
//   +32:  sampler VS index (uint8_t) -- for type==6 only
//   +33:  sampler PS index (uint8_t)
//   +34:  sampler Size index (uint8_t)
//   +35:  padding
//   +36..47: reserved
//
// Pass Array Entry (16 bytes):
//   +0:   name hash (uint32_t)
//   +4:   sub-pass array ptr
//   +8:   sub-pass write cursor (u16) / capacity (u16)
//   +12:  reserved
//
// Sub-Pass Entry (32 bytes):
//   +0:   VS index (int32_t)
//   +4..11: padding
//   +12:  PS index (int32_t)
//   +16..31: padding
//
// FXC Binary Format ("axgr"):
//   [4B]  magic: 0x61786772 ("axgr" big-endian)
//   -- Pixel Shaders --
//   [4B]  pixel shader count
//   [N*]  pixel shader entries:
//     [ReadShaderInputLayout]  count + 8-byte entries
//     [2B]  srcSize (bytecode size for this platform)
//     [2B]  dstSize (bytecode size for other platform)
//     [srcSize bytes]  bytecode
//   -- Vertex Shaders --
//   [4B]  vertex shader count
//   [M*]  vertex shader entries:
//     [ReadShaderInputLayout]  count + 8-byte entries
//     [2B]  dstSize
//     [2B]  srcSize
//     if srcSize != 0:
//       [srcSize bytes]  bytecode
//   -- Techniques --
//   [4B]  technique count
//   [T*]  technique entries (ReadTechniqueData)
//   -- Parameters --
//   [4B]  parameter count
//   [P*]  parameter entries (ReadTechniqueData)
//   -- Passes --
//   [4B]  pass count
//   [A*]  pass entries (ReadPassData)
//
// Vertex Shader Descriptor (40 or 872 bytes, created by sub_82A42BA8):
//   +0:   flags (0x100007 = vertex shader type)
//   +4:   ref count (1)
//   +8..39: bytecode header copy (40 bytes)
//   +40:  raw bytecode pointer (separate allocation)
//   If flags bit 0 set: 872 bytes total (includes register map)
//
// Pixel Shader Descriptor (872 bytes, created by sub_82A42CB8):
//   +0:   flags (|= 0x100000 for PS type)
//   +872: bytecode header copy
//   Initialized via sub_82A42668 for PS-specific register mapping
//
// =============================================================================
// Class Hierarchy Reference
// =============================================================================
//
// rage::grmShaderFactoryStandard (RTTI, vtable @ 0x82094B84)
//   Inherits: rage::grmShaderFactory
//   Vtable (8 slots):
//     [0] dtor         sub_828CD0F0
//     [1] GetShader    sub_828CCE80
//     [2] FindShader   sub_828CCEC8
//     [3] GetByHash    sub_828CD058  (refs "shadergroup")
//     [4] GetCount     sub_828CCF10  (refs "Count")
//     [5] ???          sub_828CD158
//     [6] ???          sub_828CD228
//     [7] Preload      sub_828CD290  (our hook)
//
// rage::grmShaderFx (RTTI, vtable @ 0x82093E9C)
//   Inherits: rage::grmShader -> rage::pgBase -> rage::datBase
//   Vtable (22 slots):
//     [0] dtor           sub_828C6C08
//     [2] Serialize      sub_828CF8A0
//     [3] Clone          sub_828C7A30  (refs "sva")
//     [4] GetFormat      sub_828C6080
//     [9] Apply          sub_828C67B8
//     [12] GetParamName  sub_828C6378  (refs "ResourceName", "__unknown")
//     [13] GetParamIndex sub_828C6428
//     [16] SetPass       sub_828C7258
//
