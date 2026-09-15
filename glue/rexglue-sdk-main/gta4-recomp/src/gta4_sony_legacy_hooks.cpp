#include "gta4_init.h"
#include "gta4_sony_feedback.h"
#include "gta4_touch_coordinator.h"

// Legacy SDL consumers do not compile gta4_input_hooks.cpp. Keep their
// adapter limited to the same two proven input boundaries.
#if defined(GTA4_SONY_LEGACY_HOST)
extern "C" void sub_828CCD60(PPCContext& context, uint8_t* base) {
#if defined(GTA4_TOUCH_LEGACY_HOST)
  GTA4_TouchConsumePoll(context, base, GTA4_TouchCurrentEpoch() + 1);
#endif
  __imp__sub_828CCD60(context, base);
  GTA4_SonyEndPoll(context, base);
}

extern "C" void sub_822B7DD0(PPCContext& context, uint8_t* base) {
  const uint32_t control = context.r3.u32;
  const uint32_t caller = static_cast<uint32_t>(context.lr);
  __imp__sub_822B7DD0(context, base);
  GTA4_SonyReplay(context, base, control);
#if defined(GTA4_TOUCH_LEGACY_HOST)
  GTA4_TouchObserveControlReplay(context, base, control, caller, GTA4_TouchCurrentEpoch());
#endif
}
#endif
