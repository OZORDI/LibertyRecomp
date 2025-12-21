#include "installer.h"
#include "shader_converter.h"
#include "iso_extractor.h"
#include "rpf_extractor.h"
#include "platform_paths.h"
#include "xbox360/title_update_manager.h"

#include <xxh3.h>

#include "directory_file_system.h"
#include "iso_file_system.h"
#include "xcontent_file_system.h"

// GTA IV uses simplified hash validation - DLC hashes TBD
#include "hashes/game.h"

static const std::string GameDirectory = "game";
static const std::string DLCDirectory = "dlc";
static const std::string TLADDirectory = DLCDirectory + "/TLAD";  // The Lost and Damned
static const std::string TBOGTDirectory = DLCDirectory + "/TBOGT"; // The Ballad of Gay Tony
static const std::string GameExecutableFile = "default.xex";
static const std::string DLCValidationFile = "default.xex";  // GTA IV DLC uses xex as validation
static const std::string ISOExtension = ".iso";
static const std::string ZIPExtension = ".zip";

// Temp directory for extracted ZIP files
static std::filesystem::path g_tempExtractDir;

// GTA IV RPF archives that need extraction
static const std::vector<std::string> GameRpfArchives = {
    "common.rpf",
    "xbox360.rpf",
    "audio.rpf"
};

static std::string fromU8(const std::u8string &str)
{
    return std::string(str.begin(), str.end());
}

static std::string fromPath(const std::filesystem::path &path)
{
    return fromU8(path.u8string());
}

static std::string toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
    return str;
};

// Extract ZIP file to temp directory and return extracted STFS path
static std::filesystem::path extractZipToTemp(const std::filesystem::path &zipPath)
{
    // Create temp directory if needed
    if (g_tempExtractDir.empty())
    {
        g_tempExtractDir = std::filesystem::temp_directory_path() / "LibertyRecomp_DLC";
    }
    
    // Create unique extraction folder based on ZIP filename
    std::string zipName = zipPath.stem().string();
    std::filesystem::path extractDir = g_tempExtractDir / zipName;
    
    // Clean up any existing extraction
    std::error_code ec;
    if (std::filesystem::exists(extractDir))
    {
        std::filesystem::remove_all(extractDir, ec);
    }
    std::filesystem::create_directories(extractDir, ec);
    
    // Use system unzip command (available on macOS/Linux)
    std::string cmd = fmt::format("unzip -q -o \"{}\" -d \"{}\"", 
        zipPath.string(), extractDir.string());
    int result = std::system(cmd.c_str());
    
    if (result != 0)
    {
        return {};
    }
    
    // Find the STFS file inside (usually in a subfolder like 545407F2/00000002/...)
    for (const auto& entry : std::filesystem::recursive_directory_iterator(extractDir))
    {
        if (entry.is_regular_file() && XContentFileSystem::check(entry.path()))
        {
            return entry.path();
        }
    }
    
    // If no STFS found, return the extract dir itself (might be a folder-based DLC)
    return extractDir;
}

static std::unique_ptr<VirtualFileSystem> createFileSystemFromPath(const std::filesystem::path &path)
{
    // Check for ZIP files first - extract and then parse
    if (toLower(fromPath(path.extension())) == ZIPExtension)
    {
        std::filesystem::path extractedPath = extractZipToTemp(path);
        if (!extractedPath.empty())
        {
            // Try to create file system from extracted content
            if (XContentFileSystem::check(extractedPath))
            {
                return XContentFileSystem::create(extractedPath);
            }
            else if (std::filesystem::is_directory(extractedPath))
            {
                return DirectoryFileSystem::create(extractedPath);
            }
        }
        return nullptr;
    }
    
    if (XContentFileSystem::check(path))
    {
        return XContentFileSystem::create(path);
    }
    else if (toLower(fromPath(path.extension())) == ISOExtension)
    {
        return ISOFileSystem::create(path);
    }
    else if (std::filesystem::is_directory(path))
    {
        return DirectoryFileSystem::create(path);
    }
    else
    {
        return nullptr;
    }
}

