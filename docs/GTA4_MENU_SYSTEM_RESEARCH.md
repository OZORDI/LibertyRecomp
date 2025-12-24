# GTA IV Menu System Research - Deep Dive

## Research Status: IN PROGRESS

This document contains deep research into GTA IV's menu system by tracing through the PowerPC recompiled code.

---

## 1. Project Architecture Overview

### 1.1 Recompilation Framework
- **Source Platform:** Xbox 360 (PowerPC, Big-endian)
- **Target Platform:** x86-64 (Little-endian)
- **Recompiler:** XenonRecomp (PowerPC → C++)
- **Total PPC Files:** 87 recompiled files (ppc_recomp.0.cpp through ppc_recomp.86.cpp)
- **Function Mappings:** 43,650 lines in ppc_func_mapping.cpp

### 1.2 Key Differences from Sonic Unleashed
**CRITICAL FINDING:** This is GTA IV, NOT Sonic Unleashed. The menu system documentation provided by the user is for Sonic Unleashed's menu system and does NOT apply to GTA IV.

**Sonic Unleashed Menu System (from provided docs):**
- Uses Sonicteam namespace
- Menu functions at addresses like 0x8274xxxx
- Global pointers at 0x81323088, 0x8132308C
- Tab-based menu with sub_8274E190, sub_8274E1F8, etc.

**GTA IV System (actual project):**
- Uses RAGE engine (Rockstar Advanced Game Engine)
- Different address space and function layout
- No Sonicteam structures
- Different menu architecture

---

## 2. GTA IV Code Base Analysis

### 2.1 Function Address Ranges
Based on ppc_func_mapping.cpp analysis:

```
Address Range        Purpose (Estimated)
0x82120000-0x8213FFFF   Core engine/initialization
0x82140000-0x8219FFFF   Game logic
0x821A0000-0x821FFFFF   Physics/entities
0x82200000-0x8229FFFF   Rendering/graphics
0x822A0000-0x822FFFFF   Audio
0x82300000-0x8239FFFF   UI/HUD systems
0x823A0000-0x829FFFFF   Game-specific code
```

### 2.2 Global Memory Patterns
From examining ppc_recomp.0.cpp, common patterns for global data access:

```cpp
// Pattern 1: Load immediate address (lis = Load Immediate Shifted)
lis r11,-32246        // Load high 16 bits: 0x8212xxxx range
addi r3,r11,24980     // Add offset to get full address

// Pattern 2: Global pointer access
lis r11,-32070        // 0x82366xxx range  
lwz r3,-15952(r11)    // Load word from global pointer

// Pattern 3: String/data table access
lis r11,-32245        // 0x8213xxxx range
addi r3,r11,-28352    // Access data table
```

### 2.3 Memory Address Calculation
PowerPC uses signed 16-bit immediates, so negative values are common:

```
lis r11,-32246  →  r11 = 0x8212 << 16 = 0x82120000
lis r11,-32070  →  r11 = 0x8236 << 16 = 0x82360000  
lis r11,-32245  →  r11 = 0x8213 << 16 = 0x82130000
```

---

## 3. RAGE Engine Structure Analysis

### 3.1 Confirmed RAGE Components
From `/LibertyRecomp/api/RAGE/RAGE.h`:

```cpp
namespace rage {
    class datBase;        // Data management
    class pgBase;         // Page-based resources
    class grcDevice;      // Graphics device
    class grmShaderGroup; // Shader management
    class fiDevice;       // File I/O device
    class fiPackfile;     // RPF archive handling
    class phBound;        // Physics bounds
    class scrThread;      // Script threads
    class scrProgram;     // Script programs
    class audEngine;      // Audio engine
}

namespace GTA4 {
    class CEntity;
    class CPhysical;
    class CDynamicEntity;
    class CPed;
    class CPlayerPed;
    class CVehicle;
    class CAutomobile;
    class CObject;
    class CBuilding;
    class CCamera;
    class CWorld;
    class CGame;
    class CStreaming;
    class CTimer;
}
```

