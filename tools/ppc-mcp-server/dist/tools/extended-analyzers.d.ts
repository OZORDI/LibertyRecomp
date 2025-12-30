import type { PPCIndex } from '../types.js';
interface Annotations {
    functions: Record<string, {
        notes: string;
        labels: string[];
        timestamp: string;
    }>;
    globals: Record<string, {
        name: string;
        type?: string;
        notes?: string;
    }>;
}
export declare class ExtendedAnalyzer {
    private index;
    private ppcDir;
    constructor(index: PPCIndex, ppcDir: string);
    findStringUsage(searchString: string): {
        occurrences: {
            file: string;
            line: number;
            context: string;
            function?: string;
        }[];
        dataAddresses: string[];
    };
    decodeMagicNumber(value: number | string): {
        value: number;
        known: boolean;
        name?: string;
        meaning?: string;
        context?: string;
        hexValue: string;
        usedIn: string[];
    };
    listMagicNumbers(): {
        value: number;
        hex: string;
        name: string;
        meaning: string;
    }[];
    findPattern(patternType: 'spin_loop' | 'retry_loop' | 'state_machine' | 'callback_registration'): {
        pattern: string;
        matches: {
            function: string;
            evidence: string;
            confidence: string;
        }[];
    };
    inferSignature(funcName: string): {
        function: string;
        inferredParams: {
            register: string;
            type: string;
            usage: string;
        }[];
        returnType: string;
        callingConvention: string;
        confidence: string;
    } | null;
    analyzeStructLayout(funcName: string): {
        function: string;
        baseRegisters: string[];
        accessedOffsets: {
            offset: number;
            size: number;
            type: 'read' | 'write';
            count: number;
        }[];
        inferredLayout: {
            offset: number;
            size: number;
            name: string;
        }[];
    } | null;
    exploreXrefs(funcName: string): {
        function: string;
        address: string;
        callers: string[];
        callees: string[];
        globalsRead: string[];
        globalsWritten: string[];
        kernelAPIs: string[];
        isHooked: boolean;
        hookType?: string;
    } | null;
    analyzeCallerContext(funcName: string): {
        function: string;
        callSites: {
            caller: string;
            context: string;
            argsSetup?: string;
        }[];
    } | null;
    findSimilarFunctions(funcName: string): {
        function: string;
        similar: {
            name: string;
            score: number;
            reason: string;
        }[];
    } | null;
    findDeadCode(): {
        deadFunctions: {
            name: string;
            address: string;
            reason: string;
        }[];
        totalFunctions: number;
        deadCount: number;
    };
    private loadAnnotations;
    private saveAnnotations;
    annotateFunction(funcName: string, notes: string, labels?: string[]): {
        success: boolean;
        message: string;
    };
    getAnnotations(funcName?: string): Annotations | {
        notes: string;
        labels: string[];
    } | null;
    labelGlobal(address: string, name: string, type?: string, notes?: string): {
        success: boolean;
        message: string;
    };
    mapSubsystems(): {
        initFunction: string;
        subsystems: {
            index: number;
            function: string;
            kernelAPIs: string[];
            globals: string[];
        }[];
    };
    buildImportDependencyGraph(): {
        categories: Record<string, {
            apis: string[];
            functions: string[];
        }>;
    };
    findThreadEntries(): {
        threadCreators: {
            function: string;
            entryPoints: string[];
        }[];
    };
    traceAsyncCallbacks(): {
        schedulers: {
            function: string;
            callbackInfo: string;
        }[];
        completionHandlers: string[];
    };
    detectSpinLoops(): {
        spinLoops: {
            function: string;
            waitAPI: string;
            pattern: string;
            severity: string;
        }[];
    };
    identifySyncPrimitives(): {
        primitives: {
            type: string;
            createdBy: string[];
            acquiredBy: string[];
            releasedBy: string[];
            waitedBy: string[];
        }[];
        summary: Record<string, number>;
    };
    recommendHooks(funcName: string): {
        function: string;
        blockingPath: string[];
        recommendations: {
            target: string;
            type: string;
            reason: string;
            priority: string;
        }[];
    } | null;
    analyzeExecutionOrder(rootFunc: string): {
        root: string;
        executionOrder: {
            order: number;
            function: string;
            dependencies: string[];
        }[];
    } | null;
    visualizeStateMachine(funcName: string): {
        function: string;
        stateFields: {
            offset: number;
            values: number[];
        }[];
        transitions: {
            from: string;
            to: string;
            condition: string;
        }[];
        pattern: string;
    } | null;
    traceRegisterFlow(funcName: string, register: string): {
        function: string;
        register: string;
        flow: {
            line: string;
            operation: string;
            value?: string;
        }[];
    } | null;
}
export {};