static bool checkFile(const FilePair &pair, const uint64_t *fileHashes, const std::filesystem::path &targetDirectory, std::vector<uint8_t> &fileData, Journal &journal, const std::function<bool()> &progressCallback, bool checkSizeOnly) {
    const std::string fileName(pair.first);
    const uint32_t hashCount = pair.second;
    const std::filesystem::path filePath = targetDirectory / fileName;
    if (!std::filesystem::exists(filePath))
    {
        journal.lastResult = Journal::Result::FileMissing;
        journal.lastErrorMessage = fmt::format("File {} does not exist.", fileName);
        return false;
    }

    std::error_code ec;
    size_t fileSize = std::filesystem::file_size(filePath, ec);
    if (ec)
    {
        journal.lastResult = Journal::Result::FileReadFailed;
        journal.lastErrorMessage = fmt::format("Failed to read file size for {}.", fileName);
        return false;
    }

    if (checkSizeOnly)
    {
        journal.progressTotal += fileSize;
    }
    else
    {
        std::ifstream fileStream(filePath, std::ios::binary);
        if (fileStream.is_open())
        {
            fileData.resize(fileSize);
            fileStream.read((char *)(fileData.data()), fileSize);
        }

        if (!fileStream.is_open() || fileStream.bad())
        {
            journal.lastResult = Journal::Result::FileReadFailed;
            journal.lastErrorMessage = fmt::format("Failed to read file {}.", fileName);
            return false;
        }

        // Skip hash validation if no hashes are specified (hashCount == 0)
        if (hashCount > 0)
        {
            uint64_t fileHash = XXH3_64bits(fileData.data(), fileSize);
            bool fileHashFound = false;
            for (uint32_t i = 0; i < hashCount && !fileHashFound; i++)
            {
                fileHashFound = fileHash == fileHashes[i];
            }

            if (!fileHashFound)
            {
                journal.lastResult = Journal::Result::FileHashFailed;
                journal.lastErrorMessage = fmt::format("File {} did not match any of the known hashes.", fileName);
                return false;
            }
        }

        journal.progressCounter += fileSize;
    }

    if (!progressCallback())
    {
        journal.lastResult = Journal::Result::Cancelled;
        journal.lastErrorMessage = "Check was cancelled.";
        return false;
    }

    return true;
}

