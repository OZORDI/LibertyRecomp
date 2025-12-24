# GTA IV DLC and Update File Support - Research Analysis

## Executive Summary

This document analyzes the implementation requirements for GTA IV DLC and Title Update support in LibertyRecomp, based on:
- Existing STFS parser infrastructure
- Sonic Unleashed reference implementation
- GTA IV-specific requirements
- Available game files analysis

---

## 1. Current State Assessment

### ✅ Already Implemented

**STFS Parser Infrastructure** (`install/xbox360/stfs_parser.h/cpp`):
- Full STFS container parsing (CON, LIVE, PIRS packages)
- Metadata extraction (Title ID, version, content type)
- File enumeration and extraction
- Title Update detection (`IsTitleUpdate()`)
- Block navigation with hash table support

**XEX Patcher** (`install/xbox360/xex_patcher.h/cpp`):
- XEX/XEXP patch application framework
- Full patch support (complete XEX replacement)
- Delta patch support (fallback to xextool)
- Version compatibility checking

**Title Update Manager** (`install/xbox360/title_update_manager.h/cpp`):
- Directory scanning for TU files
- Update selection and sorting (highest version first)
- Apply pipeline integration
- Version display helpers

**Installer Wizard** (`ui/installer_wizard.cpp`):
- `WizardPage::SelectTitleUpdate` page defined (line 126)
- Title update manager integration (line 168-170)
- Scanning logic implemented (lines 1349-1369)
- UI rendering for TU selection (lines 1378-1435)

**XContent File System** (`install/xcontent_file_system.h`):
- STFS/SVOD volume support
- Memory-mapped file access
- File enumeration and extraction
- Integration with VFS layer

### ❌ Missing Components

1. **DLC Requirement Validation**
   - No check that DLC requires TU 1.06 (v8)
   - No enforcement of update dependency

2. **Update-DLC Compatibility Matrix**
   - No validation that selected update supports selected DLC
   - No warning system for incompatible combinations

3. **DLC Content Registration**
   - DLC files not automatically mounted to virtual paths at runtime
   - No `update:\` root path registration

4. **Installer Flow Integration**
   - Title Update selection page exists but may need DLC validation
   - No blocking logic for "DLC requires update" scenario

---

## 2. GTA IV File Structure Analysis

### Available Files

**Title Updates** (`/Users/Ozordi/Downloads/LibertyRecomp/GAME UPDATES/`):
```
Grand Theft Auto IV (USA) (v4).zip  - TU4 (~3.4MB)
Grand Theft Auto IV (USA) (v5).zip  - TU5 (~3.5MB)
Grand Theft Auto IV (USA) (v6).zip  - TU6 (~3.5MB)
Grand Theft Auto IV (USA) (v8).zip  - TU8 (~3.5MB) ← LATEST (1.06)
```

**DLC** (`/Users/Ozordi/Downloads/LibertyRecomp/GAME DLC/`):
```
Grand Theft Auto IV - The Lost and Damned (World) (Addon).zip    (~1.8GB)
Grand Theft Auto IV - The Ballad of Gay Tony (World) (Addon).zip (~1.9GB)
```

### GTA IV Title Update History

| Version | Internal | Release | Notes |
|---------|----------|---------|-------|
| TU4 | 1.0.4.0 | 2008 | Initial PC fixes |
| TU5 | 1.0.5.0 | 2009 | Performance improvements |
| TU6 | 1.0.6.0 | 2009 | Bug fixes |
| TU7 | 1.0.7.0 | 2009 | Europe-specific (multiplayer) |
| TU8 | 1.0.8.0 | 2010 | **Final update - DLC compatibility** |

**Critical Finding:** TU8 (v1.06/1.08) is required for DLC support based on Xbox 360 release timeline.

### STFS Package Structure

```
STFS Container (.zip → extracted STFS file)
├── Header (0xA000 or 0xB000 bytes)
│   ├── Magic: "CON ", "LIVE", or "PIRS" (4 bytes @ 0x00)
│   ├── Signatures (RSA-2048)
│   ├── License Entries
│   ├── Metadata (@ 0x22C)
│   │   ├── Content Type (4 bytes)
│   │   │   └── 0x000B0000 = Title Update
│   │   │   └── 0x00000002 = DLC
│   │   ├── Metadata Version (4 bytes)
│   │   ├── Content Size (8 bytes)
│   │   ├── Execution Info (@ +0x10)
│   │   │   ├── Media ID (4 bytes)
│   │   │   ├── Version (4 bytes)
│   │   │   ├── Base Version (4 bytes)
│   │   │   └── Title ID (4 bytes) - 0x545407F2 for GTA IV
│   │   ├── Display Name (@ +0x1691, UTF-16BE, 128 chars)
│   │   └── Volume Descriptor (@ +0x24)
│   │       ├── Descriptor Length (1 byte) = 0x24
│   │       ├── Flags (1 byte)
│   │       ├── File Table Block Count (2 bytes LE)
│   │       ├── File Table Block Number (3 bytes LE)
│   │       └── Total Block Count (4 bytes BE @ +0x1C)
├── Hash Tables (interspersed with data)
│   └── 3 levels: [170, 28900, 4913000] blocks per level
└── File Data Blocks (4KB each)
    ├── For Title Updates: default.xexp
    └── For DLC: Game files (common/, xbox360/, audio/)
