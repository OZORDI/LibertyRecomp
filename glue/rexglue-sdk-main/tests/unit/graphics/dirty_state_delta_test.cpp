/**
 * @file dirty_state_delta_test.cpp
 * @brief Dirty-state extraction and versioning tests for the GTA IV native renderer.
 */

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
#include <vector>

#include "graphics/gta4_native/dirty_state_delta.h"
#include "graphics/gta4_native/frame_constant_arena.h"
#include "graphics/gta4_native/stateful_constant_state.h"
#include "graphics/gta4_native/native_shader_booleans.h"

namespace gta4 = rex::graphics::gta4_native;

namespace {

struct TestLayoutStorage {
  std::array<gta4::DirtyBitSpan, 2> vertex_constants{{{0, 0, 8, 0}, {0, 8, 8, 8}}};
  std::array<gta4::DirtyBitSpan, 2> pixel_constants{{{1, 60, 4, 12}, {2, 0, 4, 16}}};
  std::array<gta4::DirtyBitSpan, 1> texture_fetches{{{2, 8, 8, 0}}};
  std::array<gta4::DirtyBitSpan, 1> samplers{{{3, 16, 8, 4}}};
  std::array<gta4::DirtyBitSpan, 1> booleans{{{4, 0, 8, 0}}};
  std::array<gta4::DirtyBitSpan, 1> integers{{{4, 8, 8, 0}}};
  std::array<gta4::DirtyBitSpan, 1> fixed_state{{{3, 0, 8, 0}}};
  std::array<gta4::DirtyBitSpan, 1> dynamic_state{{{3, 8, 8, 0}}};

  gta4::DirtyStateLayout Layout() const {
    return {
        {vertex_constants, 16}, {pixel_constants, 20}, {texture_fetches, 8}, {samplers, 12},
        {booleans, 8},          {integers, 8},         {fixed_state, 8},     {dynamic_state, 8},
    };
  }
};

template <typename T, size_t Size>
std::span<const std::byte> Bytes(const std::array<T, Size>& values) {
  return std::as_bytes(std::span(values));
}

}  // namespace

TEST_CASE("GTA IV native dirty state produces typed coalesced ranges and masks") {
  const TestLayoutStorage storage;
  const gta4::NativeDirtyWords words = {
      0x000000000000039C, 0xC000000000000000, 0x0000000000000503,
      0x00000000000B0609, 0x0000000000008182,
  };
  const gta4::DirtyDeltaBuildResult build = gta4::BuildDirtyStateDelta(words, storage.Layout());

  REQUIRE(build.valid());
  REQUIRE(build.delta.vertex_constant_ranges.ranges ==
          std::vector<gta4::DirtyElementRange>{{2, 3}, {7, 3}});
  REQUIRE(build.delta.pixel_constant_ranges.ranges ==
          std::vector<gta4::DirtyElementRange>{{14, 4}});
  REQUIRE(build.delta.texture_fetch_stage_ranges.ranges ==
          std::vector<gta4::DirtyElementRange>{{0, 1}, {2, 1}});
  REQUIRE(build.delta.sampler_stage_ranges.ranges ==
          std::vector<gta4::DirtyElementRange>{{4, 2}, {7, 1}});
  REQUIRE(build.delta.boolean_constant_mask.words == std::vector<uint64_t>{0x82});
  REQUIRE(build.delta.integer_constant_mask.words == std::vector<uint64_t>{0x81});
  REQUIRE(build.delta.fixed_state_mask.words == std::vector<uint64_t>{0x09});
  REQUIRE(build.delta.dynamic_state_mask.words == std::vector<uint64_t>{0x06});
  REQUIRE(build.delta.TouchedComponents() == gta4::AllDirtyStateComponents());
}