static bool copyFile(const FilePair &pair, const uint64_t *fileHashes, VirtualFileSystem &sourceVfs, const std::filesystem::path &targetDirectory, bool skipHashChecks, std::vector<uint8_t> &fileData, Journal &journal, const std::function<bool()> &progressCallback) {
    const std::string filename(pair.first);
    const uint32_t hashCount = pair.second;
    if (!sourceVfs.exists(filename))
    {
        journal.lastResult = Journal::Result::FileMissing;
        journal.lastErrorMessage = fmt::format("File {} does not exist in {}.", filename, sourceVfs.getName());
        return false;
    }

    if (!sourceVfs.load(filename, fileData))
    {
        journal.lastResult = Journal::Result::FileReadFailed;
        journal.lastErrorMessage = fmt::format("Failed to read file {} from {}.", filename, sourceVfs.getName());
        return false;
    }

    if (!skipHashChecks && hashCount > 0)
    {
        uint64_t fileHash = XXH3_64bits(fileData.data(), fileData.size());
        bool fileHashFound = false;
        for (uint32_t i = 0; i < hashCount && !fileHashFound; i++)
        {
            fileHashFound = fileHash == fileHashes[i];
        }

        if (!fileHashFound)
        {
            journal.lastResult = Journal::Result::FileHashFailed;
            journal.lastErrorMessage = fmt::format("File {} from {} did not match any of the known hashes.", filename, sourceVfs.getName());
            return false;
        }
    }

    std::filesystem::path targetPath = targetDirectory / std::filesystem::path(std::u8string_view((const char8_t *)(pair.first)));
    std::filesystem::path parentPath = targetPath.parent_path();
    if (!std::filesystem::exists(parentPath))
    {
        std::error_code ec;
        std::filesystem::create_directories(parentPath, ec);
    }
    
    while (!parentPath.empty()) {
        journal.createdDirectories.insert(parentPath);

        if (parentPath != targetDirectory) {
            parentPath = parentPath.parent_path();
        }
        else {
            parentPath = std::filesystem::path();
        }
    }

    std::ofstream outStream(targetPath, std::ios::binary);
    if (!outStream.is_open())
    {
        journal.lastResult = Journal::Result::FileCreationFailed;
        journal.lastErrorMessage = fmt::format("Failed to create file at {}.", fromPath(targetPath));
        return false;
    }

    journal.createdFiles.push_back(targetPath);

    outStream.write((const char *)(fileData.data()), fileData.size());
    if (outStream.bad())
    {
        journal.lastResult = Journal::Result::FileWriteFailed;
        journal.lastErrorMessage = fmt::format("Failed to create file at {}.", fromPath(targetPath));
        return false;
    }

    journal.progressCounter += fileData.size();
    
    if (!progressCallback())
    {
        journal.lastResult = Journal::Result::Cancelled;
        journal.lastErrorMessage = "Installation was cancelled.";
        return false;
    }

    return true;
}

static DLC detectDLC(const std::filesystem::path &sourcePath, VirtualFileSystem &sourceVfs, Journal &journal)
{
    // GTA IV DLC detection - check for specific files
    // TLAD has tlad.rpf, TBOGT has tbogt.rpf
    if (sourceVfs.exists("tlad.rpf") || sourceVfs.exists("TLAD/tlad.rpf"))
    {
        return DLC::TheLostAndDamned;
    }
    else if (sourceVfs.exists("tbogt.rpf") || sourceVfs.exists("TBOGT/tbogt.rpf"))
    {
        return DLC::TheBalladOfGayTony;
    }
    
    // Try to detect by folder name
    std::string pathStr = sourcePath.filename().string();
    std::transform(pathStr.begin(), pathStr.end(), pathStr.begin(), ::tolower);
    
    if (pathStr.find("lost") != std::string::npos || pathStr.find("tlad") != std::string::npos)
    {
        return DLC::TheLostAndDamned;
    }
    else if (pathStr.find("ballad") != std::string::npos || pathStr.find("tbogt") != std::string::npos || pathStr.find("gay tony") != std::string::npos)
    {
        return DLC::TheBalladOfGayTony;
    }

    journal.lastResult = Journal::Result::UnknownDLCType;
    journal.lastErrorMessage = fmt::format("Could not detect GTA IV DLC type for {}.", sourcePath.string());
    return DLC::Unknown;
}

static bool fillDLCSource(DLC dlc, Installer::DLCSource &dlcSource) 
{
    switch (dlc)
    {
    case DLC::TheLostAndDamned:
        // TLAD uses same file structure - will need TLAD-specific hashes later
        dlcSource.filePairs = { }; // Empty for now - DLC support TBD
        dlcSource.fileHashes = nullptr;
        dlcSource.targetSubDirectory = TLADDirectory;
        return true;
    case DLC::TheBalladOfGayTony:
        // TBOGT uses same file structure - will need TBOGT-specific hashes later
        dlcSource.filePairs = { }; // Empty for now - DLC support TBD
        dlcSource.fileHashes = nullptr;
        dlcSource.targetSubDirectory = TBOGTDirectory;
        return true;
    default:
        return false;
    }
}

