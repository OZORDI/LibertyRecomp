#define REX_FIRE_TRACE_TEST
#include <rex/graphics/gta4_native/fire_escape_trace.h>
#include <cassert>
#include <iostream>
#include <limits>
using namespace rex::graphics::gta4_native;
int main() {
  std::array<uint32_t, 12> original{48, 16, 0x831C22A4, 0x12345678, 1, 2, 3, 4, 5, 6, 7, 8};
  const auto before = original;
  FireTraceContext context;
  context.occurrence = 3; context.event = 7; context.model = 0x90001000;
  context.material = 0x90002000; context.replay = 1; context.command_list = 0x90003000;
  std::memcpy(context.texture_name.data(), "sl_rustedmtl_rail01", sizeof("sl_rustedmtl_rail01"));
  const auto bytes = PackFireTraceEnvelope(original.data(), sizeof(original), 31, context);
  assert(!bytes.empty() && original == before);
  const void* command = bytes.data(); size_t size = bytes.size(); uint32_t abi = kFireTraceEnvelopeAbi;
  FireTraceContext unpacked;
  assert(UnpackFireTraceEnvelope(command, size, abi, unpacked));
  assert(abi == 31 && size == sizeof(original));
  assert(std::memcmp(command, original.data(), sizeof(original)) == 0);
  assert(unpacked.occurrence == context.occurrence && unpacked.event == context.event);
  assert(unpacked.model == context.model && unpacked.material == context.material);
  assert(unpacked.replay == context.replay && unpacked.command_list == context.command_list);
  assert(unpacked.texture_name == context.texture_name);
  std::cout << "PASS intact command bytes, ABI, identity and replay metadata\n";
  auto rejected = [&](const std::vector<uint8_t>& buffer, uint32_t tag) {
    const void* p = buffer.data(); size_t n = buffer.size(); FireTraceContext out;
    const auto old_p = p; const auto old_n = n; const auto old_tag = tag;
    const bool ok = UnpackFireTraceEnvelope(p, n, tag, out);
    assert(!ok && p == old_p && n == old_n && tag == old_tag);
  };
  for (size_t n = 0; n < bytes.size(); ++n) {
    std::vector<uint8_t> truncated(bytes.begin(), bytes.begin() + n);
    rejected(truncated, kFireTraceEnvelopeAbi);
  }
  rejected(bytes, 31);
  for (unsigned which = 0; which < 7; ++which) {
    auto corrupt = bytes; FireTraceEnvelope envelope;
    std::memcpy(&envelope, corrupt.data(), sizeof(envelope));
    switch (which) {
      case 0: envelope.version = 2; break;
      case 1: envelope.reserved = 1; break;
      case 2: envelope.context.event = 0; break;
      case 3: envelope.context.occurrence = 0; break;
      case 4: envelope.command_size = 0; break;
      case 5: envelope.command_abi = kFireTraceEnvelopeAbi; break;
      case 6: envelope.context.texture_name.back() = 'x'; break;
    }
    std::memcpy(corrupt.data(), &envelope, sizeof(envelope)); rejected(corrupt, kFireTraceEnvelopeAbi);
  }
  assert(PackFireTraceEnvelope(nullptr, sizeof(original), 31, context).empty());
  assert(PackFireTraceEnvelope(original.data(), 0, 31, context).empty());
  assert(PackFireTraceEnvelope(original.data(), sizeof(original), kFireTraceEnvelopeAbi, context).empty());
  assert(PackFireTraceEnvelope(original.data(), sizeof(original), 31, FireTraceContext{}).empty());
  assert(PackFireTraceEnvelope(original.data(), std::numeric_limits<size_t>::max(), 31, context).empty());
  std::cout << "PASS malformed, nested and truncated envelopes rejected without changing inputs\n";
  assert(FireStrongTextureName("pack:/sl_rustedmtl_rail01sl_rustedmtl_rail01a.dds"));
  assert(FireStrongTextureName("sl_rustedmtl_msh01sl_rustedmtl_msh01a"));
  assert(!FireStrongTextureName("sl_rustedmetal01_256"));
  assert(!FireStrongTextureName("unrelated_reflective_metal"));
  assert(!FireStrongTextureName(""));
  std::cout << "PASS strong material trigger excludes generic rusted metal\n";
}
