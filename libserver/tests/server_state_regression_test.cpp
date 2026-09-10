// Exercise the actual Service and persistence implementations. Test-only friend
// access sets up durable failure fixtures; public Dispatch validates responses.
#define main libserver_application_main
#include "../src/main.cpp"
#undef main

namespace {
std::size_t checks = 0;
void Check(bool value, const char* message) {
  ++checks;
  if (!value) throw std::runtime_error(message);
}
struct TemporaryDirectory {
  std::filesystem::path path = std::filesystem::temp_directory_path() /
                               RandomToken("libserver-state-regression-", 12);
  ~TemporaryDirectory() { std::error_code error; std::filesystem::remove_all(path, error); }
};

struct ServiceRegression {
  static void Recovery() {
    TemporaryDirectory directory;
    bool inject = false;
    libserver::PersistentState::FaultPoint point =
        libserver::PersistentState::FaultPoint::kBeforeTempCreate;
    libserver::PersistentState::Options options{.data_directory = directory.path};
    options.fault_injector = [&](auto current) { return inject && current == point; };
    libserver::PersistentState storage(options);
    Check(storage.Open(), "open fault store");
    Service service(&storage);
    Check(service.Initialize(), "initialize fault service");
    const Identity identity{"fault-regression", "0x000000000000abcd", "0x000000000000cdab", "Fault"};
    service.access_tokens_["regression-token"] = {identity, Clock::now() + kAccessTokenLifetime};
    Check(service.GrantEntitlement(identity.xuid, "TLAD"), "initial durable write");
    const json committed = storage.payload();
    const Request request{.method = "POST", .path = "/api/v2/sessions/0x0000000000001111/join",
                          .headers = {{"idempotency-key", "transient-retry"}},
                          .body = "{\"expected_revision\":1}"};
    std::size_t mutation_calls = 0;
    const auto mutation = [&](bool) -> Response {
      ++mutation_calls;
      auto staged = service.CaptureDurableLocked();
      staged.entitlements[identity.xuid].insert("TBOGT");
      return service.SaveAndCommitLocked(std::move(staged)) ? Response{.status = 204}
                                                          : Service::StorageFailure();
    };
    inject = true;
    Check(service.RunSessionMutation(request, identity, mutation).status == 503, "reject failed mutation");
    Check(storage.payload() == committed, "pre-commit failure preserved durable bytes");
    Check(storage.readable() && storage.health() == libserver::PersistentState::Health::kDegraded,
          "recoverable failure retains readable state");
    Check(service.Dispatch({.method = "GET", .path = "/health/ready"}).status == 200,
          "readiness remains available in degraded state");
    const auto entitlements = service.Dispatch({.method = "GET", .path = "/api/v2/entitlements",
        .headers = {{"authorization", "Bearer regression-token"}}});
    Check(entitlements.status == 200 && entitlements.body["packages"] == json::array({"TLAD"}),
          "reads return last committed data after rejected write");
    inject = false;
    Check(service.RunSessionMutation(request, identity, mutation).status == 204,
          "same idempotency key retries after recovery");
    Check(service.RunSessionMutation(request, identity, mutation).status == 204 && mutation_calls == 2,
          "successful retry then becomes idempotent");
    Check(storage.health() == libserver::PersistentState::Health::kReady, "successful save restores storage health");
    point = libserver::PersistentState::FaultPoint::kBeforeDirectorySync;
    inject = true;
    {
      std::lock_guard lock(service.mutex_);
      auto staged = service.CaptureDurableLocked();
      staged.player_names[identity.xuid] = "after-rename";
      Check(!service.SaveAndCommitLocked(std::move(staged)), "post-rename fault is reported");
    }
    Check(!storage.readable(), "ambiguous durability is not readable");
    Check(service.Dispatch({.method = "GET", .path = "/api/v2/entitlements",
        .headers = {{"authorization", "Bearer regression-token"}}}).status == 503,
        "ambiguous durability fails closed");
  }

