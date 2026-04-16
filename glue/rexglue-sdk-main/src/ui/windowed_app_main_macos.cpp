/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2021 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <spdlog/common.h>

#include <rex/cvar.h>
#include <rex/filesystem.h>
#include <rex/logging.h>
#include <rex/ui/windowed_app.h>
#include <rex/ui/windowed_app_context_macos.h>

namespace {

extern "C" void SDL_SetMainReady(void);

bool ShouldSkipAppleInjectedArgument(const char* argument) {
  if (argument == nullptr) {
    return false;
  }
  return std::strncmp(argument, "-psn_", 5) == 0;
}

}  // namespace

extern "C" int main(int argc, char** argv) {
  // ReXGlue owns the macOS entry point, so make SDL usable before any deferred
  // input or audio subsystem initialization happens later in startup.
  SDL_SetMainReady();

  std::vector<char*> filtered_argv;
  filtered_argv.reserve(static_cast<size_t>(argc));
  for (int i = 0; i < argc; ++i) {
    if (!ShouldSkipAppleInjectedArgument(argv[i])) {
      filtered_argv.push_back(argv[i]);
    }
  }
  filtered_argv.push_back(nullptr);

  auto remaining =
      rex::cvar::Init(static_cast<int>(filtered_argv.size() - 1), filtered_argv.data());
  rex::cvar::ApplyEnvironment();

  int result;

  {
    rex::ui::MacWindowedAppContext app_context;

    std::unique_ptr<rex::ui::WindowedApp> app = rex::ui::GetWindowedAppCreator()(app_context);

    const auto& option_names = app->GetPositionalOptions();
    std::map<std::string, std::string> parsed;
    size_t count = std::min(remaining.size(), option_names.size());
    for (size_t i = 0; i < count; ++i) {
      parsed[option_names[i]] = remaining[i];
    }
    app->SetParsedArguments(std::move(parsed));

    std::filesystem::path exe_dir = rex::filesystem::GetExecutableFolder();
    std::filesystem::path log_path = exe_dir / (app->GetName() + ".log");

    try {
      rex::InitLogging(log_path.string().c_str());
    } catch (const spdlog::spdlog_ex& e) {
      std::fprintf(stderr, "Logging init failed for '%s': %s\n", log_path.string().c_str(),
                   e.what());
      rex::InitLogging(nullptr);
    }

    if (app->OnInitialize()) {
      app_context.RunMainCocoaLoop();
      result = EXIT_SUCCESS;
    } else {
      result = EXIT_FAILURE;
    }

    app->InvokeOnDestroy();
  }

  rex::ShutdownLogging();
  return result;
}
