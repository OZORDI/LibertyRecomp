#!/usr/bin/env python3
"""
Scan ALL generated recomp code for references to addresses 0x831C2200..0x831C2500
(the render globals table region around the scene pointer at 0x831C2458).

Outputs:
  1. Every reference with READ/WRITE classification
  2. Offset from 0x831C0000 and from 0x831C2210 (device config base)
  3. 0x831C2458 offset from device config
  4. How device config struct at 0x831C2210 is populated
  5. Array pattern detection
  6. All WRITE references with function and value info
"""

import os
import re
import glob
from collections import defaultdict

GEN_DIR = "/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated"
RANGE_LO = 0x831C2200
RANGE_HI = 0x831C2500
TABLE_BASE = 0x831C0000
DEV_CFG_BASE = 0x831C2210
TARGET_ADDR = 0x831C2458

# Build set of all hex addresses in range
target_addrs = set()
for a in range(RANGE_LO, RANGE_HI + 4, 4):
    target_addrs.add(f"0x{a:08X}")
    target_addrs.add(f"0x{a:08x}")
    target_addrs.add(f"0x{a:X}")

# Also search for the device config base specifically
DEV_CFG_HEX = [f"0x{DEV_CFG_BASE:08X}", f"0x{DEV_CFG_BASE:08x}", f"0x{DEV_CFG_BASE:X}"]

# Regex to find addresses in our range — match 0x831C2 followed by hex digits
addr_pattern = re.compile(r'0x831[Cc]2[2-4][0-9A-Fa-f]{2}')

# Pattern to extract function name (sub_XXXXXXXX)
func_pattern = re.compile(r'void\s+(sub_[0-9A-Fa-f]+|__imp__sub_[0-9A-Fa-f]+)\s*\(')

results = []  # (file, line_no, line, address, read_or_write, function)

# Also collect broader device config references (0x831C2210)
dev_cfg_refs = []

# Scan all generated files
cpp_files = sorted(glob.glob(os.path.join(GEN_DIR, "*.cpp")))
print(f"Scanning {len(cpp_files)} files...")

for fpath in cpp_files:
    fname = os.path.basename(fpath)
    current_func = None

    with open(fpath, 'r', errors='replace') as f:
        for line_no, line in enumerate(f, 1):
            # Track current function
            fm = func_pattern.search(line)
            if fm:
                current_func = fm.group(1)

            # Search for addresses in our range
            matches = addr_pattern.findall(line)
            for m in matches:
                addr_val = int(m, 16)
                if RANGE_LO <= addr_val <= RANGE_HI:
                    # Classify as read or write
                    stripped = line.strip()
                    if 'PPC_STORE' in stripped or 'MEM_STORE' in stripped:
                        rw = 'WRITE'
                    elif 'PPC_LOAD' in stripped or 'MEM_LOAD' in stripped:
                        rw = 'READ'
                    elif '=' in stripped and m in stripped.split('=')[0]:
                        rw = 'WRITE'
                    else:
                        rw = 'OTHER'

                    results.append((fname, line_no, stripped, addr_val, rw, current_func))

            # Also track device config base refs outside the range
            for dcx in DEV_CFG_HEX:
                if dcx in line:
                    dev_cfg_refs.append((fname, line_no, line.strip(), current_func))

print(f"Found {len(results)} references in range 0x{RANGE_LO:08X}-0x{RANGE_HI:08X}")
print(f"Found {len(dev_cfg_refs)} references to device config base 0x{DEV_CFG_BASE:08X}")

# === Task 4: Offset of 0x831C2458 from device config ===
offset_from_devcfg = TARGET_ADDR - DEV_CFG_BASE
offset_from_table = TARGET_ADDR - TABLE_BASE
print(f"\n=== OFFSET ANALYSIS ===")
print(f"0x831C2458 offset from table base (0x831C0000): 0x{offset_from_table:X} ({offset_from_table} bytes)")
print(f"0x831C2458 offset from device config (0x831C2210): 0x{offset_from_devcfg:X} ({offset_from_devcfg} bytes)")

# === Group by address ===
by_addr = defaultdict(list)
for fname, line_no, line, addr_val, rw, func in results:
    by_addr[addr_val].append((fname, line_no, line, rw, func))

