#pragma once

#include <memory>

#include <rex/system/xam/live_compatibility.h>

namespace LibertyRecomp::Network {

rex::system::xam::LiveBackendServices CreateCommunityMultiplayerBackend(
    const rex::system::xam::LiveConfig &config,
    const rex::system::xam::LiveIdentity &identity);

} // namespace LibertyRecomp::Network
