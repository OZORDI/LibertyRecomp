#pragma once

namespace os::version::detail
{
    struct OSVersion
    {
        uint32_t Major{};
        uint32_t Minor{};
        uint32_t Build{};
    };

    OSVersion GetOSVersion();
}