TEST_CASE("GTA IV native dirty state handles bit 63 and masks larger than one word") {
  const std::array<gta4::DirtyBitSpan, 1> fixed_spans{{{4, 63, 1, 70}}};
  gta4::DirtyStateLayout layout;
  layout.fixed_state = {fixed_spans, 71};
  gta4::NativeDirtyWords words{};
  words[4] = uint64_t{1} << 63;

  const gta4::DirtyDeltaBuildResult build = gta4::BuildDirtyStateDelta(words, layout);
  REQUIRE(build.valid());
  REQUIRE(build.delta.fixed_state_mask.words.size() == 2);
  REQUIRE(build.delta.fixed_state_mask.words[0] == 0);
  REQUIRE(build.delta.fixed_state_mask.words[1] == 0x40);
  REQUIRE(build.delta.fixed_state_mask.Test(70));
  REQUIRE_FALSE(build.delta.fixed_state_mask.Test(71));
}

TEST_CASE("GTA IV native dirty state reuses caller-owned delta storage") {
  const TestLayoutStorage storage;
  gta4::NativeDirtyWords words = {
      0x000000000000039C, 0xC000000000000000, 0x0000000000000503,
      0x00000000000B0609, 0x0000000000008182,
  };
  gta4::DirtyStateDelta delta;
  gta4::DirtyDeltaScratch scratch;
  REQUIRE(gta4::BuildDirtyStateDelta(words, storage.Layout(), delta, scratch).valid());
  const size_t scratch_capacity = scratch.dirty_elements.capacity();
  const size_t range_capacity = delta.vertex_constant_ranges.ranges.capacity();
  const size_t mask_capacity = delta.fixed_state_mask.words.capacity();

  words = {};
  REQUIRE(gta4::BuildDirtyStateDelta(words, storage.Layout(), delta, scratch).valid());
  REQUIRE_FALSE(delta.Any());
  REQUIRE(scratch.dirty_elements.capacity() == scratch_capacity);
  REQUIRE(delta.vertex_constant_ranges.ranges.capacity() == range_capacity);
  REQUIRE(delta.fixed_state_mask.words.capacity() == mask_capacity);

  const std::array<gta4::DirtyBitSpan, 1> invalid_span{{{5, 0, 1, 0}}};
  gta4::DirtyStateLayout invalid_layout;
  invalid_layout.fixed_state = {invalid_span, 1};
  REQUIRE_FALSE(gta4::BuildDirtyStateDelta(words, invalid_layout, delta, scratch).valid());
  REQUIRE_FALSE(delta.Any());
  REQUIRE(delta.fixed_state_mask.element_count == 0);
}

TEST_CASE("GTA IV native dirty layout validation identifies the exact bad span") {
  const std::array<gta4::DirtyBitSpan, 2> spans{{{0, 0, 1, 0}, {5, 0, 1, 1}}};
  gta4::DirtyStateLayout layout;
  layout.vertex_constants = {spans, 2};
  const gta4::DirtyLayoutValidationResult validation = gta4::ValidateDirtyStateLayout(layout);

  REQUIRE_FALSE(validation.valid());
  REQUIRE(validation.error == gta4::DirtyLayoutError::kDirtyWordOutOfRange);
  REQUIRE(validation.component == gta4::DirtyStateComponent::kVertexConstants);
  REQUIRE(validation.span_index == 1);

  const std::array<gta4::DirtyBitSpan, 1> bit_overflow{{{0, 63, 2, 0}}};
  layout.vertex_constants = {bit_overflow, 2};
  REQUIRE(gta4::ValidateDirtyStateLayout(layout).error ==
          gta4::DirtyLayoutError::kDirtyBitRangeOutOfRange);

  const std::array<gta4::DirtyBitSpan, 1> element_overflow{{{0, 0, 2, 1}}};
  layout.vertex_constants = {element_overflow, 2};
  REQUIRE(gta4::ValidateDirtyStateLayout(layout).error ==
          gta4::DirtyLayoutError::kElementRangeOutOfRange);
}