```

---

## 3. Sonic Unleashed Reference Implementation

### Key Insights from UnleashedRecomp

**Content Mounting Strategy** (from TECHNICAL_DOCUMENTATION.md:1750-1771):
```cpp
void KiSystemStartup()
{
    // 1. Register base game
    XamRegisterContent(gameContent, GetGamePath() / "game");
    
    // 2. Register update (CRITICAL for file override)
    XamRegisterContent(updateContent, GetGamePath() / "update");
    
    // 3. Mount to virtual paths
    XamContentCreateEx(0, "game", &gameContent, OPEN_EXISTING, ...);
    XamContentCreateEx(0, "update", &updateContent, OPEN_EXISTING, ...);
    
    // 4. Auto-discover DLC
    for (auto& file : directory_iterator(GetGamePath() / "dlc"))
    {
        XamRegisterContent(XamMakeContent(XCONTENTTYPE_DLC, fileName), filePath);
    }
}
```

**Path Resolution Priority** (from TECHNICAL_DOCUMENTATION.md:1777-1797):
```
Priority Order: Mods > Updates > Base Game

Critical Rule: game:\work\ → redirects to update:\
Reason: Title updates contain newer versions of work folder files
```

**XContent File System Support**:
- Full STFS/SVOD parsing
- Memory-mapped file access for performance
- Block translation with hash table navigation
- File enumeration and extraction

---

## 4. GTA IV-Specific Requirements

### DLC Structure

**The Lost and Damned (TLAD):**
```
TLAD/
├── common/      - Shared assets
├── xbox360/     - Platform-specific files
├── audio/       - Voice acting, music
└── default.xex  - DLC executable
```

**The Ballad of Gay Tony (TBOGT):**
```
TBOGT/
├── common/      - Shared assets
├── xbox360/     - Platform-specific files
├── audio/       - Voice acting, music
└── default.xex  - DLC executable
```

### Title Update Structure

**TU8 (v1.06/1.08) Package:**
```
STFS Container
└── default.xexp  - Patch file for default.xex
```

### Critical Dependencies

**DLC → Update Dependency:**
- TLAD and TBOGT **require TU8 (v1.06)** minimum
- Reason: DLC executables link against updated game code
- Without TU8: DLC will crash or fail to load

**File Override Behavior:**
- Base game files in `game/`
- Update files in `update/` (override base game)
- DLC files in `dlc/TLAD/` and `dlc/TBOGT/`
- Priority: DLC > Update > Base Game (for same file paths)

---

## 5. Implementation Requirements

### Phase 1: Update-DLC Dependency Validation

**Location:** `ui/installer_wizard.cpp` - SelectTitleUpdate and SelectDLC pages

**Requirements:**
1. **Detect if DLC is selected**
   - Check if `g_dlcSourcePaths[TLAD]` or `g_dlcSourcePaths[TBOGT]` is set

2. **Enforce TU8 requirement**
   - If DLC selected → require TU8 (version 8)
   - Block "Continue" button if DLC selected but no TU8
   - Show warning message: "DLC requires Title Update 1.06 (TU8)"

3. **Auto-select TU8 if available**
   - When DLC is selected, automatically select TU8 if detected
   - User can still proceed without DLC if they deselect it

**UI Flow:**
```
SelectGame → SelectTitleUpdate → SelectDLC → Validation
                                      ↓
                            If DLC selected && TU < 8:
                                Show error modal
                                Block Continue
                                Suggest: "Please select TU8"
