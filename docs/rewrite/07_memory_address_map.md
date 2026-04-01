# 07 - State Machine Memory Address Map

Complete reference for all memory addresses used by the GTA IV scene creation
state machines. Every address has been verified with Python arithmetic from the
PPC `lis` + offset instruction sequences in the generated recomp code.

---

## Base Register Cheat Sheet

| PPC `lis` value | Base address | Region |
|---|---|---|
| `lis -32064` | `0x82C00000` | State variables, flags |
| `lis -32065` | `0x82BF0000` | Scene/content structs |
| `lis -32087` | `0x82A90000` | Error codes, player/profile indices |
| `lis -32076` | `0x82B40000` | Episode index |
| `lis -31971` | `0x831D0000` | Content transition, exit flags |
| `lis -31970` | `0x831E0000` | Player accessor |
| `lis -31975` | `0x83190000` | Scene object base |
| `lis -31972` | `0x831C0000` | Scene pointer, dict pointer |
| `lis -32077` | `0x82B30000` | MP notify object base |
| `lis -32067` | `0x82BD0000` | State exit / state-3 objects |
| `lis -32244` | `0x820C0000` | String table pointers |

---

## 1. State Variables

### 0x82BF9848 -- sub_82242910 state (scene creation, 15 states)

- **PPC**: `lis r26, -32064` (0x82C00000) + `lwz/stw rX, -26552(r26)`
- **Type**: `u32`
- **Valid values**: 0-14 (switch case), each is a state in the 15-state scene creation sub-machine
- **Read by**: sub_82242910 (entry, every iteration), sub_822438B0 hook (diagnostic)
- **Written by**: sub_82242910 (state transitions), sub_822438B0 state 1 (reset to 0)
- **State meanings**:
  - 0: Check `sub_8223DAA0` readiness; fast-path to 4 if ready, else 1
  - 1: Call `sub_826CBA70`, `sub_8223DAA0`, `sub_8223F9F0`; advance to 3
  - 2: Call `sub_8223CB60`, check readiness; write 6 to error code; advance to 3
  - 3: Check `sub_826CBA70`, `sub_8223DAA0`, `sub_82240AB0`; advance to 4
  - 4: Check `sub_8223DB20`, read platformMode, call `sub_8223F308`; advance to 5
  - 5: Check `sub_8223DB20`, read done flag; call `sub_8223F9F0`; advance to 6 or 7
  - 6: Check `sub_8223DB20`, call `sub_826CBA70`, `sub_8284AAE0` (scene load); advance to 8
  - 7: Check `sub_8223DB20`, `sub_8223CB60`, `sub_8223F9F0`; write 6 to error code; may go to 6
  - 8: Call `sub_8284AB10`, `sub_8284ABF8`, `sub_8284B490`, `sub_8284B430`; advance to 9 or 7
  - 9: Check `sub_8223DB20`, `sub_82240B78`, call `sub_8223D2F0`, `sub_8284ABA0`; advance to 10
  - 10: Call `sub_8284ABD0`, `sub_8284ABF8`, check `sub_8284B490`/`sub_8284B430`; advance to 11
  - 11: Check platformMode in {0,1,3,4}; advance to 12 or error
  - 12: Call `sub_822417B0` (scene creation dispatch); advance to 14 or done
  - 13: Check `sub_8223DB20`, `sub_82240B78`, call `sub_8223F9F0`; clear flags, reset to 0
  - 14: Call `sub_822417B0` (second pass); check scene object; may advance to 13 or done

### 0x82BF9838 -- sub_822438B0 state (state 6 inner, 8 states)

