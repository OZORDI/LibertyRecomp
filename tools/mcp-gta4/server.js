#!/usr/bin/env node
// liberty-decomp MCP Server
// Provides GTA IV decompilation analysis tools for LibertyRecomp
//
// Data sources:
//   gta_iv/gta4_functions_enriched.txt   — function list with class/vtable/string hints
//   gta_iv/gta4_vtables_enriched.txt     — vtable list with class names
//   gta_iv/xex_excavation_retail/        — call graph, callers, string refs, pseudocode, FLIRT
//   LibertyRecomp/ + LibertyRecompLib/   — hook status (GUEST_FUNCTION_HOOK, PPC_FUNC_IMPL)

const { Server } = require("@modelcontextprotocol/sdk/server/index.js");
const { StdioServerTransport } = require("@modelcontextprotocol/sdk/server/stdio.js");
const {
  CallToolRequestSchema,
  ListToolsRequestSchema,
} = require("@modelcontextprotocol/sdk/types.js");
const fs = require("fs");
const path = require("path");

// ---------------------------------------------------------------------------
// Paths
// ---------------------------------------------------------------------------
const ROOT = path.resolve(__dirname, "../..");
const GTA_IV = path.join(ROOT, "gta_iv");
const XEX_RETAIL = path.join(GTA_IV, "xex_excavation_retail");
const PSEUDOCODE_DIR = path.join(XEX_RETAIL, "pseudocode");
const LIBERTY_RECOMP = path.join(ROOT, "LibertyRecomp");
const LIBERTY_RECOMP_LIB = path.join(ROOT, "LibertyRecompLib");

// ---------------------------------------------------------------------------
// Data stores (populated at startup)
// ---------------------------------------------------------------------------
const functions = new Map();        // name -> {address, name, className, slot, stringHint}
const functionsByAddr = new Map();  // address -> name
const vtables = new Map();          // className -> {address, entryCount, className, entries:[]}
const callGraph = new Map();        // caller -> Set<callee>
const reverseCallGraph = new Map(); // callee -> Set<caller>
const funcStringRefs = new Map();   // funcName -> [strings]
const hotFunctions = new Map();     // funcName -> callCount
const hookStatus = new Map();       // funcName -> {type, file, line}
const flirtTrusted = new Map();     // address -> demangledName
const flirtSuspect = new Map();     // address -> demangledName
const pseudocodeIndex = new Set();  // set of funcNames that have pseudocode

// ---------------------------------------------------------------------------
// Parsers
// ---------------------------------------------------------------------------

function parseEnrichedFunctions() {
  const filePath = path.join(GTA_IV, "gta4_functions_enriched.txt");
  if (!fs.existsSync(filePath)) return;
  const lines = fs.readFileSync(filePath, "utf-8").split("\n");
  for (const line of lines) {
    if (line.startsWith("#") || !line.trim()) continue;
    // Format: address,name,class,slot,string_hint
    const parts = line.split(",");
    if (parts.length < 2) continue;
    const address = parts[0].trim();
    const name = parts[1].trim();
    const className = parts.length > 2 ? parts[2].trim() : "";
    const slot = parts.length > 3 ? parseInt(parts[3].trim(), 10) : -1;
    const stringHint = parts.length > 4 ? parts.slice(4).join(",").trim() : "";
    functions.set(name, { address, name, className, slot, stringHint });
    functionsByAddr.set(address.toLowerCase(), name);
  }
  console.error(`Loaded ${functions.size} functions`);
}

function parseEnrichedVtables() {
  const filePath = path.join(GTA_IV, "gta4_vtables_enriched.txt");
  if (!fs.existsSync(filePath)) return;
  const lines = fs.readFileSync(filePath, "utf-8").split("\n");
  for (const line of lines) {
    if (line.startsWith("#") || !line.trim()) continue;
    // Format: vtable_address,entry_count,class_name
    const parts = line.split(",");
    if (parts.length < 3) continue;
    const address = parts[0].trim();
    const entryCount = parseInt(parts[1].trim(), 10);
    const className = parts[2].trim();
    vtables.set(className, { address, entryCount, className });
  }
  console.error(`Loaded ${vtables.size} vtables`);
}

