#!/usr/bin/env python3
"""
Verify exact addresses for stores near the 0x831C2458 computation
in gta4_recomp.58.cpp (sub_828C15C8 render dispatch).
"""

# From gta4_recomp.58.cpp lines 115324-115340:
# ctx.r9  = -2095316992  (lis r9,-31972)
# ctx.r11 = -2095316992  (lis r11,-31972)
# ctx.r11 = ctx.r11 + 9304  (addi r11,r11,9304)
# ctx.r28 = -2095316992  (lis r28,-31972)

# Compute -2095316992 as unsigned 32-bit
r9_val  = -2095316992 & 0xFFFFFFFF
r11_val = (-2095316992 + 9304) & 0xFFFFFFFF
r28_val = -2095316992 & 0xFFFFFFFF

print(f"r9  = {r9_val:#010x} = lis(-31972)")
print(f"r11 = {r11_val:#010x} = lis(-31972) + 9304 = 0x831C0000 + 0x2458")
print(f"r28 = {r28_val:#010x} = lis(-31972)")
print()

# Store 1: PPC_STORE_U32(ctx.r9.u32 + 15844, ctx.r10.u32)
addr1 = (r9_val + 15844) & 0xFFFFFFFF
print(f"Store 1: r9 + 15844 = {addr1:#010x}")

# Store 2: PPC_STORE_U32(ctx.r11.u32 + -432, ctx.r10.u32)
# r11 = 0x831C2458, so r11 - 432 = ?
addr2 = (r11_val + (-432 & 0xFFFFFFFF)) & 0xFFFFFFFF
# Better: just add as signed
addr2 = (r11_val - 432) & 0xFFFFFFFF
print(f"Store 2: r11 + (-432) = r11 - 432 = {addr2:#010x}")

# Store 3: PPC_STORE_U32(ctx.r9.u32 + 15836, ctx.r10.u32)
addr3 = (r9_val + 15836) & 0xFFFFFFFF
print(f"Store 3: r9 + 15836 = {addr3:#010x}")

print()
print("--- Are any of these in 0x831C0000-0x831CFFFF? ---")
for name, addr in [("Store 1", addr1), ("Store 2", addr2), ("Store 3", addr3)]:
    in_range = 0x831C0000 <= addr <= 0x831CFFFF
    print(f"  {name}: {addr:#010x} -> {'YES' if in_range else 'NO'}")

# Now verify the addi 9304 hits that are NOT 0x831C based
print()
print("--- Verifying other addi+9304 sites ---")

# gta4_recomp.38.cpp: lis r11,-32151 then addi r11,r11,9304
r11_38 = -2107047936 & 0xFFFFFFFF
r11_38_addi = (r11_38 + 9304) & 0xFFFFFFFF
print(f"gta4_recomp.38.cpp: lis(-32151) = {r11_38:#010x}, + 9304 = {r11_38_addi:#010x}")
print(f"  In 0x831C range? {'YES' if 0x831C0000 <= r11_38_addi <= 0x831CFFFF else 'NO'}")

# gta4_recomp.31.cpp: lis r10,-32253 then addi r3,r10,9304
r10_31 = -2113732608 & 0xFFFFFFFF
r10_31_addi = (r10_31 + 9304) & 0xFFFFFFFF
print(f"gta4_recomp.31.cpp: lis(-32253) = {r10_31:#010x}, + 9304 = {r10_31_addi:#010x}")
print(f"  In 0x831C range? {'YES' if 0x831C0000 <= r10_31_addi <= 0x831CFFFF else 'NO'}")

# Now: check what is -2095316992
print()
val = -2095316992
print(f"-2095316992 as hex (signed 64-bit): {val:#018x}")
print(f"-2095316992 as hex (unsigned 32-bit): {val & 0xFFFFFFFF:#010x}")
print(f"This is: 0x831C0000")

# Verify 0x2458 = 9304
print(f"\n0x2458 = {0x2458} decimal")
print(f"9304 = {9304:#x} hex")