- **PPC**: `lis r31, -32064` (0x82C00000) + `lwz/stw rX, -26568(r31)`
- **Type**: `u32`
- **Valid values**: 0-7
- **Read by**: sub_822438B0 (entry), sub_82142F90 hook (diagnostic)
- **Written by**: sub_822438B0 (state transitions), sub_822422E0 (writes 1 at end)
- **State meanings**:
  - 0: Return 0 (idle/waiting)
  - 1: Set byte 0x82A95466=1, write platformMode=2, clear done flag, reset scene state=0; advance to 2
  - 2: Call sub_82242910 (scene creation); if error 33 -> return 1 (done); else advance to 3
  - 3: Call `sub_82240F80(0)`; check for "SAVE" signature in 16-byte buffer; advance to 4
  - 4: Advance to 5; check sub_822422E0 state var; call `sub_822446E8`, `sub_8223F740`
  - 5: Call `sub_82240F80(1)`; advance to 6
  - 6: Check timer struct; call `sub_826CD808`, store scene object; may return 0 (done)
  - 7: Error exit; call `sub_8223CC68(r30, 2)`, return 2

### 0x82BF9834 -- sub_822422E0 state (state 5, game start)

- **PPC**: `lis r30, -32064` (0x82C00000) + `lwz/stw rX, -26572(r30)`
- **Type**: `u32`
- **Valid values**: 0, 1, 2, 3+
- **Read by**: sub_822422E0 (entry switch), sub_82242910 state 4 (platformMode switch), imports.cpp hook
- **Written by**: sub_822422E0 (writes 2 = "done"), imports.cpp hook (resets 2 -> 0)
- **Value meanings**:
  - 0: Not started; triggers level selection logic using episode index
  - 1: In progress (advance to done)
  - 2: Done (triggers error 34 if read by sub_82242910 state 4 -- BUG, hook resets to 0)
  - >=3: Return 2 immediately (already complete)

### 0x82BF99D4 -- sub_822440F8 state (state 4 inner)

- **PPC**: `lis -32064` (0x82C00000) + offset -26156
- **Type**: `u32`
- **Read by**: sub_82142F90 hook (diagnostic only)
- **Written by**: sub_822440F8 (bypassed in recomp -- hook returns 2 directly)

### 0x82BF9830 -- sub_82243260 state

- **PPC**: `lis r31, -32064` (0x82C00000) + `lwz rX, -26576(r31)`
- **Type**: `u32`
- **Valid values**: 0-12 (switch case)
- **Read by**: sub_82243260 (entry)
- **Written by**: sub_82243260 (state transitions)

### r29 in sub_82142230 -- outer state machine register

- **Not a memory address** -- held in register r29 across the loop
- **Type**: `u32`
- **Valid values**: 0-9
- **State meanings**:
  - 0: Call `sub_822414E8`; returns 1 -> state 1, returns 2 -> loc_821422FC
  - 1: Call `sub_8223DDA8`; returns 1 or 2 -> state 2
  - 2: Call `sub_8223DEE8` (save/load check); returns 1 -> loc_821422FC, returns 2 -> state 8
  - 3: Player slot checks + content transition detection (calls `sub_8223CAD8`, `sub_821406C8`, etc.)
  - 4: Call `sub_822440F8` (state 4 inner); returns 1 -> state 7, returns 2 -> state 5
  - 5: Call `sub_822422E0` (game start); then fall through to state 6
  - 6: Call `sub_822438B0` (scene loading inner); returns 0 -> state 9, returns 2 -> state 7
  - 7: Error/completion -> triggers exit sequence (r29 > 6)
  - 8: Error/completion -> triggers exit sequence (r29 > 6)
  - 9: Also triggers exit (r29 > 6)

---

## 2. Platform / Mode Variables

### 0x82BF9844 -- platformMode

- **PPC**: `lis r29, -32064` (0x82C00000) + `lwz/stw rX, -26556(r29)`
- **Type**: `u32`
- **Valid values**: 0, 1, 2, 3, 4
- **Read by**: sub_82242910 states 4, 10, 11, 13, 14
- **Written by**: sub_822438B0 state 1 (writes 2), imports.cpp hook (forces 3)
- **Value meanings**:
  - 0, 1: Accepted in state 11 check (advance to 12)
  - 2: Triggers error 34 in state 4 switch (val > 4 or val == 2 -> error)
  - 3: Base game mode -- required for state 4 scene gate `(val-3) unsigned <= 1`
  - 4: DLC/episode mode -- also accepted by state 4 scene gate
