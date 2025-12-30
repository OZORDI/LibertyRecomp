import type { PPCIndex, SyncPrimitiveAnalysis, CallTreeAnalysis, KernelTraceUp, KernelTraceDown, GlobalDependency, StubImpact, BlockingPath, InitValidation, AddressDescription, RecursiveCallTree, VTable } from '../types.js';
export declare class PPCAnalyzer {
    private index;
    constructor(index: PPCIndex);
    analyzeSyncPrimitive(address: string): SyncPrimitiveAnalysis;
    analyzeCallTree(funcName: string): CallTreeAnalysis | null;
    traceKernelCallers(apiName: string): KernelTraceUp;
    private findCallersRecursive;
    traceToKernel(funcName: string): KernelTraceDown;
    private findKernelApisRecursive;
    analyzeGlobal(address: string): GlobalDependency;
    checkStubImpact(funcName: string): StubImpact;
    private collectWrittenGlobals;
    checkHook(funcName: string): {
        function: string;
        hook_active: boolean;
        hook_type?: string;
        host_function?: string;
        file?: string;
        line?: number;
        reason?: string;
        has_weak_symbol: boolean;
    };
    findBlockingPath(funcName: string): BlockingPath;
    validateInit(funcName: string): InitValidation;
    private collectInitializedGlobals;
    inspectVTable(address: string): VTable | null;
    describeAddress(address: string): AddressDescription;
    buildRecursiveCallTree(funcName: string, maxDepth?: number): RecursiveCallTree | null;
    private buildCallTreeRecursive;
    private inferGlobalType;
    searchFunctions(pattern: string): string[];
    listHooks(): {
        name: string;
        type: string;
        host?: string;
    }[];
    listKernelAPIs(category?: string): {
        name: string;
        category: string;
        callers: number;
        blocking: boolean;
    }[];
}
