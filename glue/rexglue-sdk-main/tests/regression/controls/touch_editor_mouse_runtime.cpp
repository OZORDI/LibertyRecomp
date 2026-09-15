#include "input/context_touch_editor_mouse.h"
#include <cassert>
#include <iostream>
#include <vector>

namespace review { bool captures = false, active = false; }
namespace gta4::input {
bool ContextTouchEditorCapturesInput(uint64_t) noexcept { return review::captures; }
bool IsContextTouchEditorActive() noexcept { return review::active; }
}
using namespace rex::input;
using rex::ui::MouseEvent;

std::vector<AbsolutePointerEvent> Drain() {
  std::vector<AbsolutePointerEvent> events;
  AbsolutePointerEvent event;
  while (GetAbsolutePointerService().TryDequeue(&event)) events.push_back(event);
  return events;
}

int main() {
  auto& pointers = GetAbsolutePointerService();
  rex::ui::GuestOutputTransform transform;
  transform.revision = 1;
  transform.surface_width = transform.host_render_target_width = transform.output_width =
      transform.guest_width = 1280;
  transform.surface_height = transform.host_render_target_height = transform.output_height =
      transform.guest_height = 720;
  pointers.SetLogicalSize(1280,720);
  pointers.UpdatePresentation(transform,0,0,1280,720,1);
  pointers.SetFocused(true,2);
  rex::ui::Window window;
  {
    gta4::input::ContextTouchEditorMouse mouse(&window);
    assert(window.input_listeners == 1 && window.listeners == 1 && window.order == 63);
    MouseEvent opening_down(&window,MouseEvent::Button::kLeft,500,300);
    mouse.OnMouseDown(opening_down);
    assert(!opening_down.is_handled() && Drain().empty());
    review::captures = review::active = true;
    MouseEvent opening_up(&window,MouseEvent::Button::kLeft,500,300);
    mouse.OnMouseUp(opening_up);
    assert(!opening_up.is_handled() && Drain().empty());
    std::cout << "PASS editor opening mouse release reaches the original menu driver without becoming a touch\n";

    MouseEvent down(&window,MouseEvent::Button::kLeft,500,300);
    MouseEvent move(&window,MouseEvent::Button::kNone,640,400);
    MouseEvent up(&window,MouseEvent::Button::kLeft,640,400);
    mouse.OnMouseDown(down); mouse.OnMouseMove(move); mouse.OnMouseUp(up);
    auto drag = Drain();
    assert(down.is_handled() && move.is_handled() && up.is_handled() && drag.size() == 3);
    assert(drag[0].phase == AbsolutePointerPhase::kDown && drag[1].phase == AbsolutePointerPhase::kMove &&
           drag[2].phase == AbsolutePointerPhase::kUp);
    assert(drag[0].pointer_id == drag[1].pointer_id && drag[1].pointer_id == drag[2].pointer_id);
    assert(drag[1].x == 640 && drag[1].logical_y == 400);
    std::cout << "PASS physical editor drag preserves pointer identity and exact window coordinates\n";

    mouse.OnMouseDown(down); Drain();
    review::active = false;
    mouse.OnMouseMove(move);
    auto cancelled = Drain();
    assert(cancelled.size() == 1 && cancelled[0].phase == AbsolutePointerPhase::kCancel);
    MouseEvent closing_down(&window,MouseEvent::Button::kRight,100,100);
    mouse.OnMouseDown(closing_down);
    assert(closing_down.is_handled() && Drain().empty());
    review::captures = false;
    MouseEvent closing_up(&window,MouseEvent::Button::kRight,100,100);
    mouse.OnMouseUp(closing_up); mouse.OnMouseUp(up);
    assert(closing_up.is_handled() && Drain().empty());
    MouseEvent normal(&window,MouseEvent::Button::kLeft,100,100);
    mouse.OnMouseDown(normal); mouse.OnMouseUp(normal);
    assert(!normal.is_handled() && Drain().empty());
    std::cout << "PASS closing fade cancels drag and consumes owned releases without changing normal menu mouse input\n";

    review::captures = review::active = true;
    mouse.OnMouseDown(down); Drain();
    rex::ui::UISetupEvent focus(&window);
    mouse.OnLostFocus(focus);
    auto lost = Drain();
    assert(lost.size() == 1 && lost[0].phase == AbsolutePointerPhase::kCancel);
    mouse.OnMouseDown(down); Drain();
    rex::ui::UIEvent close(&window);
    mouse.OnClosing(close);
    auto closed = Drain();
    assert(closed.size() == 1 && closed[0].phase == AbsolutePointerPhase::kCancel);
    assert(window.input_listeners == 0 && window.listeners == 0);
  }
  assert(window.input_listeners == 0 && window.listeners == 0 && Drain().empty());
  std::cout << "PASS editor mouse focus/close cancellation unregisters each listener exactly once\n";
}
