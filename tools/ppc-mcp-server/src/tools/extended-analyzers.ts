// Extended Analysis Tools for PPC Recompilation
import * as fs from 'fs';
import * as path from 'path';
import type { PPCIndex, PPCFunction } from '../types.js';

// Magic numbers used in Xbox 360 / GTA4 code
const MAGIC_NUMBERS: Record<number, { name: string; meaning: string; context: string }> = {
  996: { name: 'NO_PROGRESS', meaning: 'Do not advance; return 0/no progress', context: 'Async status check - operation not ready' },
  997: { name: 'PENDING', meaning: 'Pending; retry / store pending state', context: 'Async operation still in progress' },
  258: { name: 'WAIT_TIMEOUT', meaning: 'Wait operation timed out', context: 'Maps to 996 in some handlers' },
  259: { name: 'STATUS_PENDING', meaning: 'Triggers explicit wait path', context: 'NtReadFile/NtWriteFile async pending' },
  257: { name: 'WAIT_RETRY', meaning: 'Retry wait operation', context: 'Used in sub_829A9738 retry logic' },
  0x80070012: { name: 'ERROR_NO_MORE_FILES', meaning: 'Directory enumeration complete', context: 'FindNextFile returns this when done' },
  1223: { name: 'ERROR_CANCELLED', meaning: 'Operation was cancelled', context: 'User cancelled UI dialog' },
};

// Sync primitive creation APIs
const SYNC_CREATE_APIS: Record<string, { type: string; description: string }> = {
  'KeInitializeEvent': { type: 'event', description: 'Kernel event object' },
  'KeInitializeSemaphore': { type: 'semaphore', description: 'Counting semaphore' },
  'KeInitializeMutant': { type: 'mutex', description: 'Mutant/mutex object' },
  'KeInitializeSpinLock': { type: 'spinlock', description: 'Spinlock for short critical sections' },
  'ExInitializeReadWriteLock': { type: 'rwlock', description: 'Reader-writer lock' },
  'RtlInitializeCriticalSection': { type: 'critical_section', description: 'User-mode critical section' },
  'RtlInitializeCriticalSectionAndSpinCount': { type: 'critical_section', description: 'Critical section with spin count' },
};

// Sync wait APIs
const SYNC_WAIT_APIS = new Set([
  'KeWaitForSingleObject', 'KeWaitForMultipleObjects',
  'NtWaitForSingleObject', 'NtWaitForSingleObjectEx', 'NtWaitForMultipleObjects',
]);

// Sync signal APIs
const SYNC_SIGNAL_APIS: Record<string, string> = {
  'KeSetEvent': 'event',
  'KePulseEvent': 'event',
  'KeReleaseSemaphore': 'semaphore',
  'KeReleaseMutant': 'mutex',
  'KeReleaseSpinLock': 'spinlock',
  'KeReleaseSpinLockFromRaisedIrql': 'spinlock',
};

// Sync acquire APIs
const SYNC_ACQUIRE_APIS: Record<string, string> = {
  'KeAcquireSpinLock': 'spinlock',
  'KeAcquireSpinLockAtRaisedIrql': 'spinlock',
  'KeTryToAcquireSpinLockAtRaisedIrql': 'spinlock',
  'RtlEnterCriticalSection': 'critical_section',
  'RtlTryEnterCriticalSection': 'critical_section',
};

// Annotations storage path
const ANNOTATIONS_FILE = path.resolve(process.cwd(), '.ppc-annotations.json');

interface Annotations {
  functions: Record<string, { notes: string; labels: string[]; timestamp: string }>;
  globals: Record<string, { name: string; type?: string; notes?: string }>;
}

export class ExtendedAnalyzer {
  private ppcDir: string;
  
  constructor(private index: PPCIndex, ppcDir: string) {
    this.ppcDir = ppcDir;
  }

  // ========== STRING FINDER ==========
  findStringUsage(searchString: string): {
    occurrences: { file: string; line: number; context: string; function?: string }[];
    dataAddresses: string[];
  } {
    const results: { file: string; line: number; context: string; function?: string }[] = [];
    const dataAddresses: string[] = [];
    
    // Search through PPC files for string patterns in comments
    const files = fs.readdirSync(this.ppcDir)
      .filter(f => f.startsWith('ppc_recomp.') && f.endsWith('.cpp'));
    
    for (const file of files) {
      const content = fs.readFileSync(path.join(this.ppcDir, file), 'utf-8');
      const lines = content.split('\n');
      
      let currentFunc = '';
      for (let i = 0; i < lines.length; i++) {
        const line = lines[i];
        
        // Track current function
        const funcMatch = line.match(/PPC_FUNC_IMPL\(__imp__(sub_[0-9A-Fa-f]+)\)/);
        if (funcMatch) currentFunc = funcMatch[1];
        
        // Search for string in comments or data references
        if (line.toLowerCase().includes(searchString.toLowerCase())) {
          results.push({
            file,
            line: i + 1,
            context: line.trim().substring(0, 150),
            function: currentFunc || undefined,
          });
        }
      }
    }
    
    return { occurrences: results.slice(0, 50), dataAddresses };
  }

  // ========== MAGIC NUMBER DECODER ==========
  decodeMagicNumber(value: number | string): {
    value: number;
    known: boolean;
    name?: string;
    meaning?: string;
    context?: string;
    hexValue: string;
    usedIn: string[];
  } {
    const numValue = typeof value === 'string' ? parseInt(value, 0) : value;
    const known = MAGIC_NUMBERS[numValue];
    
    // Find functions that use this value
    const usedIn: string[] = [];
    for (const [, func] of this.index.functions) {
      // Check if function uses this value (simplified check)
      if (func.file) {
        // Would need to parse file content for exact match
      }
    }
    
    return {
      value: numValue,
      known: !!known,
      name: known?.name,
      meaning: known?.meaning,
      context: known?.context,
      hexValue: '0x' + numValue.toString(16).toUpperCase(),
      usedIn,
    };
  }

  listMagicNumbers(): { value: number; hex: string; name: string; meaning: string }[] {
    return Object.entries(MAGIC_NUMBERS).map(([val, info]) => ({
      value: parseInt(val),
      hex: '0x' + parseInt(val).toString(16).toUpperCase(),
      name: info.name,
      meaning: info.meaning,
    }));
  }

  // ========== PATTERN FINDER ==========
  findPattern(patternType: 'spin_loop' | 'retry_loop' | 'state_machine' | 'callback_registration'): {
    pattern: string;
    matches: { function: string; evidence: string; confidence: string }[];
  } {
    const matches: { function: string; evidence: string; confidence: string }[] = [];
    
    for (const [, func] of this.index.functions) {
      let evidence = '';
      let confidence = 'low';
      
      switch (patternType) {
        case 'spin_loop':
          // Functions with wait APIs that might spin
          if (func.kernelCalls.some(k => SYNC_WAIT_APIS.has(k))) {
            // Check for loops back to same location
            evidence = `Calls ${func.kernelCalls.filter(k => SYNC_WAIT_APIS.has(k)).join(', ')}`;
            confidence = 'medium';
            matches.push({ function: func.name, evidence, confidence });
          }
          break;
          
        case 'retry_loop':
          // Functions that call async helpers and check return values
          if (func.kernelCalls.includes('XamTaskShouldExit') || 
              func.calls.some(c => c.includes('sub_829A1F00') || c.includes('sub_829A1A50'))) {
            evidence = 'Uses async status check pattern';
            confidence = 'high';
            matches.push({ function: func.name, evidence, confidence });
          }
          break;
          
        case 'state_machine':
          // Functions with multiple state accesses at offset +0, +4
          if (func.readsGlobals.length > 3 || func.writesGlobals.length > 3) {
            evidence = `Accesses ${func.readsGlobals.length} globals, writes ${func.writesGlobals.length}`;
            confidence = 'medium';
            matches.push({ function: func.name, evidence, confidence });
          }
          break;
          
        case 'callback_registration':
          // Functions that call XamTaskSchedule or ExCreateThread
          if (func.kernelCalls.includes('XamTaskSchedule') || 
              func.kernelCalls.includes('ExCreateThread')) {
            evidence = `Calls ${func.kernelCalls.filter(k => k === 'XamTaskSchedule' || k === 'ExCreateThread').join(', ')}`;
            confidence = 'high';
            matches.push({ function: func.name, evidence, confidence });
          }
          break;
      }
    }
    
    return { pattern: patternType, matches: matches.slice(0, 50) };
  }

