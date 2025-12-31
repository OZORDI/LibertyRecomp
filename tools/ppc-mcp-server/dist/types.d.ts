export interface PPCFunction {
    address: string;
    name: string;
    file: string;
    startLine: number;
    endLine: number;
    calls: string[];
    calledBy: string[];
    readsGlobals: GlobalAccess[];
    writesGlobals: GlobalAccess[];
    kernelCalls: string[];
    hasWeakSymbol: boolean;
    isHooked: boolean;
    hookInfo?: HookInfo;
}
export interface GlobalAccess {
    address: string;
    offset?: number;
    type: 'read' | 'write';
    size: 32 | 64;
    context: string;
}
export interface HookInfo {
    type: 'GUEST_FUNCTION_HOOK' | 'GUEST_FUNCTION_STUB' | 'PPC_FUNC' | 'custom';
    hostFunction?: string;
    purpose?: string;
    file: string;
    line: number;
}
export interface SyncPrimitive {
    address: string;
    type: 'event' | 'semaphore' | 'mutex' | 'spinlock' | 'unknown';
    createdBy: string[];
    waitedOnBy: string[];
    signaledBy: string[];
    kernelApi: string;
}
export interface KernelAPI {
    name: string;
    importName: string;
    category: 'thread' | 'sync' | 'file' | 'memory' | 'video' | 'audio' | 'xam' | 'misc';
    isBlocking: boolean;
    directCallers: string[];
    hostImplementation?: string;
    isStubbed: boolean;
}
export interface VTableEntry {
    offset: number;
    funcAddress: string;
    funcName?: string;
    status: 'initialized' | 'null' | 'unknown';
}
export interface VTable {
    address: string;
    entries: VTableEntry[];
    initializedBy?: string;
    usedBy: string[];
}
export interface MemoryRegion {
    start: string;
    end: string;
    name: string;
    description: string;
}
export interface PPCIndex {
    functions: Map<string, PPCFunction>;
    functionsByName: Map<string, PPCFunction>;
    callGraph: Map<string, Set<string>>;
    reverseCallGraph: Map<string, Set<string>>;
    globals: Map<string, GlobalInfo>;
    syncPrimitives: Map<string, SyncPrimitive>;
    kernelAPIs: Map<string, KernelAPI>;
    hooks: Map<string, HookInfo>;
    vtables: Map<string, VTable>;
}
export interface GlobalInfo {
    address: string;
    readers: string[];
    writers: string[];
    type?: string;
    initializedBy?: string;
}
export interface SyncPrimitiveAnalysis {
    address: string;
    type: string;
    waited_on_by: string[];
    signaled_by: string[];
    created_by: string[];
    current_state?: string;
}
export interface CallTreeAnalysis {
    function: string;
    address: string;
    calls: string[];
    writes_globals: {
        addr: string;
        type?: string;
    }[];
    reads_globals: string[];
    creates_sync_objects: {
        type: string;
        addr: string;
    }[];
    kernel_apis: string[];
}
export interface KernelTraceUp {
    api: string;
    direct_callers: string[];
    indirect_callers: string[];
}
export interface KernelTraceDown {
    function: string;
    kernel_apis_reached: {
        api: string;
        via: string;
    }[];
}
export interface GlobalDependency {
    address: string;
    initialized_by?: string;
    read_by: string[];
    type?: string;
    warning?: string;
}
export interface StubImpact {
    function: string;
    globals_not_initialized: string[];
    functions_that_crash: string[];
    sync_objects_not_created: string[];
    recommended_action: string;
}
export interface BlockingPath {
    function: string;
    shortest_path: string[];
    blocking_api: string;
    sync_object?: string;
    fix_suggestion: string;
}
export interface InitValidation {
    function: string;
    subsystems?: number;
    completed?: number;
    blocked_at?: string;
    blocking_reason?: string;
    globals_initialized: string[];
    globals_missing: string[];
}
export interface AddressDescription {
    address: string;
    region: string;
    type?: string;
    created_by?: string;
    used_by?: string[];
    current_state?: Record<string, unknown>;
}
export interface RecursiveCallTree {
    function: string;
    address: string;
    depth: number;
    calls: RecursiveCallTree[];
    globals_accessed: string[];
    kernel_apis: string[];
}
export interface EnhancedVTableEntry {
    offset: number;
    funcAddress: string;
    funcName: string;
    initializedBy: string;
    initLine?: number;
    initContext?: string;
    status: 'initialized' | 'null' | 'unknown';
}
export interface EnhancedVTable {
    address: string;
    addressHex: string;
    region: string;
    entryCount: number;
    entries: EnhancedVTableEntry[];
    initializers: string[];
    readers: string[];
    initChain?: string[];
}
export interface VTableInitTrace {
    vtableAddress: string;
    entry: EnhancedVTableEntry;
    initChain: string[];
    rootFunction: string;
    isStubbed: boolean;
    stubbedFunctions: string[];
    warning?: string;
}
export interface VTableChainAnalysis {
    function: string;
    vtableAddress: string;
    willInitialize: boolean;
    initPath?: string[];
    directlyInitializes: boolean;
    initializesVia?: string;
    blockedBy?: string;
}
export interface VTableUsage {
    vtableAddress: string;
    users: {
        function: string;
        address: string;
        accessType: 'read' | 'indirect_call';
        offset?: number;
        context: string;
    }[];
    callSites: number;
}