- **Hook fix**: imports.cpp forces to 3 before sub_82242910 runs (prevents error 34)

---

## 3. Flag Bytes

### 0x82BF981F -- scene loaded flag

- **PPC**: `lis r11, -32064` (0x82C00000) + `lbz/stb rX, -26593(r11)`
- **Type**: `u8`
- **Valid values**: 0 (not loaded), 1 (loaded)
- **Read by**: sub_82242910 states 4 (case 3), 14
- **Written by**: sub_82242910 (set during scene loading); sub_822422E0 (cleared to 0)

### 0x82BF981E -- done flag

- **PPC**: `lis r31, -32064` (0x82C00000) + `lbz/stb rX, -26594(r31)`
- **Type**: `u8`
- **Valid values**: 0 (not done), 1 (done)
- **Read by**: sub_82242910 states 4, 5
- **Written by**: sub_82242910 states 4 (case 3 and case 4 set to 1), sub_822438B0 state 1 (cleared to 0)

### 0x82A95466 -- content byte flag

- **PPC**: `lis r11, -32087` (0x82A90000) + `lbz/stb rX, 21606(r11)`
- **Type**: `u8`
- **Valid values**: 0, 1
- **Read by**: sub_82242910 states 4 (cases 3, 4), 6, 14
- **Written by**: sub_822438B0 state 1 (writes 1), sub_82242910 state 13 (clears to 0)

### 0x82BF3A76 -- scene byte (cleared in state 5 of scene creation)

- **PPC**: `lis r10, -32065` (0x82BF0000) + `stb rX, 14966(r10)`
- **Type**: `u8`
- **Written by**: sub_82242910 state 5 (clears to 0)

### 0x82BF3CDA -- byte flag (cleared in state 13)

- **PPC**: `lis r10, -32065` (0x82BF0000) + `stb rX, 15578(r10)`
- **Type**: `u8`
- **Written by**: sub_82242910 state 13 (clears to 0)

### 0x82BF9D81 -- sub_82142230 byte check (state 3 gate)

- **PPC**: `lis r14, -32064` (0x82C00000) + `lbz rX, -25215(r14)`
- **Type**: `u8`
- **Read by**: sub_82142230 state 3 (if nonzero, skip to loc_82142504)

### 0x831D5327 -- XAM ready flag (state 3 sign-in detection)

- **PPC**: `lis r26, -31971` (0x831D0000) + `lbz/stb rX, 21287(r26)`
- **Type**: `u8`
- **Valid values**: 0 (not ready), 1 (ready)
- **Read by**: sub_82142230 state 3 (sign-in detection after sub_8223DAA0)
- **Written by**: sub_82142230 state 3 (set to 1 when sub_8223DAA0 returns 0 + flag is 0), exit sequence (cleared to 0)

### 0x831D5348 -- state machine exit byte 1

- **PPC**: `lis r10, -31971` (0x831D0000) + `stb rX, 21320(r10)`
- **Type**: `u8`
- **Written by**: sub_82142230 exit sequence (cleared to 0)

### 0x831D5337 -- state machine exit byte 2

- **PPC**: `lis r30, -31971` (0x831D0000) + `stb rX, 21303(r30)`
- **Type**: `u8`
- **Written by**: sub_82142230 exit sequence (set to 1)

### 0x82B39A95 -- byte flag (sub_822423E0)

- **PPC**: `lis r10, -32076` (0x82B40000) + `stb rX, -25963(r10)`
- **Type**: `u8`
- **Written by**: sub_822423E0 (set to 1 at entry)

---

## 4. Error and Index Variables

### 0x82A9546C -- error code

