#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace gta4::quicksave {
// Decoded installed SCO SHA-256, not the linked/mutated native operand stream.
// Derived from executable scripts and generated .60:sub_828453F8/82843700.
struct PhoneProfile {
  std::string_view name, sha256;
  uint32_t episode, code_size, local_count;
  uint32_t menu_local, options_local, phone_global, append_args;
  uint32_t append_site, accept_site, append_function, set_state_function, accept_exit;
};
inline constexpr std::array kPhoneProfiles = {
    PhoneProfile{"GTA IV", "9ab320e5b78782cefc259fb885b391a6d6db5113e06a28cf307ca1feda09b12b", 0,
                 45292, 1017, 0, 213, 18, 8, 0x2589, 0x9F96, 0x2340, 0xE21, 0xA13F},
    PhoneProfile{"TLAD", "b877756795e7e9ac594369e1b77ddeb4ab9a909f23d8d8090833521382b9002e", 1,
                 48422, 1034, 3, 216, 21, 8, 0x25C8, 0xA1AE, 0x236C, 0xE3D, 0xA6A0},
    PhoneProfile{"TBoGT", "bc0146ac8a57ed7303698506d5c19991d5feadea262705320df456add4b62719", 2,
                 49906, 1034, 3, 216, 22, 10, 0x26DE, 0xA890, 0x23A5, 0xE69, 0xABDC}};
inline constexpr uint32_t kPhoneProgramKey = 0xF1D7116F;
inline constexpr int32_t kQuicksaveAction = 0x7F01;
inline constexpr std::string_view kQuicksaveLabel = "LR_QSAVE";
inline constexpr uint32_t kRootPhoneState = 1011, kClosingPhoneState = 1001,
                          kClosedPhoneState = 1000;
inline constexpr uint32_t kOptionCapacity = 40, kOptionWords = 16;
inline const PhoneProfile* MatchPhoneProfile(uint32_t size, std::string_view digest) {
  for (const auto& p : kPhoneProfiles)
    if (p.code_size == size && p.sha256 == digest)
      return &p;
  return nullptr;
}
struct PhonePatch {
  std::vector<uint8_t> code;
  uint32_t append_extension = 0, accept_extension = 0;
};
namespace bytecode {
constexpr uint8_t kAdd = 1, kEqual = 8, kJump = 34, kJumpFalse = 35, kPushS = 40, kPush = 41,
                  kNative = 45, kCall = 46, kRefGet = 49, kLocal = 63, kString = 67;
inline uint32_t Read32(std::span<const uint8_t> v, size_t at) {
  if (at > v.size() || v.size() - at < 4)
    throw std::runtime_error("SCO operand out of bounds");
  return uint32_t(v[at]) | (uint32_t(v[at + 1]) << 8) | (uint32_t(v[at + 2]) << 16) |
         (uint32_t(v[at + 3]) << 24);
}
inline void Write32(std::vector<uint8_t>& v, size_t at, uint32_t value) {
  if (at > v.size() || v.size() - at < 4)
    throw std::runtime_error("SCO write out of bounds");
  for (unsigned i = 0; i < 4; ++i)
    v[at + i] = uint8_t(value >> (i * 8));
}
inline void Append32(std::vector<uint8_t>& v, uint32_t value) {
  for (unsigned i = 0; i < 4; ++i)
    v.push_back(uint8_t(value >> (i * 8)));
}
inline void Int(std::vector<uint8_t>& v, int32_t value) {
  if (value >= -16 && value <= 159) {
    v.push_back(uint8_t(value + 96));
    return;
  }
  if (value >= -32768 && value <= 32767) {
    v.push_back(kPushS);
    v.push_back(uint8_t(value));
    v.push_back(uint8_t(uint32_t(value) >> 8));
    return;
  }
  v.push_back(kPush);
  Append32(v, uint32_t(value));
}
inline void Local(std::vector<uint8_t>& v, uint32_t index) {
  Int(v, int32_t(index));
  v.push_back(kLocal);
}
inline void String(std::vector<uint8_t>& v, std::string_view text) {
  if (text.size() > 253)
    throw std::runtime_error("SCO string is too long");
  v.push_back(kString);
  v.push_back(uint8_t(text.size() + 1));
  v.insert(v.end(), text.begin(), text.end());
  v.push_back(0);
}
inline size_t Branch(std::vector<uint8_t>& v, uint8_t op, uint32_t target) {
  const size_t at = v.size();
  v.push_back(op);
  Append32(v, target);
  return at;
}
inline void Native(std::vector<uint8_t>& v, uint32_t linked_thunk, uint8_t arguments,
                   uint8_t returns) {
  if (!linked_thunk)
    throw std::runtime_error("Unallocated phone native thunk");
  v.push_back(kNative);
  v.push_back(arguments);
  v.push_back(returns);
  Append32(v, linked_thunk);
}
}  // namespace bytecode

