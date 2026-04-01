#!/usr/bin/env python3
"""
Analyze sub_82242910 — the 15-state scene creation state machine.
Identifies all global writes, function calls per state, and where
the scene object pointer ends up.
"""

import re
import struct

SRC = "/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.6.cpp"

# Read full function body
with open(SRC, "r") as f:
    lines = f.readlines()

# Find function boundaries
start_line = None
end_line = None
brace_depth = 0
for i, line in enumerate(lines):
    if "PPC_FUNC_IMPL(__imp__sub_82242910)" in line and "{" in line:
        start_line = i
        brace_depth = 1
        continue
    if start_line is not None and i > start_line:
        brace_depth += line.count("{") - line.count("}")
        if brace_depth == 0:
            end_line = i
            break

func_lines = lines[start_line:end_line + 1]
func_text = "".join(func_lines)

print(f"Function spans lines {start_line+1} to {end_line+1} ({end_line - start_line + 1} lines)")
print()

# =========================================================================
# 1. Resolve register base values used throughout
# =========================================================================

# r26 is set to lis -32064 => compute
def lis_val(signed_val):
    """lis loads (signed_val & 0xFFFF) << 16, sign-extended to 32 bits"""
    return signed_val & 0xFFFFFFFF

r26_base = lis_val(-2101346304)  # 0x82B00000
r27_base = lis_val(-2101346304)  # 0x82B00000
r30_base = lis_val(-2101411840)  # 0x82AF0000
r31_init = lis_val(-2095513600)  # 0x82F90000  (but r31 gets reassigned in different states)
r28_init = lis_val(-2101411840)  # 0x82AF0000 + 14944

print("=== REGISTER BASE VALUES ===")
print(f"r26 (lis -32064) = 0x{r26_base:08X}")
print(f"r27 (lis -32064) = 0x{r27_base:08X}")
print(f"r30 (lis -32065) = 0x{lis_val(-2101411840):08X}")
print(f"r28 = r30_base + 14944 = 0x{lis_val(-2101411840) + 14944:08X}")
print(f"r29 = r27_base + (-26168) = 0x{(r27_base + (-26168 & 0xFFFF)) & 0xFFFFFFFF:08X}")
# Actually need to handle signed offsets properly
def add_signed(base, offset):
    return (base + offset) & 0xFFFFFFFF

r29_val = add_signed(lis_val(-2101346304), -26168)  # same as r11_base + (-26168)
r28_val = add_signed(lis_val(-2101411840), 14944)

print(f"r29 = 0x{r29_val:08X}")
print(f"r28 = 0x{r28_val:08X}")
print()

# =========================================================================
# 2. Identify state counter address
# =========================================================================
# State is loaded from: lwz r10,-26552(r26) => r26 + (-26552)
state_addr = add_signed(r26_base, -26552)
print(f"=== STATE COUNTER ADDRESS ===")
print(f"r26 + (-26552) = 0x{r26_base:08X} + 0x{(-26552) & 0xFFFF:04X} = 0x{state_addr:08X}")
# Let's verify: -26552 in signed 16-bit
offset_u16 = (-26552) & 0xFFFF
print(f"  offset as u16 = 0x{offset_u16:04X} = {-26552}")
state_addr_correct = (r26_base + ((-26552) % (1 << 32))) & 0xFFFFFFFF
print(f"  0x{r26_base:08X} + {-26552} = 0x{state_addr_correct:08X}")
print()

# =========================================================================
# 3. Find ALL PPC_STORE patterns with resolved addresses
# =========================================================================
print("=== ALL GLOBAL WRITES (PPC_STORE with register-offset patterns) ===")
print()

# Track all stores with known register bases
# Pattern: PPC_STORE_U32(ctx.rXX.u32 + OFFSET, ...)
store_pat = re.compile(r'PPC_STORE_(U8|U16|U32|U64)\(ctx\.r(\d+)\.u32 \+ (-?\d+),\s*ctx\.r(\d+)\.(u8|u16|u32)\)')

# Build a map of register values at different points
# From the prologue setup:
reg_bases = {
    26: r26_base,     # lis r26,-32064
    27: r27_base,     # lis r27,-32064  (set same)
    30: lis_val(-2101411840),  # lis r30,-32065
}