  // ========== FUNCTION SIGNATURE INFERRER ==========
  inferSignature(funcName: string): {
    function: string;
    inferredParams: { register: string; type: string; usage: string }[];
    returnType: string;
    callingConvention: string;
    confidence: string;
  } | null {
    const func = this.index.functionsByName.get(funcName);
    if (!func) return null;
    
    // PPC calling convention: r3-r10 for params, r3 for return
    const params: { register: string; type: string; usage: string }[] = [];
    
    // Analyze based on kernel calls and patterns
    if (func.kernelCalls.length > 0) {
      // Has kernel calls - likely takes specific parameter types
      params.push({ register: 'r3', type: 'pointer/context', usage: 'First argument or this pointer' });
    }
    
    // Check if function accesses globals (likely takes no args, uses globals)
    if (func.readsGlobals.length > 5) {
      params.push({ register: 'r3', type: 'pointer', usage: 'Context/state pointer' });
    }
    
    return {
      function: funcName,
      inferredParams: params,
      returnType: 'uint32_t (in r3)',
      callingConvention: 'PPC SysV (r3-r10 params, r3 return)',
      confidence: params.length > 0 ? 'medium' : 'low',
    };
  }

  // ========== STRUCT LAYOUT ANALYZER ==========
  analyzeStructLayout(funcName: string): {
    function: string;
    baseRegisters: string[];
    accessedOffsets: { offset: number; size: number; type: 'read' | 'write'; count: number }[];
    inferredLayout: { offset: number; size: number; name: string }[];
  } | null {
    const func = this.index.functionsByName.get(funcName);
    if (!func || !func.file) return null;
    
    const filePath = path.join(this.ppcDir, func.file);
    if (!fs.existsSync(filePath)) return null;
    
    const content = fs.readFileSync(filePath, 'utf-8');
    const lines = content.split('\n');
    
    const offsetAccesses: Map<number, { size: number; type: 'read' | 'write'; count: number }> = new Map();
    const baseRegisters = new Set<string>();
    
    // Parse function body for offset accesses
    let inFunction = false;
    for (let i = func.startLine - 1; i < func.endLine && i < lines.length; i++) {
      const line = lines[i];
      
      if (line.includes(`__imp__${func.name}`)) inFunction = true;
      if (!inFunction) continue;
      
      // Match: ctx.rXX.u32 + OFFSET
      const offsetMatch = line.match(/ctx\.(r\d+)\.u32\s*\+\s*(\d+)/g);
      if (offsetMatch) {
        for (const match of offsetMatch) {
          const parts = match.match(/ctx\.(r\d+)\.u32\s*\+\s*(\d+)/);
          if (parts) {
            baseRegisters.add(parts[1]);
            const offset = parseInt(parts[2]);
            const isWrite = line.includes('PPC_STORE');
            const size = line.includes('U64') ? 8 : line.includes('U8') ? 1 : 4;
            
            const existing = offsetAccesses.get(offset);
            if (existing) {
              existing.count++;
              if (isWrite) existing.type = 'write';
            } else {
              offsetAccesses.set(offset, { size, type: isWrite ? 'write' : 'read', count: 1 });
            }
          }
        }
      }
    }
    
    const accessedOffsets = Array.from(offsetAccesses.entries())
      .map(([offset, info]) => ({ offset, ...info }))
      .sort((a, b) => a.offset - b.offset);
    
    // Infer layout from offsets
    const inferredLayout = accessedOffsets.map(a => ({
      offset: a.offset,
      size: a.size,
      name: `field_${a.offset.toString(16)}`,
    }));
    
    return {
      function: funcName,
      baseRegisters: Array.from(baseRegisters),
      accessedOffsets,
      inferredLayout,
    };
  }

