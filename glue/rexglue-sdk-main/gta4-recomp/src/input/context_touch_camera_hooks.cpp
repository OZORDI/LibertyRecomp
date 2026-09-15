#include "input/context_touch_camera_hooks.h"

#include "gta4_init.h"
#include "input/context_touch_context.h"
#include "input/context_touch_controls.h"

extern "C" void sub_822B7958(PPCContext& context, uint8_t* base) {
  const auto action = gta4::input::TouchScopedZoomActionForCaller(static_cast<uint32_t>(context.lr));
  // r28 is the control object retained by the admitted native camera caller.
  // Preserve it before calling the original decoder, whose r3/r4 are record
  // contents, not a control pointer.
  const uint32_t control = context.r28.u32;
  __imp__sub_822B7958(context, base);
  constexpr uint32_t kUserOffset = 3412;
  if (action == std::numeric_limits<uint32_t>::max() || !base || !control ||
      control > std::numeric_limits<uint32_t>::max() - kUserOffset - sizeof(uint32_t)) return;
  const uint32_t input_user = __builtin_bswap32(
      *reinterpret_cast<volatile uint32_t*>(base + control + kUserOffset));
  const auto snapshot = gta4::input::GetTouchContextSnapshot();
  context.r3.u64 = static_cast<uint32_t>(gta4::input::MergeTouchScopedZoom(
      snapshot.epoch, input_user, action, static_cast<int32_t>(context.r3.u32)));
}
