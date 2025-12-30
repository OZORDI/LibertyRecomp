// PPC Recompiled Code Parser
import * as fs from 'fs';
import * as path from 'path';
import type { PPCFunction, GlobalAccess, PPCIndex, GlobalInfo, SyncPrimitive, KernelAPI } from '../types.js';

// Regex patterns for parsing PPC recompiled code
const PATTERNS = {
  // Function definitions
  funcImpl: /PPC_FUNC_IMPL\(__imp__(sub_[0-9A-Fa-f]+)\)/g,
  weakFunc: /PPC_WEAK_FUNC\((sub_[0-9A-Fa-f]+)\)/g,
  ppcFunc: /PPC_FUNC\((sub_[0-9A-Fa-f]+)\)/g,
  
  // Function calls
  funcCall: /\b(sub_[0-9A-Fa-f]+)\(ctx,\s*base\)/g,
  impCall: /__imp__([A-Za-z0-9_]+)\(ctx,\s*base\)/g,
  
  // Memory access patterns
  loadU32: /PPC_LOAD_U32\(([^)]+)\)/g,
  loadU64: /PPC_LOAD_U64\(([^)]+)\)/g,
  storeU32: /PPC_STORE_U32\(([^,]+),\s*([^)]+)\)/g,
  storeU64: /PPC_STORE_U64\(([^,]+),\s*([^)]+)\)/g,
  
  // Address patterns
  hexAddr: /0x[0-9A-Fa-f]+/g,
  globalAddr: /0x8[23][0-9A-Fa-f]{6}/g,  // PPC globals are in 0x82/0x83 range
  
  // Function mapping
  funcMapping: /\{\s*(0x[0-9A-Fa-f]+),\s*(sub_[0-9A-Fa-f]+)\s*\}/g,
};

// Kernel APIs that are blocking
const BLOCKING_APIS = new Set([
  'KeWaitForSingleObject',
  'KeWaitForMultipleObjects', 
  'NtWaitForSingleObject',
  'NtWaitForSingleObjectEx',
  'NtWaitForMultipleObjects',
  'KeDelayExecutionThread',
  'NtDelayExecution',
  'KeAcquireSpinLock',
  'ExAcquireReadWriteLockExclusive',
  'ExAcquireReadWriteLockShared',
]);

// Sync primitive creation APIs
const SYNC_CREATE_APIS: Record<string, string> = {
  'KeInitializeEvent': 'event',
  'KeInitializeSemaphore': 'semaphore',
  'KeInitializeMutant': 'mutex',
  'KeInitializeSpinLock': 'spinlock',
  'ExInitializeReadWriteLock': 'rwlock',
  'RtlInitializeCriticalSection': 'critical_section',
};

// Sync primitive signal APIs
const SYNC_SIGNAL_APIS = new Set([
  'KeSetEvent',
  'KePulseEvent',
  'KeReleaseSemaphore',
  'KeReleaseMutant',
  'KeReleaseSpinLock',
]);

// Sync primitive wait APIs
const SYNC_WAIT_APIS = new Set([
  'KeWaitForSingleObject',
  'KeWaitForMultipleObjects',
  'NtWaitForSingleObject',
  'NtWaitForSingleObjectEx',
]);