// Copy all files from a VFS to a target directory (used for DLC installation)
static bool copyAllFiles(VirtualFileSystem &sourceVfs, const std::filesystem::path &targetDirectory, Journal &journal, const std::function<bool()> &progressCallback)
{
    std::error_code ec;
    if (!std::filesystem::exists(targetDirectory) && !std::filesystem::create_directories(targetDirectory, ec))
    {
        journal.lastResult = Journal::Result::DirectoryCreationFailed;
        journal.lastErrorMessage = "Unable to create directory at " + fromPath(targetDirectory);
        return false;
    }

    std::vector<std::string> fileList = sourceVfs.getFileList();
    std::vector<uint8_t> fileData;
    
    for (const std::string &filename : fileList)
    {
        if (!sourceVfs.load(filename, fileData))
        {
            journal.lastResult = Journal::Result::FileReadFailed;
            journal.lastErrorMessage = fmt::format("Failed to read file {} from {}.", filename, sourceVfs.getName());
            return false;
        }

        std::filesystem::path targetPath = targetDirectory / std::filesystem::path(std::u8string_view((const char8_t *)(filename.c_str())));
        std::filesystem::path parentPath = targetPath.parent_path();
        if (!std::filesystem::exists(parentPath))
        {
            std::filesystem::create_directories(parentPath, ec);
        }
        
        while (!parentPath.empty()) {
            journal.createdDirectories.insert(parentPath);

            if (parentPath != targetDirectory) {
                parentPath = parentPath.parent_path();
            }
            else {
                parentPath = std::filesystem::path();
            }
        }

        std::ofstream outStream(targetPath, std::ios::binary);
        if (!outStream.is_open())
        {
            journal.lastResult = Journal::Result::FileCreationFailed;
            journal.lastErrorMessage = fmt::format("Failed to create file at {}.", fromPath(targetPath));
            return false;
        }

        journal.createdFiles.push_back(targetPath);

        outStream.write((const char *)(fileData.data()), fileData.size());
        if (outStream.bad())
        {
            journal.lastResult = Journal::Result::FileWriteFailed;
            journal.lastErrorMessage = fmt::format("Failed to write file at {}.", fromPath(targetPath));
            return false;
        }

        journal.progressCounter += fileData.size();
        
        if (!progressCallback())
        {
            journal.lastResult = Journal::Result::Cancelled;
            journal.lastErrorMessage = "Installation was cancelled.";
            return false;
        }
    }

    return true;
}

bool Installer::checkGameInstall(const std::filesystem::path &baseDirectory, std::filesystem::path &modulePath)
{
    modulePath = baseDirectory / GameDirectory / GameExecutableFile;

    if (!std::filesystem::exists(modulePath))
        return false;

    return true;
}

bool Installer::checkDLCInstall(const std::filesystem::path &baseDirectory, DLC dlc)
{
    switch (dlc)
    {
    case DLC::TheLostAndDamned:
        return std::filesystem::exists(baseDirectory / TLADDirectory / DLCValidationFile);
    case DLC::TheBalladOfGayTony:
        return std::filesystem::exists(baseDirectory / TBOGTDirectory / DLCValidationFile);
    default:
        return false;
    }
}

bool Installer::checkAllDLC(const std::filesystem::path& baseDirectory)
{
    bool result = true;

    for (int i = 1; i < (int)DLC::Count; i++)
    {
        if (!checkDLCInstall(baseDirectory, (DLC)i))
            result = false;
    }

    return result;
}

