#include "gta_file_system.h"
#include <kernel/vfs.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <algorithm>

namespace GTA
{
    // Comprehensive file resolve logging - tracks ALL game file requests
    static int s_fileResolveCount = 0;
    
    uint32_t FileResolve(uint32_t context, const char* pathBuffer, 
                        uint32_t outputPtr, uint32_t validationToken)
    {
        ++s_fileResolveCount;
        
        std::string guestPath(pathBuffer);
        
        // COMPREHENSIVE LOGGING: Log ALL file resolve requests
        printf("[GTA::FileResolve] #%d path='%s' output=0x%08X token=%u\n", 
               s_fileResolveCount, pathBuffer, outputPtr, validationToken);
        fflush(stdout);
        
        // Resolve path via VFS
        auto resolved = VFS::Resolve(guestPath);
        
        if (!VFS::Exists(guestPath)) {
            printf("[GTA::FileResolve] #%d -> NOT FOUND\n", s_fileResolveCount);
            fflush(stdout);
            
            // Write 0 to output pointer
            if (outputPtr != 0) {
                *reinterpret_cast<uint32_t*>(g_memory.base + outputPtr) = ByteSwap(uint32_t(0));
            }
            
            return 1; // Failure
        }
        
        // Get file size
        uint64_t fileSize = VFS::GetFileSize(guestPath);
        
        printf("[GTA::FileResolve] #%d -> SUCCESS: '%s' size=%llu bytes\n", 
               s_fileResolveCount, resolved.string().c_str(), (unsigned long long)fileSize);
        fflush(stdout);
        
        // Write file size to output pointer (big-endian)
        if (outputPtr != 0) {
            *reinterpret_cast<uint32_t*>(g_memory.base + outputPtr) = 
                ByteSwap(static_cast<uint32_t>(fileSize));
        }
        
        // Return 0 for success (game checks r3 == 0)
        return 0;
    }
}
