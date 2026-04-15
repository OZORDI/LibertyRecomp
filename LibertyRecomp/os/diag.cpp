// Diagnostic gate implementation. One env lookup on first call.

#include <os/diag.h>

#include <atomic>
#include <cstdlib>
#include <cstring>
#include <strings.h>   // strcasecmp (POSIX)

namespace os::diag
{
    bool ShouldEmit() noexcept
    {
        static std::atomic<int> cached{-1};
        int v = cached.load(std::memory_order_relaxed);
        if (v != -1)
            return v != 0;

        const char* raw = std::getenv("LIBERTY_VERBOSE_DIAG");
        bool on = false;
        if (raw && *raw) {
            // Accept "1", "true", "yes", "on" (case-insensitive).
            if (!std::strcmp(raw, "1") ||
                !::strcasecmp(raw, "true") ||
                !::strcasecmp(raw, "yes") ||
                !::strcasecmp(raw, "on"))
            {
                on = true;
            }
        }
        cached.store(on ? 1 : 0, std::memory_order_relaxed);
        return on;
    }
}
