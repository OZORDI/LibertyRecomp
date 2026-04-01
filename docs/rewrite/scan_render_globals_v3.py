#!/usr/bin/env python3
"""
V3: The codegen uses decimal offsets from registers.
We need to find where registers get loaded with 0x831C0000-region base addresses,
then track which offsets are used from those bases.

Strategy:
1. Search for how the high half 0x831C gets loaded. PPC lis = addis rD,0,imm
   which the codegen emits as: ctx.rX.s64 = (int32_t)(int16_t)val << 16
   For 0x831C: (int16_t)0x831C = -31972, shifted left 16 = 0x831C0000
   In decimal: -31972 << 16 = sign-extended... Let's compute properly.

2. Search for the specific constant that represents 0x831C0000 in the codegen.

3. Also: addis can use a non-zero base (rA != 0), building addr = rA + (imm << 16)
"""

# Compute the decimal form of 0x831C as a signed 16-bit value
val_831C = 0x831C
if val_831C >= 0x8000:
    signed_831C = val_831C - 0x10000
else:
    signed_831C = val_831C

shifted = signed_831C * 65536  # << 16, in signed arithmetic
unsigned_shifted = shifted & 0xFFFFFFFF

print(f"0x831C as signed 16-bit: {signed_831C}")
print(f"Shifted left 16: {shifted} (signed), 0x{unsigned_shifted:08X} (unsigned)")
print(f"In codegen, lis r3, 0x831C becomes: ctx.r3.s64 = {shifted}")
print(f"Or: ctx.r3.u64 = {unsigned_shifted}")
print()

# The codegen for lis typically does: ctx.rX.s64 = CONSTANT;
# where CONSTANT is the sign-extended (int32_t)(int16_t) << 16 value
# 0x831C << 16 = 0x831C0000 = 2199552000 unsigned = -2095415296 signed 32-bit

import ctypes
s32 = ctypes.c_int32(unsigned_shifted).value
print(f"0x831C0000 as signed 32-bit: {s32}")
print(f"0x831C0000 as unsigned 32-bit: {unsigned_shifted}")

# So we search for both the signed and unsigned decimal representations
# lis pattern: ctx.rX.s64 = -2095415296  (= 0x831C0000)
# or:          ctx.rX.u64 = 2199552000

# BUT: the codegen might also fold lis+addi into a single constant.
# lis r3, 0x831C ; addi r3, r3, 0x2458 => r3 = 0x831C0000 + 0x2458 = 0x831C2458
# If 0x2458 is positive (< 0x8000), the codegen might fold to:
# ctx.r3.s64 = 0x831C2458 (= 2199561304 unsigned, or as signed...)

target_u32 = 0x831C2458
target_s32 = ctypes.c_int32(target_u32).value
print(f"\n0x831C2458 as signed 32-bit: {target_s32}")
print(f"0x831C2458 as unsigned 32-bit: {target_u32}")

devcfg_u32 = 0x831C2210
devcfg_s32 = ctypes.c_int32(devcfg_u32).value
print(f"\n0x831C2210 as signed 32-bit: {devcfg_s32}")
print(f"0x831C2210 as unsigned 32-bit: {devcfg_u32}")

# But more importantly: if addi has a negative offset (>= 0x8000),
# the lis gets adjusted: lis uses 0x831D, addi uses negative offset
# lis r3, 0x831D => 0x831D0000
# addi r3, r3, -0xBDA8 => 0x831D0000 - 0xBDA8 = 0x831C4258? No...
# Let's check: for addr 0x831C2458:
#   high = 0x831C, low = 0x2458
#   Since 0x2458 < 0x8000, no adjustment needed.
#   lis = 0x831C, addi = 0x2458
# For addr 0x831C8xxx (low >= 0x8000):
#   lis = 0x831D, addi = low - 0x10000 (negative)

print("\n--- Searching for constants ---")

import os, glob, re
from collections import defaultdict

GEN_DIR = "/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated"
cpp_files = sorted(glob.glob(os.path.join(GEN_DIR, "*.cpp")))

TABLE_BASE = 0x831C0000
DEV_CFG_BASE = 0x831C2210
TARGET_ADDR = 0x831C2458

func_pattern = re.compile(r'void\s+(sub_[0-9A-Fa-f]+|__imp__sub_[0-9A-Fa-f]+)\s*\(')

# Search for the base constant 0x831C0000 in both signed and unsigned decimal
base_patterns = [
    str(s32),                     # -2095415296
    str(unsigned_shifted),        # 2199552000
]