- **PPC**: `lis r10, -32087` (0x82A90000) + `stw rX, 21612(r10)`
- **Type**: `u32`
- **Valid values**: 0 (none), 6 (normal write by states 2/7), 7, 8, 9, 14, 15, 16, 24, 29, 31, 33 (controller error), 34 (platform mode error)
- **Read by**: sub_822438B0 states 2, 3, 5 (checks for 33), imports.cpp hooks (diagnostic)
- **Written by**: sub_82242910 (various states), sub_822438B0 (state 3/4 error writes)
- **Key error codes**:
  - 6: Normal state -- written by states 2 and 7 of sub_82242910
  - 33: Controller not found (sub_82242910 state 4 via `sub_8223DB20` failure) -- triggers retry in sub_822438B0
  - 34: Platform mode error (sub_82242910 state 4 or 9 when platformMode is invalid)

### 0x82A95478 -- active player/episode index

- **PPC**: `lis r17, -32087` (0x82A90000) + `lwz/stw rX, 21624(r17)`
  Also: `lis -32087` + `lwz rX, 21624(r11)` in sub_82142230 state 5
- **Type**: `u32`
- **Valid values**: 0 (base game), 1 (TLAD), 2 (TBOGT), 0xFFFFFFFF (unset)
- **Read by**: sub_82142230 state 5 (passed as arg to sub_822422E0), sub_822440F8 hook, sub_82142F90 hook
- **Written by**: sub_822440F8 hook (sets 0 if 0xFFFFFFFF)

### 0x82A95474 -- active profile index

- **PPC**: `lis r28, -32087` (0x82A90000) + `lwz/stw rX, 21620(r28)`
- **Type**: `u32`
- **Read by**: sub_82142230 state 3 (stored after `sub_8221B198` call), sub_82142F90 hook
- **Written by**: sub_82142230 states 3 and 3-exit (via sub_8221B198 result)

### 0x82A9547C -- episode DLC flag byte

- **PPC**: `lis r11, -32087` (0x82A90000) + `lbz rX, 21628(r11)`
- **Type**: `u8`
- **Valid values**: 0 (no DLC episodes), 1 (DLC present)
- **Read by**: sub_822422E0 (determines scene name copy vs episode index lookup)

### 0x82A95480 -- player data index (u32)

- **PPC**: `lis r11, -32087` (0x82A90000) + `lwz rX, 21632(r11)`
- **Type**: `u32` (can be negative = -1 for "none")
- **Read by**: sub_822423E0 (checks >= 0, used to index a 64-byte stride array)

### 0x82B39504 -- episode index

- **PPC**: `lis r11, -32076` (0x82B40000) + `lwz rX, -27388(r11)`
- **Type**: `u32`
- **Valid values**: 0 (base game), 1 (TLAD), 2 (TBOGT)
- **Read by**: sub_822422E0 (selects level 12/13/14)
- **Value to level mapping**: 0 -> level 12, 1 -> level 13, 2 -> level 14

### 0x82A9172C -- active player slot index

- **PPC**: `lis -32087` (0x82A90000) + `lwz rX, 5932(rX)` (or register variant)
- **Type**: `u32`
- **Valid values**: 0-3 (player slot), 0xFFFFFFFF (-1 = no player)
- **Read by**: sub_821406C8 (player accessor)

---

## 5. Pointers and Objects

### 0x82BF3A88 -- scene object pointer

- **PPC**: `lis -32065` (0x82BF0000) + offset 14984
  Also accessible as r27+16 where r27 = `lis -32065` + 14968 (0x82BF3A78)
- **Type**: `u32` (pointer)
- **Read by**: sub_822438B0 hook (diagnostic), sub_82242910 hook (diagnostic)
- **Written by**: sub_82242910 state 12/14 via sub_822417B0 (scene creation dispatch)

### 0x82BF99CC -- scene store address

- **PPC**: `lis r31, -32064` (0x82C00000) + `lwz/stw rX, -26164(r31)`
- **Type**: `u32` (pointer or scene handle)
- **Read by**: sub_82242910 states 6, 12, 14 (passed as arg r6 to sub_822417B0)
- **Written by**: sub_82242910 state 4 (initialized to 0, then set from r30+4)

### 0x82BF3A7C -- scene result (stored to 0x82BF99CC)

