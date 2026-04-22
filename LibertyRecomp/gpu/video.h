#pragma once

//#define ASYNC_PSO_DEBUG
/////////////////////////////////////////////////////////////////////#define PSO_CACHING
//#define PSO_CACHING_CLEANUP

#include <plume_render_interface.h>
#include <os/logger.h>
#include <cstdint>

#define D3DCLEAR_TARGET  0x1
#define D3DCLEAR_ZBUFFER 0x10
#define D3DCLEAR_STENCIL 0x20

// TODO: remove
#define SPEC_CONSTANT_ALPHA_TO_COVERAGE (1 << 3)
#define SPEC_CONSTANT_REVERSE_Z         (1 << 4)

#define SPEC_CONSTANT_CONDITIONAL_SURVEY    (1 << 5)
#define SPEC_CONSTANT_CONDITIONAL_RENDERING (1 << 6)

#define LOAD_ZSTD_TEXTURE(name) LoadTexture(decompressZstd(name, name##_uncompressed_size).get(), name##_uncompressed_size)

using namespace plume;

struct Video
{
    static inline uint32_t s_viewportWidth;
    static inline uint32_t s_viewportHeight;

    static bool CreateHostDevice(const char *sdlVideoDriver, bool graphicsApiRetry);
    static void WaitOnSwapChain();
    static void Present();
    static void StartPipelinePrecompilation();
    static void WaitForGPU();
    static void ComputeViewportDimensions();
    static void OnGuestDeviceCreated();
};

enum class Backend {
    VULKAN,
    D3D12,
    METAL
};

// Get the current graphics backend
Backend GetCurrentBackend();

struct GuestSamplerState
{
    be<uint32_t> data[6];
};

// grcDevice — GTA IV v8 (default.xex). Size 0x5780, allocated 128-byte aligned
// by sub_82A416B8. Name kept as "GuestDevice" for search-compat with the
// UnleashedRecomp hook heritage; field offsets verified against PPC
// addi/rlwinm/mulli index math in the recomp codegen — see
// docs/unleashed-mirror-research/03-grcdevice-header-spec.md.
//
// NOTE: GTA IV does NOT use Sonic/Unleashed's per-device
// setRenderStateFunctions[] / setSamplerStateFunctions[] LUTs — its
// renderstate funnel sub_828C19C0 is an inline `bctr` switch against a
// read-only jump table at .rodata:0x828C1A04. Those fields are absent here.
struct GuestDevice
{
    // ---- 0x0000 : Dirty-flag qwords (5 words, 40 bytes) ----
    be<uint64_t> dirtyFlagVsFloat;      // +0x00  sub_82A42168  VS float-const bitmask
    be<uint64_t> dirtyFlagPsFloat;      // +0x08  sub_82A42250  PS float-const bitmask
    be<uint64_t> dirtyFlagState;        // +0x10  sub_82A424A8/760/930
    be<uint64_t> dirtyFlagTexture;      // +0x18  sub_82A44B78  texture-stage bitmask
    be<uint64_t> dirtyFlagBoolInt;      // +0x20  sub_82A42338/398/3F8/450

    // ---- 0x0028..0x0480 : PM4 work-pointer pair + uncharted filler ----
    uint8_t padding0028[0x458];

    // ---- 0x0480 : Sampler state array (24 × 24 bytes) ----
    GuestSamplerState samplerStates[24];        // +0x480..+0x6C0
    uint8_t padding06C0[0xC0];                  // +0x6C0..+0x780

    // ---- 0x0780 : VS float constants [256 × vec4] (raw big-endian bytes) ----
    uint32_t vertexShaderFloatConstants[256 * 4];   // +0x0780..+0x1780

    // ---- 0x1780 : PS float constants [256 × vec4] (raw big-endian bytes) ----
    uint32_t pixelShaderFloatConstants[256 * 4];    // +0x1780..+0x2780

    // ---- 0x2780 : VS/PS bool + int constants ----
    be<uint32_t> vertexShaderBoolConstants[4];      // +0x2780
    be<uint32_t> pixelShaderBoolConstants[4];       // +0x2790
    be<uint32_t> vertexShaderIntConstants[16];      // +0x27A0
    be<uint32_t> pixelShaderIntConstants[16];       // +0x27E0

