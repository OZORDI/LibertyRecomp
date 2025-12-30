#!/usr/bin/env node
// Build the PPC index standalone (for testing)
import * as path from 'path';
import { fileURLToPath } from 'url';
import { buildIndex } from './parsers/ppc-parser.js';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const PPC_DIR = process.env.PPC_DIR || 
  path.resolve(__dirname, '../../../LibertyRecompLib/ppc');
const IMPORTS_FILE = process.env.IMPORTS_FILE || 
  path.resolve(__dirname, '../../../LibertyRecomp/kernel/imports.cpp');

async function main() {
  console.log('PPC Directory:', PPC_DIR);
  console.log('Imports File:', IMPORTS_FILE);
  console.log('');
  
  const index = await buildIndex(PPC_DIR, IMPORTS_FILE);
  
  console.log('');
  console.log('=== Index Statistics ===');
  console.log(`Total functions: ${index.functions.size}`);
  console.log(`Total globals: ${index.globals.size}`);
  console.log(`Total kernel APIs: ${index.kernelAPIs.size}`);
  console.log(`Total hooks: ${index.hooks.size}`);
  console.log(`Call graph edges: ${Array.from(index.callGraph.values()).reduce((sum, set) => sum + set.size, 0)}`);
  
  // Sample output
  console.log('');
  console.log('=== Sample Functions ===');
  let count = 0;
  for (const [addr, func] of index.functions) {
    if (count++ >= 5) break;
    console.log(`  ${func.name} @ ${addr}`);
    console.log(`    Calls: ${func.calls.slice(0, 3).join(', ')}${func.calls.length > 3 ? '...' : ''}`);
    console.log(`    Kernel APIs: ${func.kernelCalls.join(', ') || 'none'}`);
  }
  
  console.log('');
  console.log('=== Sample Kernel APIs ===');
  count = 0;
  for (const [name, api] of index.kernelAPIs) {
    if (count++ >= 10) break;
    console.log(`  ${name} (${api.category}) - ${api.directCallers.length} callers${api.isBlocking ? ' [BLOCKING]' : ''}`);
  }
  
  console.log('');
  console.log('=== Sample Hooks ===');
  count = 0;
  for (const [name, hook] of index.hooks) {
    if (count++ >= 10) break;
    console.log(`  ${name} -> ${hook.hostFunction || hook.type}`);
  }
}

main().catch(console.error);