  static void LegacyAndLimits() {
    DurableData original;
    const std::string xuid = "0x000000000000abcd";
    std::unordered_set<std::uint32_t> views;
    for (const auto& field : libserver::Gta4ExtractedStatFields()) {
      if (views.size() >= 20) break;
      if (!views.insert(field.view_id).second) continue;
      json column;
      switch (field.wire_type) {
        case libserver::StatWireType::kInt32: column = {{"type", "i32"}, {"value", 1}}; break;
        case libserver::StatWireType::kInt64: column = {{"type", "i64"}, {"value", 1}}; break;
        case libserver::StatWireType::kUnicode: column = {{"type", "unicode"}, {"value", "untouched"}}; break;
      }
      original.stats[xuid][Hex32Lower(field.view_id)][Hex32Lower(field.property_id)] = column;
    }
    Check(views.size() == 20, "fixture spans more than per-request view limit");
    json wire = SerializeDurable(original);
    wire["stats"]["0x000000000000ABCD"] = wire["stats"][xuid];
    wire["stats"].erase(xuid);
    DurableData decoded;
    std::string error;
    Check(DeserializeDurable(wire, decoded, error), "collision-free legacy snapshot normalizes");
    Check(decoded.stats.at(xuid).size() == views.size(), "all accumulated title views survive reload");
    wire["stats"][xuid] = wire["stats"]["0x000000000000ABCD"];
    Check(!DeserializeDurable(wire, decoded, error), "duplicate numeric legacy owners rejected");
    Check(error.find("collision") != std::string::npos, "collision reports a specific cause");
    json text = {{"xuid", "0x000000000000ABCD"}, {"player_name", "0x000000000000ABCD"},
                 {"blob", "0x000000000000ABCD"}, {"nonce", "0x000000000000ABCD"}};
    Check(libserver::NormalizeIdentities(text, error), "normalize structured identity fields");
    Check(text["xuid"] == xuid && text["nonce"] == xuid, "numeric fields normalized");
    Check(text["player_name"] == "0x000000000000ABCD" && text["blob"] == "0x000000000000ABCD",
          "free text and opaque bytes are untouched");
  }

  static void RankedTargetCertification() {
    Service service;
    Check(service.Initialize(), "initialize rank target fixture");
    const Identity host{"host", "0x000000000000a111", "0x000000000000b111", "Host"};
    const Identity target{"target", "0x000000000000a222", host.machine_id, "Target"};
    const std::string id = "0x000000000000c111";
    json room = {{"session_id", id}, {"host_xuid", host.xuid}, {"ranked", true},
      {"flags", 62}, {"lifecycle_state", 2}, {"state", "in_game"}, {"revision", 1},
      {"members", json::array({{{"xuid", host.xuid}}, {{"xuid", target.xuid}}})}};
    service.sessions_[id] = room;
    service.session_leases_[id] = Clock::now() + kSessionLease;
    ArbitrationState registered;
    registered.expected_machine_ids.insert(host.machine_id);
    registered.registered_machine_ids.insert(host.machine_id);
    registered.authorized_xuid_machines = {{host.xuid, host.machine_id}, {target.xuid, host.machine_id}};
    registered.registered_xuids.insert(host.xuid);
    service.arbitration_snapshots_[id] = registered;
    service.ranked_started_.insert(id);
    service.stat_next_sequences_[StatSequenceOwnerKey(host.xuid, id)] = 2;
    const Request write{.method = "POST", .path = "/api/v2/stats/writes",
      .body = SingleStatWriteRequest(id, "1", target.xuid, "0x0000006d",
                                    {{"0x2000000d", {{"type", "i64"}, {"value", 5000}}}}).dump()};
    Check(service.WriteStats(write, host).status == 409, "host cannot certify a foreign XUID by copying its machine");
    Check(!service.progression_.contains(target.xuid), "uncertified target cash remains untouched");
    service.arbitration_snapshots_[id].registered_xuids.insert(target.xuid);
    Check(service.WriteStats(write, host).status == 204, "certified target accepts authoritative result");
    Check(service.progression_.at(target.xuid).cash == 5000, "certified target result committed");
    Check(service.WriteStats(write, host).status == 204 && service.progression_.at(target.xuid).cash == 5000,
          "certified result remains idempotent");
  }
};
}  // namespace

int main() {
  try {
    ServiceRegression::Recovery();
    ServiceRegression::LegacyAndLimits();
    ServiceRegression::RankedTargetCertification();
    std::cout << "PASS server storage, legacy identities, retry receipts and ranked target certification: "
              << checks << " checks\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "FAIL after " << checks << " checks: " << error.what() << '\n';
    return 1;
  }
}
