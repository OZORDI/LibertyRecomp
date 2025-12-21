/**
 * Title Update Manager
 * Handles detection, selection, and application of Xbox 360 Title Updates
 */

#include "title_update_manager.h"
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

namespace liberty {
namespace install {

TitleUpdateManager::TitleUpdateManager()
    : m_selectedIndex(-1)
{
}

TitleUpdateManager::~TitleUpdateManager()
{
}

int TitleUpdateManager::ScanDirectory(const std::filesystem::path& directory)
{
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
        return 0;
    }
    
    int count = 0;
    
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            if (AddTitleUpdate(entry.path())) {
                count++;
            }
        }
    }
    
    // Sort by version (highest first)
    std::sort(m_updates.begin(), m_updates.end(),
        [](const DetectedTitleUpdate& a, const DetectedTitleUpdate& b) {
            return a.info.version > b.info.version;
        });
    
    return count;
}

bool TitleUpdateManager::AddTitleUpdate(const std::filesystem::path& path)
{
    StfsParser parser;
    
    if (!parser.Open(path)) {
        return false;
    }
    
    if (!parser.IsTitleUpdate()) {
        return false;
    }
    
    TitleUpdateInfo info = parser.GetTitleUpdateInfo();
    
    // Optionally filter by Title ID (GTA IV)
    // if (info.titleId != GetGTA4TitleId()) {
    //     return false;
    // }
    
    DetectedTitleUpdate update;
    update.path = path;
    update.info = info;
    update.isSelected = false;
    
    // Check for duplicates
    auto it = std::find_if(m_updates.begin(), m_updates.end(),
        [&info](const DetectedTitleUpdate& u) {
            return u.info.version == info.version && u.info.titleId == info.titleId;
        });
    
    if (it != m_updates.end()) {
        return false; // Duplicate version
    }
    
    m_updates.push_back(update);
    return true;
}

void TitleUpdateManager::SelectUpdate(int index)
{
    // Deselect all
    for (auto& update : m_updates) {
        update.isSelected = false;
    }
    
    m_selectedIndex = index;
    
    if (index >= 0 && index < static_cast<int>(m_updates.size())) {
        m_updates[index].isSelected = true;
    }
}

std::optional<DetectedTitleUpdate> TitleUpdateManager::GetSelectedUpdate() const
{
    if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_updates.size())) {
        return m_updates[m_selectedIndex];
    }
    return std::nullopt;
}

bool TitleUpdateManager::HasSelectedUpdate() const
{
    return m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_updates.size());
}

TitleUpdateApplyResult TitleUpdateManager::ApplySelectedUpdate(
    const std::filesystem::path& baseXexPath,
    const std::filesystem::path& outputDir)
{
    TitleUpdateApplyResult result;
    result.success = false;
    result.appliedVersion = 0;
    
    if (!HasSelectedUpdate()) {
        // No update selected - just copy the base XEX
        result.patchedXexPath = baseXexPath;
        result.success = true;
        return result;
    }
    
    auto selectedUpdate = GetSelectedUpdate();
    if (!selectedUpdate) {
        result.errorMessage = "No update selected";
        return result;
    }
    
    // Parse the STFS container to extract the XEXP file
    StfsParser parser;
    if (!parser.Open(selectedUpdate->path)) {
        result.errorMessage = "Failed to open Title Update package";
        return result;
    }
    
    // Look for .xexp file in the package
    auto files = parser.GetFileList();
    std::string xexpFileName;
    
    for (const auto& file : files) {
        if (file.name.size() >= 5) {
            std::string ext = file.name.substr(file.name.size() - 5);
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (ext == ".xexp") {
                xexpFileName = file.name;
                break;
            }
        }
    }
    
    // Also check for default.xexp specifically
    if (xexpFileName.empty()) {
        for (const auto& file : files) {
            std::string name = file.name;
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            if (name == "default.xexp") {
                xexpFileName = file.name;
                break;
            }
        }
    }
    
    if (xexpFileName.empty()) {
        result.errorMessage = "No .xexp patch file found in Title Update package";
        return result;
    }
    
    // Extract the XEXP to a temp location
    std::filesystem::path tempXexpPath = outputDir / "temp_patch.xexp";
    if (!parser.ExtractFileToDisk(xexpFileName, tempXexpPath)) {
        result.errorMessage = "Failed to extract .xexp from package";
        return result;
    }
    
    // Output path for patched XEX
    std::filesystem::path patchedXexPath = outputDir / "default.xex";
    
    // Try native patching first
    if (TryNativeApply(baseXexPath, tempXexpPath, patchedXexPath)) {
        result.success = true;
        result.patchedXexPath = patchedXexPath;
        result.appliedVersion = selectedUpdate->info.version;
        
        // Clean up temp file
        std::filesystem::remove(tempXexpPath);
        return result;
    }
    
    // Fall back to xextool
    if (TryXextoolApply(baseXexPath, tempXexpPath, patchedXexPath)) {
        result.success = true;
        result.patchedXexPath = patchedXexPath;
        result.appliedVersion = selectedUpdate->info.version;
        
        // Clean up temp file
        std::filesystem::remove(tempXexpPath);
        return result;
    }
    
    result.errorMessage = "Failed to apply patch. Please ensure xextool is available or use a pre-patched XEX.";
    
    // Clean up temp file
    std::filesystem::remove(tempXexpPath);
    
    return result;
}

