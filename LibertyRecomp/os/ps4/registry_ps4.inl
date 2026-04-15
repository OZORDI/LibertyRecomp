#include <os/registry.h>

// PS4: No Windows-style registry. Return false for all operations.
// Game state is persisted via PS4 SaveData API instead.

inline bool os::registry::Init()
{
    return false;
}

template<typename T>
bool os::registry::ReadValue(const std::string_view& name, T& data)
{
    return false;
}

template<typename T>
bool os::registry::WriteValue(const std::string_view& name, const T& data)
{
    return false;
}
