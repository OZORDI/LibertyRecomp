#pragma once

#include <filesystem>
#include <fstream>
#include <kernel/xdm.h>

// Magic number for FileHandle type validation (matches pattern used by NtFileHandle)
constexpr uint32_t kFileHandleMagic = 0x58464849; // 'XFHI' - XCreateFile Handle Instance

/**
 * FileHandle - Win32-style file handle used by XCreateFileA
 * 
 * This struct is exposed so that NtReadFile can fall back to reading
 * from FileHandle objects when NtFileHandle lookup fails.
 * This bridges the two file handle systems:
 *   1. Win32 API (XCreateFileA) -> FileHandle
 *   2. NT API (NtCreateFile) -> NtFileHandle
 */
struct FileHandle : KernelObject
{
    uint32_t magic = kFileHandleMagic;
    std::fstream stream;
    std::filesystem::path path;
};

struct FileSystem
{
    static std::filesystem::path ResolvePath(const std::string_view& path, bool checkForMods);
};
