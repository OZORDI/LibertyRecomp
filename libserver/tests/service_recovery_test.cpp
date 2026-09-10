// Exercise the actual Service/PersistentState boundary without a live endpoint.
#define main libserver_application_main
#include "../src/main.cpp"
#undef main

namespace {
void RequireRecovery(bool value, const char* message) {
  if (!value) throw std::runtime_error(message);
}
struct TemporaryState {
  std::filesystem::path path = std::filesystem::temp_directory_path() /
                               RandomToken("libserver-recovery-");
  ~TemporaryState() { std::error_code ignored; std::filesystem::remove_all(path, ignored); }
};
}

int main() {
  try {
    TemporaryState temporary;
    using State = libserver::PersistentState;
    bool fault = false;
    State::Options options;
    options.data_directory = temporary.path;
    options.schema_version = 1;
    options.fault_injector = [&](auto point) {
      return fault && point == State::FaultPoint::kBeforeTempCreate;
    };
    State state(options);
    RequireRecovery(state.Open(), "store did not open");
    Service service(&state);
    RequireRecovery(service.Initialize(), "service did not initialize");
    const auto committed = state.payload();
    fault = true;
    RequireRecovery(!service.GrantEntitlement("0x000000000000beef", "TBOGT"),
                    "injected write unexpectedly succeeded");
    RequireRecovery(state.payload() == committed, "failed write changed committed payload");
    RequireRecovery(state.health() == State::Health::kDegraded && state.readable(),
                    "recoverable failure disabled committed reads");
    RequireRecovery(service.Dispatch({.method="GET", .path="/health/ready"}).status == 200,
                    "ephemeral service became unavailable after a rejected write");
    RequireRecovery(service.Dispatch({.method="GET", .path="/api/v2/friends"}).status == 401,
                    "read did not reach normal authentication after rejected write");
    fault = false;
    RequireRecovery(service.GrantEntitlement("0x000000000000beef", "TBOGT"),
                    "retry after storage recovery failed");
    RequireRecovery(state.health() == State::Health::kReady && state.readable(),
                    "successful retry did not clear degraded health");
    RequireRecovery(state.payload() != committed, "successful retry did not commit");

    // Concurrent health checks and durable mutations share the service mutex.
    std::atomic<bool> ok{true};
    std::vector<std::thread> readers;
    for (std::size_t i = 0; i < 4; ++i) readers.emplace_back([&] {
      for (std::size_t j = 0; j < 100; ++j) {
        if (service.Dispatch({.method="GET", .path="/health/ready"}).status != 200) ok = false;
      }
    });
    for (std::size_t i = 0; i < 10; ++i) {
      RequireRecovery(service.GrantEntitlement("0x000000000000beef", "TLAD"),
                      "concurrent commit failed");
    }
    for (auto& reader : readers) reader.join();
    RequireRecovery(ok, "concurrent health request failed");
    std::cout << "service_recovery_test passed\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