// Kernel API categories
const API_CATEGORIES: Record<string, KernelAPI['category']> = {
  // Thread
  'ExCreateThread': 'thread', 'NtSuspendThread': 'thread', 'NtResumeThread': 'thread',
  'KeTlsAlloc': 'thread', 'KeTlsGetValue': 'thread', 'KeTlsSetValue': 'thread',
  'KeTlsFree': 'thread', 'KeSetAffinityThread': 'thread', 'ExTerminateThread': 'thread',
  
  // Sync
  'KeWaitForSingleObject': 'sync', 'KeWaitForMultipleObjects': 'sync',
  'KeInitializeEvent': 'sync', 'KeSetEvent': 'sync', 'KePulseEvent': 'sync',
  'KeInitializeSemaphore': 'sync', 'KeReleaseSemaphore': 'sync',
  'KeInitializeMutant': 'sync', 'KeReleaseMutant': 'sync',
  'KeAcquireSpinLock': 'sync', 'KeReleaseSpinLock': 'sync',
  'KeTryToAcquireSpinLockAtRaisedIrql': 'sync',
  
  // File
  'NtOpenFile': 'file', 'NtCreateFile': 'file', 'NtReadFile': 'file',
  'NtWriteFile': 'file', 'NtClose': 'file', 'NtSetInformationFile': 'file',
  'NtQueryInformationFile': 'file', 'NtQueryVolumeInformationFile': 'file',
  'NtQueryDirectoryFile': 'file', 'NtFlushBuffersFile': 'file',
  'XCreateFileA': 'file', 'XReadFile': 'file', 'XWriteFile': 'file',
  
  // Memory
  'NtAllocateVirtualMemory': 'memory', 'NtFreeVirtualMemory': 'memory',
  'MmQueryStatistics': 'memory', 'NtQueryVirtualMemory': 'memory',
  'RtlAllocateHeap': 'memory', 'RtlFreeHeap': 'memory',
  
  // Video
  'VdSwap': 'video', 'VdGetSystemCommandBuffer': 'video',
  'VdInitializeRingBuffer': 'video', 'VdShutdownEngines': 'video',
  'XGetVideoMode': 'video', 'VdQueryVideoMode': 'video',
  
  // Audio
  'XAudioGetVoiceCategoryVolume': 'audio', 'XMAReleaseContext': 'audio',
  'XMACreateContext': 'audio',
  
  // XAM
  'XamShowSigninUI': 'xam', 'XamContentCreateEx': 'xam', 'XamContentClose': 'xam',
  'XamTaskSchedule': 'xam', 'XamEnumerate': 'xam', 'XamUserGetName': 'xam',
};

export class PPCParser {
  private ppcDir: string;
  private importsFile: string;
  
  constructor(ppcDir: string, importsFile: string) {
    this.ppcDir = ppcDir;
    this.importsFile = importsFile;
  }

  async parseAll(): Promise<PPCIndex> {
    console.log('Starting PPC code parsing...');
    
    const index: PPCIndex = {
      functions: new Map(),
      functionsByName: new Map(),
      callGraph: new Map(),
      reverseCallGraph: new Map(),
      globals: new Map(),
      syncPrimitives: new Map(),
      kernelAPIs: new Map(),
      hooks: new Map(),
      vtables: new Map(),
    };

    // Parse function mapping first
    await this.parseFunctionMapping(index);
    
    // Parse all PPC recomp files
    await this.parsePPCFiles(index);
    
    // Parse imports.cpp for hooks
    await this.parseImports(index);
    
    // Build reverse call graph
    this.buildReverseCallGraph(index);
    
    // Identify sync primitives
    this.identifySyncPrimitives(index);
    
    console.log(`Parsed ${index.functions.size} functions`);
    console.log(`Found ${index.globals.size} global addresses`);
    console.log(`Found ${index.kernelAPIs.size} kernel APIs`);
    console.log(`Found ${index.hooks.size} hooks`);
    
    return index;
  }

  private async parseFunctionMapping(index: PPCIndex): Promise<void> {
    const mappingFile = path.join(this.ppcDir, 'ppc_func_mapping.cpp');
    if (!fs.existsSync(mappingFile)) return;
    
    const content = fs.readFileSync(mappingFile, 'utf-8');
    let match;
    
    while ((match = PATTERNS.funcMapping.exec(content)) !== null) {
      const [, addr, name] = match;
      const func: PPCFunction = {
        address: addr.toLowerCase(),
        name,
        file: 'ppc_func_mapping.cpp',
        startLine: 0,
        endLine: 0,
        calls: [],
        calledBy: [],
        readsGlobals: [],
        writesGlobals: [],
        kernelCalls: [],
        hasWeakSymbol: false,
        isHooked: false,
      };
      index.functions.set(addr.toLowerCase(), func);
      index.functionsByName.set(name, func);
    }
  }

  private async parsePPCFiles(index: PPCIndex): Promise<void> {
    const files = fs.readdirSync(this.ppcDir)
      .filter(f => f.startsWith('ppc_recomp.') && f.endsWith('.cpp'));
    
    for (const file of files) {
      console.log(`Parsing ${file}...`);
      await this.parsePPCFile(path.join(this.ppcDir, file), index);
    }
  }

