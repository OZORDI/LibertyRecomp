#define REX_FIRE_TRACE_TEST
#include <rex/graphics/gta4_native/fire_escape_trace.h>
#include "graphics/gta4_native/native_probe_region.h"
#include <array>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
using namespace rex::graphics::gta4_native;
int main(int argc, char**) {
  const bool rail_mode = argc > 1;
  if (rail_mode) setenv("REX_GTA4_RAIL_TRACE", "1", 1);
  else unsetenv("REX_GTA4_RAIL_TRACE");
  assert(ElevatedRailTextureName("ks_eltrak01"));
  assert(ElevatedRailTextureName("bks_tracksbxe_tracks_a"));
  assert(ElevatedRailTextureName("grn_railks03grn_railks03a"));
  assert(ElevatedRailTextureName("cm_el_girder"));
  assert(!ElevatedRailTextureName("darkmetal512"));
  assert(!ElevatedRailTextureName("sl_rustedmtl_rail01"));
  assert(!ElevatedRailTextureName("hair_diff_000"));
  assert(!ElevatedRailTextureName(""));
  if (rail_mode) {
    assert(FireStrongTextureName("ks_eltrak02"));
    assert(FireTextureName("darkmetal512"));
    assert(!FireStrongTextureName("darkmetal512"));
    assert(!FireTextureName("sl_rustedmtl_rail01"));
    assert(!FireTextureName("hair_diff_000"));
  } else {
    assert(FireStrongTextureName("sl_rustedmtl_rail01"));
    assert(!FireTextureName("ks_eltrak02"));
    assert(!FireTextureName("darkmetal512"));
  }
  const auto full = MakeNativeProbeRegion(3456, 2234);
  assert(full && *full == (NativeProbeRegion{0, 0, 3456, 2234}));
  const auto crop = MakeNativeProbeRegion(3456, 2234, 0, 0, 1024, 768);
  assert(crop && *crop == (NativeProbeRegion{0, 0, 1024, 768}));
  const auto edge = MakeNativeProbeRegion(1280, 720, 1200, 700, 1024, 768);
  assert(edge && edge->x == 1200 && edge->y == 700);
  assert(edge->width == 1280u - 1200u && edge->height == 720u - 700u);
  assert(!MakeNativeProbeRegion(0, 720));
  assert(!MakeNativeProbeRegion(1280, 0));
  assert(!MakeNativeProbeRegion(1280, 720, 1280, 0, 1, 1));
  assert(!MakeNativeProbeRegion(1280, 720, 0, 720, 1, 1));
  assert(!MakeNativeProbeRegion(1280, 720, UINT32_MAX, UINT32_MAX, 1, 1));
  const auto huge = MakeNativeProbeRegion(UINT32_MAX, UINT32_MAX, 1, 1, UINT32_MAX, UINT32_MAX);
  assert(huge && huge->width == UINT32_MAX - 1u && huge->height == UINT32_MAX - 1u);
  // A rail tag must remain side metadata: command bytes round-trip unchanged.
  const std::array<uint32_t, 6> draw{1, 2, 3, 4, 5, 6};
  FireTraceContext c; c.event = 13; c.occurrence = 8;
  std::memcpy(c.texture_name.data(), "ks_eltrak01", sizeof("ks_eltrak01"));
  const auto packed = PackFireTraceEnvelope(draw.data(), sizeof(draw), 31, c);
  const void* bytes = packed.data(); size_t size = packed.size();
  uint32_t abi = kFireTraceEnvelopeAbi; FireTraceContext read;
  assert(UnpackFireTraceEnvelope(bytes, size, abi, read));
  assert(abi == 31 && size == sizeof(draw) && std::memcmp(bytes, draw.data(), sizeof(draw)) == 0);
  assert(read.event == c.event && read.occurrence == c.occurrence && read.texture_name == c.texture_name);
  std::cout << "PASS rail selection, disabled-path compatibility, crop bounds and command preservation; rail-mode=" << rail_mode << '\n';
}
