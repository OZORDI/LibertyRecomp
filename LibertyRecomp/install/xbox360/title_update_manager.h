/**
 * Title Update Manager
 * Handles detection, selection, and application of Xbox 360 Title Updates
 */

#pragma once

#include "stfs_parser.h"
#include "xex_patcher.h"
#include <filesystem>
#include <vector>
#include <optional>

namespace liberty {
namespace install {

// Detected Title Update entry
struct DetectedTitleUpdate {
    std::filesystem::path path;
    TitleUpdateInfo info;
    bool isSelected;
};

// Result of applying a title update
struct TitleUpdateApplyResult {
    bool success;
    std::string errorMessage;
    std::filesystem::path patchedXexPath;
    uint32_t appliedVersion;
};

class TitleUpdateManager {
public:
    TitleUpdateManager();
    ~TitleUpdateManager();

    // Scan a directory for Title Update files
    // Returns number of valid TU files found
    int ScanDirectory(const std::filesystem::path& directory);
    
    // Add a single Title Update file
    bool AddTitleUpdate(const std::filesystem::path& path);
    
    // Get list of detected updates
    const std::vector<DetectedTitleUpdate>& GetDetectedUpdates() const { return m_updates; }
    
    // Select an update by index (or -1 for no update)
    void SelectUpdate(int index);
    
    // Get the currently selected update (if any)
    std::optional<DetectedTitleUpdate> GetSelectedUpdate() const;
    
    // Check if any update is selected
    bool HasSelectedUpdate() const;
    
    // Apply the selected update to a base XEX
    // Returns the path to the patched XEX (or original if no update selected)
    TitleUpdateApplyResult ApplySelectedUpdate(
        const std::filesystem::path& baseXexPath,
        const std::filesystem::path& outputDir);
    
    // Clear all detected updates
    void Clear();
    
    // Get the expected Title ID for GTA IV
    static uint32_t GetGTA4TitleId() { return 0x545407F2; }
    
    // Version string helpers
    static std::string VersionToString(uint32_t version);
    static std::string GetUpdateDisplayName(const TitleUpdateInfo& info);

private:
    std::vector<DetectedTitleUpdate> m_updates;
    int m_selectedIndex;
    
    // Try to apply update using native code
    bool TryNativeApply(const std::filesystem::path& baseXex,
                        const std::filesystem::path& xexpPath,
                        const std::filesystem::path& outputPath);
    
    // Fall back to xextool binary
    bool TryXextoolApply(const std::filesystem::path& baseXex,
                         const std::filesystem::path& xexpPath,
                         const std::filesystem::path& outputPath);
};

} // namespace install
} // namespace liberty