- **PPC**: r30+4 where r30 = `lis -32065` + 14968 = 0x82BF3A78
- **Type**: `u32`
- **Read by**: sub_82242910 state 4 (loaded and stored to 0x82BF99CC when platformMode is 3 or 4)

### 0x82BF99C8 -- r29 in sub_82242910 (address passed as arg)

- **PPC**: `lis r11, -32064` (0x82C00000) + `addi r29, r11, -26168`
- **Type**: object pointer (used as r7 in sub_822417B0 calls, also dereferenced as `lwz r11, 0(r29)`)
- **Read by**: sub_82242910 states 12, 14 (passed to sub_822417B0; dereferenced in state 14)

### 0x82BF9898 -- scene object u64 store

- **PPC**: `lis r10, -32064` (0x82C00000) + `std rX, -26472(r10)`
- **Type**: `u64`
- **Written by**: sub_822438B0 state 6 (stores scene object data from sub_829DBAA8)

### 0x831C2458 -- scene pointer (global)

- **PPC**: `lis -31972` (0x831C0000) + offset 9304
- **Type**: `u32` (pointer)
- **Read by**: sub_82142F90 hook (diagnostic)

### 0x831C2EF8 -- dictionary pointer (global)

- **PPC**: `lis -31972` (0x831C0000) + offset 12024
- **Type**: `u32` (pointer, null when not loaded)
- **Read by**: 15+ callers (null check guard)

### 0x831E4DD4 -- player accessor base

- **PPC**: `lis r27, -31970` (0x831E0000) + `lwz rX, 19924(r27)`
- **Type**: `u32` (pointer to player struct)
- **Read by**: sub_82142230 states 3 (loc_821422FC, loc_82142360), state 3 exit

### 0x82B29F18 -- player pool base

- **PPC**: `lis -32077` (0x82B30000) + offset -24808
- **Type**: object/pool
- **Read by**: sub_821B4108 (active player count)

---

## 6. Struct Bases

### 0x82BF3934 -- timer/state struct (r30 in sub_822438B0)

- **PPC**: `lis r10, -32065` (0x82BF0000) + `addi r30, r10, 14644`
- **Layout**:
  - +0 (`0x82BF3934`): `u32` counter (compared against 3000 in state 6)
  - +4 (`0x82BF3938`): `u8` flag byte
  - +8 (`0x82BF393C`): `u32` state (written 2 in state 2)

### 0x82BF3A78 -- scene creation struct (r27 in sub_822438B0, r30 in sub_82242910 state 4)

- **PPC**: `lis r10, -32065` (0x82BF0000) + `addi r27/r30, r10, 14968`
- **Layout**:
  - +0 (`0x82BF3A78`): base
  - +4 (`0x82BF3A7C`): `u32` scene result (copied to 0x82BF99CC)
  - +8 (`0x82BF3A80`): `u8` flag byte (set 1 in sub_822438B0 states 3, 5)
  - +16 (`0x82BF3A88`): `u32` scene object pointer

### 0x82BF3A60 -- r28 base in sub_82242910

- **PPC**: `lis r11, -32065` (0x82BF0000) + `addi r28, r11, 14944`
- Used as arg r5 in sub_822417B0 calls (states 12, 14)

### 0x82BF3940 -- 16-byte buffer (sub_822438B0 state 3)

- **PPC**: `lis r11, -32065` (0x82BF0000) + `addi r11, r11, 14656`
- **Type**: 16-byte character buffer
- **Read by**: sub_822438B0 state 3 (checks for "SAVE" signature at bytes 12-15: 'S'=83, 'A'=65, 'V'=86, 'E'=69)
- **Written by**: sub_82240F80 (content enumeration result)

---

## 7. Sub-function Object Pointers (sub_82142230 register setup)

