#!/usr/bin/env python3
"""Search all generated .cpp files for writes to guest address 0x831C2458."""

import os
import re
import glob
from collections import defaultdict

GEN_DIR = "/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated"

results = {
    "exact_831C2458": [],
    "near_831C24xx": [],
    "store_831C2xxx": [],
}

cpp_files = sorted(glob.glob(os.path.join(GEN_DIR, "*.cpp")))
print(f"Scanning {len(cpp_files)} .cpp files...")

for fpath in cpp_files:
    fname = os.path.basename(fpath)
    with open(fpath, "r", errors="replace") as f:
        for lineno, line in enumerate(f, 1):
            line_upper = line.upper()

            # 1. Exact match: 831C2458
            if "831C2458" in line_upper:
                results["exact_831C2458"].append((fname, lineno, line.rstrip()))

            # 2. Near match: 831C24xx (but not exact, to avoid double-counting)
            elif "831C24" in line_upper:
                results["near_831C24xx"].append((fname, lineno, line.rstrip()))

            # 3. PPC_STORE in 831C2xxx range (but not already caught above)
            elif "831C2" in line_upper and re.search(r"PPC_STORE", line, re.IGNORECASE):
                results["store_831C2xxx"].append((fname, lineno, line.rstrip()))

# Also search for base addresses that could reach 0x831C2458 via offset
# Common bases: 0x831C0000 + 0x2458, 0x831C2000 + 0x458, 0x831C2400 + 0x58
# Search for these base patterns near store instructions
base_offset_results = []
for fpath in cpp_files:
    fname = os.path.basename(fpath)
    with open(fpath, "r", errors="replace") as f:
        for lineno, line in enumerate(f, 1):
            line_upper = line.upper()
            # Look for 831C0000 or 831C2000 or 831C2400 near stores
            for base in ["831C0000", "831C2000", "831C2400"]:
                if base in line_upper:
                    base_offset_results.append((fname, lineno, base, line.rstrip()))

# Print results
print("\n" + "="*80)
print("SEARCH RESULTS: Writes to guest address 0x831C2458")
print("="*80)

print(f"\n--- Exact '831C2458' matches: {len(results['exact_831C2458'])} ---")
for fname, lineno, line in results["exact_831C2458"]:
    print(f"  {fname}:{lineno}: {line[:200]}")

print(f"\n--- Near '831C24xx' matches (excluding exact): {len(results['near_831C24xx'])} ---")
for fname, lineno, line in results["near_831C24xx"]:
    print(f"  {fname}:{lineno}: {line[:200]}")

print(f"\n--- PPC_STORE in 831C2xxx range (excluding above): {len(results['store_831C2xxx'])} ---")
for fname, lineno, line in results["store_831C2xxx"]:
    print(f"  {fname}:{lineno}: {line[:200]}")

print(f"\n--- Base address matches (831C0000/831C2000/831C2400): {len(base_offset_results)} ---")
for fname, lineno, base, line in base_offset_results:
    print(f"  {fname}:{lineno} [base=0x{base}]: {line[:200]}")

# Summary
total = (len(results["exact_831C2458"]) + len(results["near_831C24xx"]) +
         len(results["store_831C2xxx"]) + len(base_offset_results))
print(f"\n{'='*80}")
print(f"TOTAL MATCHES: {total}")
print(f"  Exact 831C2458:        {len(results['exact_831C2458'])}")
print(f"  Near 831C24xx:         {len(results['near_831C24xx'])}")
print(f"  PPC_STORE 831C2xxx:    {len(results['store_831C2xxx'])}")
print(f"  Base addr patterns:    {len(base_offset_results)}")
print(f"{'='*80}")

# Write markdown report
report_path = "/Users/Ozordi/Downloads/LibertyRecomp/docs/rewrite/scene_ptr_binary_search.md"
os.makedirs(os.path.dirname(report_path), exist_ok=True)
with open(report_path, "w") as f:
    f.write("# Binary Search: Writes to Guest Address 0x831C2458\n\n")
    f.write(f"Scanned {len(cpp_files)} generated .cpp files in `generated/`\n\n")

    f.write(f"## Exact `831C2458` Matches ({len(results['exact_831C2458'])})\n\n")
    if results["exact_831C2458"]:
        for fname, lineno, line in results["exact_831C2458"]:
            f.write(f"- `{fname}:{lineno}`: `{line.strip()[:150]}`\n")
    else:
        f.write("None found.\n")

    f.write(f"\n## Near `831C24xx` Matches ({len(results['near_831C24xx'])})\n\n")
    if results["near_831C24xx"]:
        for fname, lineno, line in results["near_831C24xx"]:
            f.write(f"- `{fname}:{lineno}`: `{line.strip()[:150]}`\n")
    else:
        f.write("None found.\n")

    f.write(f"\n## PPC_STORE in `831C2xxx` Range ({len(results['store_831C2xxx'])})\n\n")
    if results["store_831C2xxx"]:
        for fname, lineno, line in results["store_831C2xxx"]:
            f.write(f"- `{fname}:{lineno}`: `{line.strip()[:150]}`\n")
    else:
        f.write("None found.\n")

    f.write(f"\n## Base Address Patterns ({len(base_offset_results)})\n\n")
    f.write("Searched for `831C0000`, `831C2000`, `831C2400` (potential bases that reach 0x831C2458 via offset).\n\n")
    if base_offset_results:
        for fname, lineno, base, line in base_offset_results:
            f.write(f"- `{fname}:{lineno}` [base=0x{base}]: `{line.strip()[:150]}`\n")
    else:
        f.write("None found.\n")

    f.write(f"\n## Summary\n\n")
    f.write(f"| Category | Count |\n")
    f.write(f"|-|-|\n")
    f.write(f"| Exact 831C2458 | {len(results['exact_831C2458'])} |\n")
    f.write(f"| Near 831C24xx | {len(results['near_831C24xx'])} |\n")
    f.write(f"| PPC_STORE 831C2xxx | {len(results['store_831C2xxx'])} |\n")
    f.write(f"| Base addr patterns | {len(base_offset_results)} |\n")
    f.write(f"| **Total** | **{total}** |\n")

    f.write(f"\n## Conclusion\n\n")
    if len(results['exact_831C2458']) == 0 and total == 0:
        f.write("No direct or indirect writes to 0x831C2458 found in the generated recomp code.\n")
        f.write("The address is likely written via a fully computed pointer (register-based addressing)\n")
        f.write("where the base address is loaded from another memory location at runtime.\n")
    elif len(results['exact_831C2458']) > 0:
        f.write("Direct references to 0x831C2458 found. See exact matches above.\n")
    else:
        f.write("No exact match, but nearby/base references found. See details above.\n")

print(f"\nReport written to: {report_path}")
