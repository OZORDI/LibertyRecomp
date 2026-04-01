#!/usr/bin/env python3
"""
V4: Search with correct constant values for 0x831C region.
Also trace lis+addi pattern to find which functions build 0x831Cxxxx addresses.
"""

import os, glob, re, ctypes
from collections import defaultdict

GEN_DIR = "/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated"
cpp_files = sorted(glob.glob(os.path.join(GEN_DIR, "*.cpp")))

TABLE_BASE = 0x831C0000
DEV_CFG_BASE = 0x831C2210
TARGET_ADDR = 0x831C2458

func_pattern = re.compile(r'void\s+(sub_[0-9A-Fa-f]+|__imp__sub_[0-9A-Fa-f]+)\s*\(')

# Correct constants
BASE_S32 = ctypes.c_int32(TABLE_BASE).value   # -2095316992
BASE_U32 = TABLE_BASE                          # 2199650304

TARGET_S32 = ctypes.c_int32(TARGET_ADDR).value # -2095307688
TARGET_U32 = TARGET_ADDR                       # 2199659608

DEVCFG_S32 = ctypes.c_int32(DEV_CFG_BASE).value # -2095308272
DEVCFG_U32 = DEV_CFG_BASE                        # 2199659024

print(f"Searching for constants:")
print(f"  0x831C0000: signed={BASE_S32}, unsigned={BASE_U32}")
print(f"  0x831C2210: signed={DEVCFG_S32}, unsigned={DEVCFG_U32}")
print(f"  0x831C2458: signed={TARGET_S32}, unsigned={TARGET_U32}")
print()

# Also compute what addi offset would be for lis 0x831C (= 0x831C0000) + addi to reach target
# addi offset = target - base = 0x2458 = 9304
# But in the codegen, lis+addi is often folded into a single assignment.

# Strategy: search for the absolute decimal values of these constants
# The codegen typically emits: ctx.rX.s64 = CONSTANT; for lis+addi folded
# or ctx.rX.u32 = ctx.rX.u32 + OFFSET; for separate addi

# Search strings (partial matches are fine for decimals since they're unique enough)
search_map = {
    '2095316992': ('0x831C0000', 'base'),      # |signed|
    '2199650304': ('0x831C0000', 'base'),      # unsigned
    '2095307688': ('0x831C2458', 'target'),    # |signed|
    '2199659608': ('0x831C2458', 'target'),    # unsigned
    '2095308272': ('0x831C2210', 'devcfg'),    # |signed|
    '2199659024': ('0x831C2210', 'devcfg'),    # unsigned
}

results = defaultdict(list)

print(f"Scanning {len(cpp_files)} files...")

for fpath in cpp_files:
    fname = os.path.basename(fpath)
    current_func = None
    with open(fpath, 'r', errors='replace') as f:
        for line_no, line in enumerate(f, 1):
            fm = func_pattern.search(line)
            if fm:
                current_func = fm.group(1)

            for search_val, (addr_name, label) in search_map.items():
                if search_val in line:
                    results[addr_name].append((fname, line_no, line.strip()[:250], current_func, label))

for addr_name in sorted(results.keys()):
    entries = results[addr_name]
    print(f"\n=== {addr_name} ({len(entries)} refs) ===")
    for fname, line_no, line, func, label in entries[:50]:
        print(f"  {func or '?'} ({fname}:{line_no}): {line}")

# Now: broader scan. For the ENTIRE 0x831C0000-0x831CFFFF range,
# the signed decimal is -2095316992 to -2095251456.
# Let's search for the prefix "-209531" or "-209530" or "-209525" etc.
# Actually let's be precise: 0x831C0000 = -2095316992, 0x831CFFFF = -2095251457
# So all values are in [-2095316992, -2095251457]
# The first 5 digits would be -20953 or -20952

# Better approach: search for "209531" which would capture -2095316992 through -2095310000
# And "209530" for -2095309999 through -2095300000
# 0x831C2000-0x831C2FFF = -2095308800 to -2095304705
# These contain "209530" as substring

print("\n\n=== BROAD SEARCH: '209530' (covers 0x831C2xxx approx) ===")
broad_results = []
for fpath in cpp_files:
    fname = os.path.basename(fpath)
    current_func = None
    with open(fpath, 'r', errors='replace') as f:
        for line_no, line in enumerate(f, 1):
            fm = func_pattern.search(line)
            if fm:
                current_func = fm.group(1)
            if '209530' in line:
                broad_results.append((fname, line_no, line.strip()[:250], current_func))

print(f"Found: {len(broad_results)}")
for fname, line_no, line, func in broad_results[:100]:
    print(f"  {func or '?'} ({fname}:{line_no}): {line}")

print("\n\n=== BROAD SEARCH: '209531' (covers 0x831C0xxx approx) ===")
broad_results2 = []
for fpath in cpp_files:
    fname = os.path.basename(fpath)
    current_func = None
    with open(fpath, 'r', errors='replace') as f:
        for line_no, line in enumerate(f, 1):
            fm = func_pattern.search(line)
            if fm:
                current_func = fm.group(1)
            if '209531' in line:
                broad_results2.append((fname, line_no, line.strip()[:250], current_func))

print(f"Found: {len(broad_results2)}")
for fname, line_no, line, func in broad_results2[:100]:
    print(f"  {func or '?'} ({fname}:{line_no}): {line}")

# Also search for unsigned versions: 2199650304-2199715839 (0x831C0000-0x831CFFFF)
# Prefix "219965" covers 2199650000-2199659999 (roughly 0x831C0000-0x831C2700)
print("\n\n=== BROAD SEARCH: '219965' (covers 0x831C0xxx-0x831C2xxx unsigned) ===")
broad_results3 = []
for fpath in cpp_files:
    fname = os.path.basename(fpath)
    current_func = None
    with open(fpath, 'r', errors='replace') as f:
        for line_no, line in enumerate(f, 1):
            fm = func_pattern.search(line)
            if fm:
                current_func = fm.group(1)
            if '219965' in line:
                broad_results3.append((fname, line_no, line.strip()[:250], current_func))

print(f"Found: {len(broad_results3)}")
for fname, line_no, line, func in broad_results3[:100]:
    print(f"  {func or '?'} ({fname}:{line_no}): {line}")