| Register | Address | PPC | Usage |
|---|---|---|---|
| r23 | `0x820B9138` | `lis -32244 + (-28360)` | String arg for `sub_8224DC48` |
| r20 | `0x820B9120` | `lis -32244 + (-28384)` | String arg for `sub_82215530` |
| r15 | `0x820B9108` | `lis -32244 + (-28408)` | String arg for `sub_82215530` |
| r19 | `0x820B90F4` | `lis -32244 + (-28428)` | String arg for `sub_822BCA90` |
| r18 | `0x820B90E0` | `lis -32244 + (-28448)` | String arg for `sub_822BCA90` |
| r16 | `0x82B2F7E0` | `lis -32077 + (-2080)` | MP notify object for `sub_821B6FD0` |
| r22 | `0x82BCC1F8` | `lis -32067 + (-15880)` | Object for `sub_82215530` |

---

## 8. Content Transition Counter

### 0x831D5330 -- content transition state

- **PPC**: `lis r30, -31971` (0x831D0000) + `lwz/stw rX, 21296(r30)`
- **Type**: `u32`
- **Valid values**: 0 (initial), 1 (first transition detected), 2 (second transition)
- **Read by**: sub_82142230 state 3 (determines which string to display via `sub_822BCA90`, which profile call to make)
- **Written by**: sub_82142230 state 3 (transitions: 0->1 on first detection, 1->2 on second, reset to 0 on exits)

---

## 9. XAM Readiness System

### 0x82BF9B70 -- XAM dialog readiness dword

- **PPC**: `lis -32064` (0x82C00000) + offset -25744 (`lwz/stw rX, -25744(rX)`)
- **Type**: `u32` (treated as signed)
- **Valid values**: 0xFFFFFFFF (-1 = not ready/no dialog), 0+ (ready/dialog pending), incremented toward 4
- **Read by**: sub_8224FA48 (returns 0 when -1, 1 when >= 0), sub_8214C8C8 (increments)
- **Written by**:
  - sub_82254FE0: writes 1 ("ready signal" -- XAM dialog completion)
  - sub_8224FA38: writes -1 ("reset" -- clear readiness)
  - sub_8214C8C8: increments toward 4
- **State 3 logic**: `sub_8224FA48` returning 0 (value = -1) means "no XAM dialog pending, advance." Returning 1 means "dialog pending, stay in state 3."
- **Natural default**: -1 is correct for recomp (no XAM dialogs)

---

## 10. Miscellaneous

### 0x82A9800C -- file I/O event address

- **PPC**: `lis -32086` + offset -32756
- **Type**: event handle address
- **Used by**: `SignalEventByGuestAddr()` in rexcrt file hooks

### 0x82BF9828 -- state 0->1 initialization write

- **PPC**: `lis r21, -32064` (0x82C00000) + `stw rX, -26584(r21)`
- **Type**: `u32`
- **Written by**: sub_82142230 loc_8214231C (writes 0 when state 0 returns 1)

### 0x83192C50 -- scene object base (r31 in sub_82242910 states 0, 6, 8, 9, 10)

- **PPC**: `lis r11, -31975` (0x83190000) + `addi r31, r11, 11344`
- **Type**: pointer to scene creation manager object
- **Read by**: sub_82242910 (passed as r3 to `sub_8284B4B0`, `sub_8284AAE0`, `sub_8284AB10`, etc.)

### 0x82BF3D90 -- frequently loaded param

- **PPC**: `lis r11, -32065` (0x82BF0000) + `lwz rX, 15760(r11)`
- **Type**: `u32`
- **Read by**: sub_82242910 states 0, 6, 8, 9, 10 (passed as r4 to various scene load functions)

### 0x82BF3D94 -- state 10 param

- **PPC**: `lis r11, -32065` (0x82BF0000) + `addi rX, r11, 15764`
- **Type**: pointer
- **Read by**: sub_82242910 state 10 (passed as r5 to `sub_8284ABD0`)

### 0x82BF3D98 -- state 9 string/param

- **PPC**: `lis r11, -32065` (0x82BF0000) + `addi rX, r11, 15768`
- **Type**: pointer
- **Read by**: sub_82242910 state 9 (passed as r6 to `sub_8284ABA0`)

### 0x82BEFA40 -- sub_82142230 object