    // ---- 0x2820..0x2A94 : uncharted (sub_82A50820 writes qword at +0x2A80) ----
    uint8_t padding2820[0x274];

    // ---- 0x2A94 : PM4 / shader-push state ----
    be<uint32_t> streamSourceLive;              // +0x2A94
    uint8_t padding2A98[0x04];
    be<uint32_t> shaderPushCmdPtr;              // +0x2A9C
    be<uint32_t> shaderPushMask;                // +0x2AA0
    uint8_t padding2AA4[0x04];
    be<uint32_t> renderStatePacked;             // +0x2AA8

    uint8_t padding2AAC[0x10];                  // +0x2AAC..+0x2ABC
    uint8_t stateByteA;                         // +0x2ABC
    uint8_t stateByteB;                         // +0x2ABD
    uint8_t currentShaderDirty;                 // +0x2ABE
    uint8_t padding2ABF[0x365];                 // +0x2ABF..+0x2E24

    // ---- 0x2E24 : Vertex declaration + shadow ----
    be<uint32_t> vertexDeclaration;             // +0x2E24
    uint8_t padding2E28[0x08];
    uint8_t currentVdeclStreamCount;            // +0x2E30
    uint8_t padding2E31[0x75];                  // +0x2E31..+0x2EA6

    // ---- 0x2EA6 / 0x2EC0 : per-sampler shadow bytes (26 each) ----
    uint8_t samplerMipFilter[26];               // +0x2EA6..+0x2EC0
    uint8_t samplerMaxAniso[26];                // +0x2EC0..+0x2EDA
    uint8_t padding2EDA[0x20E];                 // +0x2EDA..+0x30E8

    // ---- 0x30E8 : stream-source count ----
    uint8_t streamSourceCount;                  // +0x30E8
    uint8_t padding30E9[0x0F];                  // +0x30E9..+0x30F8

    // ---- 0x30F8 : Texture slot array (26 slots, matches sampler count) ----
    be<uint32_t> textures[26];                  // +0x30F8..+0x3160
    uint8_t padding3160[0x2C];                  // +0x3160..+0x318C

    // ---- 0x318C : Current shader pointers ----
    be<uint32_t> currentPixelShader;            // +0x318C
    be<uint32_t> currentVertexShader;           // +0x3190
    uint8_t padding3194[0x10];
    be<uint32_t> previousPixelShader;           // +0x31A4
    uint8_t padding31A8[0x31C];                 // +0x31A8..+0x34C4

    // ---- 0x34C4 : PM4 ring head/tail (draw-prologue writer side) ----
    be<uint32_t> pm4RingHead;                   // +0x34C4
    be<uint32_t> pm4RingTail;                   // +0x34C8
    uint8_t padding34CC[0xF8];                  // +0x34CC..+0x35C4

    // ---- 0x35C4 : Primary PM4 command-buffer descriptor ----
    be<uint32_t> cmdBufBase;                    // +0x35C4
    be<uint32_t> cmdBufHeadPacked;              // +0x35C8
    be<uint32_t> cmdBufGpuAddr;                 // +0x35CC
    uint8_t padding35D0[0x80];                  // +0x35D0..+0x3650

    // ---- 0x3650 : PM4 descriptor ring (112 × 8 B) ----
    struct { be<uint32_t> flags; be<uint32_t> gpuAddr; } pm4Ring[112];
                                                // +0x3650..+0x39D0

    // ---- 0x39D0 : Secondary swap-chain head / GPU pointer ----
    be<uint32_t> swapRingHead;                  // +0x39D0
    be<uint32_t> swapRingGpu;                   // +0x39D4
    uint8_t padding39D8[0x6C8];                 // +0x39D8..+0x40A0