TEST_CASE("GTA IV native component versions advance once per dirty delta") {
  const TestLayoutStorage storage;
  gta4::NativeDirtyWords words{};
  words[0] = 0x03;
  words[3] = 0x01;
  const gta4::DirtyStateDelta delta = gta4::BuildDirtyStateDelta(words, storage.Layout()).delta;
  gta4::StateVersionVector versions;

  const gta4::StateVersionUpdateResult first = gta4::ApplyDirtyStateVersions(versions, delta);
  REQUIRE(first.applied());
  REQUIRE(versions.Get(gta4::DirtyStateComponent::kVertexConstants).revision == 1);
  REQUIRE(versions.Get(gta4::DirtyStateComponent::kFixedState).revision == 1);
  REQUIRE(versions.Get(gta4::DirtyStateComponent::kPixelConstants).revision == 0);

  const gta4::StateVersionUpdateResult second = gta4::ApplyDirtyStateVersions(versions, delta);
  REQUIRE(second.applied());
  REQUIRE(versions.Get(gta4::DirtyStateComponent::kVertexConstants).revision == 2);
  REQUIRE(versions.Get(gta4::DirtyStateComponent::kFixedState).revision == 2);

  const gta4::DirtyStateDelta empty_delta;
  const gta4::StateVersionVector before_empty = versions;
  REQUIRE(gta4::ApplyDirtyStateVersions(versions, empty_delta).applied());
  REQUIRE(versions.components == before_empty.components);
}

TEST_CASE("GTA IV native version exhaustion is atomic and rollover is explicit") {
  gta4::DirtyStateDelta delta;
  delta.fixed_state_mask.element_count = 1;
  delta.fixed_state_mask.words = {1};

  gta4::StateVersionVector rollover_versions;
  auto& rollover =
      rollover_versions.components[static_cast<size_t>(gta4::DirtyStateComponent::kFixedState)];
  rollover.revision = std::numeric_limits<uint64_t>::max();
  const gta4::StateVersionUpdateResult rolled =
      gta4::ApplyDirtyStateVersions(rollover_versions, delta);
  REQUIRE(rolled.applied());
  REQUIRE(rollover.epoch == 1);
  REQUIRE(rollover.revision == 0);

  gta4::StateVersionVector exhausted_versions;
  auto& exhausted =
      exhausted_versions.components[static_cast<size_t>(gta4::DirtyStateComponent::kFixedState)];
  exhausted.epoch = std::numeric_limits<uint64_t>::max();
  exhausted.revision = std::numeric_limits<uint64_t>::max();
  const gta4::StateVersionVector before = exhausted_versions;
  const gta4::StateVersionUpdateResult rejected =
      gta4::ApplyDirtyStateVersions(exhausted_versions, delta);
  REQUIRE_FALSE(rejected.applied());
  REQUIRE(rejected.status == gta4::StateVersionUpdateStatus::kVersionSpaceExhausted);
  REQUIRE(exhausted_versions.components == before.components);
}

TEST_CASE("GTA IV native draw state reuse ignores unrelated component changes") {
  gta4::StateVersionVector versions;
  const gta4::DirtyStateComponentMask dependencies =
      gta4::DirtyStateComponentBit(gta4::DirtyStateComponent::kVertexConstants) |
      gta4::DirtyStateComponentBit(gta4::DirtyStateComponent::kSamplerStages);
  const gta4::DrawStateVersionToken token =
      gta4::CaptureDrawStateVersionToken(versions, dependencies);
  REQUIRE(gta4::EvaluateDrawStateReuse(token, versions, dependencies).reusable());

  ++versions.components[static_cast<size_t>(gta4::DirtyStateComponent::kDynamicState)].revision;
  REQUIRE(gta4::EvaluateDrawStateReuse(token, versions, dependencies).reusable());

  ++versions.components[static_cast<size_t>(gta4::DirtyStateComponent::kSamplerStages)].revision;
  const gta4::DrawStateReuseDecision stale =
      gta4::EvaluateDrawStateReuse(token, versions, dependencies);
  REQUIRE_FALSE(stale.reusable());
  REQUIRE(stale.status == gta4::DrawStateReuseStatus::kComponentVersionMismatch);
  REQUIRE(stale.mismatched_components ==
          gta4::DirtyStateComponentBit(gta4::DirtyStateComponent::kSamplerStages));

  const auto smaller_dependencies =
      gta4::DirtyStateComponentBit(gta4::DirtyStateComponent::kVertexConstants);
  REQUIRE(gta4::EvaluateDrawStateReuse(token, versions, smaller_dependencies).status ==
          gta4::DrawStateReuseStatus::kDependencySetMismatch);
  REQUIRE_FALSE(gta4::EvaluateDrawStateReuse({}, versions, dependencies).reusable());
}

