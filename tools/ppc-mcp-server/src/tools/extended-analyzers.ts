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
}
