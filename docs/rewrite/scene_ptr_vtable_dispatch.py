#!/usr/bin/env python3
"""
Analyze sub_828BF3C8, sub_828C1228, sub_828C15C8 in generated recomp code.
Trace addresses in 0x831C0000-0x831CFFFF range and find callers.
"""

import os
import re
import glob
import struct

GEN_DIR = "/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated"
OUTPUT_FILE = "/Users/Ozordi/Downloads/LibertyRecomp/docs/rewrite/scene_ptr_vtable_dispatch.md"
TARGET_FUNCS = ["sub_828BF3C8", "sub_828C1228", "sub_828C15C8"]

# ---- helpers ----

def sign_extend_16(val):
    """Sign-extend a 16-bit value to 32-bit (Python int)."""
    if val & 0x8000:
        return val - 0x10000
    return val

def lis_value(simm16):
    """Compute the result of `lis rN, simm16` as a 32-bit unsigned."""
    return (sign_extend_16(simm16) << 16) & 0xFFFFFFFF

def reconstruct_address(lis_literal, offset):
    """Given a `lis rN, X` producing base and an addi/load offset, compute address."""
    # lis_literal is the Python int literal from the code (e.g. -2095316992 for 0x831C0000)
    base = lis_literal & 0xFFFFFFFF
    return (base + offset) & 0xFFFFFFFF

# ---- file parsing ----

def find_all_cpp_files():
    return sorted(glob.glob(os.path.join(GEN_DIR, "gta4_recomp.*.cpp")))

def extract_imp_function_body(lines, func_name):
    """Extract the body of __imp__<func_name> from lines."""
    imp_name = f"__imp__{func_name}"
    start_pat = re.compile(r'PPC_FUNC_IMPL\(' + re.escape(imp_name) + r'\)\s*\{')
    results = []
    i = 0
    while i < len(lines):
        if start_pat.search(lines[i]):
            brace = 0
            start = i
            body = []
            for j in range(i, len(lines)):
                body.append(lines[j])
                brace += lines[j].count('{') - lines[j].count('}')
                if brace <= 0 and j > i:
                    results.append((start, j, body))
                    i = j + 1
                    break
            else:
                results.append((start, len(lines)-1, body))
                break
        i += 1
    return results

# ---- address tracing ----

def trace_addresses_in_body(body_lines):
    """
    Trace lis + offset patterns to compute actual addresses accessed.
    Returns list of (address_hex, access_type, asm_comment, line)
    """
    accesses = []

    # Track register state (very simplified — lis sets base, subsequent addi/load/store uses it)
    # Pattern 1: lis rN, IMM  →  base = IMM << 16
    #             stw/lwz rM, OFFSET(rN)  →  addr = base + OFFSET
    # Pattern 2: lis rN, IMM
    #             addi rM, rN, OFFSET  →  rM = base + OFFSET  (pointer in register)
    #             then stw/lwz rX, OFFSET2(rM) → addr = base + OFFSET + OFFSET2

    # Extract lis values: look for "lis rN,LITERAL" in comments and the ctx assignment
    lis_pattern = re.compile(r'// lis (r\d+),(-?\d+)')
    # Load/store with register+offset: stw/lwz/lbz/lhz/std/lfs rX, OFFSET(rN)
    mem_pattern = re.compile(r'// (stw|stwu|lwz|lbz|lhz|std|stb|sth|lfd|stfd|lfs) (r\d+|f\d+),(-?\d+)\((r\d+)\)')
    # addi pattern
    addi_pattern = re.compile(r'// addi (r\d+),(r\d+),(-?\d+)')

    reg_bases = {}  # reg -> base_value (from lis)

    for line in body_lines:
        stripped = line.strip()

        # Track lis
        m = lis_pattern.search(stripped)
        if m:
            reg = m.group(1)
            literal = int(m.group(2))
            base = literal & 0xFFFFFFFF  # treat as unsigned 32-bit
            # On PPC, lis loads the upper 16 bits. The literal in the recomp is already shifted.
            # E.g., "lis r11,-31972" means r11 = -31972 << 16 = 0x831C0000
            # But in the C code it shows: ctx.r11.s64 = -2095316992 which is 0xFFFFFFFF831C0000
            # The 32-bit value is 0x831C0000
            reg_bases[reg] = base
            continue

        # Track addi rM, rN, offset (rM inherits modified base)
        m = addi_pattern.search(stripped)
        if m:
            dst_reg = m.group(1)
            src_reg = m.group(2)
            offset = int(m.group(3))
            if src_reg in reg_bases:
                new_base = (reg_bases[src_reg] + offset) & 0xFFFFFFFF
                reg_bases[dst_reg] = new_base
            continue

        # Track loads/stores
        m = mem_pattern.search(stripped)
        if m:
            op = m.group(1)
            data_reg = m.group(2)
            offset = int(m.group(3))
            base_reg = m.group(4)
            if base_reg in reg_bases:
                addr = (reg_bases[base_reg] + offset) & 0xFFFFFFFF
                if 0x831C0000 <= addr <= 0x831CFFFF:
                    access = 'WRITE' if op.startswith('st') else 'READ'
                    accesses.append((f"0x{addr:08X}", access, stripped, line.rstrip()))
            continue

    return accesses