# Collect all stores
stores = []
current_label = "prologue"
for line in func_lines:
    # Track labels
    m = re.match(r'(loc_[0-9A-Fa-f]+):', line)
    if m:
        current_label = m.group(1)

    m = store_pat.search(line)
    if m:
        width = m.group(1)
        base_reg = int(m.group(2))
        offset = int(m.group(3))
        val_reg = int(m.group(4))

        if base_reg in reg_bases:
            addr = add_signed(reg_bases[base_reg], offset)
            stores.append((current_label, addr, width, base_reg, offset, val_reg, line.strip()))

# Also catch stores where the base is r10/r11 with known values
# We need to track lis+offset for r10, r11 patterns
# Pattern: ctx.r10.s64 = -2102853632; ... PPC_STORE(r10+offset, ...)
# -2102853632 = 0x82A90000
r10_alt = lis_val(-2102853632)
print(f"r10 alt base (lis -32087) = 0x{r10_alt:08X}")

# Re-scan with tracking of r10/r11 lis assignments
lis_pat = re.compile(r'ctx\.r(\d+)\.s64 = (-?\d+);')
current_label = "prologue"
r_vals = dict(reg_bases)  # copy base regs
all_stores = []

for line in func_lines:
    m = re.match(r'\s*(loc_[0-9A-Fa-f]+):', line)
    if m:
        current_label = m.group(1)

    # Track lis assignments
    m = lis_pat.search(line)
    if m and "// lis" in line:
        reg = int(m.group(1))
        val = int(m.group(2))
        r_vals[reg] = val & 0xFFFFFFFF

    # Track addi
    if "// addi" in line:
        m2 = re.search(r'ctx\.r(\d+)\.s64 = ctx\.r(\d+)\.s64 \+ (-?\d+);', line)
        if m2:
            dst = int(m2.group(1))
            src = int(m2.group(2))
            off = int(m2.group(3))
            if src in r_vals:
                r_vals[dst] = add_signed(r_vals[src], off)

    # Now check stores
    m = store_pat.search(line)
    if m:
        width = m.group(1)
        base_reg = int(m.group(2))
        offset = int(m.group(3))
        val_reg = int(m.group(4))

        if base_reg in r_vals:
            addr = add_signed(r_vals[base_reg], offset)
            if 0x82000000 <= addr <= 0x83FFFFFF:
                all_stores.append((current_label, addr, width, val_reg, line.strip()))

# Deduplicate and print
seen_addrs = {}
for label, addr, width, val_reg, code in all_stores:
    key = (addr, width)
    if key not in seen_addrs:
        seen_addrs[key] = []
    seen_addrs[key].append((label, val_reg, code))

for (addr, width), entries in sorted(seen_addrs.items()):
    print(f"0x{addr:08X} ({width}):")
    for label, val_reg, code in entries:
        print(f"  [{label}] r{val_reg} | {code}")
    print()

# =========================================================================
# 4. State transitions - what each state does
# =========================================================================
print("=== STATE MACHINE TRANSITIONS ===")
print()

# Find all "stw r11,-26552(r26)" patterns (state counter writes)
state_write_pat = re.compile(r'PPC_STORE_U32\(ctx\.r26\.u32 \+ -26552, ctx\.r11\.u32\)')
current_label = "prologue"
state_writes = []
for line in func_lines:
    m = re.match(r'\s*(loc_[0-9A-Fa-f]+):', line)
    if m:
        current_label = m.group(1)
    if state_write_pat.search(line):
        # Find preceding "li r11, X" to get the value
        state_writes.append((current_label, line.strip()))

# Better approach: find "li r11, X" followed by state store
li_then_store = re.compile(
    r'ctx\.r11\.s64 = (\d+);\s*\n.*?PPC_STORE_U32\(ctx\.r26\.u32 \+ -26552, ctx\.r11\.u32\)',
    re.MULTILINE
)

for m in li_then_store.finditer(func_text):
    val = int(m.group(1))
    # Find which label this is under
    pos = m.start()
    preceding = func_text[:pos]
    labels = re.findall(r'(loc_[0-9A-Fa-f]+):', preceding)
    label = labels[-1] if labels else "prologue"
    print(f"  State → {val:2d}  (at {label})")

print()

# =========================================================================
# 5. Function calls per state
# =========================================================================
print("=== FUNCTION CALLS PER STATE ===")
print()