    // ---- 0x40A0 : Frame counter + ring-sync producer/consumer indices ----
    be<uint32_t> frameCounter;                  // +0x40A0
    uint8_t padding40A4[0x04];
    be<uint32_t> ringEnqueueCounter;            // +0x40A8
    uint8_t padding40AC[0x80];                  // +0x40AC..+0x412C
    be<uint32_t> ringSlotIndex;                 // +0x412C
    be<uint32_t> ringSlotLimit;                 // +0x4130
    uint8_t padding4134[0x1378];                // +0x4134..+0x54AC

    // ---- 0x54AC : Submission timebase stamp (mftb) ----
    be<uint32_t> submissionTimebase;            // +0x54AC
    uint8_t padding54B0[0x2D0];                 // +0x54B0..+0x5780
};

static_assert(sizeof(GuestDevice) == 0x5780);

enum class ResourceType
{
    Texture,
    VolumeTexture,
    ArrayTexture,
    VertexBuffer,
    IndexBuffer,
    RenderTarget,
    DepthStencil,
    VertexDeclaration,
    VertexShader,
    PixelShader
};

struct GuestResource
{
    uint32_t unused = 0;
    be<uint32_t> refCount = 1;
    ResourceType type;

    GuestResource(ResourceType type) : type(type) 
    {
    }

    void AddRef()
    {
#ifdef __ANDROID__
        uint32_t originalValue, incrementedValue;
        do
        {
            originalValue = __atomic_load_n(&refCount.value, __ATOMIC_RELAXED);
            incrementedValue = ByteSwap(ByteSwap(originalValue) + 1);
        } while (!__atomic_compare_exchange_n(&refCount.value, &originalValue, incrementedValue, true, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED));
#else
        std::atomic_ref atomicRef(refCount.value);

        uint32_t originalValue, incrementedValue;
        do
        {
            originalValue = refCount.value;
            incrementedValue = ByteSwap(ByteSwap(originalValue) + 1);
        } while (!atomicRef.compare_exchange_weak(originalValue, incrementedValue));
#endif
    }

    void Release()
    {
#ifdef __ANDROID__
        uint32_t originalValue, decrementedValue;
        do
        {
            originalValue = __atomic_load_n(&refCount.value, __ATOMIC_RELAXED);
            decrementedValue = ByteSwap(ByteSwap(originalValue) - 1);
        } while (!__atomic_compare_exchange_n(&refCount.value, &originalValue, decrementedValue, true, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED));
#else
        std::atomic_ref atomicRef(refCount.value);

        uint32_t originalValue, decrementedValue;
        do
        {
            originalValue = refCount.value;
            decrementedValue = ByteSwap(ByteSwap(originalValue) - 1);
        } while (!atomicRef.compare_exchange_weak(originalValue, decrementedValue));
#endif

        // Normally we are supposed to release here, so only use this
        // function when you know you won't be the one destructing it.
    }
};

enum GuestFormat
{
    D3DFMT_A16B16G16R16F = 0x1A22AB60,
    D3DFMT_A16B16G16R16F_2 = 0x1A2201BF,
    D3DFMT_A16B16G16R16F_EXPAND = 0x1A22AB5D,
    D3DFMT_DXT1 = 0x1A200152,
    D3DFMT_DXT4 = 0x1A200154,
    D3DFMT_A8B8G8R8 = 0x1A200186,
    D3DFMT_A8R8G8B8 = 0x18280186,
    D3DFMT_LIN_A8R8G8B8 = 0x18280086,
    D3DFMT_D24FS8 = 0x1A220197,
    D3DFMT_D24S8 = 0x2D200196,
    D3DFMT_R32F = 0x2DA2ABA4,
    D3DFMT_G16R16F = 0x2D22AB9F,
    D3DFMT_G16R16F_2 = 0x2D20AB8D,
    D3DFMT_INDEX16 = 1,
    D3DFMT_INDEX32 = 6,
    D3DFMT_A8 = 0x4900102,
    D3DFMT_L8 = 0x28000102,
    D3DFMT_L8_2 = 0x28000002,
    D3DFMT_X8R8G8B8 = 0x28280086,
    D3DFMT_LE_X8R8G8B8 = 0x28280106,
    D3DFMT_UNKNOWN = 0xFFFFFFFF
};

