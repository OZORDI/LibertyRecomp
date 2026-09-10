#pragma once
#include <cstdint>
#include <string_view>

namespace gta4::quicksave {
enum class Denial {
  kNone,
  kDisabled,
  kUnavailable,
  kMultiplayer,
  kMission,
  kActivity,
  kVehicle,
  kBusy,
  kPhone,
  kWanted,
  kTransition,
  kSessionChanged
};
constexpr std::string_view ReasonKey(Denial reason) {
  switch (reason) {
    case Denial::kDisabled:
      return "LR_QOFF";
    case Denial::kMultiplayer:
      return "LR_QNET";
    case Denial::kMission:
      return "LR_QMISS";
    case Denial::kActivity:
      return "LR_QACT";
    case Denial::kVehicle:
      return "LR_QCAR";
    case Denial::kBusy:
      return "LR_QBUSY";
    case Denial::kPhone:
      return "LR_QCALL";
    case Denial::kWanted:
      return "LR_QWANT";
    case Denial::kTransition:
      return "LR_QWAIT";
    case Denial::kSessionChanged:
      return "LR_QWAIT";
    default:
      return "LR_QERR";
  }
}
struct Conditions {
  bool enabled = true, known_program = false, player_ready = false, signed_in = false;
  bool multiplayer = false, mission = false, activity = false, in_vehicle = false;
  bool busy = false, phone_call = false, wanted = false, transitioning = false;
};
constexpr Denial CanSave(const Conditions& c) {
  if (!c.enabled)
    return Denial::kDisabled;
  if (!c.known_program || !c.player_ready || !c.signed_in)
    return Denial::kUnavailable;
  if (c.multiplayer)
    return Denial::kMultiplayer;
  if (c.mission)
    return Denial::kMission;
  if (c.activity)
    return Denial::kActivity;
  if (c.busy)
    return Denial::kBusy;
  if (c.phone_call)
    return Denial::kPhone;
  if (c.in_vehicle)
    return Denial::kVehicle;
  if (c.wanted)
    return Denial::kWanted;
  if (c.transitioning)
    return Denial::kTransition;
  return Denial::kNone;
}
struct Session {
  uint64_t epoch = 0, xuid = 0;
  uint32_t episode = 0, player = 0, ped = 0;
  constexpr bool operator==(const Session&) const = default;
};
enum class Phase { kIdle, kClosingPhone, kWaitingForUi, kSaveUi };
enum class Decision { kNone, kOpenSave, kSaved, kCancelled, kAbandoned };
struct Observation {
  Session session;
  uint64_t milliseconds = 0;
  bool phone_closed = false, frontend_visible = false, frontend_requested = false;
  bool storage_busy = false, save_succeeded = false;
  Denial eligibility = Denial::kNone;
};
class Transaction {
 public:
  bool Begin(Session owner, uint64_t milliseconds) {
    if (phase_ != Phase::kIdle)
      return false;
    owner_ = owner;
    began_ = milliseconds;
    phase_ = Phase::kClosingPhone;
    return true;
  }
  void MenuRequested(uint64_t milliseconds) {
    phase_ = Phase::kWaitingForUi;
    began_ = milliseconds;
  }
  Decision Poll(const Observation& o) {
    if (phase_ == Phase::kIdle)
      return Decision::kNone;
    if (o.session != owner_) {
      Reset();
      return Decision::kAbandoned;
    }
    const uint64_t elapsed = o.milliseconds >= began_ ? o.milliseconds - began_ : 0;
    if (phase_ == Phase::kClosingPhone) {
      if (o.eligibility != Denial::kNone || elapsed > 8000) {
        Reset();
        return Decision::kAbandoned;
      }
      if (o.phone_closed) {
        MenuRequested(o.milliseconds);
        return Decision::kOpenSave;
      }
      return Decision::kNone;
    }
    if (o.storage_busy || o.frontend_visible) {
      phase_ = Phase::kSaveUi;
      return Decision::kNone;
    }
    // Never time out or interrupt a storage write. A failed UI handoff is bounded.
    if (o.frontend_requested) {
      if (phase_ == Phase::kWaitingForUi && elapsed > 8000) {
        Reset();
        return Decision::kAbandoned;
      }
      return Decision::kNone;
    }
    if (elapsed < 100)
      return Decision::kNone;
    Reset();
    return o.save_succeeded ? Decision::kSaved : Decision::kCancelled;
  }
  void Reset() {
    phase_ = Phase::kIdle;
    owner_ = {};
    began_ = 0;
  }
  Phase phase() const { return phase_; }
  const Session& owner() const { return owner_; }

 private:
  Session owner_{};
  uint64_t began_ = 0;
  Phase phase_ = Phase::kIdle;
};
}  // namespace gta4::quicksave