# Map labels to states
label_to_state = {
    "loc_822429A0": 0,
    "loc_822429EC": 1,
    "loc_82242A4C": 2,
    "loc_82242AD0": 3,
    "loc_82242B1C": 4,
    "loc_82242C88": 5,
    "loc_82242D18": 6,
    "loc_82242D9C": 7,
    "loc_82242E08": 8,
    "loc_82242EE4": 9,
    "loc_82242F50": 10,
    "loc_82243058": 11,
    "loc_822430B4": 12,
    "loc_822430F0": 14,
    "loc_822431DC": 13,
}

# Split function into state sections
state_sections = {}
current_state = -1  # prologue
section_start = 0

for i, line in enumerate(func_lines):
    m = re.match(r'\s*(loc_[0-9A-Fa-f]+):', line)
    if m:
        label = m.group(1)
        if label in label_to_state:
            if current_state >= 0 or current_state == -1:
                pass  # handled below
            current_state = label_to_state[label]
            state_sections.setdefault(current_state, [])
        if current_state >= 0:
            state_sections.setdefault(current_state, []).append(line)
    elif current_state >= 0:
        state_sections.setdefault(current_state, []).append(line)

# Extract function calls from each state section
call_pat = re.compile(r'\b(sub_[0-9A-Fa-f]+)\(ctx, base\)')
for state in sorted(state_sections.keys()):
    section_text = "".join(state_sections[state])
    calls = call_pat.findall(section_text)
    unique_calls = list(dict.fromkeys(calls))  # preserve order, dedup
    print(f"State {state:2d}: {', '.join(unique_calls)}")

print()

# =========================================================================
# 6. Key addresses and their roles
# =========================================================================
print("=== KEY GLOBAL ADDRESSES ===")
print()

# State counter
print(f"STATE_COUNTER: 0x{state_addr_correct:08X}  (r26 + -26552)")

# Other key addresses from stores
# r26 + -26556 (loaded, not stored in this func)
load_addr_26556 = add_signed(r26_base, -26556)
# Actually let me recompute properly
print(f"  r26 = 0x{r26_base:08X}")
print(f"  r26 + (-26552): state counter")

# r31 + (-26164) used for store
r31_b00_base = lis_val(-2101346304)  # when r31 = lis -32064 = 0x82B00000
addr_26164 = add_signed(r31_b00_base, -26164)
print(f"  r31(0x82B00000) + (-26164) = 0x{addr_26164:08X}  (scene-related field)")

# r31 + (-26594) - byte store
addr_26594 = add_signed(r31_b00_base, -26594)
print(f"  r31(0x82B00000) + (-26594) = 0x{addr_26594:08X}  (byte flag)")

# r31 + (-26593)
addr_26593 = add_signed(r31_b00_base, -26593)
print(f"  r31(0x82B00000) + (-26593) = 0x{addr_26593:08X}  (byte flag, loaded)")

# r10 = -2102853632 = 0x82A90000, store at +21612
r10_82a9 = lis_val(-2102853632)
addr_21612 = add_signed(r10_82a9, 21612)
print(f"  r10(0x{r10_82a9:08X}) + 21612 = 0x{addr_21612:08X}  (error/status code)")

# r10 + 21606 (byte)
addr_21606 = add_signed(r10_82a9, 21606)
print(f"  r10(0x{r10_82a9:08X}) + 21606 = 0x{addr_21606:08X}  (byte flag, loaded/stored)")

# r10(0x82AF0000) + 14966
r10_af = lis_val(-2101411840)
addr_14966 = add_signed(r10_af, 14966)
print(f"  r10(0x{r10_af:08X}) + 14966 = 0x{addr_14966:08X}  (byte clear)")

# r10(0x82AF0000) + 15578
addr_15578 = add_signed(r10_af, 15578)
print(f"  r10(0x{r10_af:08X}) + 15578 = 0x{addr_15578:08X}  (byte clear in state 13)")

# r30(0x82AF0000) + 15760
addr_15760 = add_signed(r10_af, 15760)
print(f"  r30(0x82AF0000) + 15760 = 0x{addr_15760:08X}  (scene ID / loaded frequently)")

# r31_init(0x82F90000) + 11344 = the "scene struct" passed to many calls
r31_f9 = lis_val(-2095513600)
scene_struct = add_signed(r31_f9, 11344)
print(f"  r31(0x{r31_f9:08X}) + 11344 = 0x{scene_struct:08X}  (SCENE STRUCT base addr)")