- **PPC**: `lis r11, -32065` (0x82BF0000) + `addi r31, r11, -1472`
- **Type**: object pointer
- **Read by**: sub_82142230 exit sequence (passed to `sub_8222DB48`)

### 0x82BCF998 -- state exit object

- **PPC**: `lis r11, -32067` (0x82BD0000) + `addi r3, r11, -1640`
- **Type**: object pointer
- **Used by**: sub_82142230 exit sequence (passed to `sub_82220118`)

### 0x82B978B0 -- state exit param

- **PPC**: `lis r11, -32071` (0x82B90000) + `addi r3, r11, 30896`
- **Type**: object pointer
- **Used by**: sub_82142230 exit sequence (passed to `sub_821ED6D8`)

---

## 11. Address Clustering Summary

The state machine addresses cluster in three main regions:

**0x82A9xxxx region** (player/profile/error):
| Address | Name | Type |
|---|---|---|
| `0x82A9172C` | Active player slot index | u32 |
| `0x82A9546C` | Error code | u32 |
| `0x82A95466` | Content byte flag | u8 |
| `0x82A95474` | Active profile index | u32 |
| `0x82A95478` | Active player/episode index | u32 |
| `0x82A9547C` | Episode DLC flag | u8 |
| `0x82A95480` | Player data index | u32 |

**0x82BFxxxx region** (state vars, scene structs):
| Address | Name | Type |
|---|---|---|
| `0x82BF3934` | Timer struct base (+0 counter, +4 flag, +8 state) | struct |
| `0x82BF3940` | 16-byte SAVE buffer | buffer |
| `0x82BF3A60` | Scene creation arg (r28) | ptr |
| `0x82BF3A76` | Scene byte flag | u8 |
| `0x82BF3A78` | Scene creation struct base | struct |
| `0x82BF3A7C` | Scene result (copied to 0x82BF99CC) | u32 |
| `0x82BF3A80` | Scene flag byte | u8 |
| `0x82BF3A88` | Scene object pointer | u32 ptr |
| `0x82BF3CDA` | State 13 flag byte | u8 |
| `0x82BF3D90` | Scene load param | u32 |
| `0x82BF3D94` | State 10 param ptr | ptr |
| `0x82BF3D98` | State 9 string ptr | ptr |
| `0x82BF981E` | Done flag | u8 |
| `0x82BF981F` | Scene loaded flag | u8 |
| `0x82BF9828` | State init write | u32 |
| `0x82BF9830` | sub_82243260 state | u32 |
| `0x82BF9834` | sub_822422E0 state (state 5) | u32 |
| `0x82BF9838` | sub_822438B0 state (state 6 inner) | u32 |
| `0x82BF9844` | platformMode | u32 |
| `0x82BF9848` | sub_82242910 state (scene creation) | u32 |
| `0x82BF9898` | Scene object u64 store | u64 |
| `0x82BF99C8` | Scene creation arg (r29) | ptr |
| `0x82BF99CC` | Scene store / handle | u32 |
| `0x82BF99D4` | sub_822440F8 state (state 4 inner) | u32 |
| `0x82BF9B70` | XAM readiness dword | u32 |
| `0x82BF9D81` | State 3 gate byte | u8 |

**0x831xxxxx region** (globals, player pool):
| Address | Name | Type |
|---|---|---|
| `0x831C2458` | Scene pointer (global) | u32 ptr |
| `0x831C2EF8` | Dictionary pointer (global) | u32 ptr |
| `0x831D5327` | XAM ready flag | u8 |
| `0x831D5330` | Content transition counter | u32 |
| `0x831D5337` | Exit byte 2 | u8 |
| `0x831D5348` | Exit byte 1 | u8 |
| `0x831E4DD4` | Player accessor base | u32 ptr |

---

*Generated 2026-03-27 from gta4_recomp.0.cpp (sub_82142230), gta4_recomp.6.cpp
(sub_82242910, sub_822438B0, sub_822422E0, sub_82243260), and
LibertyRecomp/kernel/imports.cpp hook code. All addresses verified with Python
from PPC `lis` + signed offset instruction sequences.*
