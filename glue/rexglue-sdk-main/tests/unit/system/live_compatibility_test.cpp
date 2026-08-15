/**
 ******************************************************************************
 * @file        live_compatibility_test.cpp
 * @brief       Tests for the title-facing multiplayer session directory.
 ******************************************************************************
 */

#include <array>
#include <cstring>
#include <span>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <rex/net/socket.h>
#include <rex/platform.h>
#include <rex/system/xam/live_compatibility.h>
#include <rex/system/xsocket.h>

#if !REX_PLATFORM_WIN32
#include <cerrno>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace {

using rex::system::xam::InMemorySessionDirectory;
using rex::system::xam::IsValidSessionRecord;
using rex::system::xam::LanSessionDirectory;
using rex::system::xam::SessionContext;
using rex::system::xam::SessionLifecycleState;
using rex::system::xam::SessionMember;
using rex::system::xam::SessionProperty;
using rex::system::xam::SessionRecord;
using rex::system::xam::kMaximumSessionMembers;

SessionRecord MakeSession(uint64_t session_id) {
  SessionRecord session;
  session.title_id = 0x545407F2;
  session.media_id = 0x12345678;
  session.title_version = 1;
  session.protocol_version = 1;
  session.session_id = session_id;
  session.max_public_slots = 4;
  session.max_private_slots = 2;
  session.open_public_slots = 4;
  session.open_private_slots = 2;
  session.contexts.push_back({.id = 3, .value = 9});
  session.properties.push_back({.id = 7, .value = {1, 2, 3}});
  return session;
}

SessionMember MakeMember(uint64_t xuid, bool private_slot) {
  SessionMember member{};
  member.xuid = xuid;
  member.private_slot = private_slot;
  return member;
}

#if !REX_PLATFORM_WIN32
uint16_t ReserveUdpPort() {
  const int handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (handle < 0) {
    return 0;
  }

  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_port = 0;
  address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (bind(handle, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0) {
    close(handle);
    return 0;
  }

  socklen_t address_size = sizeof(address);
  if (getsockname(handle, reinterpret_cast<sockaddr*>(&address), &address_size) != 0) {
    close(handle);
    return 0;
  }
  close(handle);
  return ntohs(address.sin_port);
}
#endif

}  // namespace

TEST_CASE("session directory partitions and filters searches", "[live][session]") {
  InMemorySessionDirectory directory;
  REQUIRE(directory.Create(MakeSession(0xAE00000000000001ULL)));
  CHECK_FALSE(directory.Create(MakeSession(0xAE00000000000001ULL)));

  const std::array required_contexts = {SessionContext{.id = 3, .value = 9}};
  const std::array required_properties = {SessionProperty{.id = 7, .value = {1, 2, 3}}};
  auto matches =
      directory.Search(0x545407F2, 0x12345678, 1, 1, required_contexts, required_properties, 8);
  REQUIRE(matches.size() == 1);
  CHECK(matches.front().session_id == 0xAE00000000000001ULL);

  CHECK(directory.Search(0x545407F3, 0x12345678, 1, 1, required_contexts, required_properties, 8)
            .empty());
  CHECK(directory.Search(0x545407F2, 0x12345678, 1, 2, required_contexts, required_properties, 8)
            .empty());

  const std::array wrong_contexts = {SessionContext{.id = 3, .value = 10}};
  CHECK(directory.Search(0x545407F2, 0x12345678, 1, 1, wrong_contexts, required_properties, 8)
            .empty());
}

TEST_CASE("session member operations preserve slot accounting", "[live][session]") {
  InMemorySessionDirectory directory;
  const uint64_t session_id = 0xAE00000000000002ULL;
  REQUIRE(directory.Create(MakeSession(session_id)));

  REQUIRE(directory.Join(session_id, MakeMember(11, false)));
  REQUIRE(directory.Join(session_id, MakeMember(12, true)));
  REQUIRE(directory.Join(session_id, MakeMember(11, false)));

  auto joined = directory.Get(session_id);
  REQUIRE(joined);
  CHECK(joined->members.size() == 2);
  CHECK(joined->open_public_slots == 3);
  CHECK(joined->open_private_slots == 1);

  REQUIRE(directory.Leave(session_id, 11));
  REQUIRE(directory.Leave(session_id, 12));
  auto left = directory.Get(session_id);
  REQUIRE(left);
  CHECK(left->members.empty());
  CHECK(left->open_public_slots == left->max_public_slots);
  CHECK(left->open_private_slots == left->max_private_slots);
}