# r28 = r30 + 14944
print(f"  r28 = r30(0x82AF0000) + 14944 = 0x{r28_val:08X}  (passed as r5 to sub_822417B0)")

# r29 in state 12/14 = r27 + (-26168) = address of data struct
print(f"  r29 = r27(0x82B00000) + (-26168) = 0x{r29_val:08X}  (data struct / loaded in state 14)")

# r30 + 15764
addr_15764 = add_signed(r10_af, 15764)
print(f"  r30(0x82AF0000) + 15764 = 0x{addr_15764:08X}  (passed to sub_8284ABD0)")

# r30 + 15768
addr_15768 = add_signed(r10_af, 15768)
print(f"  r30(0x82AF0000) + 15768 = 0x{addr_15768:08X}  (passed to sub_8284ABA0)")

# r27 + (-26556) - loaded, not stored
addr_26556 = add_signed(r27_base, -26556)
print(f"  r27(0x82B00000) + (-26556) = 0x{addr_26556:08X}  (loaded: sub-state/mode)")

# r27 + (-26164) - loaded/stored
addr_27_26164 = add_signed(r27_base, -26164)
print(f"  r27(0x82B00000) + (-26164) = 0x{addr_27_26164:08X}  (same as r31 version)")

print()

# =========================================================================
# 7. What happens when all 15 states complete?
# =========================================================================
print("=== COMPLETION ANALYSIS ===")
print()
print("The state machine goes through states 0 -> 14.")
print("State 13 (loc_822431DC) is the FINAL meaningful state.")
print()
print("In state 13 (at loc_822431DC):")
print("  1. Calls sub_8223DB20 (check something)")
print("  2. Calls sub_82240B78 (check something)")
print("  3. Calls sub_8223F9F0(r3=4, r4=neg(r11), r5=&stack80)")
print(f"     where r11 = PPC_LOAD_U32(0x{add_signed(lis_val(-2101346304), -26168):08X})")
print("  4. If success and stack[80] != 0:")
print(f"     - Stores 0 (byte) to 0x{add_signed(lis_val(-2102853632), 21606):08X}")
print(f"     - Stores 0 (byte) to 0x{add_signed(lis_val(-2101411840), 15578):08X}")
print("     - Calls sub_8223DB90")
print(f"     - Stores 0 to STATE_COUNTER (0x{state_addr_correct:08X}) → resets to state 0")
print()
print("The scene object is built incrementally across states:")
print(f"  Scene struct base address: 0x{scene_struct:08X}")
print(f"  This is passed as r3 to most sub_8284XXXX functions")
print()

# =========================================================================
# 8. Trace what sub_822417B0 does (states 12 and 14)
# =========================================================================
print("=== sub_822417B0 (called in states 12 and 14) ===")
print()
print("State 12 args: r3=0, r4=1, r5=r28, r6=r27[-26164], r7=r29, r8=&stack84, r9=&stack88")
print("State 14 args: r3=0, r4=0, r5=r28, r6=r27[-26164], r7=r29, r8=&stack88, r9=&stack84")
print(f"  r28 = 0x{r28_val:08X}")
print(f"  r29 = 0x{r29_val:08X}")
print(f"  r27[-26164] loaded from 0x{add_signed(r27_base, -26164):08X}")
print()

# =========================================================================
# 9. Summarize all global writes with actual computed addresses
# =========================================================================
print("=== SUMMARY: ALL GLOBAL WRITES BY sub_82242910 ===")
print()

# Deduplicated list
all_writes = set()

# Re-parse more carefully
current_lis_vals = {}
current_label = "prologue"

for line in func_lines:
    m = re.match(r'\s*(loc_[0-9A-Fa-f]+):', line)
    if m:
        current_label = m.group(1)

    # Track lis
    if "// lis" in line:
        m = re.search(r'ctx\.r(\d+)\.s64 = (-?\d+);', line)
        if m:
            current_lis_vals[int(m.group(1))] = int(m.group(2)) & 0xFFFFFFFF

    # Track addi
    if "// addi" in line:
        m = re.search(r'ctx\.r(\d+)\.s64 = ctx\.r(\d+)\.s64 \+ (-?\d+);', line)
        if m:
            dst, src, off = int(m.group(1)), int(m.group(2)), int(m.group(3))
            if src in current_lis_vals:
                current_lis_vals[dst] = add_signed(current_lis_vals[src], off)

    # PPC_STORE
    m = re.search(r'PPC_STORE_(U8|U16|U32|U64)\(ctx\.r(\d+)\.u32 \+ (-?\d+),', line)
    if m:
        width = m.group(1)
        base_reg = int(m.group(2))
        offset = int(m.group(3))
        if base_reg in current_lis_vals:
            addr = add_signed(current_lis_vals[base_reg], offset)
            if 0x82000000 <= addr <= 0x83FFFFFF:
                all_writes.add((addr, width, current_label))