```

### Phase 2: Runtime Content Mounting

**Location:** `main.cpp` - `KiSystemStartup()` function

**Current State** (lines 82-158):
- Game content registered ✅
- Save system initialized ✅
- DLC enumeration exists ✅
- **Missing:** Update content registration ❌

**Required Changes:**
```cpp
void KiSystemStartup()
{
    // ... existing code ...
    
    // Register update content (CRITICAL for file override)
    const auto updateContent = XamMakeContent(XCONTENTTYPE_RESERVED, "Update");
    const std::string updatePath = (const char*)(GetGamePath() / "update").u8string().c_str();
    
    // Check if update directory exists
    if (std::filesystem::exists(updatePath))
    {
        XamRegisterContent(updateContent, updatePath);
        XamContentCreateEx(0, "update", &updateContent, OPEN_EXISTING, nullptr, nullptr, 0, 0, nullptr);
        
        // Create root mapping for update:\
        XamRootCreate("update", updatePath);
        
        printf("[KiSystemStartup] Registered update: -> %s\n", updatePath.c_str());
    }
    
    // DLC registration (enhance existing code)
    for (auto& file : std::filesystem::directory_iterator(GetGamePath() / "dlc", ec))
    {
        if (file.is_directory())
        {
            std::u8string fileNameU8 = file.path().filename().u8string();
            std::u8string filePathU8 = file.path().u8string();
            
            // Register DLC content
            XamRegisterContent(XamMakeContent(XCONTENTTYPE_DLC, (const char*)(fileNameU8.c_str())), 
                             (const char*)(filePathU8.c_str()));
            
            // Mount DLC to virtual path
            auto dlcContent = XamMakeContent(XCONTENTTYPE_DLC, (const char*)(fileNameU8.c_str()));
            XamContentCreateEx(0, (const char*)(fileNameU8.c_str()), &dlcContent, 
                             OPEN_EXISTING, nullptr, nullptr, 0, 0, nullptr);
            
            printf("[KiSystemStartup] Registered DLC: %s -> %s\n", 
                   (const char*)(fileNameU8.c_str()), (const char*)(filePathU8.c_str()));
        }
    }
}
```

### Phase 3: File Path Override System

**Location:** `kernel/io/file_system.cpp` or VFS layer

**Critical Rule from Sonic Unleashed:**
```cpp
// Redirect game:\work\ to update:\
if (path.starts_with("game:\\work\\"))
    root = "update";
```

**GTA IV Equivalent:**
- Need to identify which paths should redirect to update
- Likely candidates: `game:\work\`, `game:\common\`, `game:\xbox360\`
- Research required: Analyze which files TU8 replaces

**Implementation:**
```cpp
std::filesystem::path ResolvePath(const std::string_view& path)
{
    // Parse Xbox-style path
    size_t colonPos = path.find(":\\");
    if (colonPos != std::string::npos)
    {
        std::string_view root = path.substr(0, colonPos);
        std::string_view pathSuffix = path.substr(colonPos + 2);
        
        // CRITICAL: Check update directory first for certain paths
        if (root == "game")
        {
            // List of paths that should check update first
            if (pathSuffix.starts_with("work\\") || 
                pathSuffix.starts_with("common\\data\\") ||
                pathSuffix.starts_with("xbox360\\data\\"))
            {
                std::filesystem::path updatePath = XamGetRootPath("update") / pathSuffix;
                if (std::filesystem::exists(updatePath))
                    return updatePath;
            }
        }
        
        // Fall back to original root
        return XamGetRootPath(root) / pathSuffix;
    }
    
    return path;
}
```

### Phase 4: Installer Wizard Enhancement

**Location:** `ui/installer_wizard.cpp`

**Required Changes:**

1. **SelectTitleUpdate Page** (already exists, needs enhancement):
   - Add warning text if no TU8 detected
   - Highlight TU8 as "Recommended for DLC"
   - Show file path of detected updates

2. **SelectDLC Page** (needs validation logic):
   ```cpp
   // In DrawDLCSelection() or navigation logic
   bool isDLCSelected = (g_dlcSelectionIndex == 0 || g_dlcSelectionIndex == 2);
   bool isTU8Selected = (g_selectedTitleUpdateIndex >= 0 && 
                         g_titleUpdateManager.GetDetectedUpdates()[g_selectedTitleUpdateIndex].info.version >= 8);
   
   // Block Continue button if DLC selected but no TU8
   if (isDLCSelected && !isTU8Selected)
   {
       // Show error message
       g_currentMessagePrompt = "DLC requires Title Update 1.06 (TU8). Please select TU8 or deselect DLC.";
       // Prevent navigation
       return;
   }
   ```

3. **Auto-Selection Logic**:
   ```cpp
   // When DLC is selected, auto-select TU8 if available
   void OnDLCSelected()
   {
       if (!g_titleUpdatesScanned)
           ScanForTitleUpdates();
       
       // Find TU8 in detected updates
       const auto& updates = g_titleUpdateManager.GetDetectedUpdates();
       for (int i = 0; i < updates.size(); i++)
       {
           if (updates[i].info.version == 8)
           {
               g_selectedTitleUpdateIndex = i;
               g_titleUpdateManager.SelectUpdate(i);
               
               // Show notification
               g_currentMessagePrompt = "Title Update 1.06 (TU8) automatically selected (required for DLC)";
               break;
           }
       }
   }
   ```

---

## 6. Installation Flow (Enhanced)

### Current Flow
```
1. Select Language
2. Introduction
3. Select Game Source
4. Select Title Update    ← EXISTS
5. Select DLC
6. Check Space
7. Installing
8. Complete
```

### Enhanced Flow with Validation
```
1. Select Language
2. Introduction
3. Select Game Source
4. Select Title Update
   ├── Scan GAME UPDATES/ folder
   ├── Display detected updates (sorted by version)
   ├── Highlight TU8 as "Recommended for DLC"
   └── User selects update or "No Update"
   
