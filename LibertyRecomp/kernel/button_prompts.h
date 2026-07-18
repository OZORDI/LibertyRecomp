#pragma once

#include <filesystem>
#include <string>

namespace ButtonPrompts
{
    /**
     * @brief Initialize the button prompts system.
     * Creates the cache directory if needed.
     */
    void Initialize();
    
    /**
     * @brief Get the path to the button prompts XTD file for the current controller type.
     * 
     * This function:
     * 1. Checks the user config for a manual override
     * 2. Falls back to auto-detection based on connected controller
     * 3. Writes the appropriate embedded XTD to a temp file (cached)
     * 4. Returns the path to the cached XTD file
     * 
     * @return Path to the buttons_360.xtd file, or empty path if not available
     */
    std::filesystem::path GetCurrentButtonPromptsPath();
    
    /**
     * @brief Force a refresh of the cached XTD file.
     * Call this when the controller type changes at runtime.
     */
    void RefreshCache();
    
    /**
     * @brief Get the platform name for the current controller configuration.
     * @return Platform name string (e.g., "xbox360", "ps4", "switch")
     */
    std::string GetCurrentPlatformName();
}
