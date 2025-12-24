#pragma once

#include <cstdint>
#include <string>

/**
 * GTA IV File System API Hooks
 * 
 * Following UnleashedRecomp's pattern, we hook at the file API boundary
 * rather than the low-level storage device vtable layer.
 */

namespace GTA
{
    /**
     * sub_8249BE88 - File Path Resolution
     * 
     * Original calls vtable[27] and expects validation token return.
     * We bypass the vtable entirely and resolve directly via VFS.
     * 
     * @param context Context pointer (r3)
     * @param pathBuffer Formatted path buffer (r4) 
     * @param outputPtr Output pointer for file size (r5)
     * @param validationToken Expected validation code (r6) - IGNORED
     * @return 0 on success, 1 on failure
     */
    uint32_t FileResolve(uint32_t context, const char* pathBuffer, 
                        uint32_t outputPtr, uint32_t validationToken);
}