bool Installer::checkInstallIntegrity(const std::filesystem::path &baseDirectory, Journal &journal, const std::function<bool()> &progressCallback)
{
    // Run the file checks twice: once to fill out the progress counter and the file sizes, and another pass to do the hash integrity checks.
    for (uint32_t checkPass = 0; checkPass < 2; checkPass++)
    {
        bool checkSizeOnly = (checkPass == 0);
        if (!checkFiles({ GameFiles, GameFilesSize }, GameHashes, baseDirectory / GameDirectory, journal, progressCallback, checkSizeOnly))
        {
            return false;
        }

        for (int i = 1; i < (int)DLC::Count; i++)
        {
            if (checkDLCInstall(baseDirectory, (DLC)i))
            {
                Installer::DLCSource dlcSource;
                fillDLCSource((DLC)i, dlcSource);

                if (!checkFiles(dlcSource.filePairs, dlcSource.fileHashes, baseDirectory / dlcSource.targetSubDirectory, journal, progressCallback, checkSizeOnly))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool Installer::computeTotalSize(std::span<const FilePair> filePairs, const uint64_t *fileHashes, VirtualFileSystem &sourceVfs, Journal &journal, uint64_t &totalSize)
{
    for (FilePair pair : filePairs)
    {
        const std::string filename(pair.first);
        if (!sourceVfs.exists(filename))
        {
            journal.lastResult = Journal::Result::FileMissing;
            journal.lastErrorMessage = fmt::format("File {} does not exist in {}.", filename, sourceVfs.getName());
            return false;
        }

        totalSize += sourceVfs.getSize(filename);
    }

    return true;
}

bool Installer::checkFiles(std::span<const FilePair> filePairs, const uint64_t *fileHashes, const std::filesystem::path &targetDirectory, Journal &journal, const std::function<bool()> &progressCallback, bool checkSizeOnly)
{
    FilePair validationPair = {};
    uint32_t validationHashIndex = 0;
    uint32_t hashIndex = 0;
    uint32_t hashCount = 0;
    std::vector<uint8_t> fileData;
    for (FilePair pair : filePairs)
    {
        hashIndex = hashCount;
        hashCount += pair.second;

        if (!checkFile(pair, &fileHashes[hashIndex], targetDirectory, fileData, journal, progressCallback, checkSizeOnly))
        {
            return false;
        }
    }

    return true;
}

bool Installer::copyFiles(std::span<const FilePair> filePairs, const uint64_t *fileHashes, VirtualFileSystem &sourceVfs, const std::filesystem::path &targetDirectory, const std::string &validationFile, bool skipHashChecks, Journal &journal, const std::function<bool()> &progressCallback)
{
    std::error_code ec;
    if (!std::filesystem::exists(targetDirectory) && !std::filesystem::create_directories(targetDirectory, ec))
    {
        journal.lastResult = Journal::Result::DirectoryCreationFailed;
        journal.lastErrorMessage = "Unable to create directory at " + fromPath(targetDirectory);
        return false;
    }

    uint32_t hashIndex = 0;
    uint32_t hashCount = 0;
    std::vector<uint8_t> fileData;
    for (FilePair pair : filePairs)
    {
        hashIndex = hashCount;
        hashCount += pair.second;

        if (!copyFile(pair, &fileHashes[hashIndex], sourceVfs, targetDirectory, skipHashChecks, fileData, journal, progressCallback))
        {
            return false;
        }
    }

    return true;
}

bool Installer::parseContent(const std::filesystem::path &sourcePath, std::unique_ptr<VirtualFileSystem> &targetVfs, Journal &journal)
{
    targetVfs = createFileSystemFromPath(sourcePath);
    if (targetVfs != nullptr)
    {
        return true;
    }
    else
    {
        journal.lastResult = Journal::Result::VirtualFileSystemFailed;
        journal.lastErrorMessage = "Unable to open " + fromPath(sourcePath);
        return false;
    }
}

bool Installer::parseSources(const Input &input, Journal &journal, Sources &sources)
{
    journal = Journal();
    sources = Sources();

    // Parse the contents of the base game.
    if (!input.gameSource.empty())
    {
        if (!parseContent(input.gameSource, sources.game, journal))
        {
            return false;
        }

        // Use actual directory size instead of just the predefined file list
        uint64_t gameSize = sources.game->getTotalSize();
        if (gameSize > 0)
        {
            sources.totalSize += gameSize;
        }
        else
        {
            // Fallback to file list if getTotalSize not supported
            if (!computeTotalSize({ GameFiles, GameFilesSize }, GameHashes, *sources.game, journal, sources.totalSize))
            {
                return false;
            }
        }
    }

    // Parse the contents of the title update (GTA IV uses full XEX replacement).
    if (!input.updateSource.empty())
    {
        if (!parseContent(input.updateSource, sources.update, journal))
        {
            return false;
        }
        
        // Title update contains replacement default.xex - add its size
        if (sources.update->exists(GameExecutableFile))
        {
            sources.totalSize += sources.update->getSize(GameExecutableFile);
        }
    }

    // Parse the contents of the DLC Packs.
    for (const auto &path : input.dlcSources)
    {
        sources.dlc.emplace_back();
        DLCSource &dlcSource = sources.dlc.back();
        if (!parseContent(path, dlcSource.sourceVfs, journal))
        {
            return false;
        }

        DLC dlc = detectDLC(path, *dlcSource.sourceVfs, journal);
        if (!fillDLCSource(dlc, dlcSource))
        {
            return false;
        }

        // Use getTotalSize() to get actual DLC size instead of empty filePairs
        uint64_t dlcSize = dlcSource.sourceVfs->getTotalSize();
        if (dlcSize > 0)
        {
            sources.totalSize += dlcSize;
        }
        else if (!dlcSource.filePairs.empty())
        {
            // Fallback to file list if getTotalSize not supported
            if (!computeTotalSize(dlcSource.filePairs, dlcSource.fileHashes, *dlcSource.sourceVfs, journal, sources.totalSize))
            {
                return false;
            }
        }
    }

    // Add the total size in bytes as the journal progress.
    journal.progressTotal += sources.totalSize;

    return true;
}

bool Installer::install(const Sources &sources, const std::filesystem::path &targetDirectory, bool skipHashChecks, Journal &journal, std::chrono::seconds endWaitTime, const std::function<bool()> &progressCallback)
{
    // Install files in reverse order of importance. In case of a process crash or power outage, this will increase the likelihood of the installation
    // missing critical files required for the game to run. These files are used as the way to detect if the game is installed.

    // Install the DLC.
    if (!sources.dlc.empty())
    {
        journal.createdDirectories.insert(targetDirectory / DLCDirectory);
    }

    for (const DLCSource &dlcSource : sources.dlc)
    {
        // If we have a predefined file list, use it; otherwise copy all files from VFS
        if (!dlcSource.filePairs.empty())
        {
            if (!copyFiles(dlcSource.filePairs, dlcSource.fileHashes, *dlcSource.sourceVfs, targetDirectory / dlcSource.targetSubDirectory, DLCValidationFile, skipHashChecks, journal, progressCallback))
            {
                return false;
            }
        }
        else
        {
            // Copy all files from DLC VFS
            if (!copyAllFiles(*dlcSource.sourceVfs, targetDirectory / dlcSource.targetSubDirectory, journal, progressCallback))
            {
                return false;
            }
        }
    }

    // If no game was specified, we're finished. This means the user was only installing the DLC.
    if ((sources.game == nullptr))
    {
        return true;
    }

    // Install the base game.
    if (!copyFiles({ GameFiles, GameFilesSize }, GameHashes, *sources.game, targetDirectory / GameDirectory, GameExecutableFile, skipHashChecks, journal, progressCallback))
    {
        return false;
    }
    
    // If title update provided, replace default.xex with updated version
    // GTA IV title updates are full XEX replacements (not .xexp patches)
    if (sources.update != nullptr && sources.update->exists(GameExecutableFile))
    {
        std::filesystem::path gameXexPath = targetDirectory / GameDirectory / GameExecutableFile;
        std::vector<uint8_t> updateXex;
        if (sources.update->load(GameExecutableFile, updateXex))
        {
            std::ofstream outFile(gameXexPath, std::ios::binary);
            if (outFile.is_open())
            {
                outFile.write(reinterpret_cast<const char*>(updateXex.data()), updateXex.size());
                journal.progressCounter += updateXex.size();
            }
        }
    }
    
    // Ensure platform directories exist
    PlatformPaths::EnsureDirectoriesExist();
    
    // Auto-copy AES key if found in source or game directory
    // Check multiple possible locations for the AES key
    std::filesystem::path aesKeyDest = PlatformPaths::GetAesKeyPath();
    if (!std::filesystem::exists(aesKeyDest))
    {
        std::error_code ec;
        // Try to find aes_key.bin in common locations
        std::vector<std::filesystem::path> aesKeySearchPaths = {
            targetDirectory / GameDirectory / "aes_key.bin",
            targetDirectory / "aes_key.bin",
            std::filesystem::current_path() / "aes_key.bin",
            std::filesystem::current_path().parent_path() / "aes_key.bin"
        };
        
        // Also try source VFS
        if (sources.game && sources.game->exists("aes_key.bin"))
        {
            std::vector<uint8_t> keyData;
            if (sources.game->load("aes_key.bin", keyData) && keyData.size() >= 32)
            {
                std::ofstream outFile(aesKeyDest, std::ios::binary);
                if (outFile.is_open())
                {
                    outFile.write(reinterpret_cast<const char*>(keyData.data()), keyData.size());
                }
            }
        }
        else
        {
            // Search filesystem paths
            for (const auto& searchPath : aesKeySearchPaths)
            {
                if (std::filesystem::exists(searchPath, ec) && std::filesystem::file_size(searchPath, ec) >= 32)
                {
                    std::filesystem::copy_file(searchPath, aesKeyDest, std::filesystem::copy_options::overwrite_existing, ec);
                    if (!ec) break;
                }
            }
        }
    }
    
    // Extract RPF archives (GTA IV specific)
    // This extracts common.rpf, xbox360.rpf, audio.rpf to their respective folders
    std::filesystem::path gameDir = targetDirectory / GameDirectory;
    {
        std::vector<uint8_t> aesKey;
        std::filesystem::path aesKeyPath = PlatformPaths::GetAesKeyPath();
        if (std::filesystem::exists(aesKeyPath))
        {
            RpfExtractor::LoadAesKey(aesKeyPath, aesKey);
        }
        
        for (const auto& rpfName : GameRpfArchives)
        {
            std::filesystem::path rpfPath = gameDir / rpfName;
            if (!std::filesystem::exists(rpfPath))
            {
                continue; // RPF not found, skip
            }
            
            // Determine output directory based on RPF name
            std::string baseName = rpfName.substr(0, rpfName.find('.'));
            std::filesystem::path outputDir = gameDir / baseName;
            
            // Skip if already extracted
            if (std::filesystem::exists(outputDir) && !std::filesystem::is_empty(outputDir))
            {
                continue;
            }
            
            // Extract RPF contents
            std::error_code ec;
            std::filesystem::create_directories(outputDir, ec);
            
            RpfExtractor::ExtractionResult result = RpfExtractor::Extract(
                rpfPath,
                outputDir,
                aesKey
            );
            
            // RPF extraction failure is non-fatal for now
            // Game may have pre-extracted files
        }
    }
    
    // Scan for shaders - first check if already extracted, then try RPF extraction
    std::filesystem::path shaderCacheDir = targetDirectory / "shader_cache";
    std::filesystem::path extractedDir = shaderCacheDir / "extracted";
    
    // Try to find/extract shaders from RPF archives
    {
        std::vector<uint8_t> aesKey;
        std::filesystem::path aesKeyPath = PlatformPaths::GetAesKeyPath();
        if (std::filesystem::exists(aesKeyPath))
        {
            RpfExtractor::LoadAesKey(aesKeyPath, aesKey);
        }
        
        // Scan game directory for shaders (checks extracted files first, then RPFs)
        auto shaderScanResult = RpfExtractor::ScanAndExtractShaders(
            gameDir,
            extractedDir,
            aesKey,
            nullptr  // Progress callback handled separately
        );
        
        // Log shader discovery results (non-fatal if none found)
        if (!shaderScanResult.fxcFiles.empty())
        {
            // Shaders found - proceed with conversion
        }
    }
    
    // Convert shaders to platform-native format
    {
        ShaderConversionJournal shaderJournal;
        ShaderPlatform platform = ShaderConverter::detectPlatform();
        
        // Only convert if cache doesn't exist or is outdated
        if (!ShaderConverter::isCacheValid(shaderCacheDir, gameDir))
        {
            ShaderConverter::convertShaders(gameDir, shaderCacheDir, platform, shaderJournal, progressCallback);
            
            // Shader conversion failure is non-fatal - game can still run with runtime compilation
            if (shaderJournal.lastResult != ShaderConversionJournal::Result::Success &&
                shaderJournal.lastResult != ShaderConversionJournal::Result::NoShadersFound)
            {
                // Log warning but continue
            }
        }
    }
    
    // Clean up temp files
    PlatformPaths::CleanupTemp();
    
    for (uint32_t i = 0; i < 2; i++)
    {
        if (!progressCallback())
        {
            journal.lastResult = Journal::Result::Cancelled;
            journal.lastErrorMessage = "Installation was cancelled.";
            return false;
        }

        if (i == 0)
        {
            // Wait the specified amount of time to allow the consumer of the callbacks to animate, halt or cancel the installation for a while after it's finished.
            std::this_thread::sleep_for(endWaitTime);
        }
    }

    return true;
}

void Installer::rollback(Journal &journal)
{
    std::error_code ec;
    for (const auto &path : journal.createdFiles)
    {
        std::filesystem::remove(path, ec);
    }

    for (auto it = journal.createdDirectories.rbegin(); it != journal.createdDirectories.rend(); it++)
    {
        std::filesystem::remove(*it, ec);
    }
}

bool Installer::parseGame(const std::filesystem::path &sourcePath)
{
    std::unique_ptr<VirtualFileSystem> sourceVfs = createFileSystemFromPath(sourcePath);
    if (sourceVfs == nullptr)
    {
        return false;
    }

    return sourceVfs->exists(GameExecutableFile);
}

DLC Installer::parseDLC(const std::filesystem::path &sourcePath)
{
    Journal journal;
    std::unique_ptr<VirtualFileSystem> sourceVfs = createFileSystemFromPath(sourcePath);
    if (sourceVfs == nullptr)
    {
        return DLC::Unknown;
    }

    return detectDLC(sourcePath, *sourceVfs, journal);
}

bool Installer::parseUpdate(const std::filesystem::path &sourcePath)
{
    // GTA IV title updates contain a full XEX replacement (not .xexp patch)
    std::unique_ptr<VirtualFileSystem> sourceVfs = createFileSystemFromPath(sourcePath);
    if (sourceVfs == nullptr)
    {
        return false;
    }

    // Title updates contain default.xex as full replacement
    return sourceVfs->exists(GameExecutableFile);
}

TitleUpdate Installer::detectUpdateVersion(const std::filesystem::path &sourcePath)
{
    // Detect title update version from file name or content
    std::string filename = sourcePath.filename().string();
    std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    
    if (filename.find("v8") != std::string::npos)
        return TitleUpdate::V8;
    if (filename.find("v7") != std::string::npos)
        return TitleUpdate::V7;
    if (filename.find("v6") != std::string::npos)
        return TitleUpdate::V6;
    if (filename.find("v5") != std::string::npos)
        return TitleUpdate::V5;
    if (filename.find("v4") != std::string::npos)
        return TitleUpdate::V4;
    
    return TitleUpdate::Unknown;
}