TEST_CASE("GTA IV native diagnostic comparison finds snapshot semantic divergence") {
  const std::array<uint32_t, 3> matching_delta = {10, 20, 30};
  const std::array<uint32_t, 3> matching_snapshot = matching_delta;
  const std::array<uint8_t, 4> divergent_delta = {1, 2, 3, 4};
  const std::array<uint8_t, 4> divergent_snapshot = {1, 2, 9, 4};
  const std::array<uint16_t, 2> short_delta = {100, 200};
  const std::array<uint16_t, 3> long_snapshot = {100, 200, 300};

  gta4::DiagnosticSemanticStateView delta_view;
  gta4::DiagnosticSemanticStateView snapshot_view;
  const size_t vertex_index = static_cast<size_t>(gta4::DirtyStateComponent::kVertexConstants);
  const size_t fixed_index = static_cast<size_t>(gta4::DirtyStateComponent::kFixedState);
  const size_t dynamic_index = static_cast<size_t>(gta4::DirtyStateComponent::kDynamicState);
  delta_view[vertex_index] = Bytes(matching_delta);
  snapshot_view[vertex_index] = Bytes(matching_snapshot);
  delta_view[fixed_index] = Bytes(divergent_delta);
  snapshot_view[fixed_index] = Bytes(divergent_snapshot);
  delta_view[dynamic_index] = Bytes(short_delta);
  snapshot_view[dynamic_index] = Bytes(long_snapshot);

  const gta4::DiagnosticSemanticComparisonResult comparison =
      gta4::CompareDiagnosticSemanticState(delta_view, snapshot_view);
  REQUIRE_FALSE(comparison.equivalent());
  REQUIRE(comparison.mismatched_components ==
          (gta4::DirtyStateComponentBit(gta4::DirtyStateComponent::kFixedState) |
           gta4::DirtyStateComponentBit(gta4::DirtyStateComponent::kDynamicState)));
  REQUIRE(comparison.details[fixed_index].first_mismatch_offset == 2);
  REQUIRE(comparison.details[dynamic_index].first_mismatch_offset == Bytes(short_delta).size());
  REQUIRE(comparison.details[vertex_index].first_mismatch_offset == gta4::kNoSemanticMismatch);

  const auto vertex_only =
      gta4::DirtyStateComponentBit(gta4::DirtyStateComponent::kVertexConstants);
  REQUIRE(
      gta4::CompareDiagnosticSemanticState(delta_view, snapshot_view, vertex_only).equivalent());
}

TEST_CASE("GTA IV title float-constant masks map MSB-first to 64-byte groups") {
  const std::array<gta4::DirtyBitSpan, 1> vertex_spans{{{0, 0, 64, 0, true}}};
  const std::array<gta4::DirtyBitSpan, 1> pixel_spans{{{1, 8, 56, 0, true}}};
  const std::array<gta4::DirtyBitSpan, 1> boolean_spans{{{4, 56, 1, 0}}};
  gta4::DirtyStateLayout layout;
  layout.vertex_constants = {vertex_spans, 64};
  layout.pixel_constants = {pixel_spans, 56};
  layout.boolean_constants = {boolean_spans, 1};

  gta4::NativeDirtyWords words{};
  words[0] = (uint64_t{1} << 63) | (uint64_t{1} << 62) | (uint64_t{1} << 60);
  words[1] = (uint64_t{1} << 63) | (uint64_t{1} << 61) | (uint64_t{1} << 60) |
             (uint64_t{1} << 8);
  words[4] = uint64_t{1} << 56;
  const gta4::DirtyDeltaBuildResult build = gta4::BuildDirtyStateDelta(words, layout);

  REQUIRE(build.valid());
  REQUIRE(build.delta.vertex_constant_ranges.ranges ==
          std::vector<gta4::DirtyElementRange>{{0, 2}, {3, 1}});
  REQUIRE(build.delta.pixel_constant_ranges.ranges ==
          std::vector<gta4::DirtyElementRange>{{0, 1}, {2, 2}, {55, 1}});
  REQUIRE(build.delta.boolean_constant_mask.words == std::vector<uint64_t>{1});
}