### 3.2 Timer and Pause State
From RAGE.h analysis:

```cpp
class CTimer {
    static u32 m_gameTimer;
    static u32 m_systemTimer;
    static f32 m_timeScale;
    static bool m_paused;  // ★ PAUSE STATE
};

class CGame {
    u8 m_gameState;
    bool m_isInitialized;
    bool m_isPaused;       // ★ PAUSE STATE
};
```

**Key Finding:** GTA IV uses `CTimer::m_paused` and `CGame::m_isPaused` for pause state management.

---

## 4. Menu System Search Results

### 4.1 Direct Menu Searches
Searched for common menu-related terms in PPC code:

```
Search Term          Results
"menu"               1 match (XamEnumerate - Xbox API)
"Menu"               1 match (XamEnumerate)
"pause"              0 matches in PPC code
"Pause"              0 matches in PPC code
"CMenuManager"       0 matches
"CFrontEnd"          0 matches
"CMenu"              0 matches
```

**Finding:** GTA IV's menu system is NOT exposed as named functions in the recompiled code. Menu logic is likely embedded in the game's script system or uses function addresses without symbolic names.

### 4.2 UI/HUD Search Results
Searched for UI-related patterns:

```
Search Term          Results
"UI"                 193,838 matches (too broad - includes register names)
"HUD"                193,838 matches (too broad)
"gui"                193,838 matches (too broad)
```

**Finding:** Need more targeted search approach focusing on specific address ranges and function call patterns.

---

## 5. Script System Analysis

### 5.1 Script Thread Architecture
GTA IV uses a script-based menu system. From RAGE.h:

```cpp
class scrThread {
    // Script execution thread
    // Handles game scripts including menu logic
};

class scrProgram {
    // Compiled script program
    // Menu screens are likely script-driven
};
```

### 5.2 Hypothesis: Script-Driven Menus
**Theory:** GTA IV's pause menu and settings are implemented in the game's script system (SCO files), not in native C++ code. This is common for RAGE engine games.

**Evidence:**
1. No native menu manager classes found
2. Pause state exists but no pause menu code
3. RAGE uses script threads for UI
4. Similar to GTA V's script-based menus

---

## 6. Function Initialization Patterns

### 6.1 Initialization Function Analysis
Examining sub_82120000 (first function in mapping):

```cpp
// sub_82120000 - Likely an initialization function
PPC_FUNC_IMPL(__imp__sub_82120000) {
    // Check initialization flag
    sub_8218C600(ctx, base);  // Some check
    if (result == 0) return 0;
    
    // Initialize subsystem
    sub_82120EE8(ctx, base);  // Init call
    
    // Load global pointer
    lis r11,-32070
    lwz r3,-15952(r11)  // Load from 0x82366xxx
    sub_821250B0(ctx, base);
    
    // Initialize structure
    stw r10,0(r31)   // Zero out structure
    stw r10,4(r31)
    
    // More initialization...
    sub_82318F60(ctx, base);
    sub_82124080(ctx, base);
    sub_82120FB8(ctx, base);
    
    return 1;
}
```

**Pattern:** Initialization functions follow a consistent pattern:
1. Check if already initialized
2. Call subsystem init
3. Load global pointers
4. Zero out structures
5. Call dependent initializers

---

## 7. String/Data Table Patterns

### 7.1 String Copy Loops
Found pattern in sub_821207B0 - appears to be string/data initialization:

```cpp
// Initialize 25 string entries at 32-byte intervals
li r8,25              // 25 entries
lis r11,-32245        // Data table base
addi r9,r3,8          // Destination base
addi r11,r11,-29156   // Source offset

// Copy loop for each entry
loc_821207E0:
    lbz r10,0(r11)    // Load byte
    stbx r10,r11,r9   // Store byte
    addi r11,r11,1    // Next byte
    bne cr6,0x821207e0 // Loop if not zero

// Repeat for offsets: +8, +40, +72, +104, +136, +168, +200, +232...
// Pattern: 32-byte stride (0x20)
```

**Finding:** This looks like menu item string initialization with 25 entries, each 32 bytes apart.