struct GuestBaseTexture : GuestResource
{
    std::unique_ptr<RenderTexture> textureHolder;
    RenderTexture* texture = nullptr;
    std::unique_ptr<RenderTextureView> textureView;
    uint32_t width = 0;
    uint32_t height = 0;
    RenderFormat format = RenderFormat::UNKNOWN;
    uint32_t descriptorIndex = 0;
    RenderTextureLayout layout = RenderTextureLayout::UNKNOWN;

    GuestBaseTexture(ResourceType type) : GuestResource(type)
    {
    }
};

// Texture/VolumeTexture
struct GuestTexture : GuestBaseTexture
{
    uint32_t depth = 0;
    uint32_t mipLevels = 1;
    RenderTextureViewDimension viewDimension = RenderTextureViewDimension::UNKNOWN;
    void* mappedMemory = nullptr;
    ankerl::unordered_dense::map<uint32_t, std::unique_ptr<RenderFramebuffer>> framebuffers;
    std::vector<std::unique_ptr<RenderTextureView>> framebufferViews;
    std::unique_ptr<GuestTexture> patchedTexture;
    struct GuestSurface* sourceSurface = nullptr;
};

struct GuestLockedRect
{
    be<uint32_t> pitch;
    be<uint32_t> bits;
};

struct GuestBufferDesc
{
    be<uint32_t> format;
    be<uint32_t> type;
    be<uint32_t> usage;
    be<uint32_t> pool;
    be<uint32_t> size;
    be<uint32_t> fvf;
};

// VertexBuffer/IndexBuffer
struct GuestBuffer : GuestResource
{
    std::unique_ptr<RenderBuffer> buffer;
    void* mappedMemory = nullptr;
    uint32_t dataSize = 0;
    RenderFormat format = RenderFormat::UNKNOWN;
    uint32_t guestFormat = 0;
    bool lockedReadOnly = false;
};

struct GuestSurfaceDesc
{
    be<uint32_t> format;
    be<uint32_t> type;
    be<uint32_t> usage;
    be<uint32_t> pool;
    be<uint32_t> multiSampleType;
    be<uint32_t> multiSampleQuality;
    be<uint32_t> width;
    be<uint32_t> height;
};

// =============================================================================
// GuestTextureLevelDesc - Output structure from sub_829E5C38
// This is the Xbox 360 D3D texture level descriptor that the game queries
// when it needs texture dimensions/format info for render target creation.
//
// Based on IDA pseudocode analysis at lines 2677830-2677960:
// - Caller sub_8286BAE0 uses this to create matching render target backups
// - Returns type, dimensions, format, and pitch for a specific mip level
// =============================================================================
struct GuestTextureLevelDesc
{
    // Texture type (matches Xbox 360 D3D resource types):
    //   3  = 2D Texture
    //   4  = Render Target
    //   16 = Linear Texture
    //   17 = Volume/3D Texture
    //   18 = Cube Map
    //   19 = Array Texture
    //   20 = Texture Array
    uint32_t type;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t format;      // Xbox 360 D3DFMT_* code (e.g., 0x1A22AB60)
    uint32_t reserved[9]; // Padding to offset +56
    uint32_t pitch;       // Row pitch in bytes at offset +56
};