TEST_CASE("session directory accepts every extended multiplayer boundary",
          "[live][session][capacity]") {
  InMemorySessionDirectory directory;
  auto session = MakeSession(0xAE00000000000020ULL);
  session.max_public_slots = kMaximumSessionMembers;
  session.max_private_slots = 0;
  session.open_public_slots = kMaximumSessionMembers;
  session.open_private_slots = 0;
  REQUIRE(directory.Create(session));

  for (uint32_t index = 0; index < kMaximumSessionMembers; ++index) {
    REQUIRE(directory.Join(session.session_id,
                           MakeMember(0xE000000000001000ULL + index, false)));
    const auto record = directory.Get(session.session_id);
    REQUIRE(record);
    if (index == 16 || index == 32 || index == 63) {
      CHECK(record->members.size() == static_cast<size_t>(index + 1));
    }
  }

  const auto full = directory.Get(session.session_id);
  REQUIRE(full);
  CHECK(full->members.size() == kMaximumSessionMembers);
  CHECK(full->open_public_slots == 0);
  CHECK_FALSE(directory.Join(session.session_id,
                             MakeMember(0xE000000000002000ULL, false)));
}

TEST_CASE("session directory rejects a combined slot count above capacity",
          "[live][session][capacity]") {
  InMemorySessionDirectory directory;
  auto session = MakeSession(0xAE00000000000021ULL);
  session.max_public_slots = kMaximumSessionMembers;
  session.open_public_slots = kMaximumSessionMembers;
  session.max_private_slots = 1;
  session.open_private_slots = 1;
  CHECK_FALSE(directory.Create(session));
}

TEST_CASE("session record validator rejects malformed extended rosters",
          "[live][session][capacity]") {
  auto session = MakeSession(0xAE00000000000022ULL);
  session.max_public_slots = kMaximumSessionMembers;
  session.max_private_slots = 0;
  session.open_public_slots = 0;
  session.open_private_slots = 0;
  for (uint32_t index = 0; index < kMaximumSessionMembers; ++index) {
    session.members.push_back(MakeMember(0xE000000000003000ULL + index, false));
  }
  REQUIRE(IsValidSessionRecord(session));

  session.members.push_back(MakeMember(0xE000000000004000ULL, false));
  CHECK_FALSE(IsValidSessionRecord(session));
  session.members.pop_back();

  session.members.back().xuid = session.members.front().xuid;
  CHECK_FALSE(IsValidSessionRecord(session));
}

TEST_CASE("session lifecycle supports modify migrate and delete", "[live][session]") {
  InMemorySessionDirectory directory;
  const uint64_t old_id = 0xAE00000000000003ULL;
  const uint64_t new_id = 0xAE00000000000004ULL;
  auto session = MakeSession(old_id);
  REQUIRE(directory.Create(session));

  session.flags = 5;
  session.lifecycle_state = SessionLifecycleState::kInGame;
  session.open_public_slots = 3;
  session.members.push_back(MakeMember(33, false));
  REQUIRE(directory.Modify(session));
  REQUIRE(directory.Heartbeat(session));
  REQUIRE(directory.Get(old_id));
  CHECK(directory.Get(old_id)->flags == 5);
  CHECK(directory.Get(old_id)->lifecycle_state == SessionLifecycleState::kInGame);

  auto replacement = session;
  replacement.session_id = new_id;
  replacement.host_xuid = 44;
  REQUIRE(directory.Migrate(old_id, replacement));
  CHECK_FALSE(directory.Get(old_id));
  REQUIRE(directory.Get(new_id));
  CHECK(directory.Get(new_id)->host_xuid == 44);
  CHECK(directory.Get(new_id)->previous_session_id == old_id);

  REQUIRE(directory.Delete(new_id));
  CHECK_FALSE(directory.Get(new_id));
  CHECK_FALSE(directory.Delete(new_id));
}