function parseCallGraph() {
  const filePath = path.join(XEX_RETAIL, "call_graph.txt");
  if (!fs.existsSync(filePath)) return;
  const lines = fs.readFileSync(filePath, "utf-8").split("\n");
  let currentFunc = null;
  for (const line of lines) {
    if (line.startsWith(";") || !line.trim()) continue;
    if (line.startsWith("  -> ")) {
      if (currentFunc) {
        const callee = line.replace("  -> ", "").trim();
        if (!callGraph.has(currentFunc)) callGraph.set(currentFunc, new Set());
        callGraph.get(currentFunc).add(callee);
        if (!reverseCallGraph.has(callee)) reverseCallGraph.set(callee, new Set());
        reverseCallGraph.get(callee).add(currentFunc);
      }
    } else {
      // e.g. "start  0x82A110A8  [13 calls]" or "sub_82140748  0x82140748  [5 calls]"
      const match = line.match(/^(\S+)\s+0x[0-9a-fA-F]+\s+\[\d+ calls?\]/);
      if (match) currentFunc = match[1];
    }
  }
  console.error(`Loaded call graph: ${callGraph.size} callers`);
}

function parseFuncStringRefs() {
  const filePath = path.join(XEX_RETAIL, "func_string_refs.txt");
  if (!fs.existsSync(filePath)) return;
  const lines = fs.readFileSync(filePath, "utf-8").split("\n");
  let currentFunc = null;
  for (const line of lines) {
    if (line.startsWith(";") || (!line.trim() && !currentFunc)) continue;
    if (!line.trim()) { currentFunc = null; continue; }
    const headerMatch = line.match(/^(\S+)\s+\[\d+ strings?\]/);
    if (headerMatch) {
      currentFunc = headerMatch[1];
      funcStringRefs.set(currentFunc, []);
    } else if (currentFunc && line.startsWith('  "')) {
      const str = line.trim().replace(/^"|"$/g, "");
      funcStringRefs.get(currentFunc).push(str);
    } else if (currentFunc && line.startsWith("  ")) {
      funcStringRefs.get(currentFunc).push(line.trim());
    }
  }
  console.error(`Loaded string refs for ${funcStringRefs.size} functions`);
}

function parseHottestFunctions() {
  const filePath = path.join(XEX_RETAIL, "hottest_functions.txt");
  if (!fs.existsSync(filePath)) return;
  const lines = fs.readFileSync(filePath, "utf-8").split("\n");
  for (const line of lines) {
    if (line.startsWith(";") || !line.trim()) continue;
    // Format:   1234  0x82XXXXXX  sub_XXXXXXXX
    const match = line.match(/^\s*(\d+)\s+0x[0-9a-fA-F]+\s+(\S+)/);
    if (match) {
      hotFunctions.set(match[2], parseInt(match[1], 10));
    }
  }
  console.error(`Loaded ${hotFunctions.size} hot functions`);
}

function parseFunctionsWithAddrs() {
  const filePath = path.join(XEX_RETAIL, "functions_with_addrs.txt");
  if (!fs.existsSync(filePath)) return;
  const lines = fs.readFileSync(filePath, "utf-8").split("\n");
  for (const line of lines) {
    if (line.startsWith(";") || !line.trim()) continue;
    // Format: 0x82140000       1x  sub_82140000
    const match = line.match(/^\s*(0x[0-9a-fA-F]+)\s+(\d+)x\s+(\S+)/);
    if (match) {
      const addr = match[1];
      const calls = parseInt(match[2], 10);
      const name = match[3];
      if (!hotFunctions.has(name)) hotFunctions.set(name, calls);
      if (!functions.has(name)) {
        functions.set(name, { address: addr, name, className: "", slot: -1, stringHint: "" });
      }
      if (!functionsByAddr.has(addr.toLowerCase())) {
        functionsByAddr.set(addr.toLowerCase(), name);
      }
    }
  }
}