// =============================================================================
// GetFormatBytesPerPixel - Calculate bytes per pixel for Xbox 360 formats
// Used by sub_829E5C38 replacement to compute pitch from width
// =============================================================================
inline uint32_t GetFormatBytesPerPixel(uint32_t guestFormat)
{
    switch (guestFormat)
    {
    // 128-bit formats (16 bytes per pixel)
    case D3DFMT_A16B16G16R16F:
    case D3DFMT_A16B16G16R16F_2:
    case D3DFMT_A16B16G16R16F_EXPAND:
        return 8; // 4 channels * 16-bit = 64-bit = 8 bytes
    
    // 64-bit formats (8 bytes per pixel)
    case D3DFMT_G16R16F:
    case D3DFMT_G16R16F_2:
        return 4; // 2 channels * 16-bit = 32-bit = 4 bytes
    
    // 32-bit formats (4 bytes per pixel)
    case D3DFMT_A8B8G8R8:
    case D3DFMT_A8R8G8B8:
    case D3DFMT_LIN_A8R8G8B8:
    case D3DFMT_X8R8G8B8:
    case D3DFMT_LE_X8R8G8B8:
    case D3DFMT_D24FS8:
    case D3DFMT_D24S8:
    case D3DFMT_R32F:
    case D3DFMT_INDEX32:
        return 4;
    
    // 16-bit formats (2 bytes per pixel)
    case D3DFMT_INDEX16:
        return 2;
    
    // 8-bit formats (1 byte per pixel)
    case D3DFMT_A8:
    case D3DFMT_L8:
    case D3DFMT_L8_2:
        return 1;
    
    // Compressed formats - return block size / pixels per block
    case D3DFMT_DXT1:
        return 8;  // 8 bytes per 4x4 block = 0.5 bytes/pixel (handle specially)
    case D3DFMT_DXT4:
        return 16; // 16 bytes per 4x4 block = 1 byte/pixel (handle specially)
    
    default:
        return 4;  // Default to 32-bit
    }
}

// =============================================================================
// GetTextureTypeFromResource - Map GuestResource type to Xbox 360 D3D type
// =============================================================================
inline uint32_t GetTextureTypeFromResource(ResourceType type)
{
    switch (type)
    {
    case ResourceType::Texture:        return 3;  // D3DRTYPE_TEXTURE
    case ResourceType::RenderTarget:   return 4;  // D3DRTYPE_SURFACE (RT)
    case ResourceType::VolumeTexture:  return 17; // D3DRTYPE_VOLUMETEXTURE
    case ResourceType::ArrayTexture:   return 19; // D3DRTYPE_ARRAYTEXTURE
    case ResourceType::DepthStencil:   return 4;  // D3DRTYPE_SURFACE (DS)
    default:                           return 3;
    }
}

struct GuestSurfaceCreateParams
{
    be<uint32_t> base;
    be<uint32_t> hzBase;
    be<int32_t> colorExpBias;
};

// RenderTarget/DepthStencil
struct GuestSurface : GuestBaseTexture
{
    uint32_t guestFormat = 0;
    ankerl::unordered_dense::map<const RenderTexture*, std::unique_ptr<RenderFramebuffer>> framebuffers;
    RenderSampleCounts sampleCount = RenderSampleCount::COUNT_1;
    ankerl::unordered_dense::map<GuestTexture*, uint32_t> destinationTextures;
    bool wasCached = false;
};

enum GuestDeclType
{
    D3DDECLTYPE_FLOAT1 = 0x2C83A4,
    D3DDECLTYPE_FLOAT2 = 0x2C23A5,
    D3DDECLTYPE_FLOAT3 = 0x2A23B9,
    D3DDECLTYPE_FLOAT4 = 0x1A23A6,
    D3DDECLTYPE_D3DCOLOR = 0x182886,
    D3DDECLTYPE_UBYTE4 = 0x1A2286,
    D3DDECLTYPE_UBYTE4_2 = 0x1A2386,
    D3DDECLTYPE_SHORT2 = 0x2C2359,
    D3DDECLTYPE_SHORT4 = 0x1A235A,
    D3DDECLTYPE_UBYTE4N = 0x1A2086,
    D3DDECLTYPE_UBYTE4N_2 = 0x1A2186,
    D3DDECLTYPE_SHORT2N = 0x2C2159,
    D3DDECLTYPE_SHORT4N = 0x1A215A,
    D3DDECLTYPE_USHORT2N = 0x2C2059,
    D3DDECLTYPE_USHORT4N = 0x1A205A,
    D3DDECLTYPE_UINT1 = 0x2C82A1,
    D3DDECLTYPE_UDEC3 = 0x2A2287,
    D3DDECLTYPE_DEC3N = 0x2A2187,
    D3DDECLTYPE_DEC3N_2 = 0x2A2190,
    D3DDECLTYPE_DEC3N_3 = 0x2A2390,
    D3DDECLTYPE_FLOAT16_2 = 0x2C235F,
    D3DDECLTYPE_FLOAT16_4 = 0x1A2360,
    D3DDECLTYPE_UNUSED = 0xFFFFFFFF
};

