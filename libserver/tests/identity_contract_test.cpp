#include "identity_contract.h"

#include <iostream>
#include <stdexcept>

namespace {
void Require(bool value, const char* message) {
  if (!value) throw std::runtime_error(message);
}
}

int main() {
  using nlohmann::json;
  using libserver::CanonicalIdentity;
  using libserver::NormalizeIdentities;
  try {
    const std::string mixed = "0x0123456789ABCDEF";
    const std::string canonical = "0x0123456789abcdef";
    Require(CanonicalIdentity(mixed) == canonical, "numeric identity did not canonicalize");
    Require(CanonicalIdentity("device-AbCd") == "device-AbCd", "opaque ID changed");
    std::string error;
    json request = {{"xuid", mixed}, {"host_xuid", mixed}, {"machine_id", mixed},
                    {"recipients", json::array({mixed})}, {"text", mixed},
                    {"public_key", mixed}, {"device_id", mixed},
                    {"title_dictionary", {{mixed, {{"value", mixed}}}}}};
    Require(NormalizeIdentities(request, error), "request normalization failed");
    Require(request["xuid"] == canonical && request["host_xuid"] == canonical &&
            request["machine_id"] == canonical && request["recipients"][0] == canonical,
            "structured identities were not normalized");
    Require(request["text"] == mixed && request["public_key"] == mixed &&
            request["device_id"] == mixed && request["title_dictionary"].contains(mixed) &&
            request["title_dictionary"][mixed]["value"] == mixed,
            "normalization changed opaque or title-owned content");
    json durable = {{"devices", {{mixed, {{"xuid", mixed}}}}},
                    {"player_names", {{mixed, mixed}}},
                    {"relationships", {{mixed, {{mixed, "accepted"}}}}},
                    {"stat_next_sequences", {{mixed + ":" + mixed, 4}}}};
    Require(NormalizeIdentities(durable, error, {}, true), "durable normalization failed");
    Require(durable["devices"].contains(mixed), "opaque device-map key was changed");
    Require(durable["devices"][mixed]["xuid"] == canonical, "device owner not normalized");
    Require(durable["player_names"][canonical] == mixed, "player name treated as an ID");
    Require(durable["relationships"][canonical].contains(canonical),
            "nested relationship target not normalized");
    Require(durable["stat_next_sequences"].contains(canonical + ":" + canonical),
            "composite receipt owner not normalized");
    json collision = {{"progression", {{canonical, json::object()}, {mixed, json::object()}}}};
    Require(!NormalizeIdentities(collision, error, {}, true), "identity collision silently merged");
    const auto path = "/api/v2/sessions/" + mixed;
    Require(libserver::CanonicalIdentityPath(path) == "/api/v2/sessions/" + canonical,
            "path identity was not normalized");
    std::cout << "identity_contract_test passed\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
