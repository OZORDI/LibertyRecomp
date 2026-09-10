#include "network/community_multiplayer.h"

#include <array>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

namespace {
using namespace rex::system::xam;
void Require(bool value, const std::string& message) {
  if (!value) throw std::runtime_error(message);
}
LiveIdentity Identity(uint64_t xuid, uint64_t machine, uint8_t seed, const char* name) {
  LiveIdentity result;
  result.xuid = xuid;
  result.machine_id = machine;
  result.install_secret.fill(seed);
  result.player_name = name;
  return result;
}
}

int main(int argc, char** argv) {
  try {
    Require(argc == 2 || argc == 3, "usage: community_backend_integration loopback_url [--write-failure]");
    const std::string url = argv[1];
    Require(url.starts_with("http://127.0.0.1:"), "test accepts loopback only");
    LiveConfig config;
    config.backend = LiveBackend::kCommunity;
    config.community_url = url;
    config.session_protocol_version = 2;
    auto first = Identity(0xAEFACE01, 0xAEFACE11, 91, "C++ Host");
    auto second = Identity(0xAEFACE02, 0xAEFACE12, 92, "C++ Peer");
    auto host = LibertyRecomp::Network::CreateCommunityMultiplayerBackend(config, first);
    auto peer = LibertyRecomp::Network::CreateCommunityMultiplayerBackend(config, second);
    Require(host.session_directory->ready(), "host enrollment: " + host.session_directory->last_error());
    Require(peer.session_directory->ready(), "peer enrollment: " + peer.session_directory->last_error());
    SessionRecord session;
    session.title_id = 0x545407F2;
    session.media_id = 1;
    session.title_version = 1;
    session.protocol_version = 2;
    session.session_id = 0xAEFACE100;
    session.nonce = 0xAEFACE101;
    session.exchange_key.fill(0xEF);
    session.flags = 0x2E;
    session.max_public_slots = 8;
    session.open_public_slots = 6;
    session.host_xuid = first.xuid;
    session.host_machine_id = first.machine_id;
    session.host_ipv4 = 0x0100007F;
    session.host_port = 37000;
    session.members = {{.xuid=first.xuid, .machine_id=first.machine_id, .online_port=37000},
                       {.xuid=second.xuid, .machine_id=second.machine_id, .online_port=37000}};
    Require(host.session_directory->Create(session), "session create: " + host.session_directory->last_error());
    auto committed = host.session_directory->Get(session.session_id);
    Require(bool(committed), "host cannot read its session");
    Require(bool(peer.session_directory->Get(session.session_id)), "peer cannot read session");
    StatRow report;
    report.xuid = first.xuid;
    report.columns = {{.id=0x1000800A, .id_kind=StatColumnIdKind::kProperty,
                       .type=StatValueType::kInt32, .value=int32_t{7}},
                      {.id=0x1000800B, .id_kind=StatColumnIdKind::kProperty,
                       .type=StatValueType::kInt32, .value=int32_t{2}}};
    const std::array<StatView,1> reports{{{.id=0xFFFF0000, .rows={report}}}};
    const bool written = host.stats_service->Write(session.session_id, first.xuid, reports);
    Require(written == (argc == 2), "initial write did not match the requested fault scenario");
    Require(host.stats_service->Flush(session.session_id),
            "actual client Flush did not settle the original pending write after recovery");
    Require(host.stats_service->Flush(session.session_id), "second flush did not remain successful");
    std::cout << "client-generated-report-and-flush=passed\n";
    uint32_t host_address = 0, peer_address = 0;
    for (const auto& member : committed->members) {
      if (member.xuid == first.xuid) host_address = member.virtual_ipv4;
      if (member.xuid == second.xuid) peer_address = member.virtual_ipv4;
    }
    Require(host_address && peer_address && host_address != peer_address, "invalid assigned routes");
    host.peer_transport->RegisterRoute(peer_address, *committed);
    peer.peer_transport->RegisterRoute(host_address, *committed);
    const std::array<uint8_t,4> packet{1,3,5,7};
    Require(host.peer_transport->Send(peer_address, 37000, 37000, packet), "client relay enqueue failed");
    std::optional<PeerDatagram> received;
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(4);
    for (;;) {
      received = peer.peer_transport->Receive(37000, 65507);
      if (received || std::chrono::steady_clock::now() >= deadline) break;
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    Require(received && received->payload == std::vector<uint8_t>(packet.begin(), packet.end()) &&
            received->source_ipv4 == host_address, "real client relay bytes did not arrive");
    std::cout << "client-relay=passed\n";
    std::array<uint8_t,kQosTitleDataSize> title{};
    title.fill(37);
    Require(host.qos_service->UpdateListener(session.session_id, session.exchange_key,
            {.enabled=true, .title_data=title}), "client listener update failed");
    const std::array<QosTarget,1> target{{{session.session_id,session.exchange_key}}};
    const auto measured = peer.qos_service->Lookup(target);
    Require(measured.size() == 1 && measured.front().reachable &&
            measured.front().probes_recv == 1 && measured.front().title_data == title,
            "real host-worker acknowledgement was not measured: " + peer.qos_service->last_error());
    std::cout << "client-host-qos=passed rtt_ms=" << measured.front().rtt_median_milliseconds << '\n';
    // Stop only the host probe worker. The directory and relay leases remain
    // alive, reproducing the exact old false-positive reachability scenario.
    host.qos_service.reset();
    const auto stalled = peer.qos_service->Lookup(target);
    Require(stalled.size() == 1 && !stalled.front().reachable,
            "a relay lease was mistaken for a live host acknowledgement");
    std::cout << "client-no-host-qos=passed\n";
    Require(host.session_directory->Delete(session.session_id), "session cleanup failed");
    std::cout << "community_backend_integration passed\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
