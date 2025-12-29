#include "gta_file_system.h"
#include <kernel/vfs.h>
#include <kernel/memory.h>
#include <os/logger.h>
#include <algorithm>

namespace GTA
{
    uint32_t FileResolve(uint32_t context, const char* pathBuffer, 
                        uint32_t outputPtr, uint32_t validationToken)
    {
        static int s_count = 0; ++s_count;
        
        std::string guestPath(pathBuffer);
        
        // Check if this is an interesting file type for logging
        bool isInteresting = false;
        std::string pathLower = guestPath;
        std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(), ::tolower);
        if (pathLower.find(".wtd") != std::string::npos ||
            pathLower.find(".wdr") != std::string::npos ||
            pathLower.find(".wft") != std::string::npos ||
            pathLower.find(".wdd") != std::string::npos ||
            pathLower.find(".rpf") != std::string::npos ||
            pathLower.find("shader") != std::string::npos ||
            pathLower.find("texture") != std::string::npos ||
            pathLower.find("model") != std::string::npos) {
            isInteresting = true;
        }
        
        // Log first 200 calls or all interesting files
        if (s_count <= 200 || isInteresting) {
            LOGF_WARNING("[FileResolve] #{} path='{}'", s_count, pathBuffer);
        }
        
        // Resolve path via VFS
        auto resolved = VFS::Resolve(guestPath);
        
        if (!VFS::Exists(guestPath)) {
            if (s_count <= 200 || isInteresting) {
                LOGF_WARNING("[FileResolve] #{} -> NOT FOUND", s_count);
            }
            
            // Write 0 to output pointer
            if (outputPtr != 0) {
                *reinterpret_cast<uint32_t*>(g_memory.base + outputPtr) = ByteSwap(uint32_t(0));
            }
            
            return 1; // Failure
        }
        
        // Get file size
        uint64_t fileSize = VFS::GetFileSize(guestPath);
        
        if (s_count <= 200 || isInteresting) {
            LOGF_WARNING("[FileResolve] #{} -> FOUND: '{}' size={} bytes", 
                        s_count, resolved.string(), fileSize);
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
