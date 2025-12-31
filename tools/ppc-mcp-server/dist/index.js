#!/usr/bin/env node
// PPC Recompilation MCP Server
// Provides analysis tools for LibertyRecomp PPC code
import { Server } from '@modelcontextprotocol/sdk/server/index.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import { CallToolRequestSchema, ListToolsRequestSchema, } from '@modelcontextprotocol/sdk/types.js';
import * as fs from 'fs';
import * as path from 'path';
import { fileURLToPath } from 'url';
import { buildIndex } from './parsers/ppc-parser.js';
import { PPCAnalyzer } from './tools/analyzers.js';
import { ExtendedAnalyzer } from './tools/extended-analyzers.js';
import { RenderingTools } from './tools/rendering-tools.js';
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
// Default paths - can be overridden via environment variables
const DEFAULT_PPC_DIR = process.env.PPC_DIR ||
    path.resolve(__dirname, '../../../LibertyRecompLib/ppc');
const DEFAULT_IMPORTS_FILE = process.env.IMPORTS_FILE ||
    path.resolve(__dirname, '../../../LibertyRecomp/kernel/imports.cpp');
// Index cache file
const INDEX_CACHE_FILE = path.resolve(__dirname, '../.ppc-index-cache.json');
let index = null;
let analyzer = null;
let extendedAnalyzer = null;
let renderingTools = null;
// Tool definitions - Core tools
const CORE_TOOLS = [
    {
        name: 'sync_primitive_analyzer',
        description: 'Find what waits on sync objects and what signals them. Pass a memory address of a sync primitive (event, semaphore, mutex).',
        inputSchema: {
            type: 'object',
            properties: {
                address: { type: 'string', description: 'Memory address of the sync primitive (e.g., "0x82897F5C")' },
            },
            required: ['address'],
        },
    },
    {
        name: 'call_tree_analyzer',
        description: 'Analyze a function to see what it calls and what globals it initializes.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name (e.g., "sub_82857240")' },
            },
            required: ['function'],
        },
    },
    {
        name: 'kernel_api_tracer',
        description: 'Trace callers of a kernel API (direction=up) or trace what kernel APIs a function reaches (direction=down).',
        inputSchema: {
            type: 'object',
            properties: {
                target: { type: 'string', description: 'Kernel API name (for up) or function name (for down)' },
                direction: { type: 'string', enum: ['up', 'down'], description: 'up = find callers of API, down = find APIs called by function' },
            },
            required: ['target', 'direction'],
        },
    },
    {
        name: 'global_dependency_tracker',
        description: 'Given a global address, show what initializes it and what reads it.',
        inputSchema: {
            type: 'object',
            properties: {
                address: { type: 'string', description: 'Global memory address (e.g., "0x83127984")' },
            },
            required: ['address'],
        },
    },
    {
        name: 'hook_impact_analyzer',
        description: 'Before stubbing/hooking a function, see what would break.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name to analyze stub impact for' },
            },
            required: ['function'],
        },
    },
    {
        name: 'weak_symbol_checker',
        description: 'Check if a hook is actually being used or if weak symbol is overriding.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name to check hook status' },
            },
            required: ['function'],
        },
    },
    {
        name: 'blocking_path_finder',
        description: 'Find the shortest path from a function to a blocking call.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name to find blocking path from' },
            },
            required: ['function'],
        },
    },
    {
        name: 'init_sequence_validator',
        description: 'Check if an initialization sequence is complete.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Initialization function to validate' },
            },
            required: ['function'],
        },
    },
    {
        name: 'vtable_inspector',
        description: 'Analyze vtable structures and their function pointers.',
        inputSchema: {
            type: 'object',
            properties: {
                address: { type: 'string', description: 'VTable address' },
            },
            required: ['address'],
        },
    },
    {
        name: 'ppc_memory_map',
        description: 'Describe a memory address - what region, type, and usage.',
        inputSchema: {
            type: 'object',
            properties: {
                address: { type: 'string', description: 'Memory address to describe' },
            },
            required: ['address'],
        },
    },
    {
        name: 'recursive_call_tree',
        description: 'Build a full recursive call tree with data structures accessed.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Root function for call tree' },
                max_depth: { type: 'number', description: 'Maximum recursion depth (default: 5)' },
            },
            required: ['function'],
        },
    },
    {
        name: 'search_functions',
        description: 'Search for functions by name pattern (regex supported).',
        inputSchema: {
            type: 'object',
            properties: {
                pattern: { type: 'string', description: 'Search pattern (regex)' },
            },
            required: ['pattern'],
        },
    },
    {
        name: 'list_hooks',
        description: 'List all function hooks defined in imports.cpp.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'list_kernel_apis',
        description: 'List all kernel APIs found in the codebase.',
        inputSchema: {
            type: 'object',
            properties: {
                category: { type: 'string', enum: ['thread', 'sync', 'file', 'memory', 'video', 'audio', 'xam', 'misc'], description: 'Filter by category (optional)' },
            },
        },
    },
    {
        name: 'get_function_info',
        description: 'Get detailed information about a specific function.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name (e.g., "sub_82120000")' },
            },
            required: ['function'],
        },
    },
    {
        name: 'rebuild_index',
        description: 'Force rebuild of the PPC code index (use after code changes).',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'get_stats',
        description: 'Get statistics about the parsed codebase.',
        inputSchema: { type: 'object', properties: {} },
    },
];
// Extended tools
const EXTENDED_TOOLS = [
    {
        name: 'string_finder',
        description: 'Find where string literals/paths are used in the code.',
        inputSchema: {
            type: 'object',
            properties: {
                search: { type: 'string', description: 'String to search for (e.g., "platform:/textures")' },
            },
            required: ['search'],
        },
    },
    {
        name: 'magic_number_decoder',
        description: 'Decode Xbox magic values (996, 997, 258, 259, etc.).',
        inputSchema: {
            type: 'object',
            properties: {
                value: { type: 'string', description: 'Number to decode (decimal or hex)' },
            },
            required: ['value'],
        },
    },
    {
        name: 'list_magic_numbers',
        description: 'List all known magic numbers and their meanings.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'pattern_finder',
        description: 'Find code patterns: spin loops, retry loops, state machines, callback registrations.',
        inputSchema: {
            type: 'object',
            properties: {
                pattern: { type: 'string', enum: ['spin_loop', 'retry_loop', 'state_machine', 'callback_registration'], description: 'Pattern type to find' },
            },
            required: ['pattern'],
        },
    },
    {
        name: 'function_signature_inferrer',
        description: 'Infer function signatures from r3-r10 register usage patterns.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name' },
            },
            required: ['function'],
        },
    },
    {
        name: 'struct_layout_analyzer',
        description: 'Infer struct layouts from offset access patterns (e.g., obj+8, obj+16).',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name to analyze' },
            },
            required: ['function'],
        },
    },
    {
        name: 'xref_explorer',
        description: 'All references to/from a function - callers, callees, global refs, in one view.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name' },
            },
            required: ['function'],
        },
    },
    {
        name: 'caller_context_analyzer',
        description: 'See HOW a function is called from each call site (what args are passed).',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name' },
            },
            required: ['function'],
        },
    },
    {
        name: 'similarity_finder',
        description: 'Find functions with similar code structure (useful for finding related subsystems).',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name to find similar functions to' },
            },
            required: ['function'],
        },
    },
    {
        name: 'dead_code_detector',
        description: 'Find functions that are never called (safe to ignore).',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'annotate_function',
        description: 'Add persistent notes/labels to functions (stored in a JSON file).',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name' },
                notes: { type: 'string', description: 'Notes to add' },
                labels: { type: 'array', items: { type: 'string' }, description: 'Labels to add' },
            },
            required: ['function', 'notes'],
        },
    },
    {
        name: 'get_annotations',
        description: 'Retrieve all annotations for a function or all annotations.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name (optional, omit for all)' },
            },
        },
    },
    {
        name: 'label_global',
        description: 'Name a global address (e.g., 0x83127984 = "g_renderContext").',
        inputSchema: {
            type: 'object',
            properties: {
                address: { type: 'string', description: 'Global address' },
                name: { type: 'string', description: 'Name for the global' },
                type: { type: 'string', description: 'Type (optional)' },
                notes: { type: 'string', description: 'Notes (optional)' },
            },
            required: ['address', 'name'],
        },
    },
    {
        name: 'subsystem_mapper',
        description: 'Map the 63 init subsystems to their functions and dependencies.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'import_dependency_graph',
        description: 'Show which kernel APIs each subsystem/category needs.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'thread_entry_finder',
        description: 'Find functions used as thread entry points (passed to ExCreateThread).',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'async_callback_tracer',
        description: 'Track XamTaskSchedule callbacks and their completion handlers.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'spin_loop_detector',
        description: 'Find and analyze spin/wait loops with their exit conditions.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'identify_sync_primitives',
        description: 'Identify all sync primitives (semaphores, mutexes, spinlocks, events) and their usage.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'hook_recommender',
        description: 'Given a blocking function, suggest optimal hook points.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name' },
            },
            required: ['function'],
        },
    },
    {
        name: 'execution_order_analyzer',
        description: 'Predict init function execution order based on call graph.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Root function' },
            },
            required: ['function'],
        },
    },
    {
        name: 'state_machine_visualizer',
        description: 'Detect and show state machines (common in game subsystems).',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name' },
            },
            required: ['function'],
        },
    },
    {
        name: 'register_flow_tracer',
        description: 'Track how a value flows through registers in a function.',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name' },
                register: { type: 'string', description: 'Register to trace (e.g., "r3", "r31")' },
            },
            required: ['function', 'register'],
        },
    },
    // VTable Analysis Tools
    {
        name: 'vtable_inspector_enhanced',
        description: 'View entire vtable: all entries, function pointers, initializers, and readers.',
        inputSchema: {
            type: 'object',
            properties: {
                address: { type: 'string', description: 'VTable address (e.g., "0x82010F0C")' },
            },
            required: ['address'],
        },
    },
    {
        name: 'vtable_init_tracer',
        description: 'Trace what functions initialize each vtable entry and their call chains.',
        inputSchema: {
            type: 'object',
            properties: {
                address: { type: 'string', description: 'VTable address to trace initialization for' },
            },
            required: ['address'],
        },
    },
    {
        name: 'vtable_chain_analyzer',
        description: 'Check if a function will eventually initialize a vtable (traces call graph).',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function to check (e.g., "sub_8221D880")' },
                vtable: { type: 'string', description: 'VTable address to check initialization for' },
            },
            required: ['function', 'vtable'],
        },
    },
    {
        name: 'vtable_users',
        description: 'Find all functions that read from a vtable (indirect call sites).',
        inputSchema: {
            type: 'object',
            properties: {
                address: { type: 'string', description: 'VTable address' },
            },
            required: ['address'],
        },
    },
    {
        name: 'vtable_scan',
        description: 'Scan a memory region for potential vtables and their initialization status.',
        inputSchema: {
            type: 'object',
            properties: {
                region: { type: 'string', enum: ['header', 'xex_data', 'static_data'], description: 'Memory region to scan' },
            },
        },
    },
    // Sync Primitive Refactor Analysis Tools
    {
        name: 'sync_flow_analyzer',
        description: 'Map complete flow: create → wait → signal for each sync primitive.',
        inputSchema: {
            type: 'object',
            properties: {
                address: { type: 'string', description: 'Optional: specific primitive address to analyze' },
            },
        },
    },
    {
        name: 'find_broken_signal_chains',
        description: 'Find sync primitives that are waited on but never signaled (or signaler is stubbed).',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'sync_primitive_inventory',
        description: 'List ALL sync primitives: events, semaphores, mutexes with their status.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'thread_sync_map',
        description: 'Show which threads/functions interact with which sync primitives.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'stubbed_signal_detector',
        description: 'Find stubbed functions that should be signaling primitives.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'xenia_refactor_checklist',
        description: 'Generate checklist for Xenia-style object table refactor.',
        inputSchema: { type: 'object', properties: {} },
    },
];
// Rendering & Build tools
const RENDERING_TOOLS = [
    {
        name: 'render_path_tracer',
        description: 'Trace the full render path from main loop entry to VdSwap frame presentation.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'vdswap_analyzer',
        description: 'Analyze VdSwap frame presentation - hook status, events signaled, frame flow.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'gpu_command_inspector',
        description: 'Inspect GPU commands (DrawPrimitive, SetTexture, etc.) by category.',
        inputSchema: {
            type: 'object',
            properties: {
                category: { type: 'string', enum: ['draw', 'bind', 'shader', 'create', 'texture', 'buffer', 'rendertarget', 'present', 'state', 'memory'], description: 'Filter by category (optional)' },
            },
        },
    },
    {
        name: 'render_state_tracker',
        description: 'Track render state changes in a function (device offsets, shader ops).',
        inputSchema: {
            type: 'object',
            properties: {
                function: { type: 'string', description: 'Function name' },
            },
            required: ['function'],
        },
    },
    {
        name: 'shader_usage_analyzer',
        description: 'Analyze shader creation and binding patterns.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'frame_timing_analyzer',
        description: 'Analyze frame timing, VBlank, and sync points.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'render_blocking_analyzer',
        description: 'Identify what blocks rendering initialization and how to fix it.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'device_context_inspector',
        description: 'Inspect device context structure (TLS+1676) - known offsets and critical fields.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'render_context_inspector',
        description: 'Inspect render context at 0x83042DEC - structure, vtable functions.',
        inputSchema: { type: 'object', properties: {} },
    },
    {
        name: 'build_project',
        description: 'Get build commands for LibertyRecomp. Specify platform and build mode.',
        inputSchema: {
            type: 'object',
            properties: {
                platform: { type: 'string', enum: ['windows-x64', 'windows-arm64', 'linux-x64', 'linux-arm64', 'macos-arm64', 'macos-x64'], description: 'Target platform' },
                mode: { type: 'string', enum: ['debug', 'relwithdebinfo', 'release'], description: 'Build mode' },
            },
            required: ['platform', 'mode'],
        },
    },
    {
        name: 'list_build_targets',
        description: 'List all available build targets and presets.',
        inputSchema: { type: 'object', properties: {} },
    },
];
const TOOLS = [...CORE_TOOLS, ...EXTENDED_TOOLS, ...RENDERING_TOOLS];
async function loadOrBuildIndex() {
    if (fs.existsSync(INDEX_CACHE_FILE)) {
        try {
            const stat = fs.statSync(INDEX_CACHE_FILE);
            const ageHours = (Date.now() - stat.mtimeMs) / (1000 * 60 * 60);
            if (ageHours < 24) {
                console.error('Loading cached index...');
                const cached = JSON.parse(fs.readFileSync(INDEX_CACHE_FILE, 'utf-8'));
                return deserializeIndex(cached);
            }
        }
        catch (e) {
            console.error('Failed to load cache, rebuilding...');
        }
    }
    return rebuildIndex();
}
async function rebuildIndex() {
    console.error(`Building index from ${DEFAULT_PPC_DIR}...`);
    const newIndex = await buildIndex(DEFAULT_PPC_DIR, DEFAULT_IMPORTS_FILE);
    try {
        const serialized = serializeIndex(newIndex);
        fs.writeFileSync(INDEX_CACHE_FILE, JSON.stringify(serialized));
        console.error('Index cached successfully');
    }
    catch (e) {
        console.error('Failed to cache index:', e);
    }
    return newIndex;
}
function serializeIndex(idx) {
    return {
        functions: Array.from(idx.functions.entries()),
        functionsByName: Array.from(idx.functionsByName.entries()),
        callGraph: Array.from(idx.callGraph.entries()).map(([k, v]) => [k, Array.from(v)]),
        reverseCallGraph: Array.from(idx.reverseCallGraph.entries()).map(([k, v]) => [k, Array.from(v)]),
        globals: Array.from(idx.globals.entries()),
        syncPrimitives: Array.from(idx.syncPrimitives.entries()),
        kernelAPIs: Array.from(idx.kernelAPIs.entries()),
        hooks: Array.from(idx.hooks.entries()),
        vtables: Array.from(idx.vtables.entries()),
    };
}
function deserializeIndex(data) {
    return {
        functions: new Map(data.functions),
        functionsByName: new Map(data.functionsByName),
        callGraph: new Map(data.callGraph.map(([k, v]) => [k, new Set(v)])),
        reverseCallGraph: new Map(data.reverseCallGraph.map(([k, v]) => [k, new Set(v)])),
        globals: new Map(data.globals),
        syncPrimitives: new Map(data.syncPrimitives),
        kernelAPIs: new Map(data.kernelAPIs),
        hooks: new Map(data.hooks),
        vtables: new Map(data.vtables),
    };
}
async function main() {
    const server = new Server({ name: 'ppc-recomp', version: '2.0.0' }, { capabilities: { tools: {} } });
    console.error('PPC Recompilation MCP Server starting...');
    index = await loadOrBuildIndex();
    analyzer = new PPCAnalyzer(index);
    extendedAnalyzer = new ExtendedAnalyzer(index, DEFAULT_PPC_DIR);
    renderingTools = new RenderingTools(index, DEFAULT_PPC_DIR);
    console.error('Index loaded, server ready');
    server.setRequestHandler(ListToolsRequestSchema, async () => ({ tools: TOOLS }));
    server.setRequestHandler(CallToolRequestSchema, async (request) => {
        const { name, arguments: args } = request.params;
        if (!analyzer || !index || !extendedAnalyzer) {
            return { content: [{ type: 'text', text: 'Error: Index not loaded' }], isError: true };
        }
        try {
            let result;
            switch (name) {
                // Core tools
                case 'sync_primitive_analyzer':
                    result = analyzer.analyzeSyncPrimitive(args?.address);
                    break;
                case 'call_tree_analyzer':
                    result = analyzer.analyzeCallTree(args?.function);
                    break;
                case 'kernel_api_tracer':
                    result = args?.direction === 'up'
                        ? analyzer.traceKernelCallers(args?.target)
                        : analyzer.traceToKernel(args?.target);
                    break;
                case 'global_dependency_tracker':
                    result = analyzer.analyzeGlobal(args?.address);
                    break;
                case 'hook_impact_analyzer':
                    result = analyzer.checkStubImpact(args?.function);
                    break;
                case 'weak_symbol_checker':
                    result = analyzer.checkHook(args?.function);
                    break;
                case 'blocking_path_finder':
                    result = analyzer.findBlockingPath(args?.function);
                    break;
                case 'init_sequence_validator':
                    result = analyzer.validateInit(args?.function);
                    break;
                case 'vtable_inspector':
                    result = analyzer.inspectVTable(args?.address);
                    break;
                case 'ppc_memory_map':
                    result = analyzer.describeAddress(args?.address);
                    break;
                case 'recursive_call_tree':
                    result = analyzer.buildRecursiveCallTree(args?.function, args?.max_depth || 5);
                    break;
                case 'search_functions':
                    result = analyzer.searchFunctions(args?.pattern);
                    break;
                case 'list_hooks':
                    result = analyzer.listHooks();
                    break;
                case 'list_kernel_apis':
                    result = analyzer.listKernelAPIs(args?.category);
                    break;
                case 'get_function_info': {
                    const func = index.functionsByName.get(args?.function);
                    result = func ? { ...func, hook: index.hooks.get(args?.function) } : { error: 'Function not found' };
                    break;
                }
                case 'rebuild_index':
                    index = await rebuildIndex();
                    analyzer = new PPCAnalyzer(index);
                    extendedAnalyzer = new ExtendedAnalyzer(index, DEFAULT_PPC_DIR);
                    renderingTools = new RenderingTools(index, DEFAULT_PPC_DIR);
                    result = { success: true, functions: index.functions.size, globals: index.globals.size, hooks: index.hooks.size };
                    break;
                case 'get_stats':
                    result = {
                        total_functions: index.functions.size,
                        total_globals: index.globals.size,
                        total_kernel_apis: index.kernelAPIs.size,
                        total_hooks: index.hooks.size,
                        call_graph_edges: Array.from(index.callGraph.values()).reduce((sum, set) => sum + set.size, 0),
                    };
                    break;
                // Extended tools
                case 'string_finder':
                    result = extendedAnalyzer.findStringUsage(args?.search);
                    break;
                case 'magic_number_decoder':
                    result = extendedAnalyzer.decodeMagicNumber(args?.value);
                    break;
                case 'list_magic_numbers':
                    result = extendedAnalyzer.listMagicNumbers();
                    break;
                case 'pattern_finder':
                    result = extendedAnalyzer.findPattern(args?.pattern);
                    break;
                case 'function_signature_inferrer':
                    result = extendedAnalyzer.inferSignature(args?.function);
                    break;
                case 'struct_layout_analyzer':
                    result = extendedAnalyzer.analyzeStructLayout(args?.function);
                    break;
                case 'xref_explorer':
                    result = extendedAnalyzer.exploreXrefs(args?.function);
                    break;
                case 'caller_context_analyzer':
                    result = extendedAnalyzer.analyzeCallerContext(args?.function);
                    break;
                case 'similarity_finder':
                    result = extendedAnalyzer.findSimilarFunctions(args?.function);
                    break;
                case 'dead_code_detector':
                    result = extendedAnalyzer.findDeadCode();
                    break;
                case 'annotate_function':
                    result = extendedAnalyzer.annotateFunction(args?.function, args?.notes, args?.labels);
                    break;
                case 'get_annotations':
                    result = extendedAnalyzer.getAnnotations(args?.function);
                    break;
                case 'label_global':
                    result = extendedAnalyzer.labelGlobal(args?.address, args?.name, args?.type, args?.notes);
                    break;
                case 'subsystem_mapper':
                    result = extendedAnalyzer.mapSubsystems();
                    break;
                case 'import_dependency_graph':
                    result = extendedAnalyzer.buildImportDependencyGraph();
                    break;
                case 'thread_entry_finder':
                    result = extendedAnalyzer.findThreadEntries();
                    break;
                case 'async_callback_tracer':
                    result = extendedAnalyzer.traceAsyncCallbacks();
                    break;
                case 'spin_loop_detector':
                    result = extendedAnalyzer.detectSpinLoops();
                    break;
                case 'identify_sync_primitives':
                    result = extendedAnalyzer.identifySyncPrimitives();
                    break;
                case 'hook_recommender':
                    result = extendedAnalyzer.recommendHooks(args?.function);
                    break;
                case 'execution_order_analyzer':
                    result = extendedAnalyzer.analyzeExecutionOrder(args?.function);
                    break;
                case 'state_machine_visualizer':
                    result = extendedAnalyzer.visualizeStateMachine(args?.function);
                    break;
                case 'register_flow_tracer':
                    result = extendedAnalyzer.traceRegisterFlow(args?.function, args?.register);
                    break;
                // VTable Analysis Tools
                case 'vtable_inspector_enhanced':
                    result = extendedAnalyzer.inspectVTableEnhanced(args?.address);
                    break;
                case 'vtable_init_tracer':
                    result = extendedAnalyzer.traceVTableInit(args?.address);
                    break;
                case 'vtable_chain_analyzer':
                    result = extendedAnalyzer.analyzeVTableChain(args?.function, args?.vtable);
                    break;
                case 'vtable_users':
                    result = extendedAnalyzer.findVTableUsers(args?.address);
                    break;
                case 'vtable_scan':
                    result = extendedAnalyzer.scanForVTables(args?.region);
                    break;
                // Sync Primitive Refactor Analysis Tools
                case 'sync_flow_analyzer':
                    result = extendedAnalyzer.analyzeSyncFlow(args?.address);
                    break;
                case 'find_broken_signal_chains':
                    result = extendedAnalyzer.findBrokenSignalChains();
                    break;
                case 'sync_primitive_inventory':
                    result = extendedAnalyzer.getSyncPrimitiveInventory();
                    break;
                case 'thread_sync_map':
                    result = extendedAnalyzer.getThreadSyncMap();
                    break;
                case 'stubbed_signal_detector':
                    result = extendedAnalyzer.findStubbedSignalers();
                    break;
                case 'xenia_refactor_checklist':
                    result = extendedAnalyzer.generateXeniaRefactorChecklist();
                    break;
                // Rendering & Build tools
                case 'render_path_tracer':
                    result = renderingTools.traceRenderPath();
                    break;
                case 'vdswap_analyzer':
                    result = renderingTools.analyzeVdSwap();
                    break;
                case 'gpu_command_inspector':
                    result = renderingTools.inspectGpuCommands(args?.category);
                    break;
                case 'render_state_tracker':
                    result = renderingTools.trackRenderState(args?.function);
                    break;
                case 'shader_usage_analyzer':
                    result = renderingTools.analyzeShaderUsage();
                    break;
                case 'frame_timing_analyzer':
                    result = renderingTools.analyzeFrameTiming();
                    break;
                case 'render_blocking_analyzer':
                    result = renderingTools.analyzeRenderBlocking();
                    break;
                case 'device_context_inspector':
                    result = renderingTools.inspectDeviceContext();
                    break;
                case 'render_context_inspector':
                    result = renderingTools.inspectRenderContext();
                    break;
                case 'build_project':
                    result = await renderingTools.buildProject(args?.platform, args?.mode);
                    break;
                case 'list_build_targets':
                    result = renderingTools.listBuildTargets();
                    break;
                default:
                    return { content: [{ type: 'text', text: `Unknown tool: ${name}` }], isError: true };
            }
            return { content: [{ type: 'text', text: JSON.stringify(result, null, 2) }] };
        }
        catch (error) {
            return { content: [{ type: 'text', text: `Error: ${error}` }], isError: true };
        }
    });
    const transport = new StdioServerTransport();
    await server.connect(transport);
    console.error('PPC Recompilation MCP Server running on stdio');
}
main().catch(console.error);
