// ObjC helper for iOS sandbox path resolution.
// Keeps Objective-C out of the pure C++ platform_paths.cpp.
#include <Foundation/Foundation.h>
#include <string.h>
#include <stdlib.h>

static char s_documentsPath[1024] = {};
static char s_bundlePath[1024]    = {};

extern "C" const char* LIBERTY_IOS_GetDocumentsPath()
{
    if (s_documentsPath[0] == '\0')
    {
        NSArray<NSString*>* paths = NSSearchPathForDirectoriesInDomains(
            NSDocumentDirectory, NSUserDomainMask, YES);
        if ([paths count] > 0)
        {
            strncpy(s_documentsPath, [paths[0] UTF8String], sizeof(s_documentsPath) - 1);
            s_documentsPath[sizeof(s_documentsPath) - 1] = '\0';
        }
    }
    return s_documentsPath;
}

extern "C" const char* LIBERTY_IOS_GetBundlePath()
{
    if (s_bundlePath[0] == '\0')
    {
        NSString* bp = [[NSBundle mainBundle] bundlePath];
        if (bp)
        {
            strncpy(s_bundlePath, [bp UTF8String], sizeof(s_bundlePath) - 1);
            s_bundlePath[sizeof(s_bundlePath) - 1] = '\0';
        }
    }
    return s_bundlePath;
}
