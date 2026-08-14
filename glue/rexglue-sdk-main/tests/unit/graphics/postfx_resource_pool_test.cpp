#include <catch2/catch_test_macros.hpp>

#include "graphics/gta4_native/postfx_resource_pool.h"

namespace rex::graphics::gta4_native {

TEST_CASE("post-FX extents preserve odd edge texels", "[gta4-native][postfx]") {
  CHECK(CalculatePostFxExtent(1280, 720, 2) == (PostFxExtent{640, 360}));
  CHECK(CalculatePostFxExtent(1919, 1079, 2) == (PostFxExtent{960, 540}));
  CHECK(CalculatePostFxExtent(1, 1, 2) == (PostFxExtent{1, 1}));
  CHECK(CalculatePostFxExtent(1280, 720, 0) == (PostFxExtent{}));
}

TEST_CASE("post-FX scheduler captures once inside composite", "[gta4-native][postfx]") {
  PostFxScheduler scheduler;
  scheduler.BeginFrame();
  CHECK_FALSE(scheduler.scene_capture_pending());
  CHECK_FALSE(scheduler.scene_captured());

  scheduler.ObserveMarker(RenderPhase::kLightDraw, RenderPhaseEvent::kBegin);
  CHECK_FALSE(scheduler.scene_capture_pending());

  scheduler.ObserveMarker(RenderPhase::kCompositePostFx, RenderPhaseEvent::kBegin);
  CHECK(scheduler.scene_capture_pending());
  scheduler.FinishSceneCapture(true);
  CHECK_FALSE(scheduler.scene_capture_pending());
  CHECK(scheduler.scene_captured());

  scheduler.FinishSceneCapture(false);
  CHECK(scheduler.scene_captured());
  scheduler.ObserveMarker(RenderPhase::kCompositePostFx, RenderPhaseEvent::kEnd);
  CHECK_FALSE(scheduler.scene_capture_pending());
}

TEST_CASE("post-FX scheduler records an unavailable source without retry",
          "[gta4-native][postfx]") {
  PostFxScheduler scheduler;
  scheduler.BeginFrame();
  scheduler.ObserveMarker(RenderPhase::kCompositePostFx, RenderPhaseEvent::kBegin);
  scheduler.FinishSceneCapture(false);
  CHECK_FALSE(scheduler.scene_capture_pending());
  CHECK_FALSE(scheduler.scene_captured());
}

}  // namespace rex::graphics::gta4_native
