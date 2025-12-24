#pragma once

#include <kernel/xdm.h>

// Use pthreads directly on macOS to be able to increase default stack size.
#ifdef __APPLE__
#define USE_PTHREAD 1
#endif

#ifdef USE_PTHREAD
#include <pthread.h>
#endif

#define CURRENT_THREAD_HANDLE uint32_t(-2)

// =============================================================================
// Xbox 360 Thread Context Structures
// =============================================================================
// These structures match the Xbox 360 kernel's expected thread layout.
// Based on Xenia's X_KPCR and X_KTHREAD definitions.
// 
// Memory layout per thread:
//   [PCR: 0xAB0 bytes] [TLS: 0x100 bytes] [TEB: 0x2E0 bytes] [Stack: 0x80000 bytes]
//
// Register r13 points to the start of PCR (the thread context base).
// =============================================================================

// Thread context sizes
constexpr size_t X360_PCR_SIZE   = 0xAB0;   // Processor Control Region
constexpr size_t X360_TLS_SIZE   = 0x100;   // Thread Local Storage slots
constexpr size_t X360_TEB_SIZE   = 0x2E0;   // Thread Environment Block
constexpr size_t X360_STACK_SIZE = 0x80000; // 512KB stack per thread
constexpr size_t X360_THREAD_CONTEXT_TOTAL = X360_PCR_SIZE + X360_TLS_SIZE + X360_TEB_SIZE + X360_STACK_SIZE;

// Computed offsets
constexpr size_t X360_TLS_OFFSET = X360_PCR_SIZE;
constexpr size_t X360_TEB_OFFSET = X360_PCR_SIZE + X360_TLS_SIZE;
constexpr size_t X360_STACK_OFFSET = X360_PCR_SIZE + X360_TLS_SIZE + X360_TEB_SIZE;

// -----------------------------------------------------------------------------
// X360_PCR - Processor Control Region (at offset 0x00 from r13)
// -----------------------------------------------------------------------------
// This is the per-thread processor context that games access via r13.
// Based on Xenia's X_KPCR structure.
// -----------------------------------------------------------------------------
#pragma pack(push, 1)
struct X360_PCR {
    be<uint32_t> tls_ptr;              // 0x00 - Pointer to TLS block
    uint8_t      _reserved_04[0x2C];   // 0x04
    be<uint32_t> pcr_ptr;              // 0x30 - Self-pointer (for validation)
    uint8_t      _reserved_34[0x3C];   // 0x34
    be<uint32_t> stack_base;           // 0x70 - Stack base address (high addr)
    be<uint32_t> stack_limit;          // 0x74 - Stack limit address (low addr)
    uint8_t      _reserved_78[0x88];   // 0x78
    be<uint32_t> current_thread;       // 0x100 - Pointer to TEB
    uint8_t      _reserved_104[0x8];   // 0x104
    uint8_t      current_cpu;          // 0x10C - Current CPU/worker index
    uint8_t      _reserved_10D[0x43];  // 0x10D
    be<uint32_t> dpc_active;           // 0x150 - DPC active / thread initialized flag
    uint8_t      _reserved_154[0x95C]; // 0x154 - Padding to 0xAB0
};
static_assert(sizeof(X360_PCR) == X360_PCR_SIZE, "X360_PCR size mismatch");
#pragma pack(pop)

// -----------------------------------------------------------------------------
// X360_TLS - Thread Local Storage (at offset 0xAB0 from r13)
// -----------------------------------------------------------------------------
// Dynamic TLS slots allocated via KeTlsAlloc.
// Slot 0x10 has a quirky initialization (set to 0xFFFFFFFF).
// -----------------------------------------------------------------------------
#pragma pack(push, 1)
struct X360_TLS {
    uint8_t      _reserved_00[0x10];   // 0x00
    be<uint32_t> quirky_slot;          // 0x10 - Special TLS slot (init to 0xFFFFFFFF)
    uint8_t      _reserved_14[0xEC];   // 0x14 - Remaining TLS slots
};
static_assert(sizeof(X360_TLS) == X360_TLS_SIZE, "X360_TLS size mismatch");
#pragma pack(pop)

// -----------------------------------------------------------------------------
// X360_TEB - Thread Environment Block (at offset 0xBB0 from r13)
// -----------------------------------------------------------------------------
// Per-thread state including thread ID, error codes, etc.
// Based on Xenia's X_KTHREAD structure.
// -----------------------------------------------------------------------------
#pragma pack(push, 1)
struct X360_TEB {
    uint8_t      _reserved_00[0x14C];  // 0x00
    be<uint32_t> thread_id;            // 0x14C - Thread identifier
    be<uint32_t> start_address;        // 0x150 - Thread entry point
    uint8_t      _reserved_154[0xC];   // 0x154
    be<uint32_t> last_error;           // 0x160 - Win32 last error code
    be<uint32_t> fiber_ptr;            // 0x164 - Fiber data pointer (usually null)
    uint8_t      _reserved_168[0x4];   // 0x168
    be<uint32_t> creation_flags;       // 0x16C - Thread creation flags
    uint8_t      _reserved_170[0x170]; // 0x170 - Padding to 0x2E0
};
static_assert(sizeof(X360_TEB) == X360_TEB_SIZE, "X360_TEB size mismatch");
#pragma pack(pop)

struct GuestThreadContext
{
    PPCContext ppcContext{};
    uint8_t* thread = nullptr;

    GuestThreadContext(uint32_t cpuNumber);
    ~GuestThreadContext();
};

struct GuestThreadParams
{
    uint32_t function;
    uint32_t value;
    uint32_t value2;
    uint32_t flags;
};

struct GuestThreadHandle : KernelObject
{
    GuestThreadParams params;
    std::atomic<bool> suspended;
    #ifdef USE_PTHREAD
    pthread_t thread;
    #else
    std::thread thread;
    #endif
    // HACK(1)
    std::atomic<bool> isFinished = false;

    GuestThreadHandle(const GuestThreadParams& params);
    ~GuestThreadHandle() override;

    uint32_t GetThreadId() const;

    uint32_t Wait(uint32_t timeout) override;
};

struct GuestThread
{
    static uint32_t Start(const GuestThreadParams& params);
    static GuestThreadHandle* Start(const GuestThreadParams& params, uint32_t* threadId);

    static uint32_t GetCurrentThreadId();
    static void SetLastError(uint32_t error);

#ifdef _WIN32
    static void SetThreadName(uint32_t threadId, const char* name);
#endif
};