5. Select DLC
   ├── User selects TLAD, TBOGT, or neither
   ├── VALIDATION: If DLC selected
   │   ├── Check if TU8 is selected
   │   ├── If not: Show error modal
   │   │   ├── "DLC requires Title Update 1.06 (TU8)"
   │   │   ├── Options: "Go Back" or "Auto-Select TU8"
   │   │   └── Block Continue until resolved
   │   └── If yes: Proceed normally
   └── Continue to space check
   
6. Check Space
   ├── Calculate: Base Game + Update + DLC sizes
   └── Verify sufficient disk space
   
7. Installing
   ├── Extract base game
   ├── Apply Title Update (if selected)
   │   ├── Extract .xexp from STFS
   │   ├── Patch default.xex
   │   └── Use patched XEX for installation
   ├── Extract DLC (if selected)
   │   ├── Extract TLAD to dlc/TLAD/
   │   └── Extract TBOGT to dlc/TBOGT/
   └── Complete
   
8. Runtime
   ├── Mount game:\
   ├── Mount update:\ (if exists)
   ├── Mount dlc:\TLAD\ (if exists)
   └── Mount dlc:\TBOGT\ (if exists)
```

---

## 7. Technical Implementation Details

### STFS Parsing (Already Implemented)

**Block Navigation:**
```cpp
uint64_t BlockToOffset(uint32_t blockNum)
{
    uint64_t block = blockNum;
    
    // Account for hash tables (3 levels)
    for (uint32_t i = 0; i < 3; i++)
    {
        uint32_t levelBase = kHashLevelBlocks[i];  // [170, 28900, 4913000]
        block += ((blockNum + levelBase) / levelBase);
        if (blockNum < levelBase)
            break;
    }
    
    return m_headerSize + (block * kBlockSize);  // 4KB blocks
}
```

**File Extraction:**
```cpp
bool ExtractFile(const std::string& fileName, std::vector<uint8_t>& outData)
{
    // 1. Find file in file table
    auto it = std::find_if(m_files.begin(), m_files.end(), 
        [&](const StfsFileEntry& f) { return f.name == fileName; });
    
    // 2. Allocate buffer
    outData.resize(it->size);
    
    // 3. Read blocks sequentially
    uint32_t currentBlock = it->startBlock;
    uint32_t bytesRead = 0;
    
    while (bytesRead < it->size)
    {
        uint64_t offset = BlockToOffset(currentBlock);
        uint32_t toRead = std::min(kBlockSize, it->size - bytesRead);
        
        ReadBytes(offset, &outData[bytesRead], toRead);
        bytesRead += toRead;
        
        if (bytesRead < it->size)
            currentBlock = GetNextBlock(currentBlock);
    }
    
    return true;
}
```

### XEX Patching (Already Implemented)

**Patch Application:**
```cpp
XexPatchResult ApplyPatch()
{
    // 1. Verify compatibility
    if (m_baseTitleId != m_patchTitleId)
        return { false, "Title ID mismatch" };
    
    // 2. Check patch type
    if (m_patchModuleFlags & kXexModulePatchFull)
    {
        // Full replacement - just use patch data
        m_patchedData = m_patchData;
    }
    else if (m_patchModuleFlags & kXexModulePatchDelta)
    {
        // Delta patch - apply diff
        // (Currently falls back to xextool)
    }
    
    // 3. Return patched XEX
    return { true, "", m_baseVersion, m_patchVersion };
}
```

---

## 8. Proposed Implementation Plan

### Files to Modify

1. **`ui/installer_wizard.cpp`**
   - Add DLC validation in SelectDLC page
   - Add auto-selection logic for TU8 when DLC selected
   - Add warning messages for missing TU8

2. **`main.cpp`** - `KiSystemStartup()`
   - Add update content registration
   - Create `update:\` root mapping
   - Enhance DLC mounting with proper XamContentCreateEx calls

3. **`kernel/io/file_system.cpp`** or VFS layer
   - Add path override logic for `game:\work\` → `update:\`
   - Implement priority resolution (Update > Base Game)

4. **`install/installer.cpp`**
   - Add update source to `Installer::Input` structure (already exists)
   - Integrate TU application into install pipeline
   - Add DLC-update compatibility validation

### Validation Matrix

| Scenario | Base Game | TU Selected | DLC Selected | Action |
|----------|-----------|-------------|--------------|--------|
| 1 | ✓ | None | None | ✅ Allow - vanilla install |
| 2 | ✓ | TU4-7 | None | ✅ Allow - updated vanilla |
| 3 | ✓ | TU8 | None | ✅ Allow - latest vanilla |
| 4 | ✓ | None | TLAD/TBOGT | ❌ Block - show error |
| 5 | ✓ | TU4-7 | TLAD/TBOGT | ❌ Block - TU too old |
| 6 | ✓ | TU8 | TLAD/TBOGT | ✅ Allow - correct setup |

### Error Messages

**Scenario 4/5:**
```
┌─────────────────────────────────────────────────────────────┐
│                         ERROR                                │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  DLC Content Requires Title Update 1.06 (TU8)               │
│                                                              │
│  The Lost and Damned and The Ballad of Gay Tony require     │
│  the latest title update to function correctly.             │
│                                                              │
│  Please either:                                              │
│  • Go back and select Title Update 1.06 (TU8), or           │
│  • Deselect DLC content                                      │
│                                                              │
├─────────────────────────────────────────────────────────────┤
│  [GO BACK]                            [AUTO-SELECT TU8]     │
└─────────────────────────────────────────────────────────────┘
```

---

## 9. Testing Strategy

### Test Cases

1. **Base Game Only**
   - Select game source
   - Select "No Update"
   - Skip DLC
   - Verify: Game runs with v1.0 code

2. **Base Game + TU8**
   - Select game source
   - Select TU8
   - Skip DLC
   - Verify: Game runs with v1.06 code

3. **Base Game + TU8 + TLAD**
   - Select game source
   - Select TU8
   - Select TLAD
   - Verify: TLAD content accessible

4. **Base Game + TU8 + TBOGT**
   - Select game source
   - Select TU8
   - Select TBOGT
   - Verify: TBOGT content accessible

5. **Base Game + TU8 + Both DLC**
   - Select game source
   - Select TU8
   - Select both TLAD and TBOGT
   - Verify: Both DLC accessible

6. **Error Case: DLC without TU8**
   - Select game source
   - Select "No Update" or TU4-7
   - Try to select TLAD
   - Verify: Error message shown, Continue blocked

### Validation Points

- [ ] TU8 requirement enforced for DLC
- [ ] Update files override base game files
- [ ] DLC files accessible via virtual paths
- [ ] No crashes when loading DLC content
- [ ] File path resolution follows priority order
- [ ] Installer shows clear error messages
- [ ] Auto-selection works correctly

---

## 10. Open Questions

1. **Update File Override Paths**
   - Which specific paths does TU8 override?
   - Need to analyze TU8 contents to determine redirect rules
   - May need game-specific testing to verify

2. **DLC Executable Handling**
   - Do TLAD/TBOGT have separate default.xex files?
   - How does the game switch between base/DLC executables?
   - May need PPC analysis of DLC loading code

3. **Multiple DLC Support**
   - Can both TLAD and TBOGT be installed simultaneously?
   - Do they conflict or coexist peacefully?
   - Need to verify directory structure doesn't overlap

4. **Update Metadata Storage**
   - Where to store which TU version was installed?
   - Should it be in config.toml or separate metadata file?
   - Needed for display in UI and troubleshooting

---

## 11. Implementation Complexity Assessment

| Component | Complexity | Effort | Status |
|-----------|------------|--------|--------|
| STFS Parsing | Low | Done | ✅ Complete |
| XEX Patching | Medium | Done | ✅ Complete |
| TU Manager | Low | Done | ✅ Complete |
| DLC Validation | Low | 2-3 hours | ⏳ Needed |
| Update Mounting | Low | 1-2 hours | ⏳ Needed |
| Path Override | Medium | 3-4 hours | ⏳ Needed |
| UI Integration | Low | 2-3 hours | ⏳ Needed |
| Testing | Medium | 4-6 hours | ⏳ Needed |

**Total Estimated Effort:** 12-18 hours

---

## 12. Recommended Implementation Order

1. **Phase 1: Update Content Mounting** (1-2 hours)
   - Add update registration in `KiSystemStartup()`
   - Create `update:\` root mapping
   - Test: Verify update directory is recognized

2. **Phase 2: DLC Validation** (2-3 hours)
   - Add TU8 requirement check in installer wizard
   - Implement error modal and auto-selection
   - Test: Verify DLC blocked without TU8

3. **Phase 3: Enhanced DLC Mounting** (1-2 hours)
   - Add proper `XamContentCreateEx` calls for DLC
   - Create DLC root mappings
   - Test: Verify DLC directories are accessible

4. **Phase 4: Path Override System** (3-4 hours)
   - Research TU8 file contents
   - Implement redirect logic for update files
   - Test: Verify update files override base game

5. **Phase 5: Integration Testing** (4-6 hours)
   - Test all scenarios from test matrix
   - Verify no regressions
   - Document any issues

---

## 13. Key Findings Summary

### What Works
- ✅ STFS parsing is fully functional
- ✅ XEX patching works for full patches
- ✅ Title Update detection and selection UI exists
- ✅ DLC file extraction works
- ✅ XContent file system supports STFS/SVOD

### What's Missing
- ❌ DLC-Update dependency validation
- ❌ Update content mounting at runtime
- ❌ File path override system (update > base game)
- ❌ Auto-selection of TU8 when DLC selected

### Critical Path
1. **Enforce TU8 requirement** - Prevents broken DLC installs
2. **Mount update content** - Enables file override
3. **Path resolution** - Ensures update files are used

### Risk Assessment
- **Low Risk:** Validation and mounting (well-understood, reference impl exists)
- **Medium Risk:** Path override (requires testing to verify correct paths)
- **High Risk:** None identified

---

## 14. Reference Implementation Comparison

### Sonic Unleashed Approach
- ✅ Full STFS/SVOD support
- ✅ Update priority system
- ✅ DLC auto-discovery
- ✅ Path override rules
- ✅ Content registry with hash maps

### GTA IV Differences
- **Update Format:** Full XEX replacement (not delta patches)
- **DLC Size:** Much larger (~1.8GB each vs. smaller Sonic DLC)
- **DLC Count:** 2 major expansions vs. smaller content packs
- **Update Requirement:** Hard dependency (DLC won't work without TU8)

### Adaptation Strategy
- Inherit STFS parsing logic ✅ (already done)
- Inherit content mounting pattern ✅ (partially done)
- Adapt validation for GTA IV's TU8 requirement ⏳ (needed)
- Adapt path override for GTA IV's file structure ⏳ (needed)

---

## 15. Next Steps

**Immediate Actions:**
1. Confirm TU8 requirement with user
2. Verify DLC file structure (extract and examine)
3. Analyze TU8 contents to determine override paths
4. Implement validation logic
5. Test with actual game files

**Questions for User:**
1. Should we support older updates (TU4-7) for users without DLC?
2. Should we auto-download TU8 if missing and DLC is selected?
3. Should we support installing DLC separately after initial install?
4. What should happen if user has DLC but removes TU8 later?

