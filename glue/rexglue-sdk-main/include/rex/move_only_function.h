#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

// TODO(Graine25) why isnt this function included in the latest llvm wtf apple

namespace rex {

#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L
template <typename Signature>
using move_only_function = std::move_only_function<Signature>;
#else
template <typename Signature>
class move_only_function;

template <typename Result, typename... Args>
class move_only_function<Result(Args...)> {
 public:
  move_only_function() = default;
  move_only_function(std::nullptr_t) {}

  move_only_function(move_only_function&&) noexcept = default;
  move_only_function& operator=(move_only_function&&) noexcept = default;

  move_only_function(const move_only_function&) = delete;
  move_only_function& operator=(const move_only_function&) = delete;

  move_only_function& operator=(std::nullptr_t) noexcept {
    target_.reset();
    return *this;
  }

  bool operator==(std::nullptr_t) const noexcept { return !target_; }
  bool operator!=(std::nullptr_t) const noexcept { return static_cast<bool>(target_); }

  template <typename Callable>
    requires(!std::same_as<std::remove_cvref_t<Callable>, move_only_function> &&
             !std::same_as<std::remove_cvref_t<Callable>, std::nullptr_t> &&
             std::is_invocable_r_v<Result, Callable&, Args...>)
  move_only_function(Callable&& callable) {
    using Model = CallableModel<std::remove_cvref_t<Callable>>;
    target_ = std::make_unique<Model>(std::forward<Callable>(callable));
  }

  template <typename Callable>
    requires(!std::same_as<std::remove_cvref_t<Callable>, move_only_function> &&
             !std::same_as<std::remove_cvref_t<Callable>, std::nullptr_t> &&
             std::is_invocable_r_v<Result, Callable&, Args...>)
  move_only_function& operator=(Callable&& callable) {
    using Model = CallableModel<std::remove_cvref_t<Callable>>;
    target_ = std::make_unique<Model>(std::forward<Callable>(callable));
    return *this;
  }

  explicit operator bool() const noexcept { return static_cast<bool>(target_); }

  Result operator()(Args... args) { return target_->Invoke(std::forward<Args>(args)...); }

 private:
  struct CallableBase {
    virtual ~CallableBase() = default;
    virtual Result Invoke(Args... args) = 0;
  };

  template <typename Callable>
  struct CallableModel final : CallableBase {
    template <typename Candidate>
    explicit CallableModel(Candidate&& callable) : callable_(std::forward<Candidate>(callable)) {}

    Result Invoke(Args... args) override {
      if constexpr (std::is_void_v<Result>) {
        std::invoke(callable_, std::forward<Args>(args)...);
        return;
      } else {
        return std::invoke(callable_, std::forward<Args>(args)...);
      }
    }

    Callable callable_;
  };

  std::unique_ptr<CallableBase> target_;
};
#endif

}  // namespace rex