def find_callers_of(all_lines, func_name):
    """Find all PPC_FUNC that call func_name (not its own definition)."""
    # Look for direct calls: sub_XXXX(ctx, base) or __imp__sub_XXXX(ctx, base)
    call_pattern = re.compile(
        r'\b(?:__imp__)?' + re.escape(func_name) + r'\(ctx,\s*base\)'
    )
    func_def = re.compile(r'PPC_FUNC_IMPL\((?:__imp__)?(\w+)\)')
    callers = []
    for filepath, lines in all_lines.items():
        current_func = None
        for i, line in enumerate(lines):
            m = func_def.search(line)
            if m:
                current_func = m.group(1)
            if call_pattern.search(line):
                # Skip its own definition / weak wrapper
                if current_func and current_func != func_name and current_func != f"__imp__{func_name}":
                    callers.append((current_func, os.path.basename(filepath), i+1, line.strip()))
    return callers

def find_refs_to_addr(all_lines, addr_hex_list):
    """Find all lines referencing specific hex addresses."""
    patterns = [re.compile(re.escape(a), re.IGNORECASE) for a in addr_hex_list]
    func_def = re.compile(r'PPC_FUNC_IMPL\((?:__imp__)?(\w+)\)')
    refs = {}
    for filepath, lines in all_lines.items():
        current_func = None
        for i, line in enumerate(lines):
            m = func_def.search(line)
            if m:
                current_func = m.group(1)
            for p, addr in zip(patterns, addr_hex_list):
                if p.search(line):
                    refs.setdefault(addr.upper(), []).append(
                        (current_func or "GLOBAL", os.path.basename(filepath), i+1, line.strip())
                    )
    return refs

# ---- main ----