  private async parsePPCFile(filePath: string, index: PPCIndex): Promise<void> {
    const content = fs.readFileSync(filePath, 'utf-8');
    const lines = content.split('\n');
    const fileName = path.basename(filePath);
    
    let currentFunc: PPCFunction | null = null;
    let funcStartLine = 0;
    let braceCount = 0;
    
    for (let i = 0; i < lines.length; i++) {
      const line = lines[i];
      const lineNum = i + 1;
      
      // Check for function start
      const implMatch = line.match(/PPC_FUNC_IMPL\(__imp__(sub_[0-9A-Fa-f]+)\)/);
      if (implMatch) {
        const name = implMatch[1];
        const addr = '0x' + name.substring(4).toLowerCase();
        
        currentFunc = index.functions.get(addr) || {
          address: addr,
          name,
          file: fileName,
          startLine: lineNum,
          endLine: 0,
          calls: [],
          calledBy: [],
          readsGlobals: [],
          writesGlobals: [],
          kernelCalls: [],
          hasWeakSymbol: false,
          isHooked: false,
        };
        
        currentFunc.file = fileName;
        currentFunc.startLine = lineNum;
        funcStartLine = lineNum;
        braceCount = 0;
        
        index.functions.set(addr, currentFunc);
        index.functionsByName.set(name, currentFunc);
      }
      
      // Check for weak symbol
      const weakMatch = line.match(/PPC_WEAK_FUNC\((sub_[0-9A-Fa-f]+)\)/);
      if (weakMatch) {
        const name = weakMatch[1];
        const addr = '0x' + name.substring(4).toLowerCase();
        const func = index.functions.get(addr);
        if (func) func.hasWeakSymbol = true;
      }
      
      // Track braces for function boundaries
      if (currentFunc) {
        braceCount += (line.match(/{/g) || []).length;
        braceCount -= (line.match(/}/g) || []).length;
        
        // Parse function calls
        let callMatch;
        const funcCallRegex = /\b(sub_[0-9A-Fa-f]+)\(ctx,\s*base\)/g;
        while ((callMatch = funcCallRegex.exec(line)) !== null) {
          const calledName = callMatch[1];
          if (calledName !== currentFunc.name && !currentFunc.calls.includes(calledName)) {
            currentFunc.calls.push(calledName);
            
            // Update call graph
            if (!index.callGraph.has(currentFunc.name)) {
              index.callGraph.set(currentFunc.name, new Set());
            }
            index.callGraph.get(currentFunc.name)!.add(calledName);
          }
        }
        
        // Parse __imp__ calls (kernel APIs)
        const impCallRegex = /__imp__([A-Za-z0-9_]+)\(ctx,\s*base\)/g;
        while ((callMatch = impCallRegex.exec(line)) !== null) {
          const apiName = callMatch[1];
          if (!currentFunc.kernelCalls.includes(apiName)) {
            currentFunc.kernelCalls.push(apiName);
          }
          
          // Track kernel API
          if (!index.kernelAPIs.has(apiName)) {
            index.kernelAPIs.set(apiName, {
              name: apiName,
              importName: `__imp__${apiName}`,
              category: API_CATEGORIES[apiName] || 'misc',
              isBlocking: BLOCKING_APIS.has(apiName),
              directCallers: [],
              isStubbed: false,
            });
          }
          const api = index.kernelAPIs.get(apiName)!;
          if (!api.directCallers.includes(currentFunc.name)) {
            api.directCallers.push(currentFunc.name);
          }
        }
        
        // Parse memory loads (globals read)
        this.parseMemoryAccess(line, currentFunc, index, 'read');
        
        // Parse memory stores (globals write)
        this.parseMemoryAccess(line, currentFunc, index, 'write');
        
        // Check for function end
        if (braceCount === 0 && lineNum > funcStartLine + 1) {
          currentFunc.endLine = lineNum;
          // Track computed globals for this function
          this.trackComputedGlobals(lines, currentFunc.startLine - 1, lineNum, currentFunc, index);
          currentFunc = null;
        }
      }
    }
  }

