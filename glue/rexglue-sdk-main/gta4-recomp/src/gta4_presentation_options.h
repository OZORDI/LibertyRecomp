#pragma once

namespace gta4::presentation {
// Called after native.toml is loaded, before guest execution starts.
void InitializeOptions();
bool SkipIntroAtLaunch() noexcept;
bool DisableTladFilmGrain() noexcept;
bool TraceEnabled() noexcept;
}  // namespace gta4::presentation
