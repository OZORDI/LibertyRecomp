// Included inside Service. All state access uses the service mutex.
  bool QosProbeCurrentLocked(const libserver::QosProbeAttempt& probe) const {
    const auto now = Clock::now();
    const auto session = sessions_.find(probe.session_id);
    const auto listener = qos_listeners_.find(probe.session_id);
    if (!probe.eligible || session == sessions_.end() ||
        !SessionLiveLocked(probe.session_id, now) || listener == qos_listeners_.end() ||
        !listener->second.enabled || listener->second.generation != probe.listener_generation ||
        listener->second.host_xuid != probe.host_xuid ||
        listener->second.exchange_key != probe.exchange_key ||
        session->second.value("host_xuid", "") != probe.host_xuid ||
        session->second.value("host_epoch", std::int64_t{0}) != probe.host_epoch ||
        session->second.value("exchange_key", "") != probe.exchange_key) return false;
    return std::ranges::any_of(relay_routes_, [&](const auto& entry) {
      return entry.second.session_id == probe.session_id &&
             entry.second.xuid == probe.host_xuid && RelayRouteLiveLocked(entry.second, now);
    });
  }

  Response QosLookup(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object() || !body->contains("targets") ||
        !body->at("targets").is_array() || body->at("targets").empty() ||
        body->at("targets").size() > kMaximumQosTargets) {
      return Error(400, "invalid_qos_lookup", "QoS target list is malformed");
    }
    std::vector<libserver::QosProbeAttempt> proposed;
    for (const auto& target : body->at("targets")) {
      if (!target.is_object()) return Error(400, "invalid_qos_target", "invalid QoS target");
      libserver::QosProbeAttempt probe;
      probe.session_id = target.value("session_id", "");
      probe.exchange_key = Lower(target.value("exchange_key", ""));
      probe.challenge = target.value("challenge", "");
      if (!IsHex(probe.session_id, 16) || !IsHex(probe.exchange_key, 32) ||
          !IsHex(probe.challenge, kQosChallengeHexDigits) ||
          probe.challenge.size() != kQosChallengeHexCharacters) {
        return Error(400, "invalid_qos_target", "invalid QoS target identity or challenge");
      }
      proposed.push_back(std::move(probe));
    }
    const auto digest = Sha256Hex(identity.xuid + ':' + body->dump());
    if (!digest) return Error(500, "digest_failure", "QoS request digest failed");
    std::lock_guard lock(mutex_);
    const auto now = Clock::now();
    std::erase_if(qos_probe_batches_, [now](const auto& entry) {
      return entry.second.expires <= now;
    });
    auto batch = qos_probe_batches_.find(*digest);
    if (batch == qos_probe_batches_.end()) {
      const auto owned = std::ranges::count_if(qos_probe_batches_, [&](const auto& entry) {
        return entry.second.owner_xuid == identity.xuid && !entry.second.completed &&
               Clock::now() < entry.second.deadline;
      });
      if (qos_probe_batches_.size() >= libserver::kMaximumQosProbeBatches ||
          static_cast<std::size_t>(owned) >= libserver::kMaximumQosBatchesPerOwner) {
        return Error(429, "qos_probe_limit", "too many outstanding QoS lookups");
      }
      for (auto& probe : proposed) {
        const auto session = sessions_.find(probe.session_id);
        const auto listener = qos_listeners_.find(probe.session_id);
        if (session == sessions_.end() || listener == qos_listeners_.end() ||
            !CanViewSessionLocked(session->second, identity)) continue;
        probe.host_xuid = listener->second.host_xuid;
        probe.listener_generation = listener->second.generation;
        probe.host_epoch = session->second.value("host_epoch", std::int64_t{0});
        probe.eligible = true;
        if (!QosProbeCurrentLocked(probe)) {
          probe.eligible = false;
          continue;
        }
        probe.id = RandomToken("qos_", 16);
        if (probe.id.empty()) return Error(500, "entropy_failure", "QoS token generation failed");
      }
      libserver::QosProbeBatch state;
      state.owner_xuid = identity.xuid;
      state.started = now;
      state.deadline = now + libserver::kQosProbeDeadline;
      state.expires = state.deadline + libserver::kQosProbeReceiptLifetime;
      state.attempts = std::move(proposed);
      batch = qos_probe_batches_.emplace(*digest, std::move(state)).first;
    }
    bool pending = false;
    json results = json::array();
    for (auto& probe : batch->second.attempts) {
      const bool current = QosProbeCurrentLocked(probe);
      if (probe.eligible && current && !probe.acknowledged && now < batch->second.deadline) {
        pending = true;
      }
      json result = {{"reachable", false}, {"measurement", "relay-host-ack-v1"},
                     {"probes_xmit", probe.eligible ? 1 : 0}, {"probes_recv", 0}};
      if (probe.acknowledged && current) {
        result["reachable"] = true;
        result["challenge"] = probe.challenge;
        result["probes_recv"] = 1;
        result["relay_host_microseconds"] = probe.round_trip_microseconds;
        if (probe.title_data) result["title_data"] = *probe.title_data;
      }
      results.push_back(std::move(result));
    }
    if (pending) return {.status = 202, .body = {{"pending", true}}};
    batch->second.completed = true;
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        now - batch->second.started).count();
    return {.body = {{"results", std::move(results)},
                     {"server_elapsed_microseconds", std::max<std::int64_t>(elapsed, 0)},
                     {"measurement", "relay-host-ack-v1"}}};
  }

  Response QosProbes(const Request& request, const Identity& identity) {
    if (request.method != "GET") return Error(404, "not_found", "QoS probe operation not found");
    json probes = json::array();
    std::lock_guard lock(mutex_);
    const auto now = Clock::now();
    for (auto& [key, batch] : qos_probe_batches_) {
      (void)key;
      if (now >= batch.deadline) continue;
      for (auto& probe : batch.attempts) {
        if (probe.host_xuid != identity.xuid || probe.acknowledged ||
            !QosProbeCurrentLocked(probe)) continue;
        probe.delivered = true;
        probes.push_back({{"probe_id", probe.id}, {"session_id", probe.session_id},
                          {"exchange_key", probe.exchange_key}, {"challenge", probe.challenge}});
        if (probes.size() >= kMaximumQosTargets) return {.body = {{"probes", std::move(probes)}}};
      }
    }
    return {.body = {{"probes", std::move(probes)}}};
  }

  Response QosAcknowledge(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object()) return Error(400, "invalid_qos_ack", "invalid host acknowledgement");
    json entries;
    if (HasExactKeys(*body, {"acknowledgements"}) && body->at("acknowledgements").is_array()) {
      entries = body->at("acknowledgements");
    } else {
      entries = json::array({*body});
    }
    if (entries.empty() || entries.size() > kMaximumQosTargets) {
      return Error(400, "invalid_qos_ack", "host acknowledgement batch is outside its bound");
    }
    std::lock_guard lock(mutex_);
    const auto now = Clock::now();
    struct Commit { libserver::QosProbeAttempt* probe; Clock::time_point started; };
    std::vector<Commit> commits;
    std::unordered_set<std::string> unique;
    for (const auto& entry : entries) {
      if (!HasExactKeys(entry, {"probe_id", "challenge"}) ||
          !entry.at("probe_id").is_string() || !entry.at("challenge").is_string()) {
        return Error(400, "invalid_qos_ack", "invalid host acknowledgement");
      }
      const std::string id = entry.at("probe_id");
      const std::string challenge = entry.at("challenge");
      if (id.empty() || !unique.insert(id).second) {
        return Error(400, "invalid_qos_ack", "duplicate or empty host acknowledgement");
      }
      bool found = false;
      for (auto& [key, batch] : qos_probe_batches_) {
        (void)key;
        for (auto& probe : batch.attempts) {
          if (probe.id != id) continue;
          found = true;
          if (probe.host_xuid != identity.xuid || challenge != probe.challenge ||
              !probe.delivered || !QosProbeCurrentLocked(probe)) {
            return Error(403, "invalid_qos_ack", "acknowledgement does not belong to the live host");
          }
          if (!probe.acknowledged && now >= batch.deadline) {
            return Error(408, "probe_timeout", "host acknowledgement arrived too late");
          }
          if (!probe.acknowledged) commits.push_back({&probe, batch.started});
          break;
        }
        if (found) break;
      }
      if (!found) return Error(404, "probe_not_found", "QoS probe expired");
    }
    // Validate the complete batch before publishing any acknowledgement.
    for (const Commit& commit : commits) {
      auto& probe = *commit.probe;
      probe.acknowledged = true;
      const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - commit.started).count();
      probe.round_trip_microseconds = static_cast<std::uint64_t>(std::max<std::int64_t>(elapsed, 0));
      probe.title_data = qos_listeners_.at(probe.session_id).title_data;
    }
    return {.status = 204};
  }
