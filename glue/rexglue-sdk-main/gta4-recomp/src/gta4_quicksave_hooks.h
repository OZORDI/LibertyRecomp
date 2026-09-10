#pragma once
#include <cstdint>
struct PPCContext;
namespace gta4::quicksave {
// Called by the single existing native registrar / text resolver owners.
void ObserveNativeRegistration(PPCContext& context, uint8_t* base);
bool ResolveText(PPCContext& context, uint8_t* base);
}  // namespace gta4::quicksave