TEST_CASE("GTA IV constant deltas copy only coalesced dirty payload bytes") {
  std::array<uint8_t, 256> source{};
  for (size_t index = 0; index < source.size(); ++index) {
    source[index] = static_cast<uint8_t>(index);
  }
  gta4::DirtyRangeSet ranges;
  ranges.ranges = {{1, 2}, {3, 1}};
  gta4::ConstantPayloadDelta delta;

  REQUIRE(gta4::CaptureConstantPayloadDelta(source, ranges, 64, delta));
  REQUIRE(delta.ranges ==
          std::vector<gta4::ConstantDeltaRange>{{64, 0, 192}});
  REQUIRE(delta.payload.size() == 192);
  REQUIRE(std::equal(delta.payload.begin(), delta.payload.end(), source.begin() + 64));
}

TEST_CASE("GTA IV authoritative constants reuse identical writes and version real changes") {
  gta4::AuthoritativeConstantState state(16);
  std::array<uint8_t, 16> initial{};
  initial[3] = 7;
  gta4::ConstantPayloadDelta snapshot;
  REQUIRE(gta4::CaptureCompleteConstantSnapshot(initial, snapshot));
  uint32_t hash_calls = 0;
  const auto hash = [&hash_calls](std::span<const uint8_t> bytes) {
    ++hash_calls;
    uint64_t value = 0;
    for (uint8_t byte : bytes) {
      value = value * 131 + byte;
    }
    return value;
  };

  const gta4::ConstantApplyResult first = state.Apply(snapshot, hash);
  REQUIRE(first);
  REQUIRE(first.changed);
  REQUIRE(first.version->token.revision == 1);
  REQUIRE(hash_calls == 1);

  gta4::ConstantPayloadDelta identical;
  identical.ranges = {{3, 0, 1}};
  identical.payload = {7};
  const gta4::ConstantApplyResult reused = state.Apply(identical, hash);
  REQUIRE(reused);
  REQUIRE_FALSE(reused.changed);
  REQUIRE(reused.version == first.version);
  REQUIRE(hash_calls == 1);

  identical.payload[0] = 9;
  const gta4::ConstantApplyResult changed = state.Apply(identical, hash);
  REQUIRE(changed);
  REQUIRE(changed.changed);
  REQUIRE(changed.version->token.revision == 2);
  REQUIRE(hash_calls == 2);
}

TEST_CASE("GTA IV exhausted constant versions reject without mutating canonical state") {
  const uint64_t maximum = std::numeric_limits<uint64_t>::max();
  gta4::AuthoritativeConstantState state(4, {maximum, maximum - 1});
  std::array<uint8_t, 4> initial{1, 2, 3, 4};
  gta4::ConstantPayloadDelta snapshot;
  REQUIRE(gta4::CaptureCompleteConstantSnapshot(initial, snapshot));
  uint32_t hash_calls = 0;
  const auto hash = [&hash_calls](std::span<const uint8_t> bytes) {
    ++hash_calls;
    return uint64_t(bytes.front());
  };
  const auto initialized = state.Apply(snapshot, hash);
  REQUIRE(initialized);
  REQUIRE(initialized.version->token.epoch == maximum);
  REQUIRE(initialized.version->token.revision == maximum);
  const std::vector<uint8_t> before = state.canonical();
  const auto before_version = state.current_version();

  gta4::ConstantPayloadDelta write;
  write.ranges = {{1, 0, 1}};
  write.payload = {9};
  const auto rejected = state.Apply(write, hash);
  REQUIRE(rejected.status == gta4::ConstantApplyStatus::kVersionSpaceExhausted);
  REQUIRE_FALSE(rejected.changed);
  REQUIRE(state.canonical() == before);
  REQUIRE(state.current_version() == before_version);
  REQUIRE(hash_calls == 1);
}

