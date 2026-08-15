#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <rex/filesystem/devices/host_path_device.h>
#include <rex/runtime.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xam/content_device.h>
#include <rex/system/xam/content_manager.h>

namespace {

class ContentTestDirectory {
 public:
  ContentTestDirectory()
      : path_(std::filesystem::temp_directory_path() / "rex_content_manager_marketplace_test") {
    std::error_code error;
    std::filesystem::remove_all(path_, error);
    std::filesystem::create_directories(path_ / "game");
    std::filesystem::create_directories(path_ / "user");
    std::filesystem::create_directories(path_ / "marketplace" / "TLAD");
    std::filesystem::create_directories(path_ / "marketplace" / "TBOGT");
    std::ofstream(path_ / "marketplace" / "TLAD" / "setup2.xml") << "<setup/>";
    std::ofstream(path_ / "marketplace" / "TLAD" / "content.dat") << "TLAD";
    std::ofstream(path_ / "marketplace" / "TBOGT" / "setup2.xml") << "<setup/>";
    std::ofstream(path_ / "marketplace" / "TBOGT" / "content.dat") << "TBOGT";
  }

  ~ContentTestDirectory() {
    std::error_code error;
    std::filesystem::remove_all(path_, error);
  }

  const std::filesystem::path& path() const { return path_; }

 private:
  std::filesystem::path path_;
};

}  // namespace

TEST_CASE("marketplace content is common and enumerated exactly once", "[system][content]") {
  ContentTestDirectory temp;
  rex::system::xam::ContentManager manager(nullptr, temp.path() / "user",
                                           temp.path() / "marketplace");

  const auto packages = manager.ListContentForUser(
      static_cast<uint32_t>(rex::system::xam::DummyDeviceId::HDD), 0x1122334455667788,
      rex::system::XContentType::kMarketplaceContent, 0x545407F2);

  REQUIRE(packages.size() == 2);
  CHECK(packages[0].file_name() == "TBOGT");
  CHECK(packages[1].file_name() == "TLAD");
  CHECK(packages[0].xuid == 0);
  CHECK(packages[1].xuid == 0);
}

TEST_CASE("aggregate content metadata preserves package identity when copied",
          "[system][content]") {
  rex::system::xam::XCONTENT_AGGREGATE_DATA source{};
  source.set_file_name("TLAD");
  source.xuid = 0;

  rex::system::xam::XCONTENT_AGGREGATE_DATA copied(source);
  CHECK(copied.file_name() == "TLAD");
  CHECK(copied.xuid == 0);

  rex::system::xam::XCONTENT_AGGREGATE_DATA moved(std::move(copied));
  CHECK(moved.file_name() == "TLAD");
  CHECK(moved.xuid == 0);
}

TEST_CASE("saved games use the dedicated loose portable root", "[system][content][save]") {
  ContentTestDirectory temp;
  const auto saved_game_root = temp.path() / "portable-saves";
  rex::Runtime runtime(temp.path() / "game", temp.path() / "user", {}, {}, {},
                       temp.path() / "marketplace", saved_game_root);
  rex::RuntimeConfig config;
  config.tool_mode = true;
  REQUIRE(runtime.Setup(std::move(config)) == rex::X_STATUS{});

  constexpr uint64_t kXuid = UINT64_C(0x1122334455667788);
  constexpr uint32_t kTitleId = UINT32_C(0x545407F2);
  rex::system::xam::XCONTENT_AGGREGATE_DATA save{};
  save.device_id = static_cast<uint32_t>(rex::system::xam::DummyDeviceId::HDD);
  save.content_type = rex::system::XContentType::kSavedGame;
  save.set_display_name(u"GTA IV Save");
  save.set_file_name("SGTA400");
  save.xuid = kXuid;
  save.title_id = kTitleId;

  auto* manager = runtime.kernel_state()->content_manager();
  REQUIRE(manager->CreateContent("save", kXuid, save) == rex::X_RESULT{});

  const auto package_path = manager->GetOpenPackagePath("save");
  CHECK(package_path.string().starts_with(saved_game_root.string()));
  CHECK_FALSE(package_path.string().starts_with((temp.path() / "user").string()));

  auto* save_root_entry = runtime.file_system()->ResolvePath("save:\\");
  REQUIRE(save_root_entry != nullptr);
  const auto* save_device =
      dynamic_cast<const rex::filesystem::HostPathDevice*>(save_root_entry->device());
  REQUIRE(save_device != nullptr);
  CHECK(save_device->trace_io());

  REQUIRE(manager->WriteContentHeaderFile(kXuid, save) == rex::X_RESULT{});
  CHECK(std::filesystem::exists(saved_game_root));
  CHECK(manager->CloseContent("save") == rex::X_RESULT{});
  runtime.Shutdown();
}

TEST_CASE("marketplace packages mount and expose setup metadata through extra",
          "[.integration][system][content]") {
  ContentTestDirectory temp;
  rex::Runtime runtime(temp.path() / "game", temp.path() / "user", {}, {}, {},
                       temp.path() / "marketplace");
  rex::RuntimeConfig config;
  config.tool_mode = true;
  REQUIRE(runtime.Setup(std::move(config)) == rex::X_STATUS{});

  auto* manager = runtime.kernel_state()->content_manager();
  const auto packages = manager->ListContentForUser(
      static_cast<uint32_t>(rex::system::xam::DummyDeviceId::HDD), 0x1122334455667788,
      rex::system::XContentType::kMarketplaceContent, 0x545407F2);
  REQUIRE(packages.size() == 2);

  for (const auto& package_name : {std::string("TLAD"), std::string("TBOGT")}) {
    const auto package = std::find_if(packages.begin(), packages.end(), [&](const auto& item) {
      return item.file_name() == package_name;
    });
    REQUIRE(package != packages.end());

    uint32_t license = 0;
    REQUIRE(manager->OpenContent("extra", 0, *package, license) == rex::X_RESULT{});
    CHECK(runtime.file_system()->ResolvePath("extra:\\setup2.xml") != nullptr);
    CHECK(manager->CloseContent("extra") == rex::X_RESULT{});
  }

  runtime.Shutdown();
}
