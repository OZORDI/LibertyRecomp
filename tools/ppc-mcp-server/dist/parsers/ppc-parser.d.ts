import type { PPCIndex } from '../types.js';
export declare class PPCParser {
    private ppcDir;
    private importsFile;
    constructor(ppcDir: string, importsFile: string);
    parseAll(): Promise<PPCIndex>;
    private parseFunctionMapping;
    private parsePPCFiles;
    private parsePPCFile;
    private parseMemoryAccess;
    private trackComputedGlobals;
    private addGlobalAccess;
    private parseImports;
    private buildReverseCallGraph;
    private identifySyncPrimitives;
}
export declare function buildIndex(ppcDir: string, importsFile: string): Promise<PPCIndex>;