#if !REX_PLATFORM_WIN32
TEST_CASE("LAN directory synchronizes authoritative session mutations", "[live][lan]") {
  const uint16_t discovery_port = ReserveUdpPort();
  REQUIRE(discovery_port != 0);

  LanSessionDirectory host(discovery_port);
  LanSessionDirectory client(discovery_port);
  REQUIRE(host.ready());
  REQUIRE(client.ready());

  const uint64_t old_id = 0xAE00000000000011ULL;
  const uint64_t new_id = 0xAE00000000000012ULL;
  auto session = MakeSession(old_id);
  REQUIRE(host.Create(session));

  auto matches = client.Search(session.title_id, session.media_id, session.title_version,
                               session.protocol_version, {}, {}, 1);
  REQUIRE(matches.size() == 1);
  CHECK(matches.front().session_id == old_id);

  const SessionMember member = MakeMember(0xE000000000000021ULL, false);
  REQUIRE(client.Join(old_id, member));
  REQUIRE(host.Get(old_id));
  CHECK(host.Get(old_id)->members == std::vector{member});

  REQUIRE(client.Leave(old_id, member.xuid));
  REQUIRE(host.Get(old_id));
  CHECK(host.Get(old_id)->members.empty());

  auto migrated = *client.Get(old_id);
  migrated.session_id = new_id;
  migrated.host_xuid = member.xuid;
  REQUIRE(client.Migrate(old_id, migrated));
  CHECK_FALSE(client.Get(old_id));
  REQUIRE(client.Get(new_id));

  auto migrated_matches = host.Search(session.title_id, session.media_id, session.title_version,
                                      session.protocol_version, {}, {}, 1);
  REQUIRE(migrated_matches.size() == 1);
  CHECK(migrated_matches.front().session_id == new_id);
  CHECK_FALSE(host.Get(old_id));

  REQUIRE(client.Delete(new_id));
  CHECK_FALSE(client.Get(new_id));
  CHECK(host.Search(session.title_id, session.media_id, session.title_version,
                    session.protocol_version, {}, {}, 1)
            .empty());

  auto extended = MakeSession(0xAE00000000000013ULL);
  extended.max_public_slots = kMaximumSessionMembers;
  extended.max_private_slots = 0;
  extended.open_public_slots = 0;
  extended.open_private_slots = 0;
  for (uint32_t index = 0; index < kMaximumSessionMembers; ++index) {
    extended.members.push_back(MakeMember(0xE000000000005000ULL + index, false));
  }
  REQUIRE(host.Create(extended));

  const auto extended_matches =
      client.Search(extended.title_id, extended.media_id, extended.title_version,
                    extended.protocol_version, {}, {}, 1);
  REQUIRE(extended_matches.size() == 1);
  CHECK(extended_matches.front().members.size() == kMaximumSessionMembers);
  CHECK(extended_matches.front().members == extended.members);
}

