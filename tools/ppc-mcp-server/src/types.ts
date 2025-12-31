// Core types for PPC Recompilation Analysis

export interface PPCFunction {
  address: string;           // e.g., "0x82120000"
  name: string;              // e.g., "sub_82120000"
  file: string;              // source file
  startLine: number;
  endLine: number;
  calls: string[];           // functions this calls
  calledBy: string[];        // functions that call this
  readsGlobals: GlobalAccess[];
  writesGlobals: GlobalAccess[];
  kernelCalls: string[];     // __imp__ calls
  hasWeakSymbol: boolean;
  isHooked: boolean;
  hookInfo?: HookInfo;
}

export interface GlobalAccess {
  address: string;           // e.g., "0x83127984"
  offset?: number;           // computed offset if any
  type: 'read' | 'write';
  size: 32 | 64;             // PPC_LOAD_U32 vs PPC_LOAD_U64
  context: string;           // surrounding code context
}

export interface HookInfo {
  type: 'GUEST_FUNCTION_HOOK' | 'GUEST_FUNCTION_STUB' | 'PPC_FUNC' | 'custom';
  hostFunction?: string;     // e.g., "XCreateFileA"
  purpose?: string;
  file: string;
  line: number;
}

export interface SyncPrimitive {
  address: string;
  type: 'event' | 'semaphore' | 'mutex' | 'spinlock' | 'unknown';
  createdBy: string[];       // functions that create/init this
  waitedOnBy: string[];      // functions that wait on this
  signaledBy: string[];      // functions that signal this
  kernelApi: string;         // e.g., "KeInitializeEvent"
}

export interface KernelAPI {
  name: string;              // e.g., "KeWaitForSingleObject"
  importName: string;        // e.g., "__imp__KeWaitForSingleObject"
  category: 'thread' | 'sync' | 'file' | 'memory' | 'video' | 'audio' | 'xam' | 'misc';
  isBlocking: boolean;
  directCallers: string[];   // PPC functions that directly call this
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

// Index types for fast lookups
export interface PPCIndex {
  functions: Map<string, PPCFunction>;           // addr -> function
  functionsByName: Map<string, PPCFunction>;     // name -> function
  callGraph: Map<string, Set<string>>;           // caller -> callees
  reverseCallGraph: Map<string, Set<string>>;    // callee -> callers
  globals: Map<string, GlobalInfo>;              // addr -> info
  syncPrimitives: Map<string, SyncPrimitive>;
  kernelAPIs: Map<string, KernelAPI>;
  hooks: Map<string, HookInfo>;                  // func name -> hook
  vtables: Map<string, VTable>;
}

export interface GlobalInfo {
  address: string;
  readers: string[];         // functions that read
  writers: string[];         // functions that write
  type?: string;             // inferred type
  initializedBy?: string;
}

// Tool result types
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
  writes_globals: { addr: string; type?: string }[];
  reads_globals: string[];
  creates_sync_objects: { type: string; addr: string }[];
  kernel_apis: string[];
}

export interface KernelTraceUp {
  api: string;
  direct_callers: string[];
  indirect_callers: string[];  // "A → B → C → API"
}

export interface KernelTraceDown {
  function: string;
  kernel_apis_reached: {
    api: string;
    via: string;  // call path
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

// Enhanced VTable types for vtable tracing
export interface EnhancedVTableEntry {
  offset: number;              // Offset within vtable (0, 4, 8, ...)
  funcAddress: string;         // Function pointer stored (e.g., "0x82895300")
  funcName: string;            // Function name (e.g., "sub_82895300")
  initializedBy: string;       // Function that stores this entry
  initLine?: number;           // Line in PPC file where init happens
  initContext?: string;        // Code context of initialization
  status: 'initialized' | 'null' | 'unknown';
}

export interface EnhancedVTable {
  address: string;             // VTable address (e.g., "0x82010F0C")
  addressHex: string;          // Normalized hex
  region: string;              // Memory region name
  entryCount: number;          // Number of entries detected
  entries: EnhancedVTableEntry[];
  initializers: string[];      // All functions that write to this vtable
  readers: string[];           // Functions that read from this vtable
  initChain?: string[];        // Call chain to reach initializer
}

export interface VTableInitTrace {
  vtableAddress: string;
  entry: EnhancedVTableEntry;
  initChain: string[];         // sub_A → sub_B → sub_C (initializer)
  rootFunction: string;        // Top-level function that triggers init
  isStubbed: boolean;          // Whether any function in chain is stubbed
  stubbedFunctions: string[];  // Which functions are stubbed
  warning?: string;
}

export interface VTableChainAnalysis {
  function: string;
  vtableAddress: string;
  willInitialize: boolean;
  initPath?: string[];         // Path from function to vtable init
  directlyInitializes: boolean;
  initializesVia?: string;     // Intermediate function that does init
  blockedBy?: string;          // If stubbed, which stub blocks it
}

export interface VTableUsage {
  vtableAddress: string;
  users: {
    function: string;
    address: string;
    accessType: 'read' | 'indirect_call';
    offset?: number;           // Which vtable entry is accessed
    context: string;
  }[];
  callSites: number;           // Total call sites using this vtable
}