print(f"{'Address':>12} {'Width':>5}  Label")
print("-" * 50)
for addr, width, label in sorted(all_writes):
    print(f"  0x{addr:08X}  {width:>4}   {label}")

print()

# =========================================================================
# 10. The ultimate question: where does the scene pointer end up?
# =========================================================================
print("=== WHERE DOES THE SCENE OBJECT POINTER END UP? ===")
print()
print(f"The 'scene struct' base is the STATIC global at 0x{scene_struct:08X}")
print("(computed from lis r11,-31975 ; addi r31,r11,11344)")
print()
print("This address is passed as r3 to the following scene construction functions:")
print("  sub_8284B4B0 (state 0) - scene init with sceneID")
print("  sub_8284AAE0 (state 6) - scene create/configure")
print("  sub_8284AB10 (state 8) - scene check readiness")
print("  sub_8284AB70 (state 8) - scene operation")
print("  sub_8284B490 (state 8,10) - scene status query")
print("  sub_8284B430 (state 8,10) - scene state query")
print("  sub_8284ABA0 (state 9) - scene add something (r7=75)")
print("  sub_8284ABD0 (state 10) - scene check with ptr param")
print("  sub_8284ABF8 (state 10) - scene operation")
print()
print(f"The 'scene ID' is loaded from 0x{addr_15760:08X} (r30 + 15760)")
print()
print("When state 13 completes successfully, it:")
print(f"  1. Clears flag at 0x{add_signed(lis_val(-2102853632), 21606):08X}")
print(f"  2. Clears flag at 0x{add_signed(lis_val(-2101411840), 15578):08X}")
print("  3. Calls sub_8223DB90 (finalization)")
print(f"  4. Resets state counter at 0x{state_addr_correct:08X} to 0")
print()
print("The scene object is NOT stored to a new pointer location by sub_82242910.")
print(f"It is built IN-PLACE at the static global 0x{scene_struct:08X}.")
print("The function modifies the scene struct directly — it's not allocated or")
print("moved. The caller already knows the address.")
print()

# Check: is 0x82FC2C50 ever stored anywhere as a pointer?
# Search for stores where r31 is used as a VALUE (not base)
print("=== CHECKING: Is scene struct address stored as a pointer value? ===")
# Look for stw r31,...  in the function
stw_r31_pat = re.compile(r'PPC_STORE_U32\(.+?, ctx\.r31\.u32\)')
for i, line in enumerate(func_lines):
    if stw_r31_pat.search(line):
        # r31 might hold 0x82FC2C50 in some states
        print(f"  Line {start_line + i + 1}: {line.strip()}")

print()
print("=== CHECKING: stw r3 after sub_8223CB60 (which may return scene ptr) ===")
# sub_8223CB60 is called in state 2 and 7, result goes to r31 sometimes
# State 2: mr r31,r3 after sub_8223CB60 — but r31 value not stored globally

# Final output
print()
print("=" * 70)
print("CONCLUSION")
print("=" * 70)
print()
print(f"sub_82242910 operates on a STATIC scene struct at 0x{scene_struct:08X}.")
print()
print("Global addresses WRITTEN by this function:")
print(f"  0x{state_addr_correct:08X} — state counter (0-14, cycled)")
print(f"  0x{addr_26164:08X} — scene-related field (cleared/set in state 4)")
print(f"  0x{addr_26594:08X} — byte flag (deferred load indicator)")
print(f"  0x{addr_21612:08X} — error/status code (set on failures)")
print(f"  0x{addr_14966:08X} — byte clear (state 5)")
print(f"  0x{addr_15578:08X} — byte clear (state 13 completion)")
print(f"  0x{add_signed(lis_val(-2102853632), 21606):08X} — byte clear (state 13 completion)")
print()
print("The resulting scene object lives at the static address; no pointer is")
print("'produced' or stored to a new location. The scene is fully initialized")
print("when the state counter wraps back to 0 after state 13 completes.")
