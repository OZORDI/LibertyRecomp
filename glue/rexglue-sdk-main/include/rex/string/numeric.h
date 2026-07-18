/**
 * @file        rex/string/numeric.h
 * @brief       Hex formatters and string-to-numeric parsers.
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 * @license     BSD 3-Clause License
 *
 * @remarks     Derived from Xenia's xenia/base/string_util.h (Ben Vanik, 2020).
 */

#pragma once

#include <charconv>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

#include <fmt/format.h>

#include <rex/assert.h>
#include <rex/string/utf8.h>
#include <rex/vec128.h>

namespace rex::string {

inline std::string to_hex_string(uint32_t value) {
  return fmt::format("{:08X}", value);
}

inline std::string to_hex_string(uint64_t value) {
  return fmt::format("{:016X}", value);
}

inline std::string to_hex_string(float value) {
  static_assert(sizeof(uint32_t) == sizeof(value));
  uint32_t pun;
  std::memcpy(&pun, &value, sizeof(value));
  return to_hex_string(pun);
}

inline std::string to_hex_string(double value) {
  static_assert(sizeof(uint64_t) == sizeof(value));
  uint64_t pun;
  std::memcpy(&pun, &value, sizeof(value));
  return to_hex_string(pun);
}

inline std::string to_hex_string(const vec128_t& value) {
  return fmt::format("[{:08X} {:08X} {:08X} {:08X}]", value.u32[0], value.u32[1], value.u32[2],
                     value.u32[3]);
}

template <typename T>
inline T from_string(const std::string_view value, bool force_hex = false) {
  (void)value;
  (void)force_hex;
  // Missing implementation for converting type T from string
  throw;
}

namespace detail {

template <typename T, typename V = std::make_signed_t<T>>
inline T make_negative(T value) {
  if constexpr (std::is_unsigned_v<T>) {
    value = static_cast<T>(-static_cast<V>(value));
  } else {
    value = -value;
  }
  return value;
}

// integral_from_string
template <typename T>
inline T ifs(const std::string_view value, bool force_hex) {
  int base = 10;
  std::string_view range = value;
  bool is_hex = force_hex;
  bool is_negative = false;
  if (rex::string::utf8_starts_with(range, "-")) {
    is_negative = true;
    range = range.substr(1);
  }
  if (rex::string::utf8_starts_with(range, "0x")) {
    is_hex = true;
    range = range.substr(2);
  }
  if (rex::string::utf8_ends_with(range, "h")) {
    is_hex = true;
    range = range.substr(0, range.length() - 1);
  }
  T result;
  if (is_hex) {
    base = 16;
  }
  // TODO(gibbed): do something more with errors?
  auto [p, error] = std::from_chars(range.data(), range.data() + range.size(), result, base);
  if (error != std::errc()) {
    assert_always();
    return T();
  }
  if (is_negative) {
    result = make_negative(result);
  }
  return result;
}

// floating_point_from_string
template <typename T, typename PUN>
inline T fpfs(const std::string_view value, bool force_hex) {
  static_assert(sizeof(T) == sizeof(PUN));
  std::string_view range = value;
  bool is_hex = force_hex;
  bool is_negative = false;
  if (rex::string::utf8_starts_with(range, "-")) {
    is_negative = true;
    range = range.substr(1);
  }
  if (rex::string::utf8_starts_with(range, "0x")) {
    is_hex = true;
    range = range.substr(2);
  }
  if (rex::string::utf8_ends_with(range, "h")) {
    is_hex = true;
    range = range.substr(0, range.length() - 1);
  }
  T result;
  if (is_hex) {
    PUN pun = from_string<PUN>(range, true);
    if (is_negative) {
      pun = make_negative(pun);
    }
    std::memcpy(&result, &pun, sizeof(PUN));
  } else {
    auto [p, error] = std::from_chars(range.data(), range.data() + range.size(), result,
                                      std::chars_format::general);
    // TODO(gibbed): do something more with errors?
    if (error != std::errc()) {
      assert_always();
      return T();
    }
    if (is_negative) {
      result = -result;
    }
  }
  return result;
}

}  // namespace detail

template <>
inline bool from_string<bool>(const std::string_view value, bool force_hex) {
  (void)force_hex;
  return value == "true" || value == "1";
}

template <>
inline int8_t from_string<int8_t>(const std::string_view value, bool force_hex) {
  return detail::ifs<int8_t>(value, force_hex);
}

template <>
inline uint8_t from_string<uint8_t>(const std::string_view value, bool force_hex) {
  return detail::ifs<uint8_t>(value, force_hex);
}

template <>
inline int16_t from_string<int16_t>(const std::string_view value, bool force_hex) {
  return detail::ifs<int16_t>(value, force_hex);
}

template <>
inline uint16_t from_string<uint16_t>(const std::string_view value, bool force_hex) {
  return detail::ifs<uint16_t>(value, force_hex);
}

template <>
inline int32_t from_string<int32_t>(const std::string_view value, bool force_hex) {
  return detail::ifs<int32_t>(value, force_hex);
}

template <>
inline uint32_t from_string<uint32_t>(const std::string_view value, bool force_hex) {
  return detail::ifs<uint32_t>(value, force_hex);
}

template <>
inline int64_t from_string<int64_t>(const std::string_view value, bool force_hex) {
  return detail::ifs<int64_t>(value, force_hex);
}

template <>
inline uint64_t from_string<uint64_t>(const std::string_view value, bool force_hex) {
  return detail::ifs<uint64_t>(value, force_hex);
}

template <>
inline float from_string<float>(const std::string_view value, bool force_hex) {
  return detail::fpfs<float, uint32_t>(value, force_hex);
}

template <>
inline double from_string<double>(const std::string_view value, bool force_hex) {
  return detail::fpfs<double, uint64_t>(value, force_hex);
}

template <>
inline vec128_t from_string<vec128_t>(const std::string_view value, bool force_hex) {
  if (!value.size()) {
    return vec128_t();
  }
  vec128_t v;
  auto p = value.data();
  auto end = value.data() + value.size();
  bool is_hex = force_hex;
  if (p != end && *p == '[') {
    is_hex = true;
    ++p;
  } else if (p != end && *p == '(') {
    is_hex = false;
    ++p;
  } else {
    // Assume hex?
    is_hex = true;
  }
  if (p == end) {
    assert_always();
    return vec128_t();
  }
  if (is_hex) {
    for (size_t i = 0; i < 4; i++) {
      while (p != end && (*p == ' ' || *p == ',')) {
        ++p;
      }
      if (p == end) {
        assert_always();
        return vec128_t();
      }
      auto result = std::from_chars(p, end, v.u32[i], 16);
      if (result.ec != std::errc()) {
        assert_always();
        return vec128_t();
      }
      p = result.ptr;
    }
  } else {
    for (size_t i = 0; i < 4; i++) {
      while (p != end && (*p == ' ' || *p == ',')) {
        ++p;
      }
      if (p == end) {
        assert_always();
        return vec128_t();
      }
      auto result = std::from_chars(p, end, v.f32[i], std::chars_format::general);
      if (result.ec != std::errc()) {
        assert_always();
        return vec128_t();
      }
      p = result.ptr;
    }
  }
  return v;
}

}  // namespace rex::string