  // ========== XREF EXPLORER ==========
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
  } | null {
    const func = this.index.functionsByName.get(funcName);
    if (!func) return null;
    
    const hook = this.index.hooks.get(funcName);
    
    return {
      function: funcName,
      address: func.address,
      callers: func.calledBy,
      callees: func.calls,
      globalsRead: func.readsGlobals.map(g => g.address),
      globalsWritten: func.writesGlobals.map(g => g.address),
      kernelAPIs: func.kernelCalls,
      isHooked: !!hook,
      hookType: hook?.type,
    };
  }

  // ========== CALLER CONTEXT ANALYZER ==========
  analyzeCallerContext(funcName: string): {
    function: string;
    callSites: { caller: string; context: string; argsSetup?: string }[];
  } | null {
    const func = this.index.functionsByName.get(funcName);
    if (!func) return null;
    
    const callSites: { caller: string; context: string; argsSetup?: string }[] = [];
    
    for (const callerName of func.calledBy) {
      const caller = this.index.functionsByName.get(callerName);
      if (!caller || !caller.file) continue;
      
      const filePath = path.join(this.ppcDir, caller.file);
      if (!fs.existsSync(filePath)) continue;
      
      const content = fs.readFileSync(filePath, 'utf-8');
      const lines = content.split('\n');
      
      // Find call sites
      for (let i = 0; i < lines.length; i++) {
        if (lines[i].includes(`${funcName}(ctx, base)`)) {
          // Get context around call (previous 5 lines for arg setup)
          const contextStart = Math.max(0, i - 5);
          const context = lines.slice(contextStart, i + 1).join('\n');
          
          // Look for r3, r4 setup
          let argsSetup = '';
          for (let j = i - 5; j < i; j++) {
            if (j >= 0 && (lines[j].includes('ctx.r3') || lines[j].includes('ctx.r4'))) {
              argsSetup += lines[j].trim() + '; ';
            }
          }
          
          callSites.push({
            caller: callerName,
            context: `Line ${i + 1}`,
            argsSetup: argsSetup || undefined,
          });
          break; // Only first call site per caller for now
        }
      }
    }
    
    return { function: funcName, callSites: callSites.slice(0, 20) };
  }

  // ========== SIMILARITY FINDER ==========
  findSimilarFunctions(funcName: string): {
    function: string;
    similar: { name: string; score: number; reason: string }[];
  } | null {
    const func = this.index.functionsByName.get(funcName);
    if (!func) return null;
    
    const similar: { name: string; score: number; reason: string }[] = [];
    
    for (const [, other] of this.index.functions) {
      if (other.name === funcName) continue;
      
      let score = 0;
      const reasons: string[] = [];
      
      // Compare kernel calls
      const commonKernel = func.kernelCalls.filter(k => other.kernelCalls.includes(k));
      if (commonKernel.length > 0) {
        score += commonKernel.length * 10;
        reasons.push(`${commonKernel.length} common kernel APIs`);
      }
      
      // Compare call targets
      const commonCalls = func.calls.filter(c => other.calls.includes(c));
      if (commonCalls.length > 2) {
        score += commonCalls.length * 5;
        reasons.push(`${commonCalls.length} common callees`);
      }
      
      // Compare global access patterns
      const funcGlobals = new Set([...func.readsGlobals.map(g => g.address), ...func.writesGlobals.map(g => g.address)]);
      const otherGlobals = new Set([...other.readsGlobals.map(g => g.address), ...other.writesGlobals.map(g => g.address)]);
      const commonGlobals = [...funcGlobals].filter(g => otherGlobals.has(g));
      if (commonGlobals.length > 0) {
        score += commonGlobals.length * 3;
        reasons.push(`${commonGlobals.length} common globals`);
      }
      
      if (score > 20) {
        similar.push({ name: other.name, score, reason: reasons.join(', ') });
      }
    }
    
    similar.sort((a, b) => b.score - a.score);
    return { function: funcName, similar: similar.slice(0, 20) };
  }

  // ========== DEAD CODE DETECTOR ==========
  findDeadCode(): {
    deadFunctions: { name: string; address: string; reason: string }[];
    totalFunctions: number;
    deadCount: number;
  } {
    const dead: { name: string; address: string; reason: string }[] = [];
    
    for (const [, func] of this.index.functions) {
      // Skip imports and hooked functions
      if (func.name.startsWith('__imp__')) continue;
      if (this.index.hooks.has(func.name)) continue;
      
      // Check if function is never called
      if (func.calledBy.length === 0) {
        dead.push({
          name: func.name,
          address: func.address,
          reason: 'Never called by any function',
        });
      }
    }
    
    return {
      deadFunctions: dead.slice(0, 100),
      totalFunctions: this.index.functions.size,
      deadCount: dead.length,
    };
  }

  // ========== ANNOTATION FUNCTIONS ==========
  private loadAnnotations(): Annotations {
    if (fs.existsSync(ANNOTATIONS_FILE)) {
      try {
        return JSON.parse(fs.readFileSync(ANNOTATIONS_FILE, 'utf-8'));
      } catch {
        return { functions: {}, globals: {} };
      }
    }
    return { functions: {}, globals: {} };
  }

  private saveAnnotations(annotations: Annotations): void {
    fs.writeFileSync(ANNOTATIONS_FILE, JSON.stringify(annotations, null, 2));
  }

  annotateFunction(funcName: string, notes: string, labels?: string[]): { success: boolean; message: string } {
    const annotations = this.loadAnnotations();
    annotations.functions[funcName] = {
      notes,
      labels: labels || [],
      timestamp: new Date().toISOString(),
    };
    this.saveAnnotations(annotations);
    return { success: true, message: `Annotated ${funcName}` };
  }

  getAnnotations(funcName?: string): Annotations | { notes: string; labels: string[] } | null {
    const annotations = this.loadAnnotations();
    if (funcName) {
      return annotations.functions[funcName] || null;
    }
    return annotations;
  }

  labelGlobal(address: string, name: string, type?: string, notes?: string): { success: boolean; message: string } {
    const annotations = this.loadAnnotations();
    annotations.globals[address.toLowerCase()] = { name, type, notes };
    this.saveAnnotations(annotations);
    return { success: true, message: `Labeled ${address} as ${name}` };
  }

  // ========== SUBSYSTEM MAPPER ==========
  mapSubsystems(): {
    initFunction: string;
    subsystems: { index: number; function: string; kernelAPIs: string[]; globals: string[] }[];
  } {
    // sub_82120FB8 is the 63-subsystem init function
    const initFunc = this.index.functionsByName.get('sub_82120FB8');
    const subsystems: { index: number; function: string; kernelAPIs: string[]; globals: string[] }[] = [];
    
    if (initFunc) {
      let idx = 0;
      for (const callee of initFunc.calls) {
        const subFunc = this.index.functionsByName.get(callee);
        subsystems.push({
          index: idx++,
          function: callee,
          kernelAPIs: subFunc?.kernelCalls || [],
          globals: subFunc?.writesGlobals.map(g => g.address) || [],
        });
      }
    }
    
    return { initFunction: 'sub_82120FB8', subsystems };
  }

  // ========== IMPORT DEPENDENCY GRAPH ==========
  buildImportDependencyGraph(): {
    categories: Record<string, { apis: string[]; functions: string[] }>;
  } {
    const categories: Record<string, Set<string>> = {
      thread: new Set(), sync: new Set(), file: new Set(),
      memory: new Set(), video: new Set(), audio: new Set(), xam: new Set(), misc: new Set(),
    };
    
    for (const [, api] of this.index.kernelAPIs) {
      if (!categories[api.category]) categories[api.category] = new Set();
      for (const caller of api.directCallers) {
        categories[api.category].add(caller);
      }
    }
    
    const result: Record<string, { apis: string[]; functions: string[] }> = {};
    for (const [cat, funcs] of Object.entries(categories)) {
      const apis = Array.from(this.index.kernelAPIs.values())
        .filter(a => a.category === cat)
        .map(a => a.name);
      result[cat] = { apis, functions: Array.from(funcs).slice(0, 50) };
    }
    
    return { categories: result };
  }

  // ========== THREAD ENTRY FINDER ==========
  findThreadEntries(): {
    threadCreators: { function: string; entryPoints: string[] }[];
  } {
    const creators: { function: string; entryPoints: string[] }[] = [];
    
    for (const [, func] of this.index.functions) {
      if (func.kernelCalls.includes('ExCreateThread')) {
        // The entry point is typically passed in r6 for ExCreateThread
        creators.push({
          function: func.name,
          entryPoints: ['See r6 register at call site'],
        });
      }
    }
    
    return { threadCreators: creators };
  }

  // ========== ASYNC CALLBACK TRACER ==========
  traceAsyncCallbacks(): {
    schedulers: { function: string; callbackInfo: string }[];
    completionHandlers: string[];
  } {
    const schedulers: { function: string; callbackInfo: string }[] = [];
    const completionHandlers: string[] = [];
    
    for (const [, func] of this.index.functions) {
      if (func.kernelCalls.includes('XamTaskSchedule')) {
        schedulers.push({
          function: func.name,
          callbackInfo: 'Callback address in r3, context in r4',
        });
      }
      
      if (func.kernelCalls.includes('XamTaskShouldExit')) {
        completionHandlers.push(func.name);
      }
    }
    
    return { schedulers, completionHandlers };
  }

  // ========== SPIN LOOP DETECTOR ==========
  detectSpinLoops(): {
    spinLoops: { function: string; waitAPI: string; pattern: string; severity: string }[];
  } {
    const spinLoops: { function: string; waitAPI: string; pattern: string; severity: string }[] = [];
    
    for (const [, func] of this.index.functions) {
      const waitAPIs = func.kernelCalls.filter(k => SYNC_WAIT_APIS.has(k));
      
      for (const api of waitAPIs) {
        // Check if function has potential loop patterns
        const hasGlobalCheck = func.readsGlobals.length > 0;
        const hasMultipleCalls = func.kernelCalls.filter(k => k === api).length > 1 || 
                                 func.calls.includes(func.name); // Recursive
        
        if (hasGlobalCheck || hasMultipleCalls) {
          spinLoops.push({
            function: func.name,
            waitAPI: api,
            pattern: hasMultipleCalls ? 'Potential retry/spin loop' : 'Wait with global condition',
            severity: api.includes('SpinLock') ? 'high' : 'medium',
          });
        }
      }
    }
    
    return { spinLoops: spinLoops.slice(0, 50) };
  }

  // ========== SYNC PRIMITIVE IDENTIFIER ==========
  identifySyncPrimitives(): {
    primitives: {
      type: string;
      createdBy: string[];
      acquiredBy: string[];
      releasedBy: string[];
      waitedBy: string[];
    }[];
    summary: Record<string, number>;
  } {
    const byType: Map<string, {
      createdBy: Set<string>;
      acquiredBy: Set<string>;
      releasedBy: Set<string>;
      waitedBy: Set<string>;
    }> = new Map();
    
    for (const [, func] of this.index.functions) {
      for (const api of func.kernelCalls) {
        // Creation
        if (SYNC_CREATE_APIS[api]) {
          const type = SYNC_CREATE_APIS[api].type;
          if (!byType.has(type)) {
            byType.set(type, { createdBy: new Set(), acquiredBy: new Set(), releasedBy: new Set(), waitedBy: new Set() });
          }
          byType.get(type)!.createdBy.add(func.name);
        }
        
        // Acquire
        if (SYNC_ACQUIRE_APIS[api]) {
          const type = SYNC_ACQUIRE_APIS[api];
          if (!byType.has(type)) {
            byType.set(type, { createdBy: new Set(), acquiredBy: new Set(), releasedBy: new Set(), waitedBy: new Set() });
          }
          byType.get(type)!.acquiredBy.add(func.name);
        }
        
        // Release
        if (SYNC_SIGNAL_APIS[api]) {
          const type = SYNC_SIGNAL_APIS[api];
          if (!byType.has(type)) {
            byType.set(type, { createdBy: new Set(), acquiredBy: new Set(), releasedBy: new Set(), waitedBy: new Set() });
          }
          byType.get(type)!.releasedBy.add(func.name);
        }
        
        // Wait
        if (SYNC_WAIT_APIS.has(api)) {
          const type = 'event'; // Waits are typically on events
          if (!byType.has(type)) {
            byType.set(type, { createdBy: new Set(), acquiredBy: new Set(), releasedBy: new Set(), waitedBy: new Set() });
          }
          byType.get(type)!.waitedBy.add(func.name);
        }
      }
    }
    
    const primitives = Array.from(byType.entries()).map(([type, info]) => ({
      type,
      createdBy: Array.from(info.createdBy),
      acquiredBy: Array.from(info.acquiredBy),
      releasedBy: Array.from(info.releasedBy),
      waitedBy: Array.from(info.waitedBy),
    }));
    
    const summary: Record<string, number> = {};
    for (const [type, info] of byType) {
      summary[type] = info.createdBy.size + info.acquiredBy.size + info.releasedBy.size + info.waitedBy.size;
    }
    
    return { primitives, summary };
  }

  // ========== HOOK RECOMMENDER ==========
  recommendHooks(funcName: string): {
    function: string;
    blockingPath: string[];
    recommendations: { target: string; type: string; reason: string; priority: string }[];
  } | null {
    const func = this.index.functionsByName.get(funcName);
    if (!func) return null;
    
    const recommendations: { target: string; type: string; reason: string; priority: string }[] = [];
    const visited = new Set<string>();
    const blockingPath: string[] = [];
    
    // Find blocking path
    const queue: { name: string; path: string[] }[] = [{ name: funcName, path: [funcName] }];
    
    while (queue.length > 0) {
      const current = queue.shift()!;
      if (visited.has(current.name)) continue;
      visited.add(current.name);
      
      const currentFunc = this.index.functionsByName.get(current.name);
      if (!currentFunc) continue;
      
      // Check for blocking APIs
      for (const api of currentFunc.kernelCalls) {
        if (SYNC_WAIT_APIS.has(api)) {
          blockingPath.push(...current.path, api);
          
          // Recommend hooking the immediate caller of blocking API
          recommendations.push({
            target: current.name,
            type: 'hook_to_non_blocking',
            reason: `Calls ${api} - hook to return immediately`,
            priority: 'high',
          });
          
          // Also recommend hooking parent if it's a utility function
          if (current.path.length > 1) {
            const parent = current.path[current.path.length - 2];
            recommendations.push({
              target: parent,
              type: 'call_original_async',
              reason: 'Parent function - call __imp__ but ensure non-blocking',
              priority: 'medium',
            });
          }
          break;
        }
      }
      
      for (const callee of currentFunc.calls) {
        queue.push({ name: callee, path: [...current.path, callee] });
      }
    }
    
    return { function: funcName, blockingPath, recommendations };
  }

  // ========== EXECUTION ORDER ANALYZER ==========
  analyzeExecutionOrder(rootFunc: string): {
    root: string;
    executionOrder: { order: number; function: string; dependencies: string[] }[];
  } | null {
    const func = this.index.functionsByName.get(rootFunc);
    if (!func) return null;
    
    const order: { order: number; function: string; dependencies: string[] }[] = [];
    const visited = new Set<string>();
    let orderNum = 0;
    
    const visit = (name: string, deps: string[]) => {
      if (visited.has(name)) return;
      visited.add(name);
      
      const f = this.index.functionsByName.get(name);
      if (!f) return;
      
      order.push({ order: orderNum++, function: name, dependencies: deps });
      
      for (const callee of f.calls) {
        visit(callee, [name]);
      }
    };
    
    visit(rootFunc, []);
    
    return { root: rootFunc, executionOrder: order.slice(0, 100) };
  }

  // ========== STATE MACHINE VISUALIZER ==========
  visualizeStateMachine(funcName: string): {
    function: string;
    stateFields: { offset: number; values: number[] }[];
    transitions: { from: string; to: string; condition: string }[];
    pattern: string;
  } | null {
    const func = this.index.functionsByName.get(funcName);
    if (!func) return null;
    
    // Detect state machine patterns based on offset access
    const stateFields: { offset: number; values: number[] }[] = [];
    const transitions: { from: string; to: string; condition: string }[] = [];
    
    // Common state machine offsets
    if (func.readsGlobals.length > 0 || func.writesGlobals.length > 0) {
      stateFields.push({ offset: 0, values: [1, 2, 3, 4, 5] }); // Common state values
      stateFields.push({ offset: 4, values: [1, 2, 3, 4] }); // Sub-state
      
      transitions.push({ from: 'state=2', to: 'state=3', condition: 'async_status == 0' });
      transitions.push({ from: 'state=2', to: 'state=1', condition: 'async_status != 0 && != 996' });
    }
    
    return {
      function: funcName,
      stateFields,
      transitions,
      pattern: func.readsGlobals.length > 3 ? 'Multi-state workflow' : 'Simple state check',
    };
  }

  // ========== REGISTER FLOW TRACER ==========
  traceRegisterFlow(funcName: string, register: string): {
    function: string;
    register: string;
    flow: { line: string; operation: string; value?: string }[];
  } | null {
    const func = this.index.functionsByName.get(funcName);
    if (!func || !func.file) return null;
    
    const filePath = path.join(this.ppcDir, func.file);
    if (!fs.existsSync(filePath)) return null;
    
    const content = fs.readFileSync(filePath, 'utf-8');
    const lines = content.split('\n');
    
    const flow: { line: string; operation: string; value?: string }[] = [];
    const regPattern = new RegExp(`ctx\\.${register}`, 'g');
    
    for (let i = func.startLine - 1; i < func.endLine && i < lines.length; i++) {
      const line = lines[i];
      if (regPattern.test(line)) {
        const isWrite = line.includes(`ctx.${register}.`) && line.includes('=') && !line.includes('==');
        const isRead = line.includes(`ctx.${register}.`) && !isWrite;
        
        flow.push({
          line: `Line ${i + 1}`,
          operation: isWrite ? 'write' : 'read',
          value: line.trim().substring(0, 100),
        });
      }
    }
    
    return { function: funcName, register, flow: flow.slice(0, 50) };
  }

  // ========== VTABLE INSPECTOR (ENHANCED) ==========
  inspectVTableEnhanced(address: string): {
    address: string;
    addressHex: string;
    region: string;
    entryCount: number;
    entries: {
      offset: number;
      funcAddress: string;
      funcName: string;
      initializedBy: string;
      status: string;
    }[];
    initializers: string[];
    readers: string[];
    warning?: string;
  } | null {
    const normalizedAddr = address.toLowerCase().replace(/^0x/, '');
    const addrHex = '0x' + normalizedAddr.toUpperCase();
    const addrNum = parseInt(normalizedAddr, 16);
    
    // Determine memory region
    let region = 'Unknown';
    if (addrNum >= 0x82000000 && addrNum < 0x82020000) region = 'Stream Pool / Header';
    else if (addrNum >= 0x82020000 && addrNum < 0x82120000) region = 'XEX Data Region (vtables live here)';
    else if (addrNum >= 0x82120000 && addrNum < 0x82A00000) region = 'Game Code';
    else if (addrNum >= 0x83000000 && addrNum < 0x84000000) region = 'Static Data / Heap';
    
    // Find all functions that write to addresses near this vtable
    const entries: {
      offset: number;
      funcAddress: string;
      funcName: string;
      initializedBy: string;
      status: string;
    }[] = [];
    const initializers: string[] = [];
    const readers: string[] = [];
    
    // Scan for writes to this address range (vtable entries are typically 4 bytes apart)
    for (const [, func] of this.index.functions) {
      for (const write of func.writesGlobals) {
        const writeAddr = parseInt(write.address.replace(/^0x/, ''), 16);
        // Check if write is within vtable range (assume up to 64 entries, 256 bytes)
        if (writeAddr >= addrNum && writeAddr < addrNum + 256) {
          const offset = writeAddr - addrNum;
          const existingEntry = entries.find(e => e.offset === offset);
          if (!existingEntry) {
            entries.push({
              offset,
              funcAddress: write.context.match(/0x[0-9A-Fa-f]+/)?.[0] || 'unknown',
              funcName: this.extractFuncNameFromContext(write.context),
              initializedBy: func.name,
              status: 'initialized',
            });
          }
          if (!initializers.includes(func.name)) {
            initializers.push(func.name);
          }
        }
      }
      
      for (const read of func.readsGlobals) {
        const readAddr = parseInt(read.address.replace(/^0x/, ''), 16);
        if (readAddr >= addrNum && readAddr < addrNum + 256) {
          if (!readers.includes(func.name)) {
            readers.push(func.name);
          }
        }
      }
    }
    
    // Sort entries by offset
    entries.sort((a, b) => a.offset - b.offset);
    
    // Check for potential issues
    let warning: string | undefined;
    if (initializers.length === 0) {
      warning = 'No initializers found - vtable may be uninitialized or initialized via computed address';
    } else if (readers.length > 0 && initializers.length === 0) {
      warning = 'VTable is read but never initialized - will cause crash';
    }
    
    return {
      address: normalizedAddr,
      addressHex: addrHex,
      region,
      entryCount: entries.length,
      entries,
      initializers,
      readers: readers.slice(0, 20),
      warning,
    };
  }
  
  private extractFuncNameFromContext(context: string): string {
    // Try to extract function name from context like "sub_82895300" or function address
    const funcMatch = context.match(/sub_[0-9A-Fa-f]+/);
    if (funcMatch) return funcMatch[0];
    
    const addrMatch = context.match(/0x8[0-9A-Fa-f]{7}/);
    if (addrMatch) return 'sub_' + addrMatch[0].substring(2).toUpperCase();
    
    return 'unknown';
  }

  // ========== VTABLE INIT TRACER ==========
  traceVTableInit(vtableAddress: string): {
    vtableAddress: string;
    initTraces: {
      entry: number;
      funcPointer: string;
      initializer: string;
      initChain: string[];
      rootFunctions: string[];
      isBlocked: boolean;
      blockedBy?: string;
    }[];
    summary: {
      totalEntries: number;
      initializedEntries: number;
      blockedChains: number;
    };
    recommendations: string[];
  } {
    const vtable = this.inspectVTableEnhanced(vtableAddress);
    const initTraces: {
      entry: number;
      funcPointer: string;
      initializer: string;
      initChain: string[];
      rootFunctions: string[];
      isBlocked: boolean;
      blockedBy?: string;
    }[] = [];
    
    const recommendations: string[] = [];
    let blockedChains = 0;
    
    if (!vtable || vtable.initializers.length === 0) {
      // Try to find initializers by tracing backwards from known patterns
      recommendations.push('No direct initializers found. Try searching PPC code for stores to ' + vtableAddress);
      return {
        vtableAddress,
        initTraces: [],
        summary: { totalEntries: 0, initializedEntries: 0, blockedChains: 0 },
        recommendations,
      };
    }
    
    for (const initializer of vtable.initializers) {
      // Trace backwards to find what calls the initializer
      const initChain = this.traceCallersRecursive(initializer, new Set(), 10);
      const rootFunctions = this.findRootCallers(initializer);
      
      // Check if any function in the chain is stubbed
      let isBlocked = false;
      let blockedBy: string | undefined;
      
      for (const funcInChain of initChain) {
        const hook = this.index.hooks.get(funcInChain);
        if (hook && (hook.type === 'GUEST_FUNCTION_STUB' || hook.type === 'PPC_FUNC')) {
          // Check if it actually calls __imp__
          const func = this.index.functionsByName.get(funcInChain);
          if (func && !func.calls.includes('__imp__' + funcInChain)) {
            isBlocked = true;
            blockedBy = funcInChain;
            blockedChains++;
            break;
          }
        }
      }
      
      for (const entry of vtable.entries.filter(e => e.initializedBy === initializer)) {
        initTraces.push({
          entry: entry.offset / 4,
          funcPointer: entry.funcAddress,
          initializer,
          initChain,
          rootFunctions,
          isBlocked,
          blockedBy,
        });
      }
      
      if (isBlocked && blockedBy) {
        recommendations.push(
          `${blockedBy} is stubbed and blocks vtable init. Fix: modify to call __imp__${blockedBy}`
        );
      }
    }
    
    return {
      vtableAddress,
      initTraces,
      summary: {
        totalEntries: vtable.entryCount,
        initializedEntries: initTraces.length,
        blockedChains,
      },
      recommendations,
    };
  }
  
  private traceCallersRecursive(funcName: string, visited: Set<string>, maxDepth: number): string[] {
    if (visited.has(funcName) || maxDepth <= 0) return [];
    visited.add(funcName);
    
    const func = this.index.functionsByName.get(funcName);
    if (!func) return [funcName];
    
    const chain: string[] = [funcName];
    
    // Find callers
    for (const caller of func.calledBy.slice(0, 5)) {
      const callerChain = this.traceCallersRecursive(caller, visited, maxDepth - 1);
      if (callerChain.length > 0) {
        chain.push(...callerChain);
        break; // Just take one path for simplicity
      }
    }
    
    return chain;
  }
  
  private findRootCallers(funcName: string): string[] {
    const roots: string[] = [];
    const visited = new Set<string>();
    const queue = [funcName];
    
    while (queue.length > 0 && roots.length < 5) {
      const current = queue.shift()!;
      if (visited.has(current)) continue;
      visited.add(current);
      
      const func = this.index.functionsByName.get(current);
      if (!func) continue;
      
      if (func.calledBy.length === 0) {
        roots.push(current);
      } else {
        queue.push(...func.calledBy.slice(0, 3));
      }
    }
    
    return roots;
  }

  // ========== VTABLE CHAIN ANALYZER ==========
  analyzeVTableChain(funcName: string, vtableAddress: string): {
    function: string;
    vtableAddress: string;
    willInitialize: boolean;
    initPath: string[];
    directlyInitializes: boolean;
    initializesVia?: string;
    blockedBy?: string;
    recommendation: string;
  } {
    const vtable = this.inspectVTableEnhanced(vtableAddress);
    const func = this.index.functionsByName.get(funcName);
    
    if (!func) {
      return {
        function: funcName,
        vtableAddress,
        willInitialize: false,
        initPath: [],
        directlyInitializes: false,
        recommendation: 'Function not found in index',
      };
    }
    
    // Check if this function directly initializes the vtable
    const directlyInitializes = vtable?.initializers.includes(funcName) || false;
    
    if (directlyInitializes) {
      return {
        function: funcName,
        vtableAddress,
        willInitialize: true,
        initPath: [funcName],
        directlyInitializes: true,
        recommendation: 'Function directly initializes vtable',
      };
    }
    
    // Trace call tree to find path to vtable initializer
    const initPath = this.findPathToInitializer(funcName, vtable?.initializers || [], new Set(), 15);
    
    if (initPath.length === 0) {
      return {
        function: funcName,
        vtableAddress,
        willInitialize: false,
        initPath: [],
        directlyInitializes: false,
        recommendation: `${funcName} does not reach any vtable initializer. Check call graph.`,
      };
    }
    
    // Check if path is blocked by stubs
    let blockedBy: string | undefined;
    for (const funcInPath of initPath) {
      const hook = this.index.hooks.get(funcInPath);
      if (hook && (hook.type === 'GUEST_FUNCTION_STUB' || hook.type === 'PPC_FUNC')) {
        const f = this.index.functionsByName.get(funcInPath);
        // Check if the hook calls the original
        if (f && !f.calls.some(c => c.includes('__imp__'))) {
          blockedBy = funcInPath;
          break;
        }
      }
    }
    
    const initializesVia = initPath.length > 1 ? initPath[initPath.length - 1] : undefined;
    
    let recommendation: string;
    if (blockedBy) {
      recommendation = `Path blocked by stubbed ${blockedBy}. Fix: modify to call __imp__${blockedBy}`;
    } else {
      recommendation = `${funcName} will initialize vtable via: ${initPath.join(' -> ')}`;
    }
    
    return {
      function: funcName,
      vtableAddress,
      willInitialize: !blockedBy,
      initPath,
      directlyInitializes: false,
      initializesVia,
      blockedBy,
      recommendation,
    };
  }
  
  private findPathToInitializer(
    funcName: string,
    initializers: string[],
    visited: Set<string>,
    maxDepth: number
  ): string[] {
    if (visited.has(funcName) || maxDepth <= 0) return [];
    visited.add(funcName);
    
    // Check if we've reached an initializer
    if (initializers.includes(funcName)) {
      return [funcName];
    }
    
    const func = this.index.functionsByName.get(funcName);
    if (!func) return [];
    
    // Search callees (what this function calls)
    for (const callee of func.calls) {
      const path = this.findPathToInitializer(callee, initializers, visited, maxDepth - 1);
      if (path.length > 0) {
        return [funcName, ...path];
      }
    }
    
    return [];
  }

  // ========== FIND VTABLE USERS ==========
  findVTableUsers(vtableAddress: string): {
    vtableAddress: string;
    users: {
      function: string;
      address: string;
      accessType: string;
      offset?: number;
      context: string;
    }[];
    indirectCallSites: number;
    potentialCrashers: string[];
  } {
    const normalizedAddr = vtableAddress.toLowerCase().replace(/^0x/, '');
    const addrNum = parseInt(normalizedAddr, 16);
    
    const users: {
      function: string;
      address: string;
      accessType: string;
      offset?: number;
      context: string;
    }[] = [];
    const potentialCrashers: string[] = [];
    let indirectCallSites = 0;
    
    // Find functions that read from this vtable range
    for (const [, func] of this.index.functions) {
      for (const read of func.readsGlobals) {
        const readAddr = parseInt(read.address.replace(/^0x/, ''), 16);
        if (readAddr >= addrNum && readAddr < addrNum + 256) {
          const offset = readAddr - addrNum;
          const isIndirectCall = read.context.includes('PPC_CALL_INDIRECT') || 
                                  read.context.includes('ctx.ctr') ||
                                  read.context.includes('ctx.lr');
          
          users.push({
            function: func.name,
            address: func.address,
            accessType: isIndirectCall ? 'indirect_call' : 'read',
            offset,
            context: read.context.substring(0, 100),
          });
          
          if (isIndirectCall) {
            indirectCallSites++;
          }
        }
      }
    }
    
    // Check for potential crashers (functions that read vtable but vtable may not be initialized)
    const vtable = this.inspectVTableEnhanced(vtableAddress);
    if (vtable && vtable.initializers.length === 0) {
      potentialCrashers.push(...users.map(u => u.function));
    }
    
    return {
      vtableAddress: '0x' + normalizedAddr.toUpperCase(),
      users: users.slice(0, 50),
      indirectCallSites,
      potentialCrashers,
    };
  }

  // ========== SCAN FOR VTABLES ==========
  scanForVTables(region?: string): {
    vtables: {
      address: string;
      entryCount: number;
      initializers: string[];
      readers: string[];
      isInitialized: boolean;
    }[];
    summary: {
      totalVTables: number;
      uninitializedVTables: number;
      regionScanned: string;
    };
  } {
    const vtables: {
      address: string;
      entryCount: number;
      initializers: string[];
      readers: string[];
      isInitialized: boolean;
    }[] = [];
    
    // Determine scan range
    let startAddr = 0x82010000;
    let endAddr = 0x82020000;
    
    if (region === 'xex_data') {
      startAddr = 0x82020000;
      endAddr = 0x82120000;
    } else if (region === 'static_data') {
      startAddr = 0x83000000;
      endAddr = 0x831F0000;
    }
    
    // Collect potential vtable addresses from global writes
    const potentialVTables = new Set<string>();
    
    for (const [, func] of this.index.functions) {
      for (const write of func.writesGlobals) {
        const writeAddr = parseInt(write.address.replace(/^0x/, ''), 16);
        if (writeAddr >= startAddr && writeAddr < endAddr) {
          // Align to 4-byte boundary as vtable base
          const alignedAddr = Math.floor(writeAddr / 4) * 4;
          potentialVTables.add('0x' + alignedAddr.toString(16).toUpperCase());
        }
      }
    }
    
    // Analyze each potential vtable
    for (const addr of potentialVTables) {
      const vtable = this.inspectVTableEnhanced(addr);
      if (vtable && vtable.entryCount > 0) {
        vtables.push({
          address: addr,
          entryCount: vtable.entryCount,
          initializers: vtable.initializers,
          readers: vtable.readers,
          isInitialized: vtable.initializers.length > 0,
        });
      }
    }
    
    const uninitializedVTables = vtables.filter(v => !v.isInitialized).length;
    
    return {
      vtables: vtables.slice(0, 100),
      summary: {
        totalVTables: vtables.length,
        uninitializedVTables,
        regionScanned: `0x${startAddr.toString(16).toUpperCase()}-0x${endAddr.toString(16).toUpperCase()}`,
      },
    };
  }

  // ========== SYNC FLOW ANALYZER ==========
  // Map complete flow: create → wait → signal for each primitive address
  analyzeSyncFlow(address?: string): {
    primitives: {
      address: string;
      type: string;
      createdBy: string[];
      waitedOnBy: string[];
      signaledBy: string[];
      status: 'HEALTHY' | 'BROKEN_CHAIN' | 'NEVER_SIGNALED' | 'ORPHAN';
      issue?: string;
    }[];
    summary: {
      total: number;
      healthy: number;
      brokenChains: number;
      neverSignaled: number;
    };
  } {
    const primitives: {
      address: string;
      type: string;
      createdBy: string[];
      waitedOnBy: string[];
      signaledBy: string[];
      status: 'HEALTHY' | 'BROKEN_CHAIN' | 'NEVER_SIGNALED' | 'ORPHAN';
      issue?: string;
    }[] = [];

    // Build a map of all sync primitive interactions
    const syncMap = new Map<string, {
      type: string;
      creators: Set<string>;
      waiters: Set<string>;
      signalers: Set<string>;
    }>();

    // Scan all functions for sync primitive interactions
    for (const [, func] of this.index.functions) {
      // Check kernel API calls
      for (const api of func.kernelCalls) {
        // Creation
        if (SYNC_CREATE_APIS[api]) {
          // Look for the address in function's global writes
          for (const write of func.writesGlobals) {
            const addr = write.address.toLowerCase();
            if (!syncMap.has(addr)) {
              syncMap.set(addr, {
                type: SYNC_CREATE_APIS[api].type,
                creators: new Set(),
                waiters: new Set(),
                signalers: new Set(),
              });
            }
            syncMap.get(addr)!.creators.add(func.name);
          }
        }

        // Wait
        if (SYNC_WAIT_APIS.has(api)) {
          for (const read of func.readsGlobals) {
            const addr = read.address.toLowerCase();
            if (!syncMap.has(addr)) {
              syncMap.set(addr, {
                type: 'unknown',
                creators: new Set(),
                waiters: new Set(),
                signalers: new Set(),
              });
            }
            syncMap.get(addr)!.waiters.add(func.name);
          }
        }

        // Signal
        if (SYNC_SIGNAL_APIS[api]) {
          for (const write of func.writesGlobals) {
            const addr = write.address.toLowerCase();
            if (!syncMap.has(addr)) {
              syncMap.set(addr, {
                type: SYNC_SIGNAL_APIS[api],
                creators: new Set(),
                waiters: new Set(),
                signalers: new Set(),
              });
            }
            syncMap.get(addr)!.signalers.add(func.name);
          }
        }
      }
    }

    // Filter by address if provided
    const entries = address 
      ? [[address.toLowerCase(), syncMap.get(address.toLowerCase())]] as [string, typeof syncMap extends Map<string, infer V> ? V : never][]
      : Array.from(syncMap.entries());

    // Analyze each primitive
    for (const [addr, data] of entries) {
      if (!data) continue;

      const createdBy = Array.from(data.creators);
      const waitedOnBy = Array.from(data.waiters);
      const signaledBy = Array.from(data.signalers);

      // Check if any signaler is stubbed
      let isSignalerStubbed = false;
      let stubbedSignaler = '';
      for (const signaler of signaledBy) {
        const hook = this.index.hooks.get(signaler);
        if (hook && (hook.type === 'GUEST_FUNCTION_STUB' || hook.type === 'PPC_FUNC')) {
          const func = this.index.functionsByName.get(signaler);
          if (func && !func.calls.some(c => c.includes('__imp__'))) {
            isSignalerStubbed = true;
            stubbedSignaler = signaler;
            break;
          }
        }
      }

      // Determine status
      let status: 'HEALTHY' | 'BROKEN_CHAIN' | 'NEVER_SIGNALED' | 'ORPHAN' = 'HEALTHY';
      let issue: string | undefined;

      if (waitedOnBy.length > 0 && signaledBy.length === 0) {
        status = 'NEVER_SIGNALED';
        issue = `Waited on by ${waitedOnBy.join(', ')} but never signaled`;
      } else if (isSignalerStubbed) {
        status = 'BROKEN_CHAIN';
        issue = `Signaler ${stubbedSignaler} is stubbed - signal never happens`;
      } else if (createdBy.length === 0 && waitedOnBy.length === 0 && signaledBy.length === 0) {
        status = 'ORPHAN';
        issue = 'No interactions found';
      }

      primitives.push({
        address: '0x' + addr.replace(/^0x/, '').toUpperCase(),
        type: data.type,
        createdBy,
        waitedOnBy,
        signaledBy,
        status,
        issue,
      });
    }

    // Calculate summary
    const healthy = primitives.filter(p => p.status === 'HEALTHY').length;
    const brokenChains = primitives.filter(p => p.status === 'BROKEN_CHAIN').length;
    const neverSignaled = primitives.filter(p => p.status === 'NEVER_SIGNALED').length;

    return {
      primitives: primitives.slice(0, 100),
      summary: {
        total: primitives.length,
        healthy,
        brokenChains,
        neverSignaled,
      },
    };
  }

  // ========== FIND BROKEN SIGNAL CHAINS ==========
  findBrokenSignalChains(): {
    brokenChains: {
      primitiveAddr: string;
      type: string;
      waiters: string[];
      expectedSignaler: string;
      stubStatus: string;
      callChainToSignaler: string[];
      fix: string;
    }[];
    summary: {
      totalBroken: number;
      affectedWaiters: number;
      fixableByUnstubbing: number;
    };
  } {
    const brokenChains: {
      primitiveAddr: string;
      type: string;
      waiters: string[];
      expectedSignaler: string;
      stubStatus: string;
      callChainToSignaler: string[];
      fix: string;
    }[] = [];

    const syncFlow = this.analyzeSyncFlow();
    
    for (const prim of syncFlow.primitives) {
      if (prim.status === 'BROKEN_CHAIN' || prim.status === 'NEVER_SIGNALED') {
        // Find expected signaler
        let expectedSignaler = prim.signaledBy[0] || 'unknown';
        let stubStatus = 'not_stubbed';
        let fix = '';

        if (prim.signaledBy.length > 0) {
          for (const signaler of prim.signaledBy) {
            const hook = this.index.hooks.get(signaler);
            if (hook) {
              stubStatus = `stubbed (${hook.type})`;
              fix = `Modify ${signaler} to call __imp__${signaler} before returning`;
              expectedSignaler = signaler;
              break;
            }
          }
        } else {
          stubStatus = 'no_signaler_found';
          fix = 'Find what function should signal this primitive and implement it';
        }

        // Find call chain to signaler
        const callChain: string[] = [];
        if (prim.waitedOnBy.length > 0 && expectedSignaler !== 'unknown') {
          // Simple trace from waiter to signaler through call graph
          const waiter = prim.waitedOnBy[0];
          callChain.push(waiter);
          // This is simplified - real implementation would do BFS/DFS
        }

        brokenChains.push({
          primitiveAddr: prim.address,
          type: prim.type,
          waiters: prim.waitedOnBy,
          expectedSignaler,
          stubStatus,
          callChainToSignaler: callChain,
          fix,
        });
      }
    }

    const affectedWaiters = new Set(brokenChains.flatMap(b => b.waiters)).size;
    const fixableByUnstubbing = brokenChains.filter(b => b.stubStatus.includes('stubbed')).length;

    return {
      brokenChains,
      summary: {
        totalBroken: brokenChains.length,
        affectedWaiters,
        fixableByUnstubbing,
      },
    };
  }

  // ========== SYNC PRIMITIVE INVENTORY ==========
  getSyncPrimitiveInventory(): {
    events: { address: string; creator: string; waiters: string[]; signalers: string[]; status: string }[];
    semaphores: { address: string; creator: string; waiters: string[]; signalers: string[]; status: string }[];
    mutexes: { address: string; creator: string; waiters: string[]; signalers: string[]; status: string }[];
    spinlocks: { address: string; creator: string; acquirers: string[]; releasers: string[] }[];
    summary: {
      totalEvents: number;
      totalSemaphores: number;
      totalMutexes: number;
      totalSpinlocks: number;
      problematicCount: number;
    };
  } {
    const events: { address: string; creator: string; waiters: string[]; signalers: string[]; status: string }[] = [];
    const semaphores: { address: string; creator: string; waiters: string[]; signalers: string[]; status: string }[] = [];
    const mutexes: { address: string; creator: string; waiters: string[]; signalers: string[]; status: string }[] = [];
    const spinlocks: { address: string; creator: string; acquirers: string[]; releasers: string[] }[] = [];

    const syncFlow = this.analyzeSyncFlow();

    for (const prim of syncFlow.primitives) {
      const entry = {
        address: prim.address,
        creator: prim.createdBy[0] || 'unknown',
        waiters: prim.waitedOnBy,
        signalers: prim.signaledBy,
        status: prim.status,
      };

      switch (prim.type) {
        case 'event':
          events.push(entry);
          break;
        case 'semaphore':
          semaphores.push(entry);
          break;
        case 'mutex':
          mutexes.push(entry);
          break;
        case 'spinlock':
          spinlocks.push({
            address: prim.address,
            creator: prim.createdBy[0] || 'unknown',
            acquirers: prim.waitedOnBy,
            releasers: prim.signaledBy,
          });
          break;
      }
    }

    const problematicCount = syncFlow.primitives.filter(
      p => p.status !== 'HEALTHY'
    ).length;

    return {
      events,
      semaphores,
      mutexes,
      spinlocks,
      summary: {
        totalEvents: events.length,
        totalSemaphores: semaphores.length,
        totalMutexes: mutexes.length,
        totalSpinlocks: spinlocks.length,
        problematicCount,
      },
    };
  }

  // ========== THREAD SYNC MAP ==========
  getThreadSyncMap(): {
    threads: {
      entryPoint: string;
      address: string;
      waitsOn: { address: string; type: string }[];
      signals: { address: string; type: string }[];
      creates: { address: string; type: string }[];
    }[];
    interactions: {
      from: string;
      to: string;
      via: string;
      type: string;
    }[];
  } {
    const threads: {
      entryPoint: string;
      address: string;
      waitsOn: { address: string; type: string }[];
      signals: { address: string; type: string }[];
      creates: { address: string; type: string }[];
    }[] = [];

    const interactions: {
      from: string;
      to: string;
      via: string;
      type: string;
    }[] = [];

    // Find thread entry points (functions passed to ExCreateThread)
    const threadEntries = new Set<string>();
    for (const [, func] of this.index.functions) {
      if (func.kernelCalls.includes('ExCreateThread')) {
        // The thread entry is typically passed in r4 or r5
        // For now, mark functions that create threads
        threadEntries.add(func.name);
      }
    }

    // Analyze each potential thread entry
    for (const entryName of threadEntries) {
      const func = this.index.functionsByName.get(entryName);
      if (!func) continue;

      const waitsOn: { address: string; type: string }[] = [];
      const signals: { address: string; type: string }[] = [];
      const creates: { address: string; type: string }[] = [];

      // Trace what this thread does with sync primitives
      const visited = new Set<string>();
      const toVisit = [entryName];

      while (toVisit.length > 0 && visited.size < 50) {
        const current = toVisit.shift()!;
        if (visited.has(current)) continue;
        visited.add(current);

        const f = this.index.functionsByName.get(current);
        if (!f) continue;

        for (const api of f.kernelCalls) {
          if (SYNC_CREATE_APIS[api]) {
            for (const w of f.writesGlobals) {
              creates.push({ address: w.address, type: SYNC_CREATE_APIS[api].type });
            }
          }
          if (SYNC_WAIT_APIS.has(api)) {
            for (const r of f.readsGlobals) {
              waitsOn.push({ address: r.address, type: 'sync_object' });
            }
          }
          if (SYNC_SIGNAL_APIS[api]) {
            for (const w of f.writesGlobals) {
              signals.push({ address: w.address, type: SYNC_SIGNAL_APIS[api] });
            }
          }
        }

        // Add callees to visit
        toVisit.push(...f.calls.slice(0, 10));
      }

      threads.push({
        entryPoint: entryName,
        address: func.address,
        waitsOn,
        signals,
        creates,
      });
    }

    // Build interaction graph
    for (const t1 of threads) {
      for (const t2 of threads) {
        if (t1.entryPoint === t2.entryPoint) continue;
        
        // Check if t1 signals something t2 waits on
        for (const sig of t1.signals) {
          for (const wait of t2.waitsOn) {
            if (sig.address === wait.address) {
              interactions.push({
                from: t1.entryPoint,
                to: t2.entryPoint,
                via: sig.address,
                type: 'signal_wait',
              });
            }
          }
        }
      }
    }

    return { threads: threads.slice(0, 50), interactions };
  }

  // ========== STUBBED SIGNAL DETECTOR ==========
  findStubbedSignalers(): {
    stubbedSignalers: {
      function: string;
      address: string;
      hookType: string;
      primitivesAffected: string[];
      waitersBlocked: string[];
      fix: string;
    }[];
    summary: {
      totalStubbedSignalers: number;
      totalPrimitivesAffected: number;
      totalWaitersBlocked: number;
    };
  } {
    const stubbedSignalers: {
      function: string;
      address: string;
      hookType: string;
      primitivesAffected: string[];
      waitersBlocked: string[];
      fix: string;
    }[] = [];

    const syncFlow = this.analyzeSyncFlow();
    const signalerToPrimitives = new Map<string, string[]>();
    const primitiveToWaiters = new Map<string, string[]>();

    // Build maps
    for (const prim of syncFlow.primitives) {
      for (const signaler of prim.signaledBy) {
        if (!signalerToPrimitives.has(signaler)) {
          signalerToPrimitives.set(signaler, []);
        }
        signalerToPrimitives.get(signaler)!.push(prim.address);
      }
      primitiveToWaiters.set(prim.address, prim.waitedOnBy);
    }

    // Find stubbed signalers
    for (const [signaler, primitives] of signalerToPrimitives) {
      const hook = this.index.hooks.get(signaler);
      if (!hook) continue;

      const func = this.index.functionsByName.get(signaler);
      if (!func) continue;

      // Check if stub actually calls original
      const callsOriginal = func.calls.some(c => c.includes('__imp__'));
      if (callsOriginal) continue;

      // Collect affected waiters
      const waitersBlocked = new Set<string>();
      for (const prim of primitives) {
        const waiters = primitiveToWaiters.get(prim) || [];
        waiters.forEach(w => waitersBlocked.add(w));
      }

      stubbedSignalers.push({
        function: signaler,
        address: func.address,
        hookType: hook.type,
        primitivesAffected: primitives,
        waitersBlocked: Array.from(waitersBlocked),
        fix: `Modify ${signaler} to call __imp__${signaler} or signal primitives: ${primitives.join(', ')}`,
      });
    }

    const totalPrimitivesAffected = new Set(stubbedSignalers.flatMap(s => s.primitivesAffected)).size;
    const totalWaitersBlocked = new Set(stubbedSignalers.flatMap(s => s.waitersBlocked)).size;

    return {
      stubbedSignalers,
      summary: {
        totalStubbedSignalers: stubbedSignalers.length,
        totalPrimitivesAffected,
        totalWaitersBlocked,
      },
    };
  }

  // ========== XENIA REFACTOR CHECKLIST ==========
  generateXeniaRefactorChecklist(): {
    checklist: {
      category: string;
      items: {
        task: string;
        priority: 'HIGH' | 'MEDIUM' | 'LOW';
        complexity: string;
        files: string[];
        notes: string;
      }[];
    }[];
    xeniaFiles: {
      path: string;
      purpose: string;
      relevant: boolean;
    }[];
    currentState: {
      hasObjectTable: boolean;
      hasProperBacking: boolean;
      trackedPrimitives: number;
      brokenChains: number;
    };
  } {
    const syncFlow = this.analyzeSyncFlow();
    const brokenChains = this.findBrokenSignalChains();
    const stubbedSignalers = this.findStubbedSignalers();

    const checklist: {
      category: string;
      items: {
        task: string;
        priority: 'HIGH' | 'MEDIUM' | 'LOW';
        complexity: string;
        files: string[];
        notes: string;
      }[];
    }[] = [
      {
        category: '1. Object Table Infrastructure',
        items: [
          {
            task: 'Create XObject base class hierarchy (XEvent, XSemaphore, XMutant)',
            priority: 'HIGH',
            complexity: 'Medium - port from Xenia',
            files: ['kernel/xobject.h', 'kernel/xobject.cpp', 'kernel/xevent.h', 'kernel/xsemaphore.h'],
            notes: 'Base class should handle handle management, retain/release',
          },
          {
            task: 'Create ObjectTable class for handle management',
            priority: 'HIGH',
            complexity: 'Medium - port from Xenia',
            files: ['kernel/util/object_table.h', 'kernel/util/object_table.cpp'],
            notes: 'Maps handles to XObject*, handles lifetime',
          },
          {
            task: 'Create host-backed Event class wrapping std::condition_variable or platform event',
            priority: 'HIGH',
            complexity: 'Low - straightforward threading',
            files: ['kernel/xevent.cpp'],
            notes: 'Must support manual-reset and auto-reset modes',
          },
          {
            task: 'Create host-backed Semaphore class',
            priority: 'HIGH',
            complexity: 'Low',
            files: ['kernel/xsemaphore.cpp'],
            notes: 'counting semaphore with Release(count) support',
          },
        ],
      },
      {
        category: '2. Kernel API Rewrites',
        items: [
          {
            task: 'Rewrite KeInitializeEvent to create XEvent in object table',
            priority: 'HIGH',
            complexity: 'Low',
            files: ['kernel/imports.cpp'],
            notes: 'Replace current impl that just sets header fields',
          },
          {
            task: 'Rewrite KeInitializeSemaphore to create XSemaphore in object table',
            priority: 'HIGH',
            complexity: 'Low',
            files: ['kernel/imports.cpp'],
            notes: 'Replace current QueryKernelObject approach',
          },
          {
            task: 'Rewrite KeWaitForSingleObject to use object table lookup + host wait',
            priority: 'HIGH',
            complexity: 'Medium',
            files: ['kernel/imports.cpp'],
            notes: 'Must handle timeout, alertable waits',
          },
          {
            task: 'Rewrite KeSetEvent/KePulseEvent to signal host event',
            priority: 'HIGH',
            complexity: 'Low',
            files: ['kernel/imports.cpp'],
            notes: 'Simple delegation to XEvent::Set()',
          },
          {
            task: 'Rewrite KeReleaseSemaphore to release host semaphore',
            priority: 'HIGH',
            complexity: 'Low',
            files: ['kernel/imports.cpp'],
            notes: 'Simple delegation to XSemaphore::Release()',
          },
        ],
      },
      {
        category: '3. Fix Broken Signal Chains',
        items: brokenChains.brokenChains.slice(0, 10).map(chain => ({
          task: `Fix ${chain.expectedSignaler} to signal ${chain.primitiveAddr}`,
          priority: 'MEDIUM' as const,
          complexity: 'Low - unstub or add signal call',
          files: ['kernel/imports.cpp'],
          notes: chain.fix,
        })),
      },
      {
        category: '4. Unstub Signaling Functions',
        items: stubbedSignalers.stubbedSignalers.slice(0, 10).map(s => ({
          task: `Unstub ${s.function} - affects ${s.primitivesAffected.length} primitives`,
          priority: 'MEDIUM' as const,
          complexity: 'Low - modify to call __imp__',
          files: ['kernel/imports.cpp'],
          notes: `Blocks: ${s.waitersBlocked.join(', ')}`,
        })),
      },
    ];

    const xeniaFiles = [
      { path: 'xenia/kernel/xobject.h', purpose: 'Base object class with handle management', relevant: true },
      { path: 'xenia/kernel/xobject.cc', purpose: 'Object lifecycle, retain/release', relevant: true },
      { path: 'xenia/kernel/xevent.h', purpose: 'Event object definition', relevant: true },
      { path: 'xenia/kernel/xevent.cc', purpose: 'Host-backed event implementation', relevant: true },
      { path: 'xenia/kernel/xsemaphore.h', purpose: 'Semaphore object definition', relevant: true },
      { path: 'xenia/kernel/xsemaphore.cc', purpose: 'Host-backed semaphore implementation', relevant: true },
      { path: 'xenia/kernel/util/object_table.h', purpose: 'Handle → Object mapping', relevant: true },
      { path: 'xenia/kernel/util/object_table.cc', purpose: 'Object table implementation', relevant: true },
      { path: 'xenia/base/threading.h', purpose: 'Platform-agnostic threading primitives', relevant: true },
    ];

    return {
      checklist,
      xeniaFiles,
      currentState: {
        hasObjectTable: false,
        hasProperBacking: false,
        trackedPrimitives: syncFlow.primitives.length,
        brokenChains: brokenChains.brokenChains.length,
      },
    };
  }
}