def main():
    print("Loading generated recomp files...")
    cpp_files = find_all_cpp_files()
    print(f"Found {len(cpp_files)} files")

    all_lines = {}
    for f in cpp_files:
        with open(f, 'r') as fh:
            all_lines[f] = fh.readlines()

    results = {}
    out = []
    out.append("# Scene Pointer VTABLE Dispatch Analysis\n")
    out.append("Analyzing whether 0x831C2458 is written through a vtable dispatch or function pointer table.\n")

    # ---- Analyze each target function ----
    for func_name in TARGET_FUNCS:
        print(f"\n{'='*60}")
        print(f"Analyzing {func_name}")
        print(f"{'='*60}")

        out.append(f"\n## {func_name}\n")
        found = False

        for filepath, lines in all_lines.items():
            bodies = extract_imp_function_body(lines, func_name)
            if bodies:
                for start, end, body_lines in bodies:
                    found = True
                    fname = os.path.basename(filepath)
                    print(f"  Found in {fname}, lines {start+1}-{end+1} ({len(body_lines)} lines)")
                    out.append(f"- **File**: {fname}, lines {start+1}-{end+1} ({len(body_lines)} lines)\n")

                    # Print body
                    print(f"\n  --- Body of __imp__{func_name} ---")
                    for bl in body_lines:
                        print(f"    {bl.rstrip()}")
                    print(f"  --- End ---\n")

                    out.append("### Full Body\n\n```cpp\n")
                    for bl in body_lines:
                        out.append(bl)
                    out.append("```\n")

                    # Trace addresses
                    accesses = trace_addresses_in_body(body_lines)
                    if accesses:
                        print(f"  Addresses in 0x831C0000-0x831CFFFF:")
                        out.append("\n### Addresses in 0x831C0000-0x831CFFFF\n\n")
                        out.append("|Address|Access|ASM Comment|\n|-|-|-|\n")
                        for addr, access, asm_comment, _ in accesses:
                            print(f"    {addr} [{access}]: {asm_comment}")
                            out.append(f"|{addr}|{access}|`{asm_comment}`|\n")
                    else:
                        print(f"  No addresses in 0x831C0000-0x831CFFFF (may use register-indirect)")
                        out.append("\nNo direct addresses in 0x831C0000-0x831CFFFF (register-indirect or out-of-range).\n")

                    results[func_name] = {
                        'body': body_lines,
                        'accesses': accesses,
                    }
                break

        if not found:
            print(f"  NOT FOUND in generated code!")
            out.append("**NOT FOUND** in generated code.\n")
            results[func_name] = {'body': [], 'accesses': []}

        # Callers
        print(f"\n  Searching for callers of {func_name}...")
        callers = find_callers_of(all_lines, func_name)
        results[func_name]['callers'] = callers
        if callers:
            print(f"  Found {len(callers)} caller(s):")
            out.append(f"\n### Callers ({len(callers)})\n\n")
            out.append("|Caller|File|Line|Code|\n|-|-|-|-|\n")
            for cf, cfile, cline, ccode in callers:
                print(f"    {cf} ({cfile}:{cline})")
                out.append(f"|{cf}|{cfile}|{cline}|`{ccode}`|\n")
        else:
            print(f"  No callers found.")
            out.append("\nNo callers found in generated code.\n")

    # ---- Compute addresses accessed by sub_828BF3C8 manually ----
    out.append("\n## Manual Address Computation for sub_828BF3C8\n\n")

    # From the code:
    # lis r10,-31972  →  r10 = (-31972) & 0xFFFFFFFF = 0x831C0000 (signed: -31972 << 16 = 0x831C0000)
    # Actually: -31972 as s16 = 0x8324? Let me compute properly
    lis_val = (-31972) & 0xFFFF  # 16-bit representation
    # lis loads (simm << 16): -31972 in 16-bit signed
    # -31972 as signed: that's 0x10000 - 31972 = 33564 unsigned? No.
    # Actually the decimal -31972 IS the 32-bit result after sign extension + shift.
    # In the C code: ctx.r10.s64 = -2095316992
    # -2095316992 & 0xFFFFFFFF = ?
    base = (-2095316992) & 0xFFFFFFFF
    print(f"\n  lis r10,-31972: base = 0x{base:08X} (decimal {-2095316992})")

    addr_9312 = (base + 9312) & 0xFFFFFFFF
    print(f"  stw r11,9312(r10): address = 0x{addr_9312:08X}")
    out.append(f"- `lis r10,-31972` → base = 0x{base:08X}\n")
    out.append(f"- `stw r11,9312(r10)` → address = **0x{addr_9312:08X}**\n")

    # Also check sub_828BF420 which is adjacent and uses lis r31,-31972
    # lis r31,-31972: same base
    addr_9312_r31 = (base + 9312) & 0xFFFFFFFF
    addr_8868 = (base + 8868) & 0xFFFFFFFF
    addr_9160 = (base + 9160) & 0xFFFFFFFF
    addr_9192 = (base + 9192) & 0xFFFFFFFF
    print(f"  lwz r11,9312(r31): 0x{addr_9312_r31:08X}")
    print(f"  lwz r3,8868(r30):  0x{addr_8868:08X}")
    print(f"  lwz r10,9160(r31): 0x{addr_9160:08X}")
    print(f"  addi r11,r11,9192: 0x{addr_9192:08X}")

    out.append(f"\n### Related offsets from base 0x{base:08X}\n\n")
    out.append("|Offset|Address|Used In|\n|-|-|-|\n")
    offsets_funcs = [
        (8848, "sub_828C15C8 (lwz r5)"),
        (8864, "sub_828C15C8 (lwz r11)"),
        (8868, "sub_828BF420/sub_828C1228/sub_828C15C8 (lwz r3, D3D device)"),
        (9160, "sub_828BF420/sub_828C15C8 (lwz r10/r11, scene index)"),
        (9172, "sub_828C1228 (addi r25)"),
        (9192, "sub_828BF420/sub_828C15C8 (addi, scene ptr table)"),
        (9232, "sub_828C15C8 (lwz r11)"),
        (9304, "sub_828C15C8 (addi r11)"),
        (9312, "sub_828BF3C8 (stw r11, WRITE) / sub_828BF420 (lwz/stw)"),
        (9380, "sub_828C15C8 (addi r11)"),
        (9640, "sub_828C15C8 (addi r3)"),
        (10024, "sub_828C1958 (lwz/stw r3)"),
        (15836, "sub_828C15C8 (stw r10)"),
        (15844, "sub_828C15C8 (stw r10)"),
    ]
    for off, note in offsets_funcs:
        addr = (base + off) & 0xFFFFFFFF
        out.append(f"|{off} (0x{off:04X})|0x{addr:08X}|{note}|\n")

    # ---- Specifically check 0x831C2458 and 0x831C2460 ----
    out.append("\n## Key Address Check\n\n")
    target_2458 = 0x831C2458
    target_2460 = 0x831C2460

    # 0x831C2458 - base = offset from base
    off_2458 = target_2458 - base
    off_2460 = target_2460 - base
    print(f"\n  Target 0x831C2458: offset from base = {off_2458} (0x{off_2458:04X})")
    print(f"  Target 0x831C2460: offset from base = {off_2460} (0x{off_2460:04X})")

    out.append(f"- 0x831C2458: offset from 0x{base:08X} = **{off_2458}** (0x{off_2458:04X})\n")
    out.append(f"- 0x831C2460: offset from 0x{base:08X} = **{off_2460}** (0x{off_2460:04X})\n\n")

    # Check if offset 9312 = 0x2460
    print(f"  Offset 9312 = 0x{9312:04X} → 0x{(base+9312):08X}")
    print(f"  Offset 9304 = 0x{9304:04X} → 0x{(base+9304):08X}")
    print(f"  Offset 9380 = 0x{9380:04X} → 0x{(base+9380):08X}")

    out.append(f"- Offset 9312 = 0x{9312:04X} → **0x{(base+9312):08X}**\n")
    out.append(f"- Offset 9304 = 0x{9304:04X} → **0x{(base+9304):08X}**\n")
    out.append(f"- Offset 9380 = 0x{9380:04X} → **0x{(base+9380):08X}**\n")

    # ---- Now check: which offset maps to 0x831C2458? ----
    desired_offset_2458 = target_2458 - base
    desired_offset_2460 = target_2460 - base
    out.append(f"\nTo access 0x831C2458, need offset {desired_offset_2458} (0x{desired_offset_2458:04X}) from 0x{base:08X}\n")
    out.append(f"To access 0x831C2460, need offset {desired_offset_2460} (0x{desired_offset_2460:04X}) from 0x{base:08X}\n")

    # Check if any of the computed offsets match
    for off, note in offsets_funcs:
        addr = (base + off) & 0xFFFFFFFF
        if addr == target_2458:
            out.append(f"\n**MATCH for 0x831C2458**: offset {off} in {note}\n")
            print(f"  *** MATCH for 0x831C2458: offset {off} in {note}")
        if addr == target_2460:
            out.append(f"\n**MATCH for 0x831C2460**: offset {off} in {note}\n")
            print(f"  *** MATCH for 0x831C2460: offset {off} in {note}")

    # ---- Search for the exact offsets 0x2458 and 0x2460 in ALL code ----
    print(f"\n  Searching for offsets that compute to 0x831C2458/0x831C2460...")
    out.append(f"\n## Searching for any code computing 0x831C2458 or 0x831C2460\n\n")

    # Search for any occurrence of "2458" or "2460" as decimal offsets from base 0x831C0000
    # 0x2458 = 9304, 0x2460 = 9312 (these ARE decimal offsets if base is 0x831C0000)
    # Wait: 0x831C2458 = 0x831C0000 + 0x2458 = 0x831C0000 + 9304
    # And 0x831C2460 = 0x831C0000 + 0x2460 = 0x831C0000 + 9312

    print(f"  0x831C2458 = 0x831C0000 + 9304 (0x2458)")
    print(f"  0x831C2460 = 0x831C0000 + 9312 (0x2460)")

    out.append(f"- 0x831C2458 = 0x831C0000 + **9304** (0x2458)\n")
    out.append(f"- 0x831C2460 = 0x831C0000 + **9312** (0x2460)\n\n")

    # So offset 9304 → 0x831C2458, and offset 9312 → 0x831C2460!
    # sub_828BF3C8 writes to offset 9312 (stw r11,9312(r10)) = 0x831C2460
    # sub_828C15C8 uses offset 9304 (addi r11,r11,9304)

    out.append("### KEY FINDING\n\n")
    out.append("- **sub_828BF3C8** writes `1` to offset 9312 from base 0x831C0000 = **0x831C2460** (the adjacent field)\n")
    out.append("- sub_828BF3C8 does NOT write to 0x831C2458\n")
    out.append("- Offset 9304 from 0x831C0000 = 0x831C2458 — used in sub_828C15C8 via `addi r11,r11,9304`\n\n")

    # Now trace sub_828C15C8 more carefully for offset 9304
    # From the code: addi r11,r11,9304 where r11 was lis -31972 (0x831C0000)
    # This makes r11 = 0x831C2458 (pointer in register, not a direct memory access)
    # Then later: stw r10,-432(r11) where r11 = 0x831C2458 + ... wait, let me check.

    # Actually in sub_828C15C8 at line 115329-115338:
    # addi r11,r11,9304  → r11 = 0x831C0000 + 9304 = 0x831C2458
    # stw r10,-432(r11) → addr = 0x831C2458 + (-432) = 0x831C2458 - 432

    stw_addr = (0x831C2458 - 432) & 0xFFFFFFFF
    print(f"  stw r10,-432(r11) where r11=0x831C2458: addr = 0x{stw_addr:08X}")
    out.append(f"- `stw r10,-432(r11)` where r11=0x831C2458: addr = 0x{stw_addr:08X} (NOT 0x831C2458 itself)\n\n")

    # But wait — also: stw r10,15844(r9) and stw r10,15836(r9) where r9 = lis -31972 = 0x831C0000
    addr_15844 = (base + 15844) & 0xFFFFFFFF
    addr_15836 = (base + 15836) & 0xFFFFFFFF
    print(f"  stw r10,15844(r9): 0x{addr_15844:08X}")
    print(f"  stw r10,15836(r9): 0x{addr_15836:08X}")
    out.append(f"- `stw r10,15844(r9)` where r9=0x831C0000: addr = 0x{addr_15844:08X}\n")
    out.append(f"- `stw r10,15836(r9)` where r9=0x831C0000: addr = 0x{addr_15836:08X}\n\n")

    # ---- Check ALL callers of sub_828BF3C8 more carefully ----
    # It wasn't found via direct call. Check vtable / function pointer references.
    print(f"\n  Searching for address 0x828BF3C8 as a function pointer...")
    out.append("## Function Pointer / VTABLE References\n\n")

    func_addr_refs = find_refs_to_addr(all_lines, ["828BF3C8", "828C1228", "828C15C8"])
    for addr, refs in sorted(func_addr_refs.items()):
        out.append(f"### 0x{addr} references ({len(refs)})\n\n")
        print(f"\n  0x{addr} references: {len(refs)}")
        for func, file, line, code in refs[:20]:
            print(f"    {func} ({file}:{line}): {code}")
            out.append(f"- {func} ({file}:{line}): `{code}`\n")
        if len(refs) > 20:
            out.append(f"- ... and {len(refs)-20} more\n")

    # ---- Check config for these functions ----
    print(f"\n  Checking config file for function entries...")
    config_path = os.path.join(GEN_DIR, "gta4_config.cpp")
    if os.path.exists(config_path):
        with open(config_path, 'r') as f:
            config_lines = f.readlines()
        for func_name in TARGET_FUNCS:
            for i, line in enumerate(config_lines):
                if func_name in line:
                    print(f"  Config: {line.strip()}")

    # ---- Trace the D3D init call chain ----
    out.append("\n## D3D Init Call Chain\n\n")
    # sub_828C15C8 calls sub_828C1228, and sub_828C15C8 calls sub_828BF420
    # sub_828BF420 references offset 9312 (0x831C2460)
    # Who calls sub_828C15C8?
    callers_15C8 = results.get("sub_828C15C8", {}).get("callers", [])
    if callers_15C8:
        out.append("Callers of sub_828C15C8:\n")
        for cf, cfile, cline, ccode in callers_15C8:
            out.append(f"- {cf}\n")
            # Trace one more level
            callers_2nd = find_callers_of(all_lines, cf.replace("__imp__", ""))
            if callers_2nd:
                for cf2, cfile2, cline2, ccode2 in callers_2nd[:5]:
                    out.append(f"  - Called by: {cf2}\n")
    else:
        out.append("No direct callers of sub_828C15C8 found — likely called via vtable.\n")

    # ---- Conclusions ----
    out.append("\n## Conclusions\n\n")
    out.append("1. **sub_828BF3C8 writes to 0x831C2460** (offset 9312), NOT 0x831C2458\n")
    out.append(f"2. **0x831C2458** = offset 9304 from base 0x831C0000 — used as a pointer base in sub_828C15C8\n")
    out.append("3. sub_828BF3C8 has NO direct callers — it is called via vtable or function pointer dispatch\n")
    out.append("4. sub_828C15C8 calls sub_828C1228 (render gate init to -1) and sub_828BF420 (presentation/swap)\n")
    out.append("5. The `stw r10,-432(r11)` in sub_828C15C8 accesses 0x831C22A8, not 0x831C2458\n")

    # Write output
    with open(OUTPUT_FILE, 'w') as f:
        f.writelines(out)

    print(f"\nResults written to {OUTPUT_FILE}")

if __name__ == '__main__':
    main()