enum GuestDeclUsage
{
    D3DDECLUSAGE_POSITION = 0,
    D3DDECLUSAGE_BLENDWEIGHT = 1,
    D3DDECLUSAGE_BLENDINDICES = 2,
    D3DDECLUSAGE_NORMAL = 3,
    D3DDECLUSAGE_PSIZE = 4,
    D3DDECLUSAGE_TEXCOORD = 5,
    D3DDECLUSAGE_TANGENT = 6,
    D3DDECLUSAGE_BINORMAL = 7,
    D3DDECLUSAGE_TESSFACTOR = 8,
    D3DDECLUSAGE_POSITIONT = 9,
    D3DDECLUSAGE_COLOR = 10,
    D3DDECLUSAGE_FOG = 11,
    D3DDECLUSAGE_DEPTH = 12,
    D3DDECLUSAGE_SAMPLE = 13
};

struct GuestVertexElement
{
    be<uint16_t> stream;
    be<uint16_t> offset;
    be<uint32_t> type;
    uint8_t method;
    uint8_t usage;
    uint8_t usageIndex;
    uint8_t padding;
};

#define D3DDECL_END() { 255, 0, 0xFFFFFFFF, 0, 0, 0 }

struct GuestVertexDeclaration : GuestResource
{
    XXH64_hash_t hash = 0;
    std::unique_ptr<RenderInputElement[]> inputElements;
    std::unique_ptr<GuestVertexElement[]> vertexElements;
    uint32_t inputElementCount = 0;
    uint32_t vertexElementCount = 0;
    uint32_t swappedTexcoords = 0;
    uint32_t swappedNormals = 0;
    uint32_t swappedBinormals = 0;
    uint32_t swappedTangents = 0;
    uint32_t swappedBlendWeights = 0;
    bool hasR11G11B10Normal = false;
    bool vertexStreams[16]{};
    uint32_t indexVertexStream = 0;
};

// VertexShader/PixelShader
struct GuestShader : GuestResource
{
    Mutex mutex;
    std::unique_ptr<RenderShader> shader;
    struct ShaderCacheEntry* shaderCacheEntry = nullptr;
    ankerl::unordered_dense::map<uint32_t, std::unique_ptr<RenderShader>> linkedShaders;
#ifdef LIBERTY_RECOMP_D3D12
    std::vector<ComPtr<IDxcBlob>> shaderBlobs;
    ComPtr<IDxcBlobEncoding> libraryBlob;
#endif
#ifdef ASYNC_PSO_DEBUG
    const char* name = "<unknown>";
#endif
};

struct GuestViewport
{
    be<uint32_t> x;
    be<uint32_t> y;
    be<uint32_t> width;
    be<uint32_t> height;
    be<float> minZ;
    be<float> maxZ;
};

struct GuestRect
{
    be<int32_t> left;
    be<int32_t> top;
    be<int32_t> right;
    be<int32_t> bottom;
};

enum GuestRenderState
{
    D3DRS_ZENABLE = 40,
    D3DRS_ZFUNC = 44,
    D3DRS_ZWRITEENABLE = 48,
    D3DRS_CULLMODE = 56,
    D3DRS_ALPHABLENDENABLE = 60,
    D3DRS_SRCBLEND = 72,
    D3DRS_DESTBLEND = 76,
    D3DRS_BLENDOP = 80,
    D3DRS_SRCBLENDALPHA = 84,
    D3DRS_DESTBLENDALPHA = 88,
    D3DRS_BLENDOPALPHA = 92,
    D3DRS_ALPHATESTENABLE = 96,
    D3DRS_ALPHAREF = 100,
    D3DRS_STENCILENABLE = 108,
    D3DRS_TWOSIDEDSTENCILMODE = 112,
    D3DRS_STENCILFAIL = 116,
    D3DRS_STENCILZFAIL = 120,
    D3DRS_STENCILPASS = 124,
    D3DRS_STENCILFUNC = 128,
    D3DRS_STENCILREF = 132,
    D3DRS_STENCILMASK = 136,
    D3DRS_STENCILWRITEMASK = 140,
    D3DRS_CCW_STENCILFAIL = 144,
    D3DRS_CCW_STENCILZFAIL = 148,
    D3DRS_CCW_STENCILPASS = 152,
    D3DRS_CCW_STENCILFUNC = 156,
    D3DRS_CLIPPLANEENABLE = 172,
    D3DRS_SCISSORTESTENABLE = 200,
    D3DRS_SLOPESCALEDEPTHBIAS = 204,
    D3DRS_DEPTHBIAS = 208,
    D3DRS_COLORWRITEENABLE = 212
};