  private parseMemoryAccess(
    line: string, 
    func: PPCFunction, 
    index: PPCIndex, 
    type: 'read' | 'write'
  ): void {
    const patterns = type === 'read' 
      ? [/PPC_LOAD_U32\(([^)]+)\)/g, /PPC_LOAD_U64\(([^)]+)\)/g]
      : [/PPC_STORE_U32\(([^,]+)/g, /PPC_STORE_U64\(([^,]+)/g];
    
    for (const pattern of patterns) {
      let match;
      const size = pattern.source.includes('U64') ? 64 : 32;
      
      while ((match = pattern.exec(line)) !== null) {
        const addrExpr = match[1];
        
        // Extract hex addresses from the expression
        const addrMatches = addrExpr.match(/0x8[23][0-9A-Fa-f]{6}/g);
        if (addrMatches) {
          for (const addr of addrMatches) {
            const normalizedAddr = addr.toLowerCase();
            
            const access: GlobalAccess = {
              address: normalizedAddr,
              type,
              size: size as 32 | 64,
              context: line.trim().substring(0, 100),
            };
            
            if (type === 'read') {
              func.readsGlobals.push(access);
            } else {
              func.writesGlobals.push(access);
            }
            
            // Update global info
            if (!index.globals.has(normalizedAddr)) {
              index.globals.set(normalizedAddr, {
                address: normalizedAddr,
                readers: [],
                writers: [],
              });
            }
            const global = index.globals.get(normalizedAddr)!;
            if (type === 'read' && !global.readers.includes(func.name)) {
              global.readers.push(func.name);
            } else if (type === 'write' && !global.writers.includes(func.name)) {
              global.writers.push(func.name);
            }
          }
        }
      }
    }
  }

  // Track computed addresses from lis + offset patterns
  private trackComputedGlobals(
    lines: string[],
    startLine: number,
    endLine: number,
    func: PPCFunction,
    index: PPCIndex
  ): void {
    // Track register values from lis instructions
    const regValues: Map<string, number> = new Map();
    
    for (let i = startLine; i < endLine && i < lines.length; i++) {
      const line = lines[i];
      
      // Match: ctx.rXX.s64 = -NNNNN; (lis instruction - load immediate shifted)
      const lisMatch = line.match(/ctx\.(r\d+)\.s64\s*=\s*(-?\d+);/);
      if (lisMatch) {
        const [, reg, valStr] = lisMatch;
        const val = parseInt(valStr, 10);
        // lis loads into upper 16 bits, so value is already shifted
        regValues.set(reg, val >>> 0); // Convert to unsigned
      }
      
      // Match: PPC_LOAD_U32(ctx.rXX.u32 + OFFSET)
      const loadMatch = line.match(/PPC_LOAD_U(32|64)\(ctx\.(r\d+)\.u32\s*\+\s*(-?\d+)\)/);
      if (loadMatch) {
        const [, size, reg, offsetStr] = loadMatch;
        const baseVal = regValues.get(reg);
        if (baseVal !== undefined) {
          const offset = parseInt(offsetStr, 10);
          const addr = ((baseVal + offset) >>> 0).toString(16).padStart(8, '0');
          if (addr.startsWith('82') || addr.startsWith('83')) {
            this.addGlobalAccess(func, index, '0x' + addr, 'read', parseInt(size) as 32 | 64, line);
          }
        }
      }
      
      // Match: PPC_STORE_U32(ctx.rXX.u32 + OFFSET, ...)
      const storeMatch = line.match(/PPC_STORE_U(32|64)\(ctx\.(r\d+)\.u32\s*\+\s*(-?\d+),/);
      if (storeMatch) {
        const [, size, reg, offsetStr] = storeMatch;
        const baseVal = regValues.get(reg);
        if (baseVal !== undefined) {
          const offset = parseInt(offsetStr, 10);
          const addr = ((baseVal + offset) >>> 0).toString(16).padStart(8, '0');
          if (addr.startsWith('82') || addr.startsWith('83')) {
            this.addGlobalAccess(func, index, '0x' + addr, 'write', parseInt(size) as 32 | 64, line);
          }
        }
      }
    }
  }

  private addGlobalAccess(
    func: PPCFunction,
    index: PPCIndex,
    address: string,
    type: 'read' | 'write',
    size: 32 | 64,
    context: string
  ): void {
    const normalizedAddr = address.toLowerCase();
    
    const access: GlobalAccess = {
      address: normalizedAddr,
      type,
      size,
      context: context.trim().substring(0, 100),
    };
    
    if (type === 'read') {
      func.readsGlobals.push(access);
    } else {
      func.writesGlobals.push(access);
    }
    
    if (!index.globals.has(normalizedAddr)) {
      index.globals.set(normalizedAddr, {
        address: normalizedAddr,
        readers: [],
        writers: [],
      });
    }
    const global = index.globals.get(normalizedAddr)!;
    if (type === 'read' && !global.readers.includes(func.name)) {
      global.readers.push(func.name);
    } else if (type === 'write' && !global.writers.includes(func.name)) {
      global.writers.push(func.name);
    }
  }

  private async parseImports(index: PPCIndex): Promise<void> {
    if (!fs.existsSync(this.importsFile)) {
      console.log('imports.cpp not found');
      return;
    }
    
    const content = fs.readFileSync(this.importsFile, 'utf-8');
    const lines = content.split('\n');
    
    for (let i = 0; i < lines.length; i++) {
      const line = lines[i];
      const lineNum = i + 1;
      
      // GUEST_FUNCTION_HOOK(sub_XXXX, HostFunc)
      const hookMatch = line.match(/GUEST_FUNCTION_HOOK\((\w+),\s*(\w+)\)/);
      if (hookMatch) {
        const [, ppcFunc, hostFunc] = hookMatch;
        index.hooks.set(ppcFunc, {
          type: 'GUEST_FUNCTION_HOOK',
          hostFunction: hostFunc,
          file: 'imports.cpp',
          line: lineNum,
        });
        
        // Mark function as hooked
        const func = index.functionsByName.get(ppcFunc);
        if (func) {
          func.isHooked = true;
          func.hookInfo = index.hooks.get(ppcFunc);
        }
      }
      
      // GUEST_FUNCTION_STUB(sub_XXXX)
      const stubMatch = line.match(/GUEST_FUNCTION_STUB\((\w+)\)/);
      if (stubMatch) {
        const [, ppcFunc] = stubMatch;
        index.hooks.set(ppcFunc, {
          type: 'GUEST_FUNCTION_STUB',
          file: 'imports.cpp',
          line: lineNum,
        });
        
        const func = index.functionsByName.get(ppcFunc);
        if (func) {
          func.isHooked = true;
          func.hookInfo = index.hooks.get(ppcFunc);
        }
      }
      
      // PPC_FUNC(sub_XXXX) custom implementations
      const ppcFuncMatch = line.match(/^PPC_FUNC\((sub_[0-9A-Fa-f]+)\)/);
      if (ppcFuncMatch) {
        const [, ppcFunc] = ppcFuncMatch;
        if (!index.hooks.has(ppcFunc)) {
          index.hooks.set(ppcFunc, {
            type: 'PPC_FUNC',
            file: 'imports.cpp',
            line: lineNum,
          });
        }
        
        const func = index.functionsByName.get(ppcFunc);
        if (func) {
          func.isHooked = true;
          func.hookInfo = index.hooks.get(ppcFunc);
        }
      }
    }
  }

  private buildReverseCallGraph(index: PPCIndex): void {
    for (const [caller, callees] of index.callGraph) {
      for (const callee of callees) {
        if (!index.reverseCallGraph.has(callee)) {
          index.reverseCallGraph.set(callee, new Set());
        }
        index.reverseCallGraph.get(callee)!.add(caller);
        
        // Also update calledBy in function object
        const func = index.functionsByName.get(callee);
        if (func && !func.calledBy.includes(caller)) {
          func.calledBy.push(caller);
        }
      }
    }
  }

  private identifySyncPrimitives(index: PPCIndex): void {
    // Find functions that create sync primitives
    for (const [, func] of index.functions) {
      for (const apiCall of func.kernelCalls) {
        // Check if this creates a sync primitive
        if (SYNC_CREATE_APIS[apiCall]) {
          // The first argument (in r3) typically contains the address
          // We'd need to track register values for exact addresses
          // For now, we note that this function creates sync objects
        }
        
        // Track wait calls
        if (SYNC_WAIT_APIS.has(apiCall)) {
          // Function waits on sync primitives
        }
        
        // Track signal calls
        if (SYNC_SIGNAL_APIS.has(apiCall)) {
          // Function signals sync primitives
        }
      }
    }
  }
}

export async function buildIndex(ppcDir: string, importsFile: string): Promise<PPCIndex> {
  const parser = new PPCParser(ppcDir, importsFile);
  return parser.parseAll();
}
