#pragma once

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

namespace Debugger {

class DebugState;

// =============================================================================
// Command Context - passed to all command handlers
// =============================================================================
struct CommandContext {
    std::vector<std::string> args;  // args[0] is command name
    std::ostream& out;
    DebugState& state;
    void* memory;                   // Memory* from kernel - opaque to avoid dependency
    
    // Helper to get arg as uint32_t (hex or decimal)
    bool GetArgAsU32(size_t index, uint32_t& outValue) const;
    
    // Helper to get arg as string
    bool GetArgAsString(size_t index, std::string& outValue) const;
    
    // Check if we have at least N args (excluding command name)
    bool HasArgs(size_t count) const { return args.size() > count; }
    
    // Get number of args (excluding command name)
    size_t ArgCount() const { return args.size() > 0 ? args.size() - 1 : 0; }
};

// Command handler function type
// Returns true if command executed successfully
using CommandFunc = std::function<bool(CommandContext&)>;

// =============================================================================
// Command Entry
// =============================================================================
struct CommandEntry {
    std::string name;
    std::string usage;
    std::string help;
    std::string category;
    CommandFunc func;
};

// =============================================================================
// Command Parser
// =============================================================================
class CommandParser {
public:
    CommandParser();
    ~CommandParser() = default;
    
    // Register a command
    void Register(const CommandEntry& entry);
    
    // Execute a command line
    bool Execute(const std::string& line, DebugState& state, void* memory, std::ostream& out);
    
    // Print help (all commands or specific category)
    void PrintHelp(std::ostream& out, const std::string& category = "");
    
    // Print usage for specific command
    void PrintUsage(const std::string& cmdName, std::ostream& out);
    
    // Get tab completions for partial input
    std::vector<std::string> GetCompletions(const std::string& partial);
    
    // Get all command names
    std::vector<std::string> GetCommandNames() const;

private:
    std::vector<std::string> Tokenize(const std::string& line);
    CommandEntry* FindCommand(const std::string& name);
    
    std::vector<CommandEntry> m_commands;
};

// =============================================================================
// Utility Functions
// =============================================================================

// Parse address string (hex with optional 0x prefix, or decimal)
bool ParseAddress(const std::string& str, uint32_t& outAddr);

// Format address for display
std::string FormatAddr(uint32_t addr);

// Format hex bytes for display
std::string FormatHex(const uint8_t* data, size_t len, size_t bytesPerLine = 16);

// Format as ASCII (printable chars only, replace others with '.')
std::string FormatAscii(const uint8_t* data, size_t len);

// Get function name for address (returns "sub_XXXXXXXX" format)
std::string GetFuncName(uint32_t addr);

} // namespace Debugger
