#include "debugger.h"
#include "debug_state.h"
#include "command_parser.h"

#include <atomic>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>

#ifdef _WIN32
#include <conio.h>
#else
#if !defined(__SWITCH__)
#include <termios.h>
#endif
#include <unistd.h>
#endif

namespace Debugger {

// =============================================================================
// Global State
// =============================================================================

static std::atomic<bool> g_initialized{false};
static std::atomic<bool> g_cliRunning{false};
static std::thread g_cliThread;

static DebugState* g_state = nullptr;
static CommandParser* g_parser = nullptr;

static void* g_memoryBase = nullptr;
static size_t g_memorySize = 0;

static std::mutex g_funcNameMutex;
static std::unordered_map<uint32_t, std::string> g_funcNames;

static std::mutex g_threadMutex;
static std::unordered_map<uint32_t, ThreadInfo> g_threads;

// =============================================================================
// Forward declarations for command registration
// =============================================================================
void RegisterMemoryCommands(CommandParser& parser);
void RegisterThreadCommands(CommandParser& parser);
void RegisterSyncCommands(CommandParser& parser);
void RegisterExecCommands(CommandParser& parser);
void RegisterPPCCommands(CommandParser& parser);
void RegisterGPUCommands(CommandParser& parser);
void RegisterFileCommands(CommandParser& parser);
void RegisterCtorCommands(CommandParser& parser);
void RegisterUtilCommands(CommandParser& parser);

// =============================================================================
// CLI Thread
// =============================================================================

static void CLIThreadFunc() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║           LibertyRecomp Interactive Debugger                  ║\n");
    printf("║  Type 'help' for commands, 'help <cmd>' for usage             ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    std::string line;
    while (g_cliRunning.load()) {
        printf("liberty> ");
        fflush(stdout);
        
        if (!std::getline(std::cin, line)) {
            // EOF or error
            break;
        }
        
        // Trim whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end = line.find_last_not_of(" \t\r\n");
        if (start == std::string::npos) {
            continue;  // Empty line
        }
        line = line.substr(start, end - start + 1);
        
        if (line.empty()) {
            continue;
        }
        
        // Execute command
        g_parser->Execute(line, *g_state, g_memoryBase, std::cout);
    }
    
    printf("Debugger CLI exiting.\n");
}

// =============================================================================
// Lifecycle
// =============================================================================

void Initialize() {
    if (g_initialized.exchange(true)) {
        return;  // Already initialized
    }
    
    g_state = new DebugState();
    g_parser = new CommandParser();
    
    // Register all command categories
    RegisterMemoryCommands(*g_parser);
    RegisterThreadCommands(*g_parser);
    RegisterSyncCommands(*g_parser);
    RegisterExecCommands(*g_parser);
    RegisterPPCCommands(*g_parser);
    RegisterGPUCommands(*g_parser);
    RegisterFileCommands(*g_parser);
    RegisterCtorCommands(*g_parser);
    RegisterUtilCommands(*g_parser);
    
    printf("[Debugger] Initialized\n");
}

void Shutdown() {
    if (!g_initialized.load()) {
        return;
    }
    
    StopCLIThread();
    
    delete g_parser;
    g_parser = nullptr;
    
    delete g_state;
    g_state = nullptr;
    
    g_initialized.store(false);
    printf("[Debugger] Shutdown\n");
}

void StartCLIThread() {
    if (!g_initialized.load()) {
        Initialize();
    }
    
    if (g_cliRunning.exchange(true)) {
        return;  // Already running
    }
    
    g_cliThread = std::thread(CLIThreadFunc);
}

void StopCLIThread() {
    if (!g_cliRunning.exchange(false)) {
        return;  // Not running
    }
    
    // Note: We can't easily interrupt std::getline
    // The thread will exit on next input or when stdin closes
    if (g_cliThread.joinable()) {
        g_cliThread.detach();  // Don't block on exit
    }
}

bool IsInitialized() {
    return g_initialized.load();
}

// =============================================================================
// Event Hooks
// =============================================================================

void OnNullCall(uint32_t lr, uint32_t targetAddr) {
    if (!g_initialized.load() || !g_state) return;
    g_state->RecordNullCall(lr, targetAddr);
}

} // namespace Debugger (temporarily close for C linkage)