inline PhonePatch BuildPhonePatch(std::span<const uint8_t> linked, const PhoneProfile& p,
                                  uint32_t request_thunk, uint32_t observe_thunk) {
  using namespace bytecode;
  const auto fits = [&](uint32_t offset, size_t size) {
    return offset <= linked.size() && size <= linked.size() - offset;
  };
  if (linked.size() != p.code_size || p.episode >= kPhoneProfiles.size() ||
      !fits(p.append_site, 5) || !fits(p.accept_site, 5) || !fits(p.append_function, 4) ||
      !fits(p.set_state_function, 4) || !fits(p.accept_exit, 5) ||
      p.menu_local > 159 || p.options_local > 32767 ||
      (p.append_args != 8 && p.append_args != 10))
    throw std::runtime_error("Wrong phone program size or profile bounds");
  const std::array<uint8_t, 5> append_expected = {uint8_t(p.menu_local + 96), kLocal, kPushS,
                                                  uint8_t(p.options_local),
                                                  uint8_t(p.options_local >> 8)};
  const std::array<uint8_t, 5> accept_expected = {uint8_t(p.menu_local + 96), kLocal, kPushS, 60,
                                                  0};
  if (!std::equal(append_expected.begin(), append_expected.end(), linked.begin() + p.append_site) ||
      !std::equal(accept_expected.begin(), accept_expected.end(), linked.begin() + p.accept_site) ||
      linked[p.append_function] != 47 || linked[p.append_function + 1] != p.append_args ||
      linked[p.set_state_function] != 47 || linked[p.set_state_function + 1] != 1 ||
      linked[p.accept_exit] != kJump)
    throw std::runtime_error("Phone patch site contract mismatch");
  PhonePatch out;
  out.code.assign(linked.begin(), linked.end());
  auto& v = out.code;
  v.reserve(linked.size() + 256);
  out.append_extension = uint32_t(v.size());
  Local(v, p.menu_local);
  Local(v, p.options_local);
  Int(v, kQuicksaveAction);
  String(v, kQuicksaveLabel);
  String(v, "");
  Int(v, 1);
  Int(v, 0);
  Int(v, 0);
  if (p.append_args == 10) {
    Int(v, 0);
    Int(v, 0);
  }
  Branch(v, kCall, p.append_function);
  Int(v, int32_t(p.episode));
  Native(v, observe_thunk, 1, 0);
  v.insert(v.end(), append_expected.begin(), append_expected.end());
  Branch(v, kJump, p.append_site + 5);
  out.accept_extension = uint32_t(v.size());
  Local(v, p.menu_local);
  Int(v, 60);
  v.push_back(kAdd);
  v.push_back(kRefGet);
  Int(v, kQuicksaveAction);
  v.push_back(kEqual);
  const size_t unchanged = Branch(v, kJumpFalse, 0);
  Int(v, int32_t(p.episode));
  Native(v, request_thunk, 1, 1);
  Branch(v, kJumpFalse, p.accept_exit);
  Int(v, int32_t(kClosingPhoneState));
  Branch(v, kCall, p.set_state_function);
  Branch(v, kJump, p.accept_exit);
  Write32(v, unchanged + 1, uint32_t(v.size()));
  v.insert(v.end(), accept_expected.begin(), accept_expected.end());
  Branch(v, kJump, p.accept_site + 5);
  v[p.append_site] = kJump;
  Write32(v, p.append_site + 1, out.append_extension);
  v[p.accept_site] = kJump;
  Write32(v, p.accept_site + 1, out.accept_extension);
  return out;
}
}  // namespace gta4::quicksave
