#pragma once

#include <filesystem>

namespace rex {
class Runtime;
}

namespace gta4::button_prompts {

// Builds (or reuses) a controller-specific shadow xbox360.rpf and redirects
// only that archive path in the guest VFS. The installed archive is read-only.
bool PrepareAndMount(rex::Runtime& runtime, const std::filesystem::path& game_data_root,
                     const std::filesystem::path& cache_root);

}  // namespace gta4::button_prompts