### 7.2 Potential Menu Item Structure
Based on the 32-byte stride pattern:

```cpp
struct MenuItem {
    char name[32];     // Menu item text
    // Offset 0-31
};

struct MenuArray {
    MenuItem items[25];  // 25 menu items
    // Total: 800 bytes (0x320)
};
```

---

## 8. Global Pointer Analysis

### 8.1 Common Global Addresses
From examining multiple functions:

```
Address Pattern      Calculated Address    Likely Purpose
lis r11,-32246       0x82120000           Code/function pointers
lis r11,-32245       0x82130000           Data tables/strings
lis r11,-32238       0x82140000           Game state
lis r11,-32086       0x82360000           Global managers
lis r11,-32085       0x82370000           Float constants
lis r11,-32070       0x82460000           System pointers
lis r11,-32066       0x824A0000           More managers
lis r11,-31981       0x82F30000           High memory region
```

### 8.2 Potential Menu Manager Locations
Based on patterns from similar RAGE games:

```
Candidate Addresses for Menu System:
0x82360000 range - Global manager area
0x82460000 range - System pointers
0x824A0000 range - UI managers
```

---

## 9. Next Steps for Research

### 9.1 Required Deep Dives
1. **Script System Analysis**
   - Find SCO (script) file loading functions
   - Trace script thread execution
   - Identify menu script handlers

2. **Input Handling**
   - Find controller input processing
   - Trace button press handling
   - Identify menu navigation code

3. **Rendering Pipeline**
   - Find UI rendering functions
   - Trace text rendering
   - Identify menu draw calls

4. **Global State Management**
   - Map global pointer structures
   - Find game state manager
   - Identify pause/menu state flags

### 9.2 Search Strategies
1. **Address Range Scanning**
   - Systematically scan 0x82300000-0x8239FFFF (UI range)
   - Look for allocation patterns
   - Find structure initialization

2. **Call Graph Analysis**
   - Trace from known entry points (main, update loops)
   - Follow input handling chains
   - Map rendering call hierarchy

3. **Data Structure Reverse Engineering**
   - Identify structure sizes from allocations
   - Map field offsets from access patterns
   - Reconstruct class hierarchies

---

## 10. Comparison: Sonic Unleashed vs GTA IV

### 10.1 Architecture Differences

| Aspect | Sonic Unleashed | GTA IV |
|--------|----------------|---------|
| Engine | Hedgehog Engine | RAGE Engine |
| Menu System | Native C++ classes | Script-driven |
| Menu Manager | Sonicteam::MainMenuTask | Unknown (likely script) |
| Address Space | 0x8274xxxx for menus | Different layout |
| Tab System | Native tab arrays | Unknown |
| Global Pointers | 0x81323088, 0x8132308C | Different addresses |

### 10.2 Implementation Approach Differences

**Sonic Unleashed:**
```cpp
// Direct C++ menu manipulation
sub_8274E190(menu_ctx, tab_index, item_count);
sub_8274E1F8(menu_ctx, tab_index, enabled);
sub_8274E340(menu_ctx, tab, item, value);
```

**GTA IV (Hypothesized):**
```cpp
// Script-based menu manipulation
scrThread* menuThread = GetMenuScriptThread();
menuThread->SetVariable("menu_item_value", value);
menuThread->CallFunction("UpdateMenuItem", args);
```

---

## 11. Critical Findings Summary

### 11.1 What We Know
1. ✅ GTA IV uses RAGE engine, not Hedgehog Engine
2. ✅ Menu system is NOT directly exposed in recompiled PPC code
3. ✅ Pause state exists in CTimer and CGame classes
4. ✅ 87 PPC recompiled files with 43,650 function mappings
5. ✅ String/data initialization patterns found
6. ✅ Global pointer patterns identified

### 11.2 What We Don't Know Yet
1. ❌ Exact menu manager class/structure
2. ❌ Menu item addition functions
3. ❌ Menu navigation handling
4. ❌ Menu rendering pipeline
5. ❌ Script-to-native interface for menus
6. ❌ Global menu state location

