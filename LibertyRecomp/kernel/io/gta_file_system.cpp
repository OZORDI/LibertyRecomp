#include "gta_file_system.h"
#include <kernel/vfs.h>
#include <kernel/memory.h>
#include <os/logger.h>

namespace GTA
{
    uint32_t FileResolve(uint32_t context, const char* pathBuffer, 
                        uint32_t outputPtr, uint32_t validationToken)
    {
        static int s_count = 0; ++s_count;
        
        if (s_count <= 10) {
            LOGF_WARNING("[GTA::FileResolve] #{} path='{}' output=0x{:08X} token={}", 
                        s_count, pathBuffer, outputPtr, validationToken);
        }
        
        // Resolve path via VFS
        std::string guestPath(pathBuffer);
        
        if (!VFS::Exists(guestPath)) {
            if (s_count <= 10) {
                LOGF_WARNING("[GTA::FileResolve] #{} -> FILE NOT FOUND", s_count);
            }
            
            // Write 0 to output pointer
            if (outputPtr != 0) {
                *reinterpret_cast<uint32_t*>(g_memory.base + outputPtr) = ByteSwap(uint32_t(0));
            }
            
            return 1; // Failure
        }
        
        // Get file size
        uint64_t fileSize = VFS::GetFileSize(guestPath);
        
        if (s_count <= 10) {
            LOGF_WARNING("[GTA::FileResolve] #{} -> SUCCESS size={} bytes", 
                        s_count, fileSize);
        }
        
        // Write file size to output pointer (big-endian)
        if (outputPtr != 0) {
            *reinterpret_cast<uint32_t*>(g_memory.base + outputPtr) = 
                ByteSwap(static_cast<uint32_t>(fileSize));
        }
        
        // Return 0 for success (game checks r3 == 0)
        return 0;
    }
}
