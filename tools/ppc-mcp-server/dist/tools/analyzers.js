// Memory regions for address classification
const MEMORY_REGIONS = [
    { start: 0x82000000, end: 0x82120000, name: 'PPC image header', description: 'XEX image header and metadata' },
    { start: 0x82120000, end: 0x82A13D5C, name: 'PPC code section', description: 'Recompiled PPC functions' },
    { start: 0x82A13D5C, end: 0x831F0000, name: 'PPC data section', description: 'Global variables, vtables, strings' },
    { start: 0x83000000, end: 0x84000000, name: 'PPC heap/stack', description: 'Dynamic allocations and stack' },
];
// Blocking kernel APIs
const BLOCKING_APIS = new Set([
    'KeWaitForSingleObject', 'KeWaitForMultipleObjects',
    'NtWaitForSingleObject', 'NtWaitForSingleObjectEx', 'NtWaitForMultipleObjects',
    'KeDelayExecutionThread', 'NtDelayExecution',
    'KeAcquireSpinLock', 'ExAcquireReadWriteLockExclusive',
]);
// Sync primitive APIs
const SYNC_CREATE_APIS = {
    'KeInitializeEvent': 'event',
    'KeInitializeSemaphore': 'semaphore',
    'KeInitializeMutant': 'mutex',
    'KeInitializeSpinLock': 'spinlock',
};
const SYNC_SIGNAL_APIS = ['KeSetEvent', 'KePulseEvent', 'KeReleaseSemaphore', 'KeReleaseMutant', 'KeReleaseSpinLock'];
const SYNC_WAIT_APIS = ['KeWaitForSingleObject', 'KeWaitForMultipleObjects', 'NtWaitForSingleObject', 'NtWaitForSingleObjectEx'];
export class PPCAnalyzer {
    index;
    constructor(index) {
        this.index = index;
    }
    // Tool 1: sync_primitive_analyzer
    analyzeSyncPrimitive(address) {
        const normalizedAddr = address.toLowerCase();
        const primitive = this.index.syncPrimitives.get(normalizedAddr);
        // Find functions that interact with this address
        const waiters = [];
        const signalers = [];
        const creators = [];
        for (const [, func] of this.index.functions) {
            // Check if function accesses this address
            const accessesAddr = func.readsGlobals.some(g => g.address === normalizedAddr) ||
                func.writesGlobals.some(g => g.address === normalizedAddr);
            if (accessesAddr) {
                // Check what kernel APIs it calls
                for (const api of func.kernelCalls) {
                    if (SYNC_WAIT_APIS.includes(api)) {
                        waiters.push(`${func.name} @ ${func.address}`);
                    }
                    if (SYNC_SIGNAL_APIS.includes(api)) {
                        signalers.push(`${func.name} @ ${func.address}`);
                    }
                    if (SYNC_CREATE_APIS[api]) {
                        creators.push(`${func.name} via ${api}`);
                    }
                }
            }
        }
        return {
            address: normalizedAddr,
            type: primitive?.type || 'unknown',
            waited_on_by: waiters,
            signaled_by: signalers,
            created_by: creators,
        };
    }
    // Tool 2: call_tree_analyzer
    analyzeCallTree(funcName) {
        const func = this.index.functionsByName.get(funcName);
        if (!func)
            return null;
        const writesGlobals = func.writesGlobals.map(g => ({
            addr: g.address,
            type: this.inferGlobalType(g.address),
        }));
        const syncObjects = [];
        for (const api of func.kernelCalls) {
            if (SYNC_CREATE_APIS[api]) {
                syncObjects.push({ type: SYNC_CREATE_APIS[api], addr: 'runtime' });
            }
        }
        return {
            function: funcName,
            address: func.address,
            calls: func.calls,
            writes_globals: writesGlobals,
            reads_globals: func.readsGlobals.map(g => g.address),
            creates_sync_objects: syncObjects,
            kernel_apis: func.kernelCalls,
        };
    }
    // Tool 3: kernel_api_tracer - trace UP (callers)
    traceKernelCallers(apiName) {
        const api = this.index.kernelAPIs.get(apiName);
        const directCallers = api?.directCallers || [];
        // Find indirect callers via BFS
        const indirectPaths = [];
        const visited = new Set();
        for (const directCaller of directCallers) {
            this.findCallersRecursive(directCaller, [apiName], visited, indirectPaths, 5);
        }
        return {
            api: apiName,
            direct_callers: directCallers,
            indirect_callers: indirectPaths,
        };
    }
    findCallersRecursive(funcName, pathSoFar, visited, results, maxDepth) {
        if (maxDepth <= 0 || visited.has(funcName))
            return;
        visited.add(funcName);
        const callers = this.index.reverseCallGraph.get(funcName);
        if (!callers || callers.size === 0) {
            if (pathSoFar.length > 1) {
                results.push([funcName, ...pathSoFar].join(' → '));
            }
            return;
        }
        for (const caller of callers) {
            this.findCallersRecursive(caller, [funcName, ...pathSoFar], visited, results, maxDepth - 1);
        }
    }
    // Tool 3: kernel_api_tracer - trace DOWN (to kernel)
    traceToKernel(funcName) {
        const func = this.index.functionsByName.get(funcName);
        if (!func)
            return { function: funcName, kernel_apis_reached: [] };
        const results = [];
        const visited = new Set();
        this.findKernelApisRecursive(funcName, [], visited, results, 10);
        return {
            function: funcName,
            kernel_apis_reached: results,
        };
    }
    findKernelApisRecursive(funcName, pathSoFar, visited, results, maxDepth) {
        if (maxDepth <= 0 || visited.has(funcName))
            return;
        visited.add(funcName);
        const func = this.index.functionsByName.get(funcName);
        if (!func)
            return;
        const currentPath = [...pathSoFar, funcName];
        // Check direct kernel calls
        for (const api of func.kernelCalls) {
            results.push({
                api,
                via: currentPath.join(' → '),
            });
        }
        // Recurse into called functions
        for (const callee of func.calls) {
            this.findKernelApisRecursive(callee, currentPath, visited, results, maxDepth - 1);
        }
    }
    // Tool 4: global_dependency_tracker
    analyzeGlobal(address) {
        const normalizedAddr = address.toLowerCase();
        const global = this.index.globals.get(normalizedAddr);
        if (!global) {
            return {
                address: normalizedAddr,
                read_by: [],
                warning: 'Global address not found in parsed code',
            };
        }
        let warning;
        // Check for potential issues
        if (global.writers.length === 0 && global.readers.length > 0) {
            warning = 'No writers found - may be initialized elsewhere or statically';
        }
        // Check if any writers are stubbed
        for (const writer of global.writers) {
            const hook = this.index.hooks.get(writer);
            if (hook?.type === 'GUEST_FUNCTION_STUB') {
                warning = `Writer ${writer} is stubbed - global may not be initialized`;
                break;
            }
        }
        return {
            address: normalizedAddr,
            initialized_by: global.writers.length > 0
                ? `${global.writers[0]} (and ${global.writers.length - 1} others)`
                : undefined,
            read_by: global.readers,
            type: this.inferGlobalType(normalizedAddr),
            warning,
        };
    }
    // Tool 5: hook_impact_analyzer
    checkStubImpact(funcName) {
        const func = this.index.functionsByName.get(funcName);
        if (!func) {
            return {
                function: funcName,
                globals_not_initialized: [],
                functions_that_crash: [],
                sync_objects_not_created: [],
                recommended_action: 'Function not found',
            };
        }
        // Find all globals this function writes (directly and indirectly)
        const globalsNotInit = [];
        const functionsAffected = [];
        const syncNotCreated = [];
        const visited = new Set();
        this.collectWrittenGlobals(funcName, globalsNotInit, syncNotCreated, visited);
        // Find functions that read these globals
        for (const globalAddr of globalsNotInit) {
            const global = this.index.globals.get(globalAddr);
            if (global) {
                for (const reader of global.readers) {
                    if (!functionsAffected.includes(reader)) {
                        functionsAffected.push(`${reader} (reads ${globalAddr})`);
                    }
                }
            }
        }
        // Determine recommendation
        let recommendation = 'Safe to stub - no critical dependencies found';
        if (globalsNotInit.length > 0) {
            recommendation = 'Hook blocking sub-functions instead, let parent run full initialization';
        }
        if (syncNotCreated.length > 0) {
            recommendation = 'Create sync objects before stubbing, or hook only the blocking wait calls';
        }
        return {
            function: funcName,
            globals_not_initialized: globalsNotInit,
            functions_that_crash: functionsAffected.slice(0, 10),
            sync_objects_not_created: syncNotCreated,
            recommended_action: recommendation,
        };
    }
    collectWrittenGlobals(funcName, globals, syncObjects, visited) {
        if (visited.has(funcName))
            return;
        visited.add(funcName);
        const func = this.index.functionsByName.get(funcName);
        if (!func)
            return;
        // Add directly written globals
        for (const write of func.writesGlobals) {
            if (!globals.includes(write.address)) {
                globals.push(write.address);
            }
        }
        // Check for sync object creation
        for (const api of func.kernelCalls) {
            if (SYNC_CREATE_APIS[api]) {
                syncObjects.push(`${SYNC_CREATE_APIS[api]} via ${api}`);
            }
        }
        // Recurse into called functions
        for (const callee of func.calls) {
            this.collectWrittenGlobals(callee, globals, syncObjects, visited);
        }
    }
    // Tool 6: weak_symbol_checker
    checkHook(funcName) {
        const func = this.index.functionsByName.get(funcName);
        const hook = this.index.hooks.get(funcName);
        if (!func) {
            return {
                function: funcName,
                hook_active: false,
                reason: 'Function not found in index',
                has_weak_symbol: false,
            };
        }
        if (!hook) {
            return {
                function: funcName,
                hook_active: false,
                reason: 'No hook defined for this function',
                has_weak_symbol: func.hasWeakSymbol,
            };
        }
        // Check if weak symbol might override
        const potentialIssue = func.hasWeakSymbol && hook.type === 'PPC_FUNC';
        return {
            function: funcName,
            hook_active: !potentialIssue,
            hook_type: hook.type,
            host_function: hook.hostFunction,
            file: hook.file,
            line: hook.line,
            reason: potentialIssue
                ? 'Weak symbol in static library may override hook - use GUEST_FUNCTION_HOOK instead'
                : undefined,
            has_weak_symbol: func.hasWeakSymbol,
        };
    }
    // Tool 7: blocking_path_finder
    findBlockingPath(funcName) {
        const func = this.index.functionsByName.get(funcName);
        if (!func) {
            return {
                function: funcName,
                shortest_path: [],
                blocking_api: 'unknown',
                fix_suggestion: 'Function not found',
            };
        }
        // BFS to find shortest path to blocking call
        const queue = [{ name: funcName, path: [funcName] }];
        const visited = new Set();
        while (queue.length > 0) {
            const current = queue.shift();
            if (visited.has(current.name))
                continue;
            visited.add(current.name);
            const currentFunc = this.index.functionsByName.get(current.name);
            if (!currentFunc)
                continue;
            // Check for blocking API calls
            for (const api of currentFunc.kernelCalls) {
                if (BLOCKING_APIS.has(api)) {
                    return {
                        function: funcName,
                        shortest_path: [...current.path, api],
                        blocking_api: api,
                        fix_suggestion: `Hook ${current.name} to bypass blocking call, or signal the waited object before`,
                    };
                }
            }
            // Add callees to queue
            for (const callee of currentFunc.calls) {
                if (!visited.has(callee)) {
                    queue.push({ name: callee, path: [...current.path, callee] });
                }
            }
        }
        return {
            function: funcName,
            shortest_path: [],
            blocking_api: 'none',
            fix_suggestion: 'No blocking calls found in reachable code',
        };
    }
    // Tool 8: init_sequence_validator
    validateInit(funcName) {
        const func = this.index.functionsByName.get(funcName);
        if (!func) {
            return {
                function: funcName,
                globals_initialized: [],
                globals_missing: [],
            };
        }
        const globalsInit = [];
        const globalsMissing = [];
        const visited = new Set();
        let blockedAt;
        let blockingReason;
        // Collect all globals that would be initialized
        this.collectInitializedGlobals(funcName, globalsInit, visited);
        // Check for blocking points
        const blockingPath = this.findBlockingPath(funcName);
        if (blockingPath.blocking_api !== 'none') {
            blockedAt = blockingPath.shortest_path[blockingPath.shortest_path.length - 2];
            blockingReason = `${blockingPath.blocking_api} call`;
        }
        // Check if any stubbed functions would prevent initialization
        for (const callee of func.calls) {
            const hook = this.index.hooks.get(callee);
            if (hook?.type === 'GUEST_FUNCTION_STUB') {
                // Find what globals the stubbed function would have written
                const stubbedFunc = this.index.functionsByName.get(callee);
                if (stubbedFunc) {
                    for (const write of stubbedFunc.writesGlobals) {
                        if (!globalsMissing.includes(write.address)) {
                            globalsMissing.push(write.address);
                        }
                    }
                }
            }
        }
        return {
            function: funcName,
            blocked_at: blockedAt,
            blocking_reason: blockingReason,
            globals_initialized: globalsInit.slice(0, 20),
            globals_missing: globalsMissing,
        };
    }
    collectInitializedGlobals(funcName, globals, visited) {
        if (visited.has(funcName))
            return;
        visited.add(funcName);
        const func = this.index.functionsByName.get(funcName);
        if (!func)
            return;
        for (const write of func.writesGlobals) {
            if (!globals.includes(write.address)) {
                globals.push(write.address);
            }
        }
        for (const callee of func.calls) {
            // Skip if callee is stubbed
            const hook = this.index.hooks.get(callee);
            if (hook?.type !== 'GUEST_FUNCTION_STUB') {
                this.collectInitializedGlobals(callee, globals, visited);
            }
        }
    }
    // Tool 9: vtable_inspector
    inspectVTable(address) {
        const normalizedAddr = address.toLowerCase();
        // VTables are typically in the data section with function pointers
        const global = this.index.globals.get(normalizedAddr);
        if (!global)
            return null;
        // This is a simplified vtable detection
        // In practice, we'd need to analyze memory layout
        const entries = [];
        const usedBy = [];
        // Find functions that read this address (vtable users)
        for (const reader of global.readers) {
            usedBy.push(reader);
        }
        return {
            address: normalizedAddr,
            entries,
            initializedBy: global.writers[0],
            usedBy,
        };
    }
    // Tool 10: ppc_memory_map
    describeAddress(address) {
        const addr = parseInt(address, 16);
        const normalizedAddr = address.toLowerCase();
        // Find region
        let region = 'Unknown';
        let description = 'Address not in known PPC memory range';
        for (const r of MEMORY_REGIONS) {
            if (addr >= r.start && addr < r.end) {
                region = r.name;
                description = r.description;
                break;
            }
        }
        // Check if it's a known function
        const func = this.index.functions.get(normalizedAddr);
        if (func) {
            return {
                address: normalizedAddr,
                region,
                type: 'function',
                used_by: func.calledBy.slice(0, 10),
            };
        }
        // Check if it's a known global
        const global = this.index.globals.get(normalizedAddr);
        if (global) {
            return {
                address: normalizedAddr,
                region,
                type: this.inferGlobalType(normalizedAddr),
                created_by: global.writers[0],
                used_by: global.readers.slice(0, 10),
            };
        }
        return {
            address: normalizedAddr,
            region,
            type: description,
        };
    }
    // Tool 11: recursive_call_tree
    buildRecursiveCallTree(funcName, maxDepth = 5) {
        const func = this.index.functionsByName.get(funcName);
        if (!func)
            return null;
        return this.buildCallTreeRecursive(funcName, new Set(), maxDepth);
    }
    buildCallTreeRecursive(funcName, visited, remainingDepth) {
        const func = this.index.functionsByName.get(funcName);
        const tree = {
            function: funcName,
            address: func?.address || 'unknown',
            depth: 0,
            calls: [],
            globals_accessed: [],
            kernel_apis: func?.kernelCalls || [],
        };
        if (!func || remainingDepth <= 0 || visited.has(funcName)) {
            return tree;
        }
        visited.add(funcName);
        // Collect globals
        tree.globals_accessed = [
            ...func.readsGlobals.map(g => `R:${g.address}`),
            ...func.writesGlobals.map(g => `W:${g.address}`),
        ].slice(0, 10);
        // Recurse into calls
        for (const callee of func.calls) {
            const childTree = this.buildCallTreeRecursive(callee, new Set(visited), remainingDepth - 1);
            childTree.depth = tree.depth + 1;
            tree.calls.push(childTree);
        }
        return tree;
    }
    // Helper: infer global type
    inferGlobalType(address) {
        const addr = parseInt(address, 16);
        // Heuristics based on address patterns
        if (addr >= 0x82010000 && addr < 0x82020000) {
            return 'vtable_pointer';
        }
        if (addr >= 0x828D0000 && addr < 0x82900000) {
            return 'game_state';
        }
        if (addr >= 0x83100000 && addr < 0x83200000) {
            return 'dynamic_object';
        }
        return undefined;
    }
    // Utility: search functions
    searchFunctions(pattern) {
        const regex = new RegExp(pattern, 'i');
        const results = [];
        for (const [name] of this.index.functionsByName) {
            if (regex.test(name)) {
                results.push(name);
            }
        }
        return results.slice(0, 50);
    }
    // Utility: list all hooks
    listHooks() {
        const hooks = [];
        for (const [name, hook] of this.index.hooks) {
            hooks.push({
                name,
                type: hook.type,
                host: hook.hostFunction,
            });
        }
        return hooks;
    }
    // Utility: list kernel APIs
    listKernelAPIs(category) {
        const apis = [];
        for (const [, api] of this.index.kernelAPIs) {
            if (!category || api.category === category) {
                apis.push({
                    name: api.name,
                    category: api.category,
                    callers: api.directCallers.length,
                    blocking: api.isBlocking,
                });
            }
        }
        return apis.sort((a, b) => b.callers - a.callers);
    }
}