TEST_CASE("GTA IV queued constant versions retain command ordering immutably") {
  gta4::AuthoritativeConstantState state(8);
  std::array<uint8_t, 8> initial{};
  gta4::ConstantPayloadDelta snapshot;
  REQUIRE(gta4::CaptureCompleteConstantSnapshot(initial, snapshot));
  const auto hash = [](std::span<const uint8_t> bytes) {
    uint64_t value = 0;
    for (uint8_t byte : bytes) {
      value = value * 257 + byte;
    }
    return value;
  };
  REQUIRE(state.Apply(snapshot, hash));

  gta4::ConstantPayloadDelta first_write;
  first_write.ranges = {{2, 0, 2}};
  first_write.payload = {10, 11};
  const auto first = state.Apply(first_write, hash).version;
  gta4::ConstantPayloadDelta second_write;
  second_write.ranges = {{2, 0, 2}};
  second_write.payload = {20, 21};
  const auto second = state.Apply(second_write, hash).version;

  const auto first_bytes = gta4::AuthoritativeConstantState::Materialize(first);
  const auto second_bytes = gta4::AuthoritativeConstantState::Materialize(second);
  REQUIRE(first_bytes);
  REQUIRE(second_bytes);
  REQUIRE((*first_bytes)[2] == 10);
  REQUIRE((*first_bytes)[3] == 11);
  REQUIRE((*second_bytes)[2] == 20);
  REQUIRE((*second_bytes)[3] == 21);
}

TEST_CASE("GTA IV compare transport reports exact mismatch and snapshot fallback") {
  gta4::AuthoritativeConstantState state(8);
  std::array<uint8_t, 8> initial{};
  gta4::ConstantPayloadDelta initial_snapshot;
  REQUIRE(gta4::CaptureCompleteConstantSnapshot(initial, initial_snapshot));
  const auto hash = [](std::span<const uint8_t> bytes) {
    uint64_t value = 0;
    for (uint8_t byte : bytes) {
      value = value * 17 + byte;
    }
    return value;
  };
  REQUIRE(state.Apply(initial_snapshot, hash));

  gta4::ConstantPayloadDelta incomplete_delta;
  incomplete_delta.ranges = {{5, 0, 1}};
  incomplete_delta.payload = {4};
  REQUIRE(state.Apply(incomplete_delta, hash));
  std::array<uint8_t, 8> authoritative_snapshot{};
  authoritative_snapshot[3] = 9;
  authoritative_snapshot[5] = 4;
  REQUIRE(gta4::FindFirstConstantMismatch(state.canonical(), authoritative_snapshot) == 3);

  gta4::ConstantPayloadDelta fallback;
  REQUIRE(gta4::CaptureCompleteConstantSnapshot(authoritative_snapshot, fallback));
  const gta4::ConstantApplyResult repaired = state.Apply(fallback, hash);
  REQUIRE(repaired);
  REQUIRE(repaired.changed);
  REQUIRE(gta4::FindFirstConstantMismatch(state.canonical(), authoritative_snapshot) ==
          gta4::kNoSemanticMismatch);
}

TEST_CASE("GTA IV borrowed constant views stay immutable while the command owns its version") {
  gta4::AuthoritativeConstantState state(4);
  const std::array<uint8_t, 4> initial{1, 2, 3, 4};
  gta4::ConstantPayloadDelta snapshot;
  REQUIRE(gta4::CaptureCompleteConstantSnapshot(initial, snapshot));
  const auto hash = [](std::span<const uint8_t>) { return uint64_t(99); };
  const auto first = state.Apply(snapshot, hash).version;
  REQUIRE(first);
  const auto* first_view = gta4::AuthoritativeConstantState::MaterializeView(first);
  REQUIRE(first_view);
  REQUIRE(*first_view == std::vector<uint8_t>{1, 2, 3, 4});
  REQUIRE(gta4::AuthoritativeConstantState::MaterializeView(first) == first_view);
  REQUIRE(first->materialized.use_count() == 1);

  gta4::ConstantPayloadDelta write;
  write.ranges = {{1, 0, 1}};
  write.payload = {8};
  const auto second = state.Apply(write, hash).version;
  REQUIRE(second);
  const auto* second_view = gta4::AuthoritativeConstantState::MaterializeView(second);
  REQUIRE(second_view);
  REQUIRE(*second_view == std::vector<uint8_t>{1, 8, 3, 4});
  REQUIRE(*first_view == std::vector<uint8_t>{1, 2, 3, 4});
  REQUIRE_FALSE(second->parent);
  REQUIRE(gta4::AuthoritativeConstantState::Materialize(first).get() == first_view);
  REQUIRE(gta4::AuthoritativeConstantState::MaterializeView({}) == nullptr);
}

