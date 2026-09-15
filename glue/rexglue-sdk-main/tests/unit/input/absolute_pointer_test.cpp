#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <vector>

#include <rex/input/absolute_pointer.h>

namespace {

using Catch::Approx;
using rex::input::AbsolutePointerEvent;
using rex::input::AbsolutePointerPhase;
using rex::input::AbsolutePointerService;
using rex::input::ShouldEnableTouchControls;
using rex::input::TouchControlsMode;
using rex::input::TouchPresentationState;

rex::ui::GuestOutputTransform MakeTransform(uint64_t revision = 1) {
  rex::ui::GuestOutputTransform transform;
  transform.revision = revision;
  transform.surface_width = 1920;
  transform.surface_height = 1080;
  transform.host_render_target_width = 1920;
  transform.host_render_target_height = 1080;
  transform.output_x = 240;
  transform.output_y = 0;
  transform.output_width = 1440;
  transform.output_height = 1080;
  transform.guest_width = 1280;
  transform.guest_height = 720;
  return transform;
}

std::vector<AbsolutePointerEvent> Drain(AbsolutePointerService& service) {
  std::vector<AbsolutePointerEvent> events;
  AbsolutePointerEvent event;
  while (service.TryDequeue(&event)) {
    events.push_back(event);
  }
  return events;
}

void Prepare(AbsolutePointerService& service) {
  service.UpdatePresentation(MakeTransform(), 0, 0, 1920, 1080, 1);
  service.SetFocused(true, 2);
}

TEST_CASE("absolute pointers retain identity and use guest output pixels",
          "[input][absolute_pointer]") {
  AbsolutePointerService service;
  Prepare(service);

  service.SubmitPointer(11, 101, AbsolutePointerPhase::kDown, 240.0f, 0.0f, 0.5f, 3);
  service.SubmitPointer(11, 202, AbsolutePointerPhase::kDown, 960.0f, 540.0f, 0.75f, 4);
  service.SubmitPointer(11, 101, AbsolutePointerPhase::kMove, 0.0f, 1080.0f, 0.25f, 5);
  service.SubmitPointer(11, 101, AbsolutePointerPhase::kUp, 1680.0f, 1080.0f, 0.0f, 6);
  service.SubmitPointer(11, 202, AbsolutePointerPhase::kCancel, 960.0f, 540.0f, 0.0f, 7);

  const auto events = Drain(service);
  REQUIRE(events.size() == 5);
  CHECK(events[0].pointer_id == events[2].pointer_id);
  CHECK(events[0].pointer_id == events[3].pointer_id);
  CHECK(events[1].pointer_id == events[4].pointer_id);
  CHECK(events[0].pointer_id != events[1].pointer_id);
  for (size_t i = 1; i < events.size(); ++i) {
    CHECK(events[i].sequence > events[i - 1].sequence);
    CHECK(events[i].generation == events[0].generation);
    CHECK(events[i].output_width == 1280.0f);
    CHECK(events[i].output_height == 720.0f);
  }
  CHECK(events[0].x == 0.0f);
  CHECK(events[1].x == 640.0f);
  CHECK(events[1].y == 360.0f);
  CHECK(events[2].x == Approx(-213.333333f));
  CHECK(events[2].y == 720.0f);
  CHECK(events[3].x == 1280.0f);
  CHECK(events[3].phase == AbsolutePointerPhase::kUp);
  CHECK(events[4].phase == AbsolutePointerPhase::kCancel);
}

TEST_CASE("absolute pointer move pressure is bounded without dropping lifecycle edges",
          "[input][absolute_pointer]") {
  AbsolutePointerService service(2);
  Prepare(service);

  service.SubmitPointer(1, 1, AbsolutePointerPhase::kDown, 240.0f, 0.0f, 1.0f, 3);
  service.SubmitPointer(1, 2, AbsolutePointerPhase::kDown, 240.0f, 0.0f, 1.0f, 4);
  service.SubmitPointer(1, 3, AbsolutePointerPhase::kDown, 240.0f, 0.0f, 1.0f, 5);
  service.SubmitPointer(1, 1, AbsolutePointerPhase::kMove, 300.0f, 10.0f, 1.0f, 6);
  service.SubmitPointer(1, 1, AbsolutePointerPhase::kMove, 400.0f, 20.0f, 1.0f, 7);
  service.SubmitPointer(1, 2, AbsolutePointerPhase::kMove, 500.0f, 30.0f, 1.0f, 8);
  service.SubmitPointer(1, 3, AbsolutePointerPhase::kMove, 600.0f, 40.0f, 1.0f, 9);
  service.SubmitPointer(1, 1, AbsolutePointerPhase::kUp, 400.0f, 20.0f, 0.0f, 10);
  service.SubmitPointer(1, 2, AbsolutePointerPhase::kCancel, 500.0f, 30.0f, 0.0f, 11);
  service.SubmitPointer(1, 3, AbsolutePointerPhase::kUp, 600.0f, 40.0f, 0.0f, 12);

  CHECK(service.queued_move_count() == 2);
  const auto events = Drain(service);
  size_t down_count = 0;
  size_t terminal_count = 0;
  size_t move_count = 0;
  for (const auto& event : events) {
    if (event.phase == AbsolutePointerPhase::kDown) {
      ++down_count;
    } else if (event.phase == AbsolutePointerPhase::kMove) {
      ++move_count;
    } else {
      ++terminal_count;
    }
  }
  CHECK(down_count == 3);
  CHECK(move_count == 2);
  CHECK(terminal_count == 3);
}

TEST_CASE("presentation and focus resets cancel active pointers before generation advances",
          "[input][absolute_pointer]") {
  AbsolutePointerService service;
  Prepare(service);
  service.SubmitPointer(1, 1, AbsolutePointerPhase::kDown, 960.0f, 540.0f, 1.0f, 3);
  const auto down = Drain(service);
  REQUIRE(down.size() == 1);

  auto changed = MakeTransform(2);
  changed.output_x = 0;
  changed.output_width = 1920;
  service.UpdatePresentation(changed, 0, 0, 1920, 1080, 4);
  auto reset_events = Drain(service);
  REQUIRE(reset_events.size() == 1);
  CHECK(reset_events[0].phase == AbsolutePointerPhase::kCancel);
  CHECK(reset_events[0].generation == down[0].generation);

  service.SubmitPointer(1, 1, AbsolutePointerPhase::kDown, 960.0f, 540.0f, 1.0f, 5);
  auto next_down = Drain(service);
  REQUIRE(next_down.size() == 1);
  CHECK(next_down[0].generation > reset_events[0].generation);
  service.SetFocused(false, 6);
  auto focus_events = Drain(service);
  REQUIRE(focus_events.size() == 1);
  CHECK(focus_events[0].phase == AbsolutePointerPhase::kCancel);
  CHECK_FALSE(service.TouchControlsActive(TouchControlsMode::kOn));
}

TEST_CASE("touch presentation exposes safe area and reverse physical transform",
          "[input][absolute_pointer]") {
  AbsolutePointerService service;
  service.UpdatePresentation(MakeTransform(), 300, 60, 1320, 960, 1);
  service.SetFocused(true, 2);

  TouchPresentationState state;
  REQUIRE(service.GetPresentationState(&state));
  CHECK(state.output_width == 1280.0f);
  CHECK(state.output_height == 720.0f);
  CHECK(state.physical_output_x == 240.0f);
  CHECK(state.physical_output_y == 0.0f);
  CHECK(state.physical_output_width == 1440.0f);
  CHECK(state.physical_output_height == 1080.0f);
  CHECK(state.physical_surface_width == 1920.0f);
  CHECK(state.physical_surface_height == 1080.0f);
  CHECK(state.safe_area_x == 54);
  CHECK(state.safe_area_y == 40);
  CHECK(state.safe_area_width == 1172);
  CHECK(state.safe_area_height == 640);
}

TEST_CASE("touch auto requires no physical input devices while On overrides presence",
          "[input][absolute_pointer]") {
  CHECK(ShouldEnableTouchControls(TouchControlsMode::kAuto, false, false, true));
  CHECK_FALSE(ShouldEnableTouchControls(TouchControlsMode::kAuto, true, false, true));
  CHECK_FALSE(ShouldEnableTouchControls(TouchControlsMode::kAuto, false, true, true));
  CHECK_FALSE(ShouldEnableTouchControls(TouchControlsMode::kAuto, false, false, true, true));
  CHECK_FALSE(ShouldEnableTouchControls(TouchControlsMode::kAuto, false, false, false));
  CHECK(ShouldEnableTouchControls(TouchControlsMode::kOn, true, true, true));
  CHECK_FALSE(ShouldEnableTouchControls(TouchControlsMode::kOff, false, false, true));

  AbsolutePointerService service;
  service.SetFocused(true, 1);
  CHECK(service.TouchControlsActive(TouchControlsMode::kAuto));
  service.AddPhysicalKeyboard(7, 2);
  CHECK(service.HasPhysicalKeyboard());
  CHECK_FALSE(service.TouchControlsActive(TouchControlsMode::kAuto));
  service.RemovePhysicalKeyboard(7, 3);
  service.AddGameController(8, 4);
  CHECK(service.HasGameController());
  CHECK_FALSE(service.TouchControlsActive(TouchControlsMode::kAuto));
  CHECK(service.TouchControlsActive(TouchControlsMode::kOn));
  service.RemoveGameController(8,5);
  service.AddPhysicalMouse(9,6);
  CHECK(service.HasPhysicalMouse());
  CHECK_FALSE(service.TouchControlsActive(TouchControlsMode::kAuto));
  service.RemovePhysicalMouse(9,7);
  CHECK(service.TouchControlsActive(TouchControlsMode::kAuto));
}

}  // namespace
