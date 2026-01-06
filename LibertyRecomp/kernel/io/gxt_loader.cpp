#include "gxt_loader.h"
#include <os/logger.h>
#include <fstream>
#include <algorithm>
#include <cstring>

namespace GxtLoader
{
    static bool g_initialized = false;
    static std::unordered_map<uint32_t, std::wstring> g_customEntries;
    static Stats g_stats;

    // GXT2 file header
    #pragma pack(push, 1)
    struct GxtHeader
    {
        uint16_t version;       // GXT version (should be 4 for GXT2)
        uint16_t bits;          // Character width (16 for Unicode)
    };

    struct GxtTableHeader
    {
        char name[4];           // Table name (e.g., "TABL", "TKEY", "TDAT")
        uint32_t size;          // Size of table data
    };

    struct GxtKeyEntry
    {
        uint32_t offset;        // Offset into TDAT
        uint32_t hash;          // Jenkins hash of key
    };
    #pragma pack(pop)

    // =========================================================================
    // Jenkins Hash Implementation
    // =========================================================================

    uint32_t JenkinsHash(const char* str)
    {
        if (!str) return 0;
        
        uint32_t hash = 0;
        while (*str)
        {
            hash += static_cast<unsigned char>(std::tolower(*str));
            hash += (hash << 10);
            hash ^= (hash >> 6);
            str++;
        }
        
        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);
        
        return hash;
    }

    // =========================================================================
    // GXT Parsing
    // =========================================================================

    static bool ParseGxt2(const std::vector<uint8_t>& data, std::vector<GxtEntry>& entries)
    {
        if (data.size() < sizeof(GxtHeader))
            return false;

        const GxtHeader* header = reinterpret_cast<const GxtHeader*>(data.data());
        
        // Validate header
        if (header->version != 4 || header->bits != 16)
        {
            // Try parsing as GXT1 or other formats
            return false;
        }

        size_t offset = sizeof(GxtHeader);
        
        std::vector<GxtKeyEntry> keys;
        const wchar_t* textData = nullptr;
        size_t textDataSize = 0;

        // Parse tables
        while (offset + sizeof(GxtTableHeader) <= data.size())
        {
            const GxtTableHeader* table = reinterpret_cast<const GxtTableHeader*>(data.data() + offset);
            offset += sizeof(GxtTableHeader);

            if (offset + table->size > data.size())
                break;

            std::string tableName(table->name, 4);

            if (tableName == "TABL")
            {
                // Table of sub-tables - skip for now
            }
            else if (tableName == "TKEY")
            {
                // Key entries
                size_t keyCount = table->size / sizeof(GxtKeyEntry);
                keys.reserve(keyCount);
                
                const GxtKeyEntry* keyData = reinterpret_cast<const GxtKeyEntry*>(data.data() + offset);
                for (size_t i = 0; i < keyCount; i++)
                {
                    keys.push_back(keyData[i]);
                }
            }
            else if (tableName == "TDAT")
            {
                // Text data (Unicode strings)
                textData = reinterpret_cast<const wchar_t*>(data.data() + offset);
                textDataSize = table->size;
            }

            offset += table->size;
        }

        // Build entries
        if (!keys.empty() && textData)
        {
            for (const auto& key : keys)
            {
                if (key.offset < textDataSize)
                {
                    const wchar_t* str = reinterpret_cast<const wchar_t*>(
                        reinterpret_cast<const uint8_t*>(textData) + key.offset);
                    
                    GxtEntry entry;
                    entry.hash = key.hash;
                    
                    // Find null terminator
                    size_t maxLen = (textDataSize - key.offset) / sizeof(wchar_t);
                    size_t len = 0;
                    while (len < maxLen && str[len] != 0) len++;
                    
                    entry.text = std::wstring(str, len);
                    entries.push_back(entry);
                }
            }
        }

        return !entries.empty();
    }

    // =========================================================================
    // Public API
    // =========================================================================

    void Initialize()
    {
        if (g_initialized)
            return;

        g_initialized = true;
        g_customEntries.clear();
        g_stats = Stats{};

        LOGF_UTILITY("[GxtLoader] Initialized");
    }

    bool IsInitialized()
    {
        return g_initialized;
    }

    size_t LoadGxtFile(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file)
            return 0;

        size_t fileSize = static_cast<size_t>(file.tellg());
        file.seekg(0);

        std::vector<uint8_t> data(fileSize);
        if (!file.read(reinterpret_cast<char*>(data.data()), fileSize))
            return 0;

        std::vector<GxtEntry> entries;
        if (!ParseGxt2(data, entries))
        {
            LOGF_WARNING("[GxtLoader] Failed to parse GXT file: {}", path.string());
            return 0;
        }

        // Merge into custom entries
        for (const auto& entry : entries)
        {
            g_customEntries[entry.hash] = entry.text;
        }

        g_stats.filesLoaded++;
        g_stats.entriesLoaded += entries.size();

        LOGF_UTILITY("[GxtLoader] Loaded {} entries from: {}", 
            entries.size(), path.filename().string());

        return entries.size();
    }

    void ScanForCustomGxt(const std::filesystem::path& overlayRoot)
    {
        std::error_code ec;
        if (!std::filesystem::exists(overlayRoot, ec))
            return;

        // FusionFix GXT suffixes
        std::vector<std::string> suffixes = { "FF", "RR", "CV" };
        for (int i = 1; i <= 9; i++)
            suffixes.push_back(std::to_string(i));

        // Look for text files in common/text/
        std::filesystem::path textPath = overlayRoot / "common" / "text";
        if (!std::filesystem::exists(textPath, ec))
            return;

        for (const auto& entry : std::filesystem::directory_iterator(textPath, ec))
        {
            if (ec || !entry.is_regular_file())
                continue;

            std::string filename = entry.path().filename().string();
            std::string ext = entry.path().extension().string();
            
            // Convert to lowercase for comparison
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            
            if (ext != ".gxt")
                continue;

            // Check for FusionFix suffix pattern (e.g., american_FF.gxt)
            std::string stem = entry.path().stem().string();
            for (const auto& suffix : suffixes)
            {
                std::string pattern = "_" + suffix;
                if (stem.size() > pattern.size() && 
                    stem.substr(stem.size() - pattern.size()) == pattern)
                {
                    LoadGxtFile(entry.path());
                    break;
                }
            }
        }
    }

    std::optional<std::wstring> GetCustomText(uint32_t hash)
    {
        g_stats.lookups++;
        
        auto it = g_customEntries.find(hash);
        if (it != g_customEntries.end())
        {
            g_stats.hits++;
            return it->second;
        }
        return std::nullopt;
    }

    std::optional<std::wstring> GetCustomText(const std::string& key)
    {
        return GetCustomText(JenkinsHash(key.c_str()));
    }

    const std::unordered_map<uint32_t, std::wstring>& GetAllCustomEntries()
    {
        return g_customEntries;
    }

    void ClearCustomEntries()
    {
        g_customEntries.clear();
    }

    Stats GetStats()
    {
        return g_stats;
    }
}