print(f"\n=== ADDRESSES FOUND (sorted) ===")
for addr in sorted(by_addr.keys()):
    entries = by_addr[addr]
    off_table = addr - TABLE_BASE
    off_devcfg = addr - DEV_CFG_BASE
    reads = sum(1 for e in entries if e[3] == 'READ')
    writes = sum(1 for e in entries if e[3] == 'WRITE')
    others = sum(1 for e in entries if e[3] == 'OTHER')
    print(f"  0x{addr:08X}  table+0x{off_table:04X}  devcfg+0x{off_devcfg:04X}  R={reads} W={writes} O={others}")

# === Task 6: Array detection ===
print(f"\n=== ARRAY PATTERN DETECTION ===")
sorted_addrs = sorted(by_addr.keys())
if len(sorted_addrs) >= 2:
    diffs = []
    for i in range(1, len(sorted_addrs)):
        d = sorted_addrs[i] - sorted_addrs[i-1]
        diffs.append(d)
    print(f"Address gaps (bytes): {[f'0x{d:X}' for d in diffs]}")
    # Check for regular stride
    from collections import Counter
    stride_counts = Counter(diffs)
    for stride, cnt in stride_counts.most_common(5):
        if cnt >= 2:
            print(f"  Repeated stride: 0x{stride:X} ({stride} bytes) appears {cnt} times — possible array element size")
else:
    print("Not enough addresses to detect array pattern")

# Check if 0x831C2458 falls on a regular stride from any base
print(f"\nChecking if 0x{TARGET_ADDR:08X} aligns to common struct/array strides from devcfg base:")
for stride in [4, 8, 16, 32, 64, 128, 248, 256, 512]:
    remainder = offset_from_devcfg % stride
    index = offset_from_devcfg // stride
    print(f"  stride {stride} (0x{stride:X}): offset/stride = {index} remainder {remainder}")

# === Task 7: ALL WRITE references with function and value ===
print(f"\n=== ALL WRITE REFERENCES ===")
write_count = 0
for addr in sorted(by_addr.keys()):
    for fname, line_no, line, rw, func in by_addr[addr]:
        if rw == 'WRITE':
            write_count += 1
            print(f"  0x{addr:08X} in {func or 'unknown'} ({fname}:{line_no})")
            # Truncate very long lines
            display = line[:200] + "..." if len(line) > 200 else line
            print(f"    {display}")

if write_count == 0:
    print("  (No direct WRITE references found in range)")

# === Task 5: Device config population (0x831C2210) ===
print(f"\n=== DEVICE CONFIG (0x{DEV_CFG_BASE:08X}) REFERENCES ===")
for fname, line_no, line, func in dev_cfg_refs:
    display = line[:200] + "..." if len(line) > 200 else line
    print(f"  {func or 'unknown'} ({fname}:{line_no}): {display}")

# === Now search for ALL references that WRITE to addresses near 0x831C2458 ===
# Also look for computed address patterns (base + offset) that could reference this area
print(f"\n=== SEARCHING FOR COMPUTED REFERENCES (base+offset patterns) ===")
# Search for 0x831C2 with any suffix, capturing the full address
broad_pattern = re.compile(r'0x831[Cc]2[0-9A-Fa-f]{3}\b')
computed_refs = []
for fpath in cpp_files:
    fname = os.path.basename(fpath)
    current_func = None
    with open(fpath, 'r', errors='replace') as f:
        for line_no, line in enumerate(f, 1):
            fm = func_pattern.search(line)
            if fm:
                current_func = fm.group(1)
            matches = broad_pattern.findall(line)
            for m in matches:
                addr_val = int(m, 16)
                # Broader range: 0x831C2000 to 0x831C2FFF
                if 0x831C2000 <= addr_val <= 0x831C2FFF:
                    if addr_val not in by_addr or line.strip() not in [e[2] for e in by_addr[addr_val]]:
                        if 'STORE' in line or ('=' in line and m in line.split('=')[0]):
                            computed_refs.append((addr_val, fname, line_no, line.strip(), current_func))

if computed_refs:
    print(f"Additional computed references in 0x831C2000-0x831C2FFF:")
    for addr_val, fname, line_no, line, func in sorted(computed_refs, key=lambda x: x[0]):
        display = line[:200] + "..." if len(line) > 200 else line
        print(f"  0x{addr_val:08X} in {func or 'unknown'} ({fname}:{line_no}): {display}")
else:
    print("No additional computed references found")