TEST_CASE("GTA IV frame constant arena reserves once per immutable version") {
  gta4::FrameConstantArenaIndex arena;
  REQUIRE(arena.SetByteCapacity(512));
  const gta4::FrameConstantIdentity vertex{
      gta4::FrameConstantKind::kVertex, 0x1000};

  const auto first = arena.FindOrReserve(vertex, 64, 16);
  REQUIRE(first);
  REQUIRE_FALSE(first->reused);
  REQUIRE(first->offset == 0);
  REQUIRE(arena.reservation_count() == 1);
  REQUIRE(arena.bytes_used() == 64);

  const auto reused = arena.FindOrReserve(vertex, 64, 16);
  REQUIRE(reused);
  REQUIRE(reused->reused);
  REQUIRE(reused->offset == first->offset);
  REQUIRE(arena.reservation_count() == 1);
  REQUIRE(arena.bytes_used() == 64);
  // A capacity replacement must be preflighted while the old buffer still
  // owns live offsets. The index refuses rebinding until the slot is reset.
  REQUIRE_FALSE(arena.SetByteCapacity(1024));

  const auto other_kind = arena.FindOrReserve(
      {gta4::FrameConstantKind::kPixel, vertex.immutable_identity}, 32, 16);
  REQUIRE(other_kind);
  REQUIRE_FALSE(other_kind->reused);
  REQUIRE(other_kind->offset == 64);
  const auto aligned = arena.FindOrReserve(
      {gta4::FrameConstantKind::kVertex, 0x2000}, 16, 64);
  REQUIRE(aligned);
  REQUIRE(aligned->offset == 128);
  REQUIRE(arena.bytes_used() == 144);
}

TEST_CASE("GTA IV frame constant arena resets only after its exact fence") {
  gta4::FrameConstantArenaIndex arena;
  REQUIRE(arena.SetByteCapacity(256));
  REQUIRE(arena.FindOrReserve({gta4::FrameConstantKind::kVertex, 1}, 64, 16));
  REQUIRE(arena.MarkSubmitted(7));

  REQUIRE_FALSE(arena.ResetAfterCompletion(6));
  REQUIRE_FALSE(arena.SetByteCapacity(512));
  REQUIRE(arena.in_flight_submission() == 7);
  REQUIRE(arena.reservation_count() == 1);
  REQUIRE(arena.bytes_used() == 64);

  REQUIRE(arena.ResetAfterCompletion(7));
  REQUIRE(arena.in_flight_submission() == 0);
  REQUIRE(arena.reservation_count() == 0);
  REQUIRE(arena.bytes_used() == 0);
  REQUIRE(arena.SetByteCapacity(512));
  const auto next_frame =
      arena.FindOrReserve({gta4::FrameConstantKind::kVertex, 1}, 64, 16);
  REQUIRE(next_frame);
  REQUIRE_FALSE(next_frame->reused);
  REQUIRE(next_frame->offset == 0);
  REQUIRE(arena.ResetUnsubmitted());
}