enum GuestCullMode
{
    D3DCULL_NONE_CCW = 0,
    D3DCULL_FRONT_CCW = 1,
    D3DCULL_BACK_CCW = 2,
    D3DCULL_NONE_CW = 4,
    D3DCULL_FRONT_CW = 5,
    D3DCULL_BACK_CW = 6
};

enum GuestBlendMode
{
    D3DBLEND_ZERO = 0,
    D3DBLEND_ONE = 1,
    D3DBLEND_SRCCOLOR = 4,
    D3DBLEND_INVSRCCOLOR = 5,
    D3DBLEND_SRCALPHA = 6,
    D3DBLEND_INVSRCALPHA = 7,
    D3DBLEND_DESTCOLOR = 8,
    D3DBLEND_INVDESTCOLOR = 9,
    D3DBLEND_DESTALPHA = 10,
    D3DBLEND_INVDESTALPHA = 11
};

enum GuestBlendOp
{
    D3DBLENDOP_ADD = 0,
    D3DBLENDOP_SUBTRACT = 1,
    D3DBLENDOP_MIN = 2,
    D3DBLENDOP_MAX = 3,
    D3DBLENDOP_REVSUBTRACT = 4
};

enum GuestCmpFunc
{
    D3DCMP_NEVER = 0,
    D3DCMP_LESS = 1,
    D3DCMP_EQUAL = 2,
    D3DCMP_LESSEQUAL = 3,
    D3DCMP_GREATER = 4,
    D3DCMP_NOTEQUAL = 5,
    D3DCMP_GREATEREQUAL = 6,
    D3DCMP_ALWAYS = 7
};

enum GuestStencilOp
{
    D3DSTENCILOP_KEEP = 0,
    D3DSTENCILOP_ZERO = 1,
    D3DSTENCILOP_REPLACE = 2,
    D3DSTENCILOP_INCRSAT = 3,
    D3DSTENCILOP_DECRSAT = 4,
    D3DSTENCILOP_INVERT = 5,
    D3DSTENCILOP_INCR = 6,
    D3DSTENCILOP_DECR = 7
};

enum GuestPrimitiveType
{
    D3DPT_POINTLIST = 1,
    D3DPT_LINELIST = 2,
    D3DPT_LINESTRIP = 3,
    D3DPT_TRIANGLELIST = 4,
    D3DPT_TRIANGLEFAN = 5,
    D3DPT_RECTLIST = 6,      // Xenos 3-vertex screen-aligned quad (postfx, UI)
    D3DPT_TRIANGLESTRIP = 8, // Corrected: Xenos TriStrip opcode is 8 not 6
    D3DPT_QUADLIST = 13
};

enum GuestTextureFilterType
{
    D3DTEXF_POINT = 0,
    D3DTEXF_LINEAR = 1,
    D3DTEXF_NONE = 2
};

enum GuestTextureAddress
{
    D3DTADDRESS_WRAP = 0,
    D3DTADDRESS_MIRROR = 1,
    D3DTADDRESS_CLAMP = 2,
    D3DTADDRESS_MIRRORONCE = 3,
    D3DTADDRESS_BORDER = 6
};

inline bool g_needsResize;

extern std::unique_ptr<GuestTexture> LoadTexture(const uint8_t* data, size_t dataSize, RenderComponentMapping componentMapping = RenderComponentMapping());

extern void VideoConfigValueChangedCallback(class IConfigDef* config);