void TitleUpdateManager::Clear()
{
    m_updates.clear();
    m_selectedIndex = -1;
}

std::string TitleUpdateManager::VersionToString(uint32_t version)
{
    // Xbox 360 version format: major.minor.build.qfe
    uint8_t major = (version >> 28) & 0x0F;
    uint8_t minor = (version >> 24) & 0x0F;
    uint16_t build = (version >> 8) & 0xFFFF;
    uint8_t qfe = version & 0xFF;
    
    std::ostringstream ss;
    ss << static_cast<int>(major) << "."
       << static_cast<int>(minor) << "."
       << build << "."
       << static_cast<int>(qfe);
    return ss.str();
}

std::string TitleUpdateManager::GetUpdateDisplayName(const TitleUpdateInfo& info)
{
    std::ostringstream ss;
    ss << "Title Update v" << VersionToString(info.version);
    if (!info.displayName.empty()) {
        ss << " - " << info.displayName;
    }
    return ss.str();
}

bool TitleUpdateManager::TryNativeApply(const std::filesystem::path& baseXex,
                                         const std::filesystem::path& xexpPath,
                                         const std::filesystem::path& outputPath)
{
    XexPatcher patcher;
    
    if (!patcher.LoadBaseXex(baseXex)) {
        return false;
    }
    
    if (!patcher.LoadPatch(xexpPath)) {
        return false;
    }
    
    auto result = patcher.ApplyPatch();
    if (!result.success) {
        return false;
    }
    
    return patcher.SavePatchedXex(outputPath);
}

bool TitleUpdateManager::TryXextoolApply(const std::filesystem::path& baseXex,
                                          const std::filesystem::path& xexpPath,
                                          const std::filesystem::path& outputPath)
{
    // Try to find and run xextool
    std::string command;
    
#ifdef _WIN32
    // On Windows, try xextool.exe directly
    command = "xextool.exe -p \"" + xexpPath.string() + "\" -o \"" + 
              outputPath.string() + "\" \"" + baseXex.string() + "\"";
    
    // Try to execute
    int result = std::system(command.c_str());
    return result == 0;
#else
    // On macOS/Linux, try with wine
    command = "wine xextool.exe -p \"" + xexpPath.string() + "\" -o \"" + 
              outputPath.string() + "\" \"" + baseXex.string() + "\" 2>/dev/null";
    
    int result = std::system(command.c_str());
    if (result == 0 && std::filesystem::exists(outputPath)) {
        return true;
    }
    
    // Try without wine (if somehow available natively)
    command = "xextool -p \"" + xexpPath.string() + "\" -o \"" + 
              outputPath.string() + "\" \"" + baseXex.string() + "\" 2>/dev/null";
    
    result = std::system(command.c_str());
    return result == 0 && std::filesystem::exists(outputPath);
#endif
}

} // namespace install
} // namespace liberty
