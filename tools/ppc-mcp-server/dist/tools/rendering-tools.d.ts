import type { PPCIndex } from '../types.js';
export declare class RenderingTools {
    private index;
    private ppcDir;
    private projectRoot;
    constructor(index: PPCIndex, ppcDir: string);
    traceRenderPath(): {
        renderPath: {
            function: string;
            description: string;
            file?: string;
            hooked: boolean;
        }[];
        initChain: {
            function: string;
            description: string;
            blocking?: string;
        }[];
        gpuFunctions: {
            function: string;
            category: string;
            description: string;
            calledBy: string[];
        }[];
    };
    analyzeVdSwap(): {
        vdSwapFunction: string;
        hookStatus: {
            isHooked: boolean;
            hookType?: string;
        };
        callers: string[];
        events: string[];
        frameFlow: {
            step: string;
            description: string;
        }[];
        knownIssues: string[];
    };
    inspectGpuCommands(category?: string): {
        categories: string[];
        commands: {
            function: string;
            category: string;
            description: string;
            params?: string;
            hookStatus: string;
        }[];
        drawCallFunctions: string[];
        resourceFunctions: string[];
        stateFunctions: string[];
    };
    trackRenderState(funcName: string): {
        function: string;
        stateAccess: {
            type: string;
            offset: number;
            description: string;
        }[];
        deviceOffsets: {
            offset: number;
            usage: string;
        }[];
        renderTargetOps: string[];
        shaderOps: string[];
    } | null;
    analyzeShaderUsage(): {
        vertexShaderFunctions: {
            name: string;
            callers: number;
        }[];
        pixelShaderFunctions: {
            name: string;
            callers: number;
        }[];
        shaderCreation: {
            function: string;
            type: string;
        }[];
        shaderBinding: {
            function: string;
            description: string;
        }[];
    };
    analyzeFrameTiming(): {
        vblankTarget: string;
        frameTimeMs: number;
        timingFunctions: {
            function: string;
            description: string;
        }[];
        syncPoints: {
            location: string;
            type: string;
            description: string;
        }[];
        magicValues: {
            value: number;
            name: string;
            effect: string;
        }[];
    };
    analyzeRenderBlocking(): {
        blockingPoints: {
            function: string;
            reason: string;
            solution: string;
        }[];
        initBlockers: {
            function: string;
            blocksAt: string;
            workaround: string;
        }[];
        syncPrimitiveIssues: {
            primitive: string;
            problem: string;
            fix: string;
        }[];
    };
    buildProject(platform: string, mode: string): Promise<{
        success: boolean;
        preset: string;
        buildDir: string;
        commands: string[];
        output?: string;
        error?: string;
    }>;
    listBuildTargets(): {
        platforms: {
            name: string;
            presets: string[];
        }[];
        modes: string[];
        currentOS: string;
        recommendedPreset: string;
    };
    inspectDeviceContext(): {
        tlsOffset: number;
        deviceSize: string;
        knownOffsets: {
            offset: number;
            name: string;
            type: string;
            usage: string;
        }[];
        criticalFields: {
            offset: number;
            description: string;
            impact: string;
        }[];
    };
    inspectRenderContext(): {
        globalAddress: string;
        structure: {
            offset: number;
            name: string;
            type: string;
            description: string;
        }[];
        vtableFunctions: {
            index: number;
            function: string;
            description: string;
            blocking: boolean;
        }[];
    };
}