# Search for the folded constants for key addresses in range 0x831C2000-0x831C2FFF
# Generate all possible folded constant patterns
folded_targets = {}
for addr in range(0x831C2000, 0x831C3000, 4):
    s = ctypes.c_int32(addr).value
    u = addr
    folded_targets[str(s)] = addr
    folded_targets[str(u)] = addr

# Build a regex that matches any of these constants when they appear as standalone numbers
# This is a lot of numbers, so let's be smart about it.
# All addresses in 0x831C2000-0x831C2FFF as signed are in range:
lo_s = ctypes.c_int32(0x831C2000).value
hi_s = ctypes.c_int32(0x831C2FFF).value
lo_u = 0x831C2000
hi_u = 0x831C2FFF
print(f"Signed range: {lo_s} to {hi_s}")
print(f"Unsigned range: {lo_u} to {hi_u}")

# Key specific addresses to search for:
key_addrs = [
    0x831C0000,  # table base
    0x831C2210,  # device config
    0x831C2458,  # scene pointer target
]

# Also check: maybe the codegen just uses the hex form with 0x prefix
# Let me also look for addis patterns with the specific immediate

# In PPC recomp codegen, lis/addis often becomes:
# ctx.rX.s64 = (some_constant);  where the constant = (int16_t)imm << 16
# Then ori/addi becomes: ctx.rX.u32 = ctx.rX.u32 | low_bits; or + low_bits

# Let's search for ALL forms of 0x831C
search_terms = set()
for addr in key_addrs:
    s = ctypes.c_int32(addr).value
    search_terms.add(str(abs(s)))  # absolute value to catch -XXX patterns too
    search_terms.add(str(addr))
    search_terms.add(f"0x{addr:08X}")
    search_terms.add(f"0x{addr:08x}")
    search_terms.add(f"0x{addr:X}")
    search_terms.add(f"0x{addr:x}")

# Also search for the lis constant: -2095415296 / 2199552000
# And the decimal form of 0x831C = 33564
search_terms.add("33564")  # 0x831C in decimal
search_terms.add("831C")   # hex without prefix

# For offsets from the base (used in addi):
# 0x2458 = 9304, 0x2210 = 8720
search_terms.add("9304")   # 0x2458 offset
search_terms.add("8720")   # 0x2210 offset

print(f"\nSearching for {len(search_terms)} terms...")

# Main scan: look for the signed decimal of 0x831C0000
results = defaultdict(list)
base_load_results = []

for fpath in cpp_files:
    fname = os.path.basename(fpath)
    current_func = None
    with open(fpath, 'r', errors='replace') as f:
        for line_no, line in enumerate(f, 1):
            fm = func_pattern.search(line)
            if fm:
                current_func = fm.group(1)

            stripped = line.strip()

            # Check for base load: the signed decimal -2095415296 or unsigned 2199552000
            if '2095415296' in stripped or '2199552000' in stripped:
                base_load_results.append((fname, line_no, stripped, current_func))

            # Check for folded target address constants
            if '2199561304' in stripped or '2095406792' in stripped:  # 0x831C2458 unsigned/signed(abs)
                results['0x831C2458_folded'].append((fname, line_no, stripped, current_func))

            if '2199560720' in stripped or '2095407376' in stripped:  # 0x831C2210
                results['0x831C2210_folded'].append((fname, line_no, stripped, current_func))

            # Check for offset 9304 in memory ops (0x2458)
            if '9304' in stripped and ('PPC_LOAD' in stripped or 'PPC_STORE' in stripped or 'MEM_' in stripped):
                results['offset_9304'].append((fname, line_no, stripped, current_func))

            # Check for offset 8720 in memory ops (0x2210)
            if '8720' in stripped and ('PPC_LOAD' in stripped or 'PPC_STORE' in stripped or 'MEM_' in stripped):
                results['offset_8720'].append((fname, line_no, stripped, current_func))

print(f"\n=== BASE LOAD (0x831C0000) ===")
print(f"Found: {len(base_load_results)}")
for fname, line_no, line, func in base_load_results[:30]:
    print(f"  {func} ({fname}:{line_no}): {line[:200]}")

for key in sorted(results.keys()):
    print(f"\n=== {key} ===")
    print(f"Found: {len(results[key])}")
    for fname, line_no, line, func in results[key][:30]:
        print(f"  {func} ({fname}:{line_no}): {line[:200]}")

# Now, let's compute the ACTUAL signed/unsigned for ALL addresses we care about
print("\n=== PRECISE CONSTANT TABLE ===")
for addr in [0x831C0000, 0x831C2000, 0x831C2210, 0x831C2458, 0x831C2500]:
    s = ctypes.c_int32(addr).value
    print(f"  0x{addr:08X} = {addr} (unsigned) = {s} (signed)")
