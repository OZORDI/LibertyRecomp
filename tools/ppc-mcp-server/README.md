# PPC Recompilation MCP Server

MCP (Model Context Protocol) server for analyzing PPC recompiled code in the LibertyRecomp project. Makes execution tracing and debugging trivial.

## Features

### 17 Analysis Tools

| Tool | Description |
|------|-------------|
| `sync_primitive_analyzer` | Find what waits on sync objects and what signals them |
| `call_tree_analyzer` | See what a function calls and what globals it initializes |
| `kernel_api_tracer` | Trace UP (callers of API) or DOWN (APIs a function reaches) |
| `global_dependency_tracker` | Show what initializes and reads a global address |
| `hook_impact_analyzer` | See what breaks if you stub/hook a function |
| `weak_symbol_checker` | Verify if a hook is active or overridden |
| `blocking_path_finder` | Find shortest path from function to blocking call |
| `init_sequence_validator` | Check if init sequence is complete |
| `vtable_inspector` | Analyze vtable structures |
| `ppc_memory_map` | Describe memory address region and usage |
| `recursive_call_tree` | Full recursive call tree with data structures |
| `search_functions` | Search functions by regex pattern |
| `list_hooks` | List all defined hooks |
| `list_kernel_apis` | List kernel APIs by category |
| `get_function_info` | Get detailed function info |
| `rebuild_index` | Force rebuild of the index |
| `get_stats` | Get codebase statistics |

## Installation

```bash
cd tools/ppc-mcp-server
npm install
npm run build
```

## Usage

### With Windsurf/Cascade

Add to your MCP settings (`~/.codeium/windsurf/mcp_config.json`):

```json
{
  "mcpServers": {
    "ppc-recomp": {
      "command": "node",
      "args": ["/path/to/LibertyRecomp/tools/ppc-mcp-server/dist/index.js"],
      "env": {
        "PPC_DIR": "/path/to/LibertyRecomp/LibertyRecompLib/ppc",
        "IMPORTS_FILE": "/path/to/LibertyRecomp/LibertyRecomp/kernel/imports.cpp"
      }
    }
  }
}
```

### Development Mode

```bash
npm run dev
```

## Example Usage

### Find blocking path from a function
```
blocking_path_finder({ function: "sub_821A8868" })
```
Returns:
```json
{
  "function": "sub_821A8868",
  "shortest_path": [
    "sub_821A8868",
    "sub_82300C78",
    "sub_827DB988",
    "sub_829A3560",
    "KeWaitForSingleObject"
  ],
  "blocking_api": "KeWaitForSingleObject",
  "fix_suggestion": "Hook sub_829A3560 to bypass blocking call"
}
```

### Check stub impact before hooking
```
hook_impact_analyzer({ function: "sub_82857240" })
```
Returns:
```json
{
  "function": "sub_82857240",
  "globals_not_initialized": ["0x83127984"],
  "functions_that_crash": ["sub_822B4D68 (reads 0x83127984)"],
  "sync_objects_not_created": [],
  "recommended_action": "Hook blocking sub-functions instead, let parent run"
}
```

### Trace what kernel APIs a function reaches
```
kernel_api_tracer({ target: "sub_821A8868", direction: "down" })
```
Returns:
```json
{
  "function": "sub_821A8868",
  "kernel_apis_reached": [
    { "api": "KeWaitForSingleObject", "via": "sub_821A8868 → sub_82300C78 → sub_827DB988 → sub_829A3560" },
    { "api": "NtCreateFile", "via": "sub_821A8868 → sub_82300C78 → sub_8249BA90" }
  ]
}
```

## Kernel API Categories

The server categorizes kernel APIs for easier filtering:

- **thread**: ExCreateThread, NtSuspendThread, KeTlsAlloc, etc.
- **sync**: KeWaitForSingleObject, KeInitializeEvent, KeSetEvent, etc.
- **file**: NtOpenFile, NtCreateFile, NtReadFile, XCreateFileA, etc.
- **memory**: NtAllocateVirtualMemory, RtlAllocateHeap, etc.
- **video**: VdSwap, VdInitializeRingBuffer, XGetVideoMode, etc.
- **audio**: XAudioGetVoiceCategoryVolume, XMACreateContext, etc.
- **xam**: XamShowSigninUI, XamContentCreateEx, XamTaskSchedule, etc.

## Index Caching

The server caches the parsed index to `.ppc-index-cache.json` for faster startup. The cache is automatically invalidated after 24 hours.

To force rebuild:
```
rebuild_index({})
```

## Architecture

```
src/
├── index.ts           # MCP server entry point
├── types.ts           # TypeScript type definitions
├── parsers/
│   └── ppc-parser.ts  # PPC code parser
└── tools/
    └── analyzers.ts   # Analysis tool implementations
```

## Parsed Patterns

The server parses these patterns from PPC recompiled code:

- `PPC_FUNC_IMPL(__imp__sub_XXXXXXXX)` - Function implementations
- `PPC_WEAK_FUNC(sub_XXXXXXXX)` - Weak symbol wrappers
- `sub_XXXXXXXX(ctx, base)` - Function calls
- `__imp__KernelAPI(ctx, base)` - Kernel API calls
- `PPC_LOAD_U32/U64(addr)` - Memory reads (globals)
- `PPC_STORE_U32/U64(addr, val)` - Memory writes (globals)
- `GUEST_FUNCTION_HOOK(sub_XXX, HostFunc)` - Hook definitions
- `GUEST_FUNCTION_STUB(sub_XXX)` - Stub definitions