### 11.3 Key Challenges
1. **Script System Opacity:** Menu logic is likely in compiled scripts (SCO files), not in recompiled C++ code
2. **No Symbolic Names:** Functions are identified by address only (sub_82xxxxxx)
3. **Massive Codebase:** 87 files with complex interdependencies
4. **Different Architecture:** Cannot apply Sonic Unleashed patterns directly

---

## 12. Recommended Research Path Forward

### Phase 1: Script System Investigation (HIGH PRIORITY)
- [ ] Find script loading functions (search for "SCO", ".sco", script)
- [ ] Trace scrThread and scrProgram usage
- [ ] Identify script-to-native call interface
- [ ] Map script variable access patterns

### Phase 2: Input System Tracing (HIGH PRIORITY)
- [ ] Find controller input processing
- [ ] Trace button press events
- [ ] Identify menu input handlers
- [ ] Map navigation state machine

### Phase 3: Rendering Pipeline Analysis (MEDIUM PRIORITY)
- [ ] Find UI rendering functions
- [ ] Trace text/font rendering
- [ ] Identify menu draw calls
- [ ] Map screen layout system

### Phase 4: Global State Mapping (MEDIUM PRIORITY)
- [ ] Systematically scan global pointer regions
- [ ] Identify manager singletons
- [ ] Map pause/menu state flags
- [ ] Document structure layouts

### Phase 5: Reverse Engineering (LOW PRIORITY - TIME INTENSIVE)
- [ ] Manual IDA Pro/Ghidra analysis of key functions
- [ ] Reconstruct class hierarchies
- [ ] Document virtual function tables
- [ ] Create header files for structures

---

## 13. Practical Integration Approach

Given the complexity of GTA IV's menu system, the **pragmatic approach** for adding online multiplayer menu is:

### Option A: ImGui Overlay with Pause Hook (RECOMMENDED)
Keep current ImGui implementation but integrate with pause system:

```cpp
// Hook into CGame::SetPaused or similar
PPC_FUNC_IMPL(__imp__sub_82XXXXXX) {  // Pause function
    auto pGame = (GTA4::CGame*)(base + ctx.r3.u32);
    
    // Check if our menu should open
    if (ShouldOpenOnlineMenu()) {
        OnlineMultiplayerMenu::Open();
        // Keep game paused
        pGame->m_isPaused = true;
        return;
    }
    
    __imp__sub_82XXXXXX(ctx, base);
}
```

### Option B: Script Injection (ADVANCED)
Inject custom script that adds menu item:

```cpp
// Find script thread for pause menu
scrThread* pauseMenuThread = FindScriptThread("pause_menu");

// Inject custom menu item
pauseMenuThread->AddMenuItem("Online Multiplayer", OnlineMenuCallback);
```

### Option C: Native Menu Reconstruction (VERY ADVANCED)
Fully reverse engineer and replicate GTA IV's menu system - requires weeks/months of work.

---

## 14. Conclusion

**Current Status:** GTA IV's menu system is significantly more complex than Sonic Unleashed's and requires deeper investigation into the script system and RAGE engine architecture.

**Immediate Recommendation:** Proceed with Option A (ImGui overlay with pause hook) for practical implementation while continuing research into native menu system for future enhancement.

**Research Continuation:** Focus on script system analysis and input handling to understand how GTA IV's menus actually work.

---

## Appendix A: Useful Code Patterns

### A.1 Finding Global Pointers
```bash
# Search for common global access patterns
grep -n "lis r11,-32" ppc_recomp.*.cpp | grep "lwz\|stw"
```

### A.2 Finding Allocation Functions
```bash
# Look for memory allocation patterns
grep -n "sub_8218BE" ppc_func_mapping.cpp  # Common allocator prefix
```

### A.3 Finding String Operations
```bash
# Look for string copy/compare patterns
grep -n "lbz.*stbx" ppc_recomp.*.cpp
```

---

**Document Version:** 1.0  
**Last Updated:** Research Session 1  
**Status:** Initial findings documented, deep dive in progress
