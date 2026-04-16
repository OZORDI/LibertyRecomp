/**
 * macOS / Homebrew libc++ compatibility shims for C++23 features missing from
 * all currently available Homebrew libc++ builds (llvm@18 through llvm@21).
 *
 * Injected features:
 *  - std::move_only_function<Sig>          (P0288R9, C++23)
 *  - std::chrono::clock_time_conversion    (P0355R7, C++20 — absent from
 *                                           Homebrew libc++ on macOS)
 *
 * These shims are guarded by __APPLE__ so they compile away entirely on
 * Windows / Linux where the standard implementations are available.
 */

#pragma once

#if defined(__APPLE__) || defined(__ORBIS__) || defined(LIBERTY_RECOMP_PS4) || \
    defined(__SWITCH__) || defined(LIBERTY_RECOMP_NX)

// ──────────────────────────────────────────────────────────────────────────────
// 1.  std::move_only_function<R(Args...)>
//
//     std::function requires the callable to be CopyConstructible.
//     std::move_only_function relaxes that — it is itself move-only and
//     accepts move-only callables.  We implement a minimal but spec-conformant
//     subset sufficient for the uses in the rex SDK.
// ──────────────────────────────────────────────────────────────────────────────
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#if !defined(__cpp_lib_move_only_function)

namespace std {

template <typename Sig>
class move_only_function;

// Specialisation for non-const, non-noexcept signatures: R(Args...)
template <typename R, typename... Args>
class move_only_function<R(Args...)> {
 private:
  // ---- type-erased callable storage ----
  struct _Base {
    virtual ~_Base() = default;
    virtual R _invoke(Args&&... args) = 0;
  };

  template <typename F>
  struct _Holder final : _Base {
    F _f;
    template <typename Fwd>
    explicit _Holder(Fwd&& f) : _f(std::forward<Fwd>(f)) {}
    R _invoke(Args&&... args) override {
      return static_cast<R>(_f(std::forward<Args>(args)...));
    }
  };

  std::unique_ptr<_Base> _ptr;

 public:
  // ---- constructors ----
  move_only_function() noexcept = default;
  move_only_function(std::nullptr_t) noexcept {}  // NOLINT

  template <typename F,
            typename = std::enable_if_t<
                !std::is_same_v<std::decay_t<F>, move_only_function> &&
                std::is_invocable_r_v<R, std::decay_t<F>&, Args...>>>
  move_only_function(F&& f)  // NOLINT
      : _ptr(std::make_unique<_Holder<std::decay_t<F>>>(std::forward<F>(f))) {}

  // ---- move only ----
  move_only_function(move_only_function&&) noexcept = default;
  move_only_function& operator=(move_only_function&&) noexcept = default;
  move_only_function(const move_only_function&) = delete;
  move_only_function& operator=(const move_only_function&) = delete;

  // ---- assignment from null ----
  move_only_function& operator=(std::nullptr_t) noexcept {
    _ptr.reset();
    return *this;
  }

  // ---- call ----
  R operator()(Args... args) {
    return _ptr->_invoke(std::forward<Args>(args)...);
  }

  // ---- bool conversion ----
  explicit operator bool() const noexcept { return static_cast<bool>(_ptr); }

  // ---- swap ----
  void swap(move_only_function& other) noexcept { _ptr.swap(other._ptr); }
};

template <typename R, typename... Args>
bool operator==(const move_only_function<R(Args...)>& f, std::nullptr_t) noexcept {
  return !f;
}
template <typename R, typename... Args>
bool operator==(std::nullptr_t, const move_only_function<R(Args...)>& f) noexcept {
  return !f;
}

}  // namespace std

#endif  // !__cpp_lib_move_only_function


// ──────────────────────────────────────────────────────────────────────────────
// 2.  std::chrono::clock_time_conversion<DestClock, SourceClock>
//
//     Primary template introduced in C++20 (P0355R7).  Homebrew libc++ omits
//     it entirely.  We only need the empty primary template so that
//     rex/chrono/chrono.h can legally provide explicit specialisations.
// ──────────────────────────────────────────────────────────────────────────────
#include <chrono>

namespace std::chrono {  // NOLINT(cert-dcl58-cpp) — intentional std extension

#if !defined(__cpp_lib_chrono_udls) || !defined(_LIBCPP_STD_VER) || (_LIBCPP_STD_VER < 20)
// Fallback: the type is missing entirely.
#else
// libc++ has the chrono udls but might still be missing clock_time_conversion.
#endif

// Guard: only inject if it hasn't been defined by the standard library.
// GCC 15+ (devkitPro) already provides clock_time_conversion in C++20 mode.
#if !defined(__GLIBCXX__) || (__cplusplus < 202002L)
template <typename _DestClock, typename _SourceClock>
struct clock_time_conversion {};                       // primary template — empty, per standard
#endif

}  // namespace std::chrono


// ──────────────────────────────────────────────────────────────────────────────
// 3.  std::chrono::clock_cast<DestClock>(tp)
//
//     C++20 utility that converts a time_point between clock types using
//     clock_time_conversion specialisations.  Homebrew libc++ omits it.
//     We implement the three mandatory conversion paths from the standard:
//       (a)  DestClock == SourceClock            → identity
//       (b)  Direct clock_time_conversion<D,S>() operator defined
//       (c)  Via system_clock as intermediate
// ──────────────────────────────────────────────────────────────────────────────

// GCC 15+ (devkitPro) already provides clock_cast in C++20 mode.
#if !defined(__GLIBCXX__) || (__cplusplus < 202002L)

namespace std::chrono {  // NOLINT(cert-dcl58-cpp)

namespace _clk_cast_detail {

// Helper: detect if clock_time_conversion<D,S> has a call operator
template <typename D, typename S, typename Dur, typename = void>
struct _has_direct : std::false_type {};
template <typename D, typename S, typename Dur>
struct _has_direct<D, S, Dur,
    std::void_t<decltype(clock_time_conversion<D,S>{}(
        std::declval<std::chrono::time_point<S, Dur>>()))>>
    : std::true_type {};

}  // namespace _clk_cast_detail

template <typename DestClock, typename SourceClock, typename Duration>
auto clock_cast(const std::chrono::time_point<SourceClock, Duration>& t)
    -> typename DestClock::time_point
{
    // (a) Same clock — identity
    if constexpr (std::is_same_v<DestClock, SourceClock>) {
        return std::chrono::time_point_cast<typename DestClock::duration>(t);
    }
    // (b) Direct conversion available
    else if constexpr (_clk_cast_detail::_has_direct<DestClock, SourceClock, Duration>::value) {
        return clock_time_conversion<DestClock, SourceClock>{}(t);
    }
    // (c) Route through system_clock
    else {
        auto via_sys = clock_time_conversion<std::chrono::system_clock, SourceClock>{}(t);
        return clock_time_conversion<DestClock, std::chrono::system_clock>{}(via_sys);
    }
}

}

#endif  // !__GLIBCXX__

#endif  // __APPLE__ || __ORBIS__ || LIBERTY_RECOMP_PS4 || __SWITCH__ || LIBERTY_RECOMP_NX