// C linkage wrapper for macro integration
extern "C" void LibertyDebugger_OnNullCall(uint32_t lr, uint32_t targetAddr) {
    Debugger::OnNullCall(lr, targetAddr);
}

namespace Debugger { // reopen namespace

void OnFunctionEntry(uint32_t addr, void* ctx) {
    if (!g_initialized.load() || !g_state) return;
    
    g_state->Stats().functionCalls++;
    
    // Check for trace
    auto* trace = g_state->GetTrace(addr);
    if (trace && trace->enabled) {
        trace->callCount++;
        
        if (trace->logArgs) {
            // TODO: Log function arguments from ctx
            printf("[TRACE] Enter %s (call #%u)\n", 
                   GetFunctionName(addr).c_str(), trace->callCount);
        }
        
        // Disable if max calls reached
        if (trace->maxCalls > 0 && trace->callCount >= trace->maxCalls) {
            trace->enabled = false;
            printf("[TRACE] %s reached max calls (%u), tracing disabled\n",
                   GetFunctionName(addr).c_str(), trace->maxCalls);
        }
    }
    
    // Check for breakpoint
    auto* bp = g_state->GetBreakpoint(addr);
    if (bp && bp->enabled) {
        bp->hitCount++;
        printf("\n[BREAKPOINT] Hit at %s (hit #%lu)\n",
               GetFunctionName(addr).c_str(), bp->hitCount);
        g_state->Pause();
    }
    
    // Wait if paused
    g_state->WaitIfPaused();
}

void OnFunctionExit(uint32_t addr, void* ctx) {
    if (!g_initialized.load() || !g_state) return;
    
    auto* trace = g_state->GetTrace(addr);
    if (trace && trace->enabled && trace->logReturn) {
        // TODO: Log return value from ctx
        printf("[TRACE] Exit %s\n", GetFunctionName(addr).c_str());
    }
}

void OnMemoryRead(uint32_t addr, uint32_t size) {
    if (!g_initialized.load() || !g_state) return;
    
    if (g_state->CheckWatchpoint(addr, size, false)) {
        printf("\n[WATCHPOINT] Read at 0x%08X (size=%u)\n", addr, size);
        // Could pause here if desired
    }
}

void OnMemoryWrite(uint32_t addr, uint32_t size, uint64_t value) {
    if (!g_initialized.load() || !g_state) return;
    
    if (g_state->CheckWatchpoint(addr, size, true)) {
        printf("\n[WATCHPOINT] Write at 0x%08X: 0x%llX (size=%u)\n", 
               addr, (unsigned long long)value, size);
        // Could pause here if desired
    }
}

void OnFrame() {
    if (!g_initialized.load() || !g_state) return;
    g_state->Stats().frameCount++;
}

void OnAlloc(uint32_t addr, uint32_t size) {
    if (!g_initialized.load() || !g_state) return;
    g_state->Stats().allocCount++;
    g_state->Stats().totalAllocBytes += size;
}

void OnFree(uint32_t addr) {
    if (!g_initialized.load() || !g_state) return;
    g_state->Stats().freeCount++;
}

// =============================================================================
// State Access
// =============================================================================

DebugState& GetState() {
    if (!g_state) {
        Initialize();
    }
    return *g_state;
}

CommandParser& GetParser() {
    if (!g_parser) {
        Initialize();
    }
    return *g_parser;
}

bool IsPaused() {
    return g_state && g_state->IsPaused();
}

void Pause() {
    if (g_state) g_state->Pause();
}

void Continue() {
    if (g_state) g_state->Continue();
}

// =============================================================================
// Memory Access
// =============================================================================

void SetMemoryBase(void* base, size_t size) {
    g_memoryBase = base;
    g_memorySize = size;
}

void* TranslateAddress(uint32_t guestAddr) {
    if (!g_memoryBase || guestAddr >= g_memorySize) {
        return nullptr;
    }
    return static_cast<uint8_t*>(g_memoryBase) + guestAddr;
}

uint32_t MapVirtual(const void* hostPtr) {
    if (!g_memoryBase || !hostPtr) {
        return 0;
    }
    const uint8_t* host = static_cast<const uint8_t*>(hostPtr);
    const uint8_t* base = static_cast<const uint8_t*>(g_memoryBase);
    if (host < base || host >= base + g_memorySize) {
        return 0;
    }
    return static_cast<uint32_t>(host - base);
}

bool IsValidAddress(uint32_t guestAddr) {
    return guestAddr < g_memorySize;
}

bool ReadMemory(uint32_t guestAddr, void* dest, size_t size) {
    if (!IsValidAddress(guestAddr) || !IsValidAddress(guestAddr + size - 1)) {
        return false;
    }
    void* src = TranslateAddress(guestAddr);
    if (!src) return false;
    memcpy(dest, src, size);
    return true;
}

bool ReadU32BE(uint32_t guestAddr, uint32_t& outValue) {
    uint8_t bytes[4];
    if (!ReadMemory(guestAddr, bytes, 4)) {
        return false;
    }
    outValue = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    return true;
}

bool ReadU16BE(uint32_t guestAddr, uint16_t& outValue) {
    uint8_t bytes[2];
    if (!ReadMemory(guestAddr, bytes, 2)) {
        return false;
    }
    outValue = (bytes[0] << 8) | bytes[1];
    return true;
}

// =============================================================================
// Function Names
// =============================================================================

void RegisterFunctionName(uint32_t addr, const std::string& name) {
    std::lock_guard<std::mutex> lock(g_funcNameMutex);
    g_funcNames[addr] = name;
}

std::string GetFunctionName(uint32_t addr) {
    {
        std::lock_guard<std::mutex> lock(g_funcNameMutex);
        auto it = g_funcNames.find(addr);
        if (it != g_funcNames.end()) {
            return it->second;
        }
    }
    // Return default format
    char buf[32];
    snprintf(buf, sizeof(buf), "sub_%08X", addr);
    return buf;
}

// =============================================================================
// Thread Registry
// =============================================================================

void RegisterThread(uint32_t threadId, uint32_t entryPoint, const std::string& name) {
    std::lock_guard<std::mutex> lock(g_threadMutex);
    ThreadInfo info;
    info.id = threadId;
    info.entryPoint = entryPoint;
    info.name = name.empty() ? ("Thread_" + std::to_string(threadId)) : name;
    info.state = ThreadInfo::State::Running;
    g_threads[threadId] = info;
}

void UnregisterThread(uint32_t threadId) {
    std::lock_guard<std::mutex> lock(g_threadMutex);
    g_threads.erase(threadId);
}

void SetThreadState(uint32_t threadId, int state, uint32_t waitingOn) {
    std::lock_guard<std::mutex> lock(g_threadMutex);
    auto it = g_threads.find(threadId);
    if (it != g_threads.end()) {
        it->second.state = static_cast<ThreadInfo::State>(state);
        it->second.waitingOn = waitingOn;
    }
}

// =============================================================================
// Command Registration Stubs (implemented in separate files)
// =============================================================================

// These will be implemented in commands/*.cpp files
// For now, provide empty implementations

#ifndef LIBERTY_DEBUGGER_EXTERNAL_COMMANDS

void RegisterMemoryCommands(CommandParser& parser) {
    parser.Register({
        "mem",
        "mem <addr> [size]",
        "Dump memory at guest address",
        "memory",
        [](CommandContext& ctx) -> bool {
            uint32_t addr;
            if (!ctx.GetArgAsU32(0, addr)) {
                ctx.out << "Usage: mem <addr> [size]\n";
                return false;
            }
            uint32_t size = 64;
            if (ctx.ArgCount() > 1) {
                ctx.GetArgAsU32(1, size);
            }
            
            void* ptr = TranslateAddress(addr);
            if (!ptr) {
                ctx.out << "Invalid address: " << FormatAddr(addr) << "\n";
                return false;
            }
            
            ctx.out << FormatAddr(addr) << ":\n";
            ctx.out << FormatHex(static_cast<uint8_t*>(ptr), size);
            return true;
        }
    });
    
    parser.Register({
        "watch",
        "watch <addr> [r|w|rw] [size]",
        "Set memory watchpoint",
        "memory",
        [](CommandContext& ctx) -> bool {
            uint32_t addr;
            if (!ctx.GetArgAsU32(0, addr)) {
                ctx.out << "Usage: watch <addr> [r|w|rw] [size]\n";
                return false;
            }
            
            WatchType type = WatchType::Write;
            uint32_t size = 4;
            
            if (ctx.ArgCount() > 1) {
                std::string typeStr;
                ctx.GetArgAsString(1, typeStr);
                if (typeStr == "r") type = WatchType::Read;
                else if (typeStr == "rw") type = WatchType::ReadWrite;
            }
            if (ctx.ArgCount() > 2) {
                ctx.GetArgAsU32(2, size);
            }
            
            if (ctx.state.AddWatchpoint(addr, size, type)) {
                ctx.out << "Watchpoint set at " << FormatAddr(addr) << "\n";
            } else {
                ctx.out << "Watchpoint already exists at " << FormatAddr(addr) << "\n";
            }
            return true;
        }
    });
    
    parser.Register({
        "vtable",
        "vtable <obj_addr> [count]",
        "Dump vtable entries for object",
        "memory",
        [](CommandContext& ctx) -> bool {
            uint32_t objAddr;
            if (!ctx.GetArgAsU32(0, objAddr)) {
                ctx.out << "Usage: vtable <obj_addr> [count]\n";
                return false;
            }
            
            uint32_t count = 8;
            if (ctx.ArgCount() > 1) {
                ctx.GetArgAsU32(1, count);
            }
            
            // Read vtable pointer from object
            uint32_t vtableAddr;
            if (!ReadU32BE(objAddr, vtableAddr)) {
                ctx.out << "Failed to read vtable pointer at " << FormatAddr(objAddr) << "\n";
                return false;
            }
            
            ctx.out << "Object at " << FormatAddr(objAddr) << "\n";
            ctx.out << "Vtable at " << FormatAddr(vtableAddr) << ":\n";
            
            for (uint32_t i = 0; i < count; ++i) {
                uint32_t funcAddr;
                if (!ReadU32BE(vtableAddr + i * 4, funcAddr)) {
                    ctx.out << "  [" << i << "] <read error>\n";
                    continue;
                }
                
                ctx.out << "  [" << i << "] " << FormatAddr(funcAddr);
                if (funcAddr == 0) {
                    ctx.out << "  (NULL)";
                } else if (funcAddr < 0x82000000 || funcAddr >= 0x83000000) {
                    ctx.out << "  (INVALID)";
                } else {
                    ctx.out << "  " << GetFunctionName(funcAddr);
                }
                ctx.out << "\n";
            }
            return true;
        }
    });
    
    parser.Register({
        "find",
        "find <pattern> [start] [end]",
        "Search memory for byte pattern",
        "memory",
        [](CommandContext& ctx) -> bool {
            // TODO: Implement memory search
            ctx.out << "Not yet implemented\n";
            return true;
        }
    });
}

void RegisterThreadCommands(CommandParser& parser) {
    parser.Register({
        "threads",
        "threads",
        "List all guest threads",
        "thread",
        [](CommandContext& ctx) -> bool {
            std::lock_guard<std::mutex> lock(g_threadMutex);
            
            if (g_threads.empty()) {
                ctx.out << "No threads registered\n";
                return true;
            }
            
            ctx.out << "ID    State      Entry       Name\n";
            ctx.out << "----  ---------  ----------  ----------------\n";
            
            for (const auto& [id, info] : g_threads) {
                const char* stateStr = "UNKNOWN";
                switch (info.state) {
                    case ThreadInfo::State::Running: stateStr = "RUNNING"; break;
                    case ThreadInfo::State::Waiting: stateStr = "WAITING"; break;
                    case ThreadInfo::State::Suspended: stateStr = "SUSPEND"; break;
                    case ThreadInfo::State::Terminated: stateStr = "TERM"; break;
                }
                
                char line[128];
                snprintf(line, sizeof(line), "%-4u  %-9s  0x%08X  %s",
                         info.id, stateStr, info.entryPoint, info.name.c_str());
                ctx.out << line;
                
                if (info.state == ThreadInfo::State::Waiting && info.waitingOn) {
                    ctx.out << "  (on 0x" << std::hex << info.waitingOn << std::dec << ")";
                }
                ctx.out << "\n";
            }
            return true;
        }
    });
    
    parser.Register({
        "waits",
        "waits",
        "Show threads waiting on sync objects",
        "thread",
        [](CommandContext& ctx) -> bool {
            std::lock_guard<std::mutex> lock(g_threadMutex);
            
            bool found = false;
            for (const auto& [id, info] : g_threads) {
                if (info.state == ThreadInfo::State::Waiting) {
                    if (!found) {
                        ctx.out << "Blocked threads:\n";
                        found = true;
                    }
                    ctx.out << "  Thread #" << id << " (" << info.name << ") -> "
                            << FormatAddr(info.waitingOn) << "\n";
                }
            }
            
            if (!found) {
                ctx.out << "No threads currently waiting\n";
            }
            return true;
        }
    });
}

void RegisterSyncCommands(CommandParser& parser) {
    parser.Register({
        "events",
        "events",
        "List tracked Event objects",
        "sync",
        [](CommandContext& ctx) -> bool {
            ctx.out << "Event tracking not yet connected to kernel\n";
            return true;
        }
    });
    
    parser.Register({
        "sems",
        "sems",
        "List tracked Semaphore objects",
        "sync",
        [](CommandContext& ctx) -> bool {
            ctx.out << "Semaphore tracking not yet connected to kernel\n";
            return true;
        }
    });
    
    parser.Register({
        "signal",
        "signal <handle>",
        "Manually signal Event or Semaphore",
        "sync",
        [](CommandContext& ctx) -> bool {
            ctx.out << "Signal not yet connected to kernel\n";
            return true;
        }
    });
}

void RegisterExecCommands(CommandParser& parser) {
    parser.Register({
        "break",
        "break <addr|func_name>",
        "Set breakpoint",
        "exec",
        [](CommandContext& ctx) -> bool {
            uint32_t addr;
            if (!ctx.GetArgAsU32(0, addr)) {
                ctx.out << "Usage: break <addr>\n";
                return false;
            }
            
            if (ctx.state.AddBreakpoint(addr)) {
                ctx.out << "Breakpoint set at " << FormatAddr(addr) << "\n";
            } else {
                ctx.out << "Breakpoint already exists at " << FormatAddr(addr) << "\n";
            }
            return true;
        }
    });
    
    parser.Register({
        "trace",
        "trace <addr> [count]",
        "Trace function calls",
        "exec",
        [](CommandContext& ctx) -> bool {
            uint32_t addr;
            if (!ctx.GetArgAsU32(0, addr)) {
                ctx.out << "Usage: trace <addr> [count]\n";
                return false;
            }
            
            uint32_t count = 0;
            if (ctx.ArgCount() > 1) {
                ctx.GetArgAsU32(1, count);
            }
            
            if (ctx.state.AddTrace(addr, count)) {
                ctx.out << "Tracing " << GetFunctionName(addr);
                if (count > 0) {
                    ctx.out << " (max " << count << " calls)";
                }
                ctx.out << "\n";
            } else {
                ctx.out << "Already tracing " << FormatAddr(addr) << "\n";
            }
            return true;
        }
    });
    
    parser.Register({
        "continue",
        "continue",
        "Continue execution",
        "exec",
        [](CommandContext& ctx) -> bool {
            if (ctx.state.IsPaused()) {
                ctx.state.Continue();
                ctx.out << "Continuing...\n";
            } else {
                ctx.out << "Not paused\n";
            }
            return true;
        }
    });
    
    parser.Register({
        "pause",
        "pause",
        "Pause execution",
        "exec",
        [](CommandContext& ctx) -> bool {
            ctx.state.Pause();
            ctx.out << "Paused\n";
            return true;
        }
    });
}

void RegisterPPCCommands(CommandParser& parser) {
    parser.Register({
        "regs",
        "regs",
        "Dump PPC registers",
        "ppc",
        [](CommandContext& ctx) -> bool {
            ctx.out << "Register dump requires active breakpoint context\n";
            return true;
        }
    });
    
    parser.Register({
        "tls",
        "tls [offset]",
        "Dump TLS data",
        "ppc",
        [](CommandContext& ctx) -> bool {
            ctx.out << "TLS dump not yet implemented\n";
            return true;
        }
    });
}

void RegisterGPUCommands(CommandParser& parser) {
    parser.Register({
        "device",
        "device [offset]",
        "Show guest device context",
        "gpu",
        [](CommandContext& ctx) -> bool {
            ctx.out << "Device context inspection not yet implemented\n";
            return true;
        }
    });
    
    parser.Register({
        "shaders",
        "shaders",
        "List loaded shaders",
        "gpu",
        [](CommandContext& ctx) -> bool {
            // Shader table at 0x830E5900
            ctx.out << "Shader table at 0x830E5900:\n";
            
            for (int i = 0; i < 16; ++i) {
                uint32_t slotAddr = 0x830E5900 + i * 4;
                uint32_t shaderAddr;
                if (ReadU32BE(slotAddr, shaderAddr)) {
                    if (shaderAddr != 0) {
                        ctx.out << "  [" << i << "] " << FormatAddr(shaderAddr) << "\n";
                    }
                }
            }
            return true;
        }
    });
}

void RegisterFileCommands(CommandParser& parser) {
    parser.Register({
        "handles",
        "handles",
        "List open file handles",
        "file",
        [](CommandContext& ctx) -> bool {
            ctx.out << "File handle tracking not yet implemented\n";
            return true;
        }
    });
}

void RegisterCtorCommands(CommandParser& parser) {
    parser.Register({
        "ctors",
        "ctors",
        "List global constructors",
        "ctor",
        [](CommandContext& ctx) -> bool {
            ctx.out << "Global constructors in 0x82A00000-0x82B00000 range\n";
            ctx.out << "(270 functions, see ARCHITECTURE.md for details)\n";
            return true;
        }
    });
    
    parser.Register({
        "bss",
        "bss <addr>",
        "Check .bss address initialization",
        "ctor",
        [](CommandContext& ctx) -> bool {
            uint32_t addr;
            if (!ctx.GetArgAsU32(0, addr)) {
                ctx.out << "Usage: bss <addr>\n";
                return false;
            }
            
            uint32_t value;
            if (!ReadU32BE(addr, value)) {
                ctx.out << "Failed to read " << FormatAddr(addr) << "\n";
                return false;
            }
            
            ctx.out << FormatAddr(addr) << " = " << FormatAddr(value);
            
            if (value == 0) {
                ctx.out << " (NULL - uninitialized)";
            } else if (value >= 0x82140000 && value < 0x82260000) {
                ctx.out << " (valid vtable in .rdata)";
            } else if (value == 0x01010101) {
                ctx.out << " (debug fill pattern)";
            } else if (value == 0xFFFFFFFF) {
                ctx.out << " (uninitialized/freed)";
            }
            ctx.out << "\n";
            return true;
        }
    });
}

void RegisterUtilCommands(CommandParser& parser) {
    parser.Register({
        "null-calls",
        "null-calls",
        "List null function call attempts",
        "util",
        [](CommandContext& ctx) -> bool {
            auto nullCalls = ctx.state.GetNullCalls();
            
            if (nullCalls.empty()) {
                ctx.out << "No null calls recorded\n";
                return true;
            }
            
            ctx.out << "#     LR          Target      Count  Function\n";
            ctx.out << "----  ----------  ----------  -----  --------\n";
            
            int n = 0;
            for (const auto& info : nullCalls) {
                char line[128];
                snprintf(line, sizeof(line), "%-4d  0x%08X  0x%08X  %-5lu  %s\n",
                         ++n, info.lastLR, info.targetAddr, 
                         (unsigned long)info.count,
                         GetFunctionName(info.lastLR & ~3).c_str());
                ctx.out << line;
                
                if (n >= 20) {
                    ctx.out << "... and " << (nullCalls.size() - 20) << " more\n";
                    break;
                }
            }
            
            ctx.out << "\nTotal null calls: " << ctx.state.Stats().nullCallCount.load() << "\n";
            return true;
        }
    });
    
    parser.Register({
        "stats",
        "stats",
        "Show runtime statistics",
        "util",
        [](CommandContext& ctx) -> bool {
            const auto& stats = ctx.state.Stats();
            
            ctx.out << "Runtime Statistics:\n";
            ctx.out << "  Frames:        " << stats.frameCount.load() << "\n";
            ctx.out << "  Function calls: " << stats.functionCalls.load() << "\n";
            ctx.out << "  Allocations:   " << stats.allocCount.load() << "\n";
            ctx.out << "  Frees:         " << stats.freeCount.load() << "\n";
            ctx.out << "  Total alloc:   " << (stats.totalAllocBytes.load() / 1024) << " KB\n";
            ctx.out << "  Null calls:    " << stats.nullCallCount.load() << "\n";
            ctx.out << "  Invalid calls: " << stats.invalidAddrCallCount.load() << "\n";
            ctx.out << "  Watchpoint hits: " << stats.watchpointHits.load() << "\n";
            ctx.out << "  Breakpoint hits: " << stats.breakpointHits.load() << "\n";
            return true;
        }
    });
    
    parser.Register({
        "addr",
        "addr <host_ptr>",
        "Convert host pointer to guest address",
        "util",
        [](CommandContext& ctx) -> bool {
            // This would need the actual host pointer which is tricky from CLI
            ctx.out << "Use from code: Debugger::MapVirtual(hostPtr)\n";
            return true;
        }
    });
    
    parser.Register({
        "guest",
        "guest <addr>",
        "Show host pointer for guest address",
        "util",
        [](CommandContext& ctx) -> bool {
            uint32_t addr;
            if (!ctx.GetArgAsU32(0, addr)) {
                ctx.out << "Usage: guest <addr>\n";
                return false;
            }
            
            void* ptr = TranslateAddress(addr);
            if (ptr) {
                ctx.out << FormatAddr(addr) << " -> " << ptr << "\n";
            } else {
                ctx.out << "Invalid guest address\n";
            }
            return true;
        }
    });
    
    parser.Register({
        "log",
        "log <category> [on|off]",
        "Toggle log category",
        "util",
        [](CommandContext& ctx) -> bool {
            if (ctx.ArgCount() < 1) {
                ctx.out << "Usage: log <category> [on|off]\n";
                return false;
            }
            
            std::string category;
            ctx.GetArgAsString(0, category);
            
            bool enable = true;
            if (ctx.ArgCount() > 1) {
                std::string onoff;
                ctx.GetArgAsString(1, onoff);
                enable = (onoff == "on" || onoff == "1" || onoff == "true");
            }
            
            ctx.state.SetLogCategory(category, enable);
            ctx.out << "Log category '" << category << "' " 
                    << (enable ? "enabled" : "disabled") << "\n";
            return true;
        }
    });
    
    parser.Register({
        "quiet",
        "quiet",
        "Suppress all logging",
        "util",
        [](CommandContext& ctx) -> bool {
            ctx.state.SetQuietMode(true);
            ctx.out << "Quiet mode enabled\n";
            return true;
        }
    });
    
    parser.Register({
        "verbose",
        "verbose",
        "Enable all logging",
        "util",
        [](CommandContext& ctx) -> bool {
            ctx.state.SetQuietMode(false);
            ctx.out << "Verbose mode enabled\n";
            return true;
        }
    });
}

#endif // LIBERTY_DEBUGGER_EXTERNAL_COMMANDS

} // namespace Debugger
