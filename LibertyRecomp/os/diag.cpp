// Diagnostic gate implementation. The launch policy is command-line owned;
// environment variables and config files cannot enable it later.

#include <os/diag.h>

#include <rex/diagnostics/policy.h>

namespace os::diag
{
    bool ShouldEmit() noexcept
    {
        return rex::diagnostics::IsEnabled(
            rex::diagnostics::Category::kLogging);
    }
}