# === Generate markdown output ===
md_path = "/Users/Ozordi/Downloads/LibertyRecomp/docs/rewrite/render_globals_table.md"
with open(md_path, 'w') as md:
    md.write("# Render Globals Table: 0x831C2200-0x831C2500\n\n")

    md.write("## Key Offsets\n\n")
    md.write(f"- Table base: `0x{TABLE_BASE:08X}`\n")
    md.write(f"- Device config base: `0x{DEV_CFG_BASE:08X}`\n")
    md.write(f"- Target (scene ptr?): `0x{TARGET_ADDR:08X}`\n")
    md.write(f"- Target offset from table base: `0x{offset_from_table:X}` ({offset_from_table} bytes)\n")
    md.write(f"- Target offset from device config: `0x{offset_from_devcfg:X}` ({offset_from_devcfg} bytes)\n\n")

    md.write("## Address Map\n\n")
    md.write("| Address | table+ | devcfg+ | Reads | Writes | Other |\n")
    md.write("|-|-|-|-|-|-|\n")
    for addr in sorted(by_addr.keys()):
        entries = by_addr[addr]
        off_table = addr - TABLE_BASE
        off_devcfg = addr - DEV_CFG_BASE
        reads = sum(1 for e in entries if e[3] == 'READ')
        writes = sum(1 for e in entries if e[3] == 'WRITE')
        others = sum(1 for e in entries if e[3] == 'OTHER')
        md.write(f"| `0x{addr:08X}` | `0x{off_table:04X}` | `0x{off_devcfg:04X}` | {reads} | {writes} | {others} |\n")

    md.write("\n## Write References\n\n")
    if write_count == 0:
        md.write("No direct STORE/write references found in the scanned range.\n\n")
    else:
        for addr in sorted(by_addr.keys()):
            for fname, line_no, line, rw, func in by_addr[addr]:
                if rw == 'WRITE':
                    display = line[:300] + "..." if len(line) > 300 else line
                    md.write(f"### `0x{addr:08X}` in `{func or 'unknown'}`\n")
                    md.write(f"File: `{fname}:{line_no}`\n")
                    md.write(f"```cpp\n{display}\n```\n\n")

    md.write("## Read References\n\n")
    for addr in sorted(by_addr.keys()):
        for fname, line_no, line, rw, func in by_addr[addr]:
            if rw == 'READ':
                display = line[:300] + "..." if len(line) > 300 else line
                md.write(f"### `0x{addr:08X}` in `{func or 'unknown'}`\n")
                md.write(f"File: `{fname}:{line_no}`\n")
                md.write(f"```cpp\n{display}\n```\n\n")

    md.write("## Other References\n\n")
    for addr in sorted(by_addr.keys()):
        for fname, line_no, line, rw, func in by_addr[addr]:
            if rw == 'OTHER':
                display = line[:300] + "..." if len(line) > 300 else line
                md.write(f"### `0x{addr:08X}` in `{func or 'unknown'}`\n")
                md.write(f"File: `{fname}:{line_no}`\n")
                md.write(f"```cpp\n{display}\n```\n\n")

    md.write("## Device Config (0x831C2210) Population\n\n")
    if dev_cfg_refs:
        for fname, line_no, line, func in dev_cfg_refs:
            display = line[:300] + "..." if len(line) > 300 else line
            md.write(f"- `{func or 'unknown'}` (`{fname}:{line_no}`): `{display}`\n")
    else:
        md.write("No direct references to device config base found.\n")

    md.write("\n## Array Pattern Analysis\n\n")
    if len(sorted_addrs) >= 2:
        md.write("Address gaps between consecutive referenced addresses:\n\n")
        for i in range(1, len(sorted_addrs)):
            d = sorted_addrs[i] - sorted_addrs[i-1]
            md.write(f"- `0x{sorted_addrs[i-1]:08X}` → `0x{sorted_addrs[i]:08X}`: gap = `0x{d:X}` ({d} bytes)\n")

        md.write(f"\nStride analysis for `0x{TARGET_ADDR:08X}` from device config:\n\n")
        md.write("| Stride | Index | Remainder |\n")
        md.write("|-|-|-|\n")
        for stride in [4, 8, 16, 32, 64, 128, 248, 256, 512]:
            remainder = offset_from_devcfg % stride
            index = offset_from_devcfg // stride
            md.write(f"| {stride} (0x{stride:X}) | {index} | {remainder} |\n")
    else:
        md.write("Insufficient addresses to detect array patterns.\n")

print(f"\nMarkdown written to {md_path}")