TEST_CASE("GTA IV high constant churn remains bounded to each frame slot") {
  std::array<gta4::FrameConstantArenaIndex, 2> slots;
  REQUIRE(slots[0].SetByteCapacity(1024));
  REQUIRE(slots[1].SetByteCapacity(1024));
  for (uint64_t identity = 1; identity <= 64; ++identity) {
    const auto reservation =
        slots[0].FindOrReserve({gta4::FrameConstantKind::kShared, identity}, 16, 16);
    REQUIRE(reservation);
    REQUIRE_FALSE(reservation->reused);
  }
  REQUIRE(slots[0].reservation_count() == 64);
  REQUIRE(slots[0].bytes_used() == 1024);
  REQUIRE_FALSE(
      slots[0].FindOrReserve({gta4::FrameConstantKind::kShared, 65}, 16, 16));
  REQUIRE(slots[1].reservation_count() == 0);
  REQUIRE(slots[1].bytes_used() == 0);

  const size_t slot_bucket_high_water = slots[0].lookup_bucket_count();
  REQUIRE(slots[0].MarkSubmitted(9));
  REQUIRE(slots[0].ResetAfterCompletion(9));
  for (uint64_t identity = 101; identity <= 164; ++identity) {
    REQUIRE(slots[0].FindOrReserve(
        {gta4::FrameConstantKind::kShared, identity}, 16, 16));
  }
  REQUIRE(slots[0].lookup_bucket_count() == slot_bucket_high_water);
  REQUIRE(slots[0].reservation_count() == 64);
  REQUIRE(slots[1].lookup_bucket_count() == 0);
}

TEST_CASE("GTA IV shared constant identity includes every semantic input family") {
  using Key = gta4::SharedConstantSemanticKey<2>;
  Key baseline{};
  const auto distinct = [&baseline](const auto& mutate) {
    Key changed = baseline;
    mutate(changed);
    REQUIRE_FALSE(changed == baseline);
  };

  distinct([](Key& key) { key.texture_descriptor_indices[0] = 1; });
  distinct([](Key& key) { key.sampler_descriptor_indices[1] = 1; });
  distinct([](Key& key) { key.sampler_lod_bias_bits[1] = 1; });
  distinct([](Key& key) { key.boolean_version.revision = 1; });
  distinct([](Key& key) { key.image_descriptor_epoch = 1; });
  distinct([](Key& key) { key.sampler_descriptor_epoch = 1; });
  distinct([](Key& key) { key.cached_descriptor_epoch = 1; });
  distinct([](Key& key) { key.descriptor_page = 0; });
  distinct([](Key& key) { key.descriptor_copy = 1; });
  distinct([](Key& key) { key.descriptor_backend = 1; });
  distinct([](Key& key) { key.width = 1; });
  distinct([](Key& key) { key.height = 1; });
  distinct([](Key& key) { key.logical_width = 1; });
  distinct([](Key& key) { key.logical_height = 1; });
  distinct([](Key& key) { key.sample_count = 1; });
  distinct([](Key& key) { key.alpha_reference_bits = 1; });
  distinct([](Key& key) { key.alpha_to_mask = 1; });
  distinct([](Key& key) { key.color_output_info[0] = 1; });
  distinct([](Key& key) { key.color_output_info[3] = 1; });
  distinct([](Key& key) { key.color_output_mask = 1; });
  distinct([](Key& key) { key.clip_plane_bits[0] = 1; });
  distinct([](Key& key) { key.clip_plane_enable_mask = 1; });
  distinct([](Key& key) { key.vertex_booleans = 1; });
  distinct([](Key& key) { key.pixel_booleans = 1; });
  distinct([](Key& key) { key.environmental_data_hash = 1; });
  distinct([](Key& key) { key.environmental_sequence = 1; });
  distinct([](Key& key) { key.environment_present = 1; });
  distinct([](Key& key) { key.device = 1; });
}

TEST_CASE("Native Boolean transport preserves both full shader banks", "[gta4-native][shader-booleans]") {
  CHECK(gta4::PackNativeShaderBooleans(0, 0) == 0);
  CHECK(gta4::PackNativeShaderBooleans(0xFFFFFFFFu, 0xFFFFFFFFu) == 0xFFFFFFFFu);
  CHECK(gta4::PackNativeShaderBooleans(0xFFFF0000u, 0xFFFF0000u) == 0);
  for (uint32_t bit = 0; bit < 16; ++bit) {
    CHECK(gta4::PackNativeShaderBooleans(1u << bit, 0) == (1u << bit));
    CHECK(gta4::PackNativeShaderBooleans(0, 1u << bit) == (1u << (bit + 16)));
  }
  CHECK(gta4::PackNativeShaderBooleans(0x902u, 2u) == 0x00020902u);
}
