#include "native_gpu_attribution.h"

#include <algorithm>
#include <limits>
#include <tuple>
#include <utility>

namespace rex::graphics::gta4_native::attribution {

namespace {

template <typename T>
T SaturatingAdd(T left, T right) {
  const T maximum = std::numeric_limits<T>::max();
  return right > maximum - left ? maximum : T(left + right);
}

int ClassificationPriority(NativePassClassification classification) {
  switch (classification) {
    case NativePassClassification::kKnown:
      return 0;
    case NativePassClassification::kOther:
      return 1;
    case NativePassClassification::kUnknown:
      return 2;
  }
  return 0;
}

NativePassClassification StrongerClassification(NativePassClassification left,
                                                NativePassClassification right) {
  return ClassificationPriority(left) >= ClassificationPriority(right) ? left : right;
}

bool SurfaceKeyLess(const NativePassSurfaceKey& left, const NativePassSurfaceKey& right) {
  return std::tie(left.handle, left.generation, left.format, left.sample_count) <
         std::tie(right.handle, right.generation, right.format, right.sample_count);
}

bool CandidateBetter(NativePassClassification left_classification, uint64_t left_nanoseconds,
                     uint32_t left_command, const NativePassKey& left_key, uint64_t left_sequence,
                     NativePassClassification right_classification, uint64_t right_nanoseconds,
                     uint32_t right_command, const NativePassKey& right_key,
                     uint64_t right_sequence) {
  if (left_nanoseconds != right_nanoseconds) {
    return left_nanoseconds > right_nanoseconds;
  }
  const int left_class = ClassificationPriority(left_classification);
  const int right_class = ClassificationPriority(right_classification);
  if (left_class != right_class) {
    return left_class > right_class;
  }
  if (left_command != right_command) {
    return left_command < right_command;
  }
  if (left_key != right_key) {
    return NativePassKeyLess(left_key, right_key);
  }
  return left_sequence < right_sequence;
}

}  // namespace

NativeDepthBlendClass ClassifyNativeDepthBlend(const NativePassKeyInput& input) {
  if (input.command_class == NativePassCommandClass::kResolve ||
      input.command_class == NativePassCommandClass::kClear ||
      input.command_class == NativePassCommandClass::kDepthHandoff) {
    return NativeDepthBlendClass::kTransfer;
  }
  if (input.has_color_target && input.has_pixel_shader && input.color_write_mask) {
    return input.blend_enabled ? NativeDepthBlendClass::kBlended : NativeDepthBlendClass::kOpaque;
  }
  if (input.depth_enabled || input.depth_target.handle) {
    return input.depth_write_enabled ? NativeDepthBlendClass::kDepthOnlyWrite
                                     : NativeDepthBlendClass::kDepthOnlyRead;
  }
  return NativeDepthBlendClass::kNoAttachments;
}

NativePassKey BuildNativePassKey(const NativePassKeyInput& input) {
  NativePassKey key{};
  key.command_class = input.command_class;
  key.color_targets = input.color_targets;
  key.depth_target = input.depth_target;
  key.reflection_family = input.reflection_family;
  key.render_phase = input.render_phase;
  key.origin = input.origin;
  key.shader_family = input.shader_family;
  key.vertex_shader_hash = input.vertex_shader_hash;
  key.pixel_shader_hash = input.pixel_shader_hash;
  key.depth_blend_class = ClassifyNativeDepthBlend(input);
  return key;
}

bool NativePassKeyLess(const NativePassKey& left, const NativePassKey& right) {
  if(left.origin!=right.origin)return left.origin<right.origin;
  if (left.command_class != right.command_class) {
    return left.command_class < right.command_class;
  }
  for (size_t index = 0; index < left.color_targets.size(); ++index) {
    if (left.color_targets[index] == right.color_targets[index]) {
      continue;
    }
    return SurfaceKeyLess(left.color_targets[index], right.color_targets[index]);
  }
  if (left.depth_target != right.depth_target) {
    return SurfaceKeyLess(left.depth_target, right.depth_target);
  }
  return std::tie(left.reflection_family, left.render_phase, left.shader_family,
                  left.vertex_shader_hash, left.pixel_shader_hash, left.depth_blend_class) <
         std::tie(right.reflection_family, right.render_phase, right.shader_family,
                  right.vertex_shader_hash, right.pixel_shader_hash, right.depth_blend_class);
}

uint64_t EstimateNativePrimitiveCount(uint32_t primitive_type, uint32_t element_count) {
  switch (primitive_type) {
    case 0x01:  // Point list.
      return element_count;
    case 0x02:  // Line list.
      return element_count / 2;
    case 0x03:  // Line strip.
    case 0x15:  // 2D line strip.
      return element_count >= 2 ? element_count - 1 : 0;
    case 0x04:  // Triangle list.
      return element_count / 3;
    case 0x05:  // Triangle fan.
    case 0x06:  // Triangle strip.
    case 0x0F:  // Polygon.
    case 0x16:  // 2D triangle strip.
      return element_count >= 3 ? element_count - 2 : 0;
    case 0x08:  // Rectangle list (three guest vertices per rectangle).
      return element_count / 3;
    case 0x0C:  // Line loop.
      return element_count >= 2 ? element_count : 0;
    case 0x0D:  // Quad list.
      return element_count / 4;
    case 0x0E:  // Quad strip.
      return element_count >= 4 ? (element_count - 2) / 2 : 0;
    default:
      return 0;
  }
}

// A new invocation must remain a separate observed region, but a targeted
// next-frame drill-down must match the same source phase in the new invocation.
static bool SameNativePassForDrilldown(NativePassKey left, NativePassKey right) {
  const auto normalize=[](GpuPassOrigin& origin){
    const auto phase=origin.retail_phase;const bool known=origin.attributed();
    origin={};origin.retail_phase=phase;
    origin.source=known?GpuPassOriginSource::kListHeader:GpuPassOriginSource::kNone;
  };
  normalize(left.origin);normalize(right.origin);return left==right;
}

std::vector<NativePassGroup> GroupNativePassObservations(
    std::span<const NativePassObservation> observations) {
  std::vector<NativePassGroup> groups;
  groups.reserve(observations.size());
  for (size_t observation_index = 0; observation_index < observations.size(); ++observation_index) {
    const NativePassObservation& observation = observations[observation_index];
    const bool compatible = !groups.empty() &&
                            groups.back().coarse_range == observation.coarse_range &&
                            groups.back().classification == observation.classification &&
                            groups.back().key == observation.key;
    if (!compatible) {
      NativePassGroup group{};
      group.observation_begin = observation_index;
      group.observation_end = observation_index + 1;
      group.command_start = observation.command_index;
      group.command_end = observation.command_index;
      group.coarse_range = observation.coarse_range;
      group.key = observation.key;
      group.classification = observation.classification;
      group.draw_count = observation.draw_count;
      group.primitive_count = observation.primitive_count;
      groups.push_back(std::move(group));
      continue;
    }
    NativePassGroup& group = groups.back();
    group.observation_end = observation_index + 1;
    group.command_end = observation.command_index;
    group.draw_count = SaturatingAdd(group.draw_count, uint64_t(observation.draw_count));
    group.primitive_count = SaturatingAdd(group.primitive_count, observation.primitive_count);
  }
  return groups;
}

NativeAttributionQueryBudget CalculateNativeAttributionQueryBudget(
    std::span<const NativePassObservation> observations,
    const NativeAttributionQueryBudgetInput& input) {
  NativeAttributionQueryBudget result{};
  uint64_t mandatory_coarse_boundaries = 0;
  uint64_t mandatory_attribution_boundaries = 0;
  if (!observations.empty()) {
    if (input.current_coarse_range == observations.front().coarse_range) {
      mandatory_attribution_boundaries = 1;
    } else {
      mandatory_coarse_boundaries = 1;
    }
  }
  for (size_t observation_index = 1; observation_index < observations.size(); ++observation_index) {
    if (observations[observation_index - 1].coarse_range !=
        observations[observation_index].coarse_range) {
      mandatory_coarse_boundaries = SaturatingAdd(mandatory_coarse_boundaries, uint64_t(1));
    }
  }
  result.mandatory_coarse_boundaries = uint32_t(
      std::min<uint64_t>(mandatory_coarse_boundaries, std::numeric_limits<uint32_t>::max()));
  result.mandatory_attribution_boundaries = uint32_t(
      std::min<uint64_t>(mandatory_attribution_boundaries, std::numeric_limits<uint32_t>::max()));
  result.required_query_count =
      SaturatingAdd(SaturatingAdd(uint64_t(input.queries_already_used),
                                  uint64_t(input.remaining_frame_endpoint_queries)),
                    mandatory_coarse_boundaries);
  result.mandatory_boundaries_fit = result.required_query_count <= uint64_t(input.query_capacity);
  const uint64_t available_query_count =
      result.mandatory_boundaries_fit ? uint64_t(input.query_capacity) - result.required_query_count
                                      : 0;
  result.maximum_detail_boundaries =
      uint32_t(std::min<uint64_t>(available_query_count, std::min(input.maximum_detail_boundaries, kHardDetailBoundaryLimit)));
  const uint32_t attribution_start_boundaries =
      std::min(result.mandatory_attribution_boundaries, result.maximum_detail_boundaries);
  result.available_detail_boundaries =
      result.maximum_detail_boundaries - attribution_start_boundaries;
  result.plan_budget.total_detail_boundaries = result.available_detail_boundaries;
  result.plan_budget.hard_detail_boundaries = std::min(input.maximum_detail_boundaries, kHardDetailBoundaryLimit);
  result.plan_budget.drilldown_boundaries = result.plan_budget.hard_detail_boundaries / 4;
  result.plan_budget.regular_detail_boundaries =
      result.plan_budget.hard_detail_boundaries - result.plan_budget.drilldown_boundaries;
  if(input.maximum_detail_boundaries > kMaximumExtraDetailBoundaries) {
    result.plan_budget.drilldown_boundaries=std::min(input.maximum_detail_boundaries/4,64u);
    result.plan_budget.regular_detail_boundaries=input.maximum_detail_boundaries-result.plan_budget.drilldown_boundaries;
  }
  result.maximum_planned_query_count =
      SaturatingAdd(result.required_query_count, uint64_t(result.maximum_detail_boundaries));
  return result;
}

NativeAttributionPlan BuildNativeAttributionPlan(
    std::span<const NativePassObservation> observations, const NativeAttributionPlanBudget& budget,
    const std::optional<NativeDrilldownTarget>& drilldown_target) {
  NativeAttributionPlan plan{};
  plan.region_by_observation.resize(observations.size(), UINT32_MAX);
  if (observations.empty()) {
    return plan;
  }

  const size_t boundary_count = observations.size();
  std::vector<bool> selected(boundary_count, false);
  selected[0] = true;

  struct BoundaryCandidate {
    size_t observation_index = 0;
    int classification_priority = 0;
    bool target_edge = false;
  };
  std::vector<BoundaryCandidate> regular_candidates;
  std::vector<BoundaryCandidate> target_candidates;
  regular_candidates.reserve(boundary_count);
  target_candidates.reserve(boundary_count);

  for (size_t observation_index = 1; observation_index < observations.size(); ++observation_index) {
    const NativePassObservation& previous = observations[observation_index - 1];
    const NativePassObservation& current = observations[observation_index];
    if (previous.coarse_range != current.coarse_range) {
      selected[observation_index] = true;
      continue;
    }
    const bool previous_target = drilldown_target && SameNativePassForDrilldown(previous.key, drilldown_target->key);
    const bool current_target = drilldown_target && SameNativePassForDrilldown(current.key, drilldown_target->key);
    if (previous_target || current_target) {
      target_candidates.push_back({observation_index,
                                   std::max(ClassificationPriority(previous.classification),
                                            ClassificationPriority(current.classification)),
                                   previous_target != current_target});
      continue;
    }
    if (previous.key == current.key && previous.classification == current.classification) {
      continue;
    }
    regular_candidates.push_back({observation_index,
                                  (previous.key.origin!=current.key.origin ? 4 : 0) +
                                  std::max(ClassificationPriority(previous.classification),
                                           ClassificationPriority(current.classification)),
                                  false});
  }

  std::stable_sort(target_candidates.begin(), target_candidates.end(),
                   [](const BoundaryCandidate& left, const BoundaryCandidate& right) {
                     if (left.target_edge != right.target_edge) {
                       return left.target_edge > right.target_edge;
                     }
                     return left.observation_index < right.observation_index;
                   });
  const uint32_t hard_budget = std::min(budget.hard_detail_boundaries, kHardDetailBoundaryLimit);
  const uint32_t total_budget = std::min(budget.total_detail_boundaries, hard_budget);
  const uint32_t hard_target_budget = hard_budget / 4;
  const uint32_t target_budget = std::min(budget.drilldown_boundaries, hard_target_budget);
  const uint32_t target_candidate_count =
      uint32_t(std::min<size_t>(target_candidates.size(), std::numeric_limits<uint32_t>::max()));
  const uint32_t target_limit = std::min({target_budget, target_candidate_count, total_budget});
  for (uint32_t index = 0; index < target_limit; ++index) {
    const size_t boundary = target_candidates[index].observation_index;
    selected[boundary] = true;
    ++plan.drilldown_boundaries;
  }

  std::stable_sort(regular_candidates.begin(), regular_candidates.end(),
                   [](const BoundaryCandidate& left, const BoundaryCandidate& right) {
                     if (left.classification_priority != right.classification_priority) {
                       return left.classification_priority > right.classification_priority;
                     }
                     return left.observation_index < right.observation_index;
                   });
  const uint32_t regular_budget =
      std::min(budget.regular_detail_boundaries, hard_budget - hard_target_budget);
  const uint32_t remaining_budget = total_budget - plan.drilldown_boundaries;
  const uint32_t regular_candidate_count =
      uint32_t(std::min<size_t>(regular_candidates.size(), std::numeric_limits<uint32_t>::max()));
  const uint32_t regular_limit =
      std::min({regular_budget, regular_candidate_count, remaining_budget});
  for (uint32_t index = 0; index < regular_limit; ++index) {
    selected[regular_candidates[index].observation_index] = true;
    ++plan.regular_detail_boundaries;
  }

  if (drilldown_target) {
    plan.target_id = drilldown_target->target_id;
    plan.target_matched = std::any_of(observations.begin(), observations.end(),
                                      [&](const NativePassObservation& observation) {
                                        return SameNativePassForDrilldown(observation.key, drilldown_target->key);
                                      });
  }

  for (size_t region_begin = 0; region_begin < observations.size();) {
    size_t region_end = region_begin + 1;
    while (region_end < observations.size() && !selected[region_end]) {
      ++region_end;
    }

    const NativePassObservation& first = observations[region_begin];
    NativePassRegion region{};
    region.observation_begin = region_begin;
    region.observation_end = region_end;
    region.command_start = first.command_index;
    region.command_end = observations[region_end - 1].command_index;
    region.coarse_range = first.coarse_range;
    region.key = first.key;
    region.classification = first.classification;
    region.pass_key_count = 1;
    NativePassKey previous_key = first.key;
    NativePassClassification previous_classification = first.classification;
    bool all_target = drilldown_target && SameNativePassForDrilldown(first.key, drilldown_target->key);
    for (size_t observation_index = region_begin; observation_index < region_end;
         ++observation_index) {
      const NativePassObservation& observation = observations[observation_index];
      if (observation_index != region_begin &&
          (observation.key != previous_key ||
           observation.classification != previous_classification)) {
        ++region.pass_key_count;
      }
      if(observation.key.origin!=region.key.origin){
        region.key.origin={};region.key.origin.source=GpuPassOriginSource::kMixed;
      }
      previous_key = observation.key;
      previous_classification = observation.classification;
      region.classification =
          StrongerClassification(region.classification, observation.classification);
      region.draw_count = SaturatingAdd(region.draw_count, uint64_t(observation.draw_count));
      region.primitive_count = SaturatingAdd(region.primitive_count, observation.primitive_count);
      all_target &= drilldown_target && SameNativePassForDrilldown(observation.key, drilldown_target->key);
    }
    if (region.pass_key_count > 1) {
      region.detail = NativePassRegionDetail::kCoalesced;
      region.key.command_class = NativePassCommandClass::kMixed;
      region.key.depth_blend_class = NativeDepthBlendClass::kMixed;
    } else if (all_target && region_end - region_begin == 1 && drilldown_target) {
      region.detail = NativePassRegionDetail::kDrilldown;
    }
    const uint32_t region_index = uint32_t(plan.regions.size());
    for (size_t observation_index = region_begin; observation_index < region_end;
         ++observation_index) {
      plan.region_by_observation[observation_index] = region_index;
    }
    plan.regions.push_back(std::move(region));
    region_begin = region_end;
  }
  return plan;
}

NativeAttributionCoverage CheckNativeAttributionCoverage(
    std::span<const NativePassObservation> observations, const NativeAttributionPlan& plan) {
  NativeAttributionCoverage coverage{};
  coverage.observation_count = observations.size();
  std::vector<uint8_t> visits(observations.size(), 0);
  for (const NativePassObservation& observation : observations) {
    coverage.source_draw_count =
        SaturatingAdd(coverage.source_draw_count, uint64_t(observation.draw_count));
    coverage.source_primitive_count =
        SaturatingAdd(coverage.source_primitive_count, observation.primitive_count);
  }
  for (const NativePassRegion& region : plan.regions) {
    if (region.observation_begin >= region.observation_end ||
        region.observation_end > observations.size()) {
      return coverage;
    }
    coverage.planned_draw_count = SaturatingAdd(coverage.planned_draw_count, region.draw_count);
    coverage.planned_primitive_count =
        SaturatingAdd(coverage.planned_primitive_count, region.primitive_count);
    for (size_t observation_index = region.observation_begin;
         observation_index < region.observation_end; ++observation_index) {
      if (visits[observation_index] == std::numeric_limits<uint8_t>::max()) {
        return coverage;
      }
      ++visits[observation_index];
    }
  }
  coverage.complete =
      plan.region_by_observation.size() == observations.size() &&
      std::all_of(visits.begin(), visits.end(),
                  [](uint8_t visits_for_observation) { return visits_for_observation == 1; }) &&
      coverage.source_draw_count == coverage.planned_draw_count &&
      coverage.source_primitive_count == coverage.planned_primitive_count;
  if (!coverage.complete) {
    return coverage;
  }
  for (size_t observation_index = 0; observation_index < observations.size(); ++observation_index) {
    const uint32_t region_index = plan.region_by_observation[observation_index];
    if (region_index >= plan.regions.size()) {
      coverage.complete = false;
      break;
    }
    const NativePassRegion& region = plan.regions[region_index];
    if (observation_index < region.observation_begin ||
        observation_index >= region.observation_end) {
      coverage.complete = false;
      break;
    }
  }
  return coverage;
}

NativeAttributionAccounting CalculateNativeAttributionAccounting(
    uint64_t flat_envelope_ticks, std::span<const NativeResolvedPassRegion> detail_regions) {
  NativeAttributionAccounting accounting{};
  accounting.flat_envelope_ticks = flat_envelope_ticks;
  for (const NativeResolvedPassRegion& region : detail_regions) {
    if (region.available) {
      accounting.detail_ticks = SaturatingAdd(accounting.detail_ticks, region.elapsed_ticks);
    }
  }
  accounting.exported_total_ticks = flat_envelope_ticks;
  return accounting;
}

NativeDrilldownScheduler::NativeDrilldownScheduler(uint64_t slow_region_nanoseconds,
                                                   uint32_t expiry_frames)
    : slow_region_nanoseconds_(slow_region_nanoseconds), expiry_frames_(expiry_frames) {}

void NativeDrilldownScheduler::Clear() {
  pending_ = {};
  next_target_id_ = 1;
}

void NativeDrilldownScheduler::ObserveCompleted(uint32_t frame, uint64_t sequence,
                                                std::span<const NativeResolvedPassRegion> regions) {
  if (!frame || !sequence) {
    return;
  }
  Expire(frame);
  for (const NativeResolvedPassRegion& region : regions) {
    if (!region.available || region.elapsed_nanoseconds < slow_region_nanoseconds_ ||
        region.region.classification == NativePassClassification::kKnown ||
        region.region.pass_key_count != 1) {
      continue;
    }

    auto existing = std::find_if(pending_.begin(), pending_.end(), [&](const auto& pending) {
      return pending && SameNativePassForDrilldown(pending->key, region.region.key);
    });
    if (existing != pending_.end()) {
      PendingTarget& pending = **existing;
      if (CandidateBetter(region.region.classification, region.elapsed_nanoseconds,
                          region.region.command_start, region.region.key, sequence,
                          pending.classification, pending.priority_nanoseconds,
                          pending.source_command, pending.key, pending.source_sequence)) {
        pending.classification = region.region.classification;
        pending.priority_nanoseconds = region.elapsed_nanoseconds;
        pending.source_sequence = sequence;
        pending.source_frame = frame;
        pending.source_command = region.region.command_start;
      }
      continue;
    }

    PendingTarget candidate{};
    candidate.target_id = next_target_id_++;
    if (!candidate.target_id) {
      candidate.target_id = next_target_id_++;
    }
    candidate.key = region.region.key;
    candidate.classification = region.region.classification;
    candidate.priority_nanoseconds = region.elapsed_nanoseconds;
    candidate.source_sequence = sequence;
    candidate.source_frame = frame;
    candidate.source_command = region.region.command_start;
    candidate.expires_after_frame = SaturatingAdd(frame, expiry_frames_);

    auto empty = std::find_if(pending_.begin(), pending_.end(),
                              [](const auto& pending) { return !pending; });
    if (empty != pending_.end()) {
      *empty = std::move(candidate);
      continue;
    }

    auto worst = pending_.end();
    for (auto pending = pending_.begin(); pending != pending_.end(); ++pending) {
      if ((*pending)->reservation_id) {
        continue;
      }
      if (worst == pending_.end() ||
          CandidateBetter((*worst)->classification, (*worst)->priority_nanoseconds,
                          (*worst)->source_command, (*worst)->key, (*worst)->source_sequence,
                          (*pending)->classification, (*pending)->priority_nanoseconds,
                          (*pending)->source_command, (*pending)->key,
                          (*pending)->source_sequence)) {
        worst = pending;
      }
    }
    if (worst != pending_.end() &&
        CandidateBetter(candidate.classification, candidate.priority_nanoseconds,
                        candidate.source_command, candidate.key, candidate.source_sequence,
                        (*worst)->classification, (*worst)->priority_nanoseconds,
                        (*worst)->source_command, (*worst)->key, (*worst)->source_sequence)) {
      *worst = std::move(candidate);
    }
  }
}

std::optional<NativeDrilldownReservation> NativeDrilldownScheduler::Acquire(
    uint32_t frame, uint64_t reservation_id) {
  if (!frame || !reservation_id) {
    return std::nullopt;
  }
  Expire(frame);
  auto best = pending_.end();
  for (auto pending = pending_.begin(); pending != pending_.end(); ++pending) {
    if (!*pending || (*pending)->reservation_id || (*pending)->source_frame >= frame) {
      continue;
    }
    if (best == pending_.end() ||
        CandidateBetter((*pending)->classification, (*pending)->priority_nanoseconds,
                        (*pending)->source_command, (*pending)->key, (*pending)->source_sequence,
                        (*best)->classification, (*best)->priority_nanoseconds,
                        (*best)->source_command, (*best)->key, (*best)->source_sequence)) {
      best = pending;
    }
  }
  if (best == pending_.end()) {
    return std::nullopt;
  }
  (*best)->reservation_id = reservation_id;
  return NativeDrilldownReservation{(*best)->target_id, reservation_id,
                                    (*best)->expires_after_frame, (*best)->key};
}

bool NativeDrilldownScheduler::Complete(uint64_t target_id, uint64_t reservation_id,
                                        uint32_t completed_frame, bool matched) {
  auto target = std::find_if(pending_.begin(), pending_.end(), [&](const auto& pending) {
    return pending && pending->target_id == target_id;
  });
  if (target == pending_.end() || !reservation_id || (*target)->reservation_id != reservation_id) {
    return false;
  }
  if (matched || completed_frame > (*target)->expires_after_frame) {
    target->reset();
  } else {
    (*target)->reservation_id = 0;
  }
  return true;
}

void NativeDrilldownScheduler::Expire(uint32_t frame) {
  for (auto& pending : pending_) {
    if (pending && !pending->reservation_id && frame > pending->expires_after_frame) {
      pending.reset();
    }
  }
}

size_t NativeDrilldownScheduler::pending_count() const {
  return size_t(std::count_if(pending_.begin(), pending_.end(),
                              [](const auto& pending) { return pending.has_value(); }));
}

size_t NativeDrilldownScheduler::in_flight_count() const {
  return size_t(std::count_if(pending_.begin(), pending_.end(), [](const auto& pending) {
    return pending && pending->reservation_id;
  }));
}

void NativeAttributionCompletionQueue::Clear() {
  slots_ = {};
  size_ = 0;
}

bool NativeAttributionCompletionQueue::Complete(size_t slot,
                                                NativeCompletedAttributionFrame frame) {
  if (slot >= slots_.size() || !frame.sequence || slots_[slot]) {
    return false;
  }
  for (const auto& completed : slots_) {
    if (completed && completed->sequence == frame.sequence) {
      return false;
    }
  }
  slots_[slot] = std::move(frame);
  ++size_;
  return true;
}

bool NativeAttributionCompletionQueue::PopNext(uint64_t sequence,
                                               NativeCompletedAttributionFrame* frame) {
  if (!sequence || !frame) {
    return false;
  }
  for (auto& completed : slots_) {
    if (!completed || completed->sequence != sequence) {
      continue;
    }
    *frame = std::move(*completed);
    completed.reset();
    --size_;
    return true;
  }
  return false;
}

bool NativeAttributionCompletionQueue::occupied(size_t slot) const {
  return slot < slots_.size() && slots_[slot].has_value();
}

const char* NativePassCommandClassName(NativePassCommandClass command_class) {
  switch (command_class) {
    case NativePassCommandClass::kDraw:
      return "draw";
    case NativePassCommandClass::kDrawUp:
      return "draw-up";
    case NativePassCommandClass::kDrawIndexed:
      return "draw-indexed";
    case NativePassCommandClass::kResolve:
      return "resolve";
    case NativePassCommandClass::kClear:
      return "clear";
    case NativePassCommandClass::kDepthHandoff:
      return "depth-handoff";
    case NativePassCommandClass::kMixed:
      return "mixed";
  }
  return "invalid";
}

const char* NativePassClassificationName(NativePassClassification classification) {
  switch (classification) {
    case NativePassClassification::kKnown:
      return "known";
    case NativePassClassification::kOther:
      return "other";
    case NativePassClassification::kUnknown:
      return "unknown";
  }
  return "invalid";
}

const char* NativeDepthBlendClassName(NativeDepthBlendClass depth_blend_class) {
  switch (depth_blend_class) {
    case NativeDepthBlendClass::kOpaque:
      return "opaque";
    case NativeDepthBlendClass::kBlended:
      return "blended";
    case NativeDepthBlendClass::kDepthOnlyRead:
      return "depth-only-read";
    case NativeDepthBlendClass::kDepthOnlyWrite:
      return "depth-only-write";
    case NativeDepthBlendClass::kTransfer:
      return "transfer";
    case NativeDepthBlendClass::kNoAttachments:
      return "no-attachments";
    case NativeDepthBlendClass::kMixed:
      return "mixed";
  }
  return "invalid";
}

const char* NativePassRegionDetailName(NativePassRegionDetail detail) {
  switch (detail) {
    case NativePassRegionDetail::kGrouped:
      return "grouped";
    case NativePassRegionDetail::kCoalesced:
      return "coalesced";
    case NativePassRegionDetail::kDrilldown:
      return "drilldown";
  }
  return "invalid";
}

}  // namespace rex::graphics::gta4_native::attribution