TEST_CASE("UDP wrapper preserves Xbox network byte order", "[live][socket]") {
  rex::system::XSocket receiver(nullptr);
  REQUIRE(XSUCCEEDED(receiver.Initialize(rex::system::XSocket::X_AF_INET,
                                         rex::system::XSocket::X_SOCK_DGRAM,
                                         rex::system::XSocket::X_IPPROTO_UDP)));

  rex::system::N_XSOCKADDR_IN bind_address{};
  bind_address.sin_family = AF_INET;
  bind_address.sin_port = 0;
  bind_address.sin_addr = INADDR_LOOPBACK;
  REQUIRE(XSUCCEEDED(receiver.Bind(&bind_address, sizeof(bind_address))));

  sockaddr_in receiver_address{};
  socklen_t receiver_address_size = sizeof(receiver_address);
  REQUIRE(getsockname(static_cast<int>(receiver.native_handle()),
                      reinterpret_cast<sockaddr*>(&receiver_address), &receiver_address_size) == 0);

  timeval timeout{.tv_sec = 1, .tv_usec = 0};
  REQUIRE(setsockopt(static_cast<int>(receiver.native_handle()), SOL_SOCKET, SO_RCVTIMEO, &timeout,
                     sizeof(timeout)) == 0);

  rex::system::XSocket sender(nullptr);
  REQUIRE(XSUCCEEDED(sender.Initialize(rex::system::XSocket::X_AF_INET,
                                       rex::system::XSocket::X_SOCK_DGRAM,
                                       rex::system::XSocket::X_IPPROTO_UDP)));

  rex::system::N_XSOCKADDR_IN destination{};
  destination.sin_family = AF_INET;
  destination.sin_port = ntohs(receiver_address.sin_port);
  destination.sin_addr = ntohl(receiver_address.sin_addr.s_addr);
  std::array<uint8_t, 4> payload = {0x47, 0x54, 0x41, 0x34};
  REQUIRE(sender.SendTo(payload.data(), static_cast<uint32_t>(payload.size()), 0, &destination,
                        sizeof(destination)) == static_cast<int>(payload.size()));

  sockaddr_in sender_address{};
  socklen_t sender_address_size = sizeof(sender_address);
  REQUIRE(getsockname(static_cast<int>(sender.native_handle()),
                      reinterpret_cast<sockaddr*>(&sender_address), &sender_address_size) == 0);

  std::array<uint8_t, 4> received{};
  rex::system::N_XSOCKADDR_IN source{};
  uint32_t source_size = sizeof(source);
  REQUIRE(receiver.RecvFrom(received.data(), static_cast<uint32_t>(received.size()), 0, &source,
                            &source_size) == static_cast<int>(received.size()));
  CHECK(received == payload);
  CHECK(static_cast<uint32_t>(source.sin_addr) == ntohl(receiver_address.sin_addr.s_addr));
  CHECK(static_cast<uint16_t>(source.sin_port) == ntohs(sender_address.sin_port));
}

TEST_CASE("POSIX socket failures preserve WinSock polling semantics", "[live][socket]") {
  errno = EAGAIN;
  CHECK(rex::net::socket_last_error() == 10035);
  errno = ECONNREFUSED;
  CHECK(rex::net::socket_last_error() == 10061);

  rex::system::XSocket receiver(nullptr);
  REQUIRE(XSUCCEEDED(receiver.Initialize(rex::system::XSocket::X_AF_INET,
                                         rex::system::XSocket::X_SOCK_DGRAM,
                                         rex::system::XSocket::X_IPPROTO_UDP)));
  unsigned long nonblocking = 1;
  REQUIRE(ioctl(static_cast<int>(receiver.native_handle()), FIONBIO, &nonblocking) == 0);

  std::array<uint8_t, 4> received{};
  rex::system::N_XSOCKADDR_IN source{};
  source.sin_family = AF_INET;
  source.sin_port = 77;
  source.sin_addr = 0x10203040;
  const auto original_source = source;
  uint32_t source_size = sizeof(source);
  REQUIRE(receiver.RecvFrom(received.data(), static_cast<uint32_t>(received.size()), 0, &source,
                            &source_size) == -1);
  CHECK(rex::net::socket_last_error() == 10035);
  CHECK(source.sin_family == original_source.sin_family);
  CHECK(source.sin_port == original_source.sin_port);
  CHECK(source.sin_addr == original_source.sin_addr);
  CHECK(source_size == sizeof(source));

  REQUIRE(XSUCCEEDED(receiver.Close()));
  CHECK(XSUCCEEDED(receiver.Close()));
}

TEST_CASE("peer route addresses use network byte order", "[live][socket]") {
  in_addr address{};
  REQUIRE(inet_pton(AF_INET, "10.67.4.9", &address) == 1);

  rex::system::N_XSOCKADDR_IN guest_destination{};
  guest_destination.sin_addr = ntohl(address.s_addr);
  const uint32_t route_key = htonl(static_cast<uint32_t>(guest_destination.sin_addr));
  CHECK(route_key == address.s_addr);

  rex::system::N_XSOCKADDR_IN guest_source{};
  guest_source.sin_addr = ntohl(route_key);
  CHECK(static_cast<uint32_t>(guest_source.sin_addr) == ntohl(address.s_addr));
}
#endif
