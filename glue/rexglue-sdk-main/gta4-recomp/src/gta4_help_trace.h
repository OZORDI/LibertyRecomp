#pragma once
#include <cstdint>

struct PPCContext;
// Observers only. The existing legal-screen hook retains its touch observation.
void GTA4_HelpTraceTextSubmit(PPCContext& ctx, uint8_t* base,
                             void (*original)(PPCContext&, uint8_t*));
void GTA4_HelpTraceFinalize(uint8_t* base, uint32_t dc, uint32_t caller);
void GTA4_HelpTraceNativeSubmission(uint32_t type, uint32_t device,
                                  bool accepted, bool captured);
