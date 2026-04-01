#!/usr/bin/env python3
"""
V2: Search for how addresses in the 0x831C0000 region are accessed.
In recompiled PPC code, globals are accessed via:
  - lis rX, 0x831C (load upper immediate = high 16 bits)
  - then addi/ori rX, rX, offset (low 16 bits)
  - then lwz/stw off the resulting register

So we search for the high half pattern: 0x831C (as lis operand)
and for direct memory access macros like PPC_LOAD_U32(0x831Cxxxx)
"""

import os
import re
import glob
from collections import defaultdict

GEN_DIR = "/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated"
TABLE_BASE = 0x831C0000
DEV_CFG_BASE = 0x831C2210
TARGET_ADDR = 0x831C2458

# In the recompiled code, addresses are usually constructed or used directly.
# The codegen typically emits PPC_LOAD_U32(addr), PPC_STORE_U32(addr, val) etc.
# with the full address. But if none found, addresses are built from lis+addi.
#
# Let's search for:
# 1. Any literal containing "831C" that's part of a PPC memory operation
# 2. The pattern where r.gpr[X] = (int)(short)0xYYYY << 16 (lis emulation)

# First: broader search for 0x831C as a 32-bit address prefix in memory ops
# The codegen might use decimal or might split the address

# Search strategy: grep for "2210" (low 16 of device config), "2458" (low 16 of target)
# These would appear as offsets in addi/ori after lis 0x831C

cpp_files = sorted(glob.glob(os.path.join(GEN_DIR, "*.cpp")))
print(f"Scanning {len(cpp_files)} files...")

# Pattern 1: Direct address in PPC memory macros
# e.g., PPC_LOAD_U32(0x831C2458) or MEM_LOAD_U32(0x831C2458)
direct_pattern = re.compile(r'(?:PPC_|MEM_)(?:LOAD|STORE)_[A-Z0-9]+\s*\(\s*0x831[Cc][0-9A-Fa-f]{4}')

# Pattern 2: lis-style: shift left by 16 to form high half
# e.g., ctx.r3 = 0x831C0000  or  r3 = (0x831C << 16)
lis_pattern = re.compile(r'= .*(?:0x831[Cc]0000|0x831[Cc] *<< *16)')

# Pattern 3: addis/lis emitted as: rX = (int)(short)0x831C << 16
# or: rX = 0x831C0000
addis_pattern = re.compile(r'0x831[Cc]0000')

# Pattern 4: offset 0x2458 or 0x2210 used as displacement
# In recompiled code: PPC_LOAD_U32(rX + 0x2458) where rX holds 0x831C0000
offset_pattern_2458 = re.compile(r'(?:0x2458|9304)')  # hex or decimal offset
offset_pattern_2210 = re.compile(r'0x2210')

func_pattern = re.compile(r'void\s+(sub_[0-9A-Fa-f]+|__imp__sub_[0-9A-Fa-f]+)\s*\(')

# Collect results
results_direct = []
results_lis = []
results_off2458 = []
results_off2210 = []

for fpath in cpp_files:
    fname = os.path.basename(fpath)
    current_func = None
    with open(fpath, 'r', errors='replace') as f:
        for line_no, line in enumerate(f, 1):
            fm = func_pattern.search(line)
            if fm:
                current_func = fm.group(1)

            stripped = line.strip()

            if direct_pattern.search(stripped):
                results_direct.append((fname, line_no, stripped, current_func))

            if addis_pattern.search(stripped):
                # Filter out the instruction-address false positives
                # (loc_82831Cxx, ctx.lr = 0x82831Cxx patterns)
                if 'PPC_' in stripped or 'MEM_' in stripped or 'gpr' in stripped or 'r3' in stripped.lower():
                    results_lis.append((fname, line_no, stripped, current_func))
                elif '831C0000' in stripped:
                    results_lis.append((fname, line_no, stripped, current_func))

            # Only look for offset patterns in lines that have memory operations
            if ('PPC_' in stripped or 'MEM_' in stripped) and 'LOAD' in stripped or 'STORE' in stripped:
                if offset_pattern_2458.search(stripped):
                    results_off2458.append((fname, line_no, stripped, current_func))
                if offset_pattern_2210.search(stripped):
                    results_off2210.append((fname, line_no, stripped, current_func))

print(f"\n=== DIRECT ADDRESS REFERENCES (PPC/MEM macros with 0x831Cxxxx) ===")
print(f"Found: {len(results_direct)}")
for fname, line_no, line, func in results_direct[:50]:
    print(f"  {func} ({fname}:{line_no}): {line[:200]}")

print(f"\n=== LIS-STYLE REFERENCES (0x831C0000) ===")
print(f"Found: {len(results_lis)}")
for fname, line_no, line, func in results_lis[:50]:
    print(f"  {func} ({fname}:{line_no}): {line[:200]}")

print(f"\n=== OFFSET 0x2458 IN MEMORY OPS ===")
print(f"Found: {len(results_off2458)}")
for fname, line_no, line, func in results_off2458[:50]:
    print(f"  {func} ({fname}:{line_no}): {line[:200]}")

print(f"\n=== OFFSET 0x2210 IN MEMORY OPS ===")
print(f"Found: {len(results_off2210)}")
for fname, line_no, line, func in results_off2210[:50]:
    print(f"  {func} ({fname}:{line_no}): {line[:200]}")

# Now let's look for the actual codegen pattern - how does it emit memory access?
# Let's grab a sample PPC_LOAD line to understand the format
print(f"\n=== SAMPLE PPC_LOAD/PPC_STORE LINES (first 10) ===")
sample_count = 0
for fpath in cpp_files[:3]:
    with open(fpath, 'r', errors='replace') as f:
        for line_no, line in enumerate(f, 1):
            if ('PPC_LOAD_U32' in line or 'PPC_STORE_U32' in line) and sample_count < 10:
                print(f"  {line.strip()[:200]}")
                sample_count += 1