function parseHookStatus() {
  hookStatus.clear(); // rescan is authoritative each time; drop stale entries
  const dirs = [LIBERTY_RECOMP, LIBERTY_RECOMP_LIB];
  const patterns = [
    /GUEST_FUNCTION_HOOK\s*\(\s*(sub_[0-9a-fA-F]+)/g,
    /PPC_FUNC_IMPL\s*\(\s*__imp__(sub_[0-9a-fA-F]+)/g,
    /PPC_FUNC_IMPL\s*\(\s*(sub_[0-9a-fA-F]+)/g,
  ];
  for (const dir of dirs) {
    if (!fs.existsSync(dir)) continue;
    const files = walkCppFiles(dir);
    for (const file of files) {
      const content = fs.readFileSync(file, "utf-8");
      const lines = content.split("\n");
      for (let i = 0; i < lines.length; i++) {
        const line = lines[i];
        // Skip commented-out hooks
        if (line.trimStart().startsWith("//")) continue;
        for (const pat of patterns) {
          pat.lastIndex = 0;
          let m;
          while ((m = pat.exec(line)) !== null) {
            const funcName = m[1];
            const hookType = line.includes("GUEST_FUNCTION_HOOK") ? "GUEST_FUNCTION_HOOK" : "PPC_FUNC_IMPL";
            hookStatus.set(funcName, { type: hookType, file: path.relative(ROOT, file), line: i + 1 });
          }
        }
      }
    }
  }
  
}

function walkCppFiles(dir) {
  const results = [];
  try {
    const entries = fs.readdirSync(dir, { withFileTypes: true });
    for (const entry of entries) {
      const fullPath = path.join(dir, entry.name);
      if (entry.isDirectory() && !entry.name.startsWith(".") && entry.name !== "node_modules" && entry.name !== "build") {
        results.push(...walkCppFiles(fullPath));
      } else if (entry.isFile() && (entry.name.endsWith(".cpp") || entry.name.endsWith(".h"))) {
        results.push(fullPath);
      }
    }
  } catch {}
  return results;
}

function parseFlirt() {
  const trustedPath = path.join(XEX_RETAIL, "flirt_labeled_trusted.txt");
  const suspectPath = path.join(XEX_RETAIL, "flirt_labeled_suspect.txt");
  function loadFlirt(filePath, map) {
    if (!fs.existsSync(filePath)) return;
    const lines = fs.readFileSync(filePath, "utf-8").split("\n");
    for (const line of lines) {
      if (!line.trim() || line.startsWith(";") || line.startsWith("#")) continue;
      // Format: 0x821778a0\tLIB\t?GetBufferPointer@...
      const parts = line.split("\t");
      if (parts.length >= 3) {
        map.set(parts[0].trim().toLowerCase(), parts[2].trim());
      }
    }
  }
  loadFlirt(trustedPath, flirtTrusted);
  loadFlirt(suspectPath, flirtSuspect);
  console.error(`Loaded FLIRT: ${flirtTrusted.size} trusted, ${flirtSuspect.size} suspect`);
}

function buildPseudocodeIndex() {
  if (!fs.existsSync(PSEUDOCODE_DIR)) return;
  try {
    const files = fs.readdirSync(PSEUDOCODE_DIR);
    for (const f of files) {
      if (f.endsWith(".c")) {
        // e.g. sub_82140748_0x82140748.c -> sub_82140748
        const match = f.match(/^(\S+?)_0x[0-9a-fA-F]+\.c$/);
        if (match) pseudocodeIndex.add(match[1]);
      }
    }
  } catch {}
  console.error(`Pseudocode available for ${pseudocodeIndex.size} functions`);
}

function parseVtablesDetailed() {
  const filePath = path.join(XEX_RETAIL, "vtables_with_addrs.txt");
  if (!fs.existsSync(filePath)) return;
  const lines = fs.readFileSync(filePath, "utf-8").split("\n");
  let currentClass = null;
  let entries = [];
  for (const line of lines) {
    if (line.startsWith(";") || (!line.trim() && !currentClass)) continue;
    if (!line.trim() && currentClass) {
      if (vtables.has(currentClass)) {
        vtables.get(currentClass).entries = entries;
      }
      currentClass = null;
      entries = [];
      continue;
    }
    // Header: 0x82000974  CBaseDC  [6 vfuncs]
    const headerMatch = line.match(/^(0x[0-9a-fA-F]+)\s+(\S+)\s+\[(\d+) vfuncs?\]/);
    if (headerMatch) {
      currentClass = headerMatch[2];
      if (!vtables.has(currentClass)) {
        vtables.set(currentClass, { address: headerMatch[1], entryCount: parseInt(headerMatch[3], 10), className: currentClass });
      }
      entries = [];
      continue;
    }
    // Entry:   0x82000974  [  0]  sub_821BB5F0    0x821BB5F0
    if (currentClass) {
      const entryMatch = line.match(/^\s*(0x[0-9a-fA-F]+)\s+\[\s*(\d+)\]\s+(\S+)\s+(0x[0-9a-fA-F]+)/);
      if (entryMatch) {
        entries.push({ vtableAddr: entryMatch[1], slot: parseInt(entryMatch[2], 10), funcName: entryMatch[3], funcAddr: entryMatch[4] });
      }
    }
  }
  if (currentClass && vtables.has(currentClass)) {
    vtables.get(currentClass).entries = entries;
  }
}

// ---------------------------------------------------------------------------
// Tool implementations
// ---------------------------------------------------------------------------

function getFunctionInfo(funcName) {
  const func = functions.get(funcName);
  if (!func) {
    // Try by address
    const byAddr = functionsByAddr.get(funcName.toLowerCase());
    if (byAddr) return getFunctionInfo(byAddr);
    return { error: `Function '${funcName}' not found` };
  }

  const callees = callGraph.get(funcName);
  const callers = reverseCallGraph.get(funcName);
  const strings = funcStringRefs.get(funcName);
  const callCount = hotFunctions.get(funcName);
  const hook = hookStatus.get(funcName);
  const hasPseudocode = pseudocodeIndex.has(funcName);

  // Check FLIRT
  let flirtName = null;
  const addrLower = func.address.toLowerCase();
  if (flirtTrusted.has(addrLower)) {
    flirtName = { name: flirtTrusted.get(addrLower), confidence: "trusted" };
  } else if (flirtSuspect.has(addrLower)) {
    flirtName = { name: flirtSuspect.get(addrLower), confidence: "suspect" };
  }

  // Class from vtable
  let vtableInfo = null;
  if (func.className) {
    const vt = vtables.get(func.className);
    if (vt) {
      vtableInfo = { className: func.className, vtableAddress: vt.address, slot: func.slot };
    }
  }

  return {
    name: func.name,
    address: func.address,
    className: func.className || null,
    vtable: vtableInfo,
    stringHint: func.stringHint || null,
    callCount: callCount || 0,
    hasPseudocode,
    hookStatus: hook || null,
    flirtMatch: flirtName,
    calleesCount: callees ? callees.size : 0,
    calleesSummary: callees ? Array.from(callees).slice(0, 20).join(", ") + (callees.size > 20 ? ` ... (+${callees.size - 20} more)` : "") : "none",
    callersCount: callers ? callers.size : 0,
    callersSummary: callers ? Array.from(callers).slice(0, 10).join(", ") + (callers.size > 10 ? ` ... (+${callers.size - 10} more)` : "") : "none",
    stringRefsCount: strings ? strings.length : 0,
    stringRefsSummary: strings ? strings.slice(0, 10).join("; ") + (strings.length > 10 ? ` ... (+${strings.length - 10} more)` : "") : "none",
  };
}

function getFunctionPseudocode(funcName) {
  const func = functions.get(funcName);
  if (!func) return { error: `Function '${funcName}' not found` };
  if (!pseudocodeIndex.has(funcName)) return { error: `No pseudocode available for '${funcName}'` };

  const filename = `${funcName}_${func.address}.c`;
  const filePath = path.join(PSEUDOCODE_DIR, filename);
  if (!fs.existsSync(filePath)) {
    // Try glob-style match
    try {
      const files = fs.readdirSync(PSEUDOCODE_DIR).filter(f => f.startsWith(funcName + "_"));
      if (files.length > 0) {
        return { pseudocode: fs.readFileSync(path.join(PSEUDOCODE_DIR, files[0]), "utf-8") };
      }
    } catch {}
    return { error: `Pseudocode file not found for '${funcName}'` };
  }
  return { pseudocode: fs.readFileSync(filePath, "utf-8") };
}

function getStringRefs(funcName, searchString) {
  if (funcName) {
    const strings = funcStringRefs.get(funcName);
    if (!strings) return { error: `No string refs for '${funcName}'` };
    if (searchString) {
      const filtered = strings.filter(s => s.toLowerCase().includes(searchString.toLowerCase()));
      return { function: funcName, matchingStrings: filtered, totalStrings: strings.length };
    }
    return { function: funcName, strings };
  }
  if (searchString) {
    const results = [];
    for (const [fn, strings] of funcStringRefs) {
      const matches = strings.filter(s => s.toLowerCase().includes(searchString.toLowerCase()));
      if (matches.length > 0) {
        results.push({ function: fn, matchingStrings: matches });
      }
    }
    return { searchString, totalFunctions: results.length, results: results.slice(0, 50) };
  }
  return { error: "Provide function_name or search_string" };
}

function searchFunctions(pattern, limit) {
  limit = limit || 50;
  const results = [];
  let regex;
  try {
    regex = new RegExp(pattern, "i");
  } catch {
    return { error: `Invalid regex: ${pattern}` };
  }
  for (const [name, func] of functions) {
    if (regex.test(name) || (func.className && regex.test(func.className)) || (func.stringHint && regex.test(func.stringHint))) {
      results.push({
        name: func.name,
        address: func.address,
        className: func.className || null,
        stringHint: func.stringHint || null,
        hooked: hookStatus.has(func.name),
        hasPseudocode: pseudocodeIndex.has(func.name),
      });
      if (results.length >= limit) break;
    }
  }
  return { pattern, totalResults: results.length, results };
}

function getFlirtApi(searchPattern, trustedOnly, limit) {
  trustedOnly = trustedOnly !== false;
  limit = limit || 50;
  const results = [];
  let regex;
  try {
    regex = new RegExp(searchPattern, "i");
  } catch {
    return { error: `Invalid regex: ${searchPattern}` };
  }

  function searchMap(map, confidence) {
    for (const [addr, name] of map) {
      if (regex.test(name)) {
        const funcName = functionsByAddr.get(addr);
        results.push({
          address: addr,
          flirtName: name,
          functionName: funcName || null,
          confidence,
          hooked: funcName ? hookStatus.has(funcName) : false,
        });
        if (results.length >= limit) return;
      }
    }
  }

  searchMap(flirtTrusted, "trusted");
  if (!trustedOnly && results.length < limit) {
    searchMap(flirtSuspect, "suspect");
  }

  return { searchPattern, trustedOnly, totalResults: results.length, results };
}

function getCallees(funcName) {
  const callees = callGraph.get(funcName);
  if (!callees) return { error: `No callee data for '${funcName}'` };
  return {
    function: funcName,
    calleeCount: callees.size,
    callees: Array.from(callees).map(c => ({
      name: c,
      hooked: hookStatus.has(c),
      hasPseudocode: pseudocodeIndex.has(c),
    })),
  };
}

function getCallers(funcName) {
  const callers = reverseCallGraph.get(funcName);
  if (!callers) return { error: `No caller data for '${funcName}'` };
  return {
    function: funcName,
    callerCount: callers.size,
    callers: Array.from(callers).map(c => ({
      name: c,
      hooked: hookStatus.has(c),
      hasPseudocode: pseudocodeIndex.has(c),
    })),
  };
}

function getVtableInfo(className) {
  const vt = vtables.get(className);
  if (!vt) {
    // Try partial match
    const matches = [];
    for (const [name, v] of vtables) {
      if (name.toLowerCase().includes(className.toLowerCase())) {
        matches.push(v);
      }
    }
    if (matches.length === 0) return { error: `Vtable '${className}' not found` };
    if (matches.length === 1) return matches[0];
    return { matches: matches.map(m => ({ className: m.className, address: m.address, entryCount: m.entryCount })) };
  }
  return vt;
}

function searchStrings(searchText) {
  return getStringRefs(null, searchText);
}

// ---------------------------------------------------------------------------
// Tool definitions
// ---------------------------------------------------------------------------

const TOOLS = [
  {
    name: "get_function_info",
    description: "Get metadata: address, estimated size, call count, class assignment (from vtable), vtable slot, pseudocode availability, hook status, string refs, callees summary. Call first to understand any function before deeper analysis.",
    inputSchema: {
      type: "object",
      properties: {
        function_name: { type: "string", description: "Function name (e.g. sub_82140748) or address (e.g. 0x82140748)" },
      },
      required: ["function_name"],
    },
  },
  {
    name: "get_function_pseudocode",
    description: "Get Hex-Rays decompiled pseudocode for a function. Only available for functions where get_function_info shows hasPseudocode=true.",
    inputSchema: {
      type: "object",
      properties: {
        function_name: { type: "string", description: "Function name (e.g. sub_82140748)" },
      },
      required: ["function_name"],
    },
  },
  {
    name: "search_functions",
    description: "Search for functions by name, class, or string hint using regex. Use to find functions related to a subsystem before diving into specifics.",
    inputSchema: {
      type: "object",
      properties: {
        pattern: { type: "string", description: "Regex pattern to search function names, class names, and string hints" },
        limit: { type: "integer", description: "Max results to return (default: 50)", default: 50 },
      },
      required: ["pattern"],
    },
  },
  {
    name: "get_callees",
    description: "Get all functions called by a given function (outgoing edges in call graph).",
    inputSchema: {
      type: "object",
      properties: {
        function_name: { type: "string", description: "Function name to get callees for" },
      },
      required: ["function_name"],
    },
  },
  {
    name: "get_callers",
    description: "Get all functions that call a given function (incoming edges in call graph). Useful for understanding how a function is reached.",
    inputSchema: {
      type: "object",
      properties: {
        function_name: { type: "string", description: "Function name to get callers for" },
      },
      required: ["function_name"],
    },
  },
  {
    name: "get_vtable_info",
    description: "Get vtable details for a class: address, entry count, virtual function slots with their target functions.",
    inputSchema: {
      type: "object",
      properties: {
        class_name: { type: "string", description: "Class name (e.g. CBaseDC, rage::grmSetup) — supports partial match" },
      },
      required: ["class_name"],
    },
  },
  {
    name: "get_flirt_api",
    description: "Search FLIRT-matched library/API function names. Finds D3D, XAudio, X*, CRT, NT, Xe-kernel API matches in the binary by searching naming sub_XXXXXXXX functions.",
    inputSchema: {
      type: "object",
      properties: {
        search_pattern: { type: "string", description: "Regex to search FLIRT names, e.g. D3D, CreateTexture, XAudio, X*, CRT, NT, Xe-kernel" },
        trusted_only: { type: "boolean", description: "Skip suspect UE/GoW3 false positives (default: true)", default: true },
        limit: { type: "integer", description: "Max unique names to return (default: 50)", default: 50 },
      },
      required: ["search_pattern"],
    },
  },
  {
    name: "get_string_refs",
    description: "Get string references for a function, or search all functions for a string. Useful for finding sub_XXXXXXXX functions by discovering which reference meaningful strings, helping naming unnamed sub_XXXXXXXX functions.",
    inputSchema: {
      type: "object",
      properties: {
        function_name: { type: "string", description: "Function name to get string refs for" },
        search_string: { type: "string", description: "Text to search for in all strings" },
      },
    },
  },
];

// ---------------------------------------------------------------------------
// MCP Server
// ---------------------------------------------------------------------------

async function main() {
  console.error("liberty-decomp MCP server starting...");

  // Load all data
  parseEnrichedFunctions();
  parseFunctionsWithAddrs();
  parseEnrichedVtables();
  parseVtablesDetailed();
  parseCallGraph();
  parseFuncStringRefs();
  parseHottestFunctions();
  parseFlirt();
  buildPseudocodeIndex();
  parseHookStatus();

  console.error("All data loaded. Starting MCP server...");

  const server = new Server(
    { name: "liberty-decomp", version: "1.0.0" },
    { capabilities: { tools: {} } }
  );

  server.setRequestHandler(ListToolsRequestSchema, async () => ({ tools: TOOLS }));

  server.setRequestHandler(CallToolRequestSchema, async (request) => {
    const { name, arguments: args } = request.params;
    try {
      parseHookStatus(); // live rescan: hook coverage changes constantly during active dev
      let result;
      switch (name) {
        case "get_function_info":
          result = getFunctionInfo(args.function_name);
          break;
        case "get_function_pseudocode":
          result = getFunctionPseudocode(args.function_name);
          break;
        case "search_functions":
          result = searchFunctions(args.pattern, args.limit);
          break;
        case "get_callees":
          result = getCallees(args.function_name);
          break;
        case "get_callers":
          result = getCallers(args.function_name);
          break;
        case "get_vtable_info":
          result = getVtableInfo(args.class_name);
          break;
        case "get_flirt_api":
          result = getFlirtApi(args.search_pattern, args.trusted_only, args.limit);
          break;
        case "get_string_refs":
          result = getStringRefs(args.function_name, args.search_string);
          break;
        default:
          return { content: [{ type: "text", text: `Unknown tool: ${name}` }], isError: true };
      }
      return { content: [{ type: "text", text: JSON.stringify(result, null, 2) }] };
    } catch (error) {
      return { content: [{ type: "text", text: `Error: ${error.message || error}` }], isError: true };
    }
  });

  const transport = new StdioServerTransport();
  await server.connect(transport);
  console.error("liberty-decomp MCP server running on stdio");
}

main().catch((err) => {
  console.error("Fatal error:", err);
  process.exit(1);
});
