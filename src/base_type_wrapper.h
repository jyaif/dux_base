#ifndef DUX_BASE_SRC_BASE_TYPE_WRAPPER_H_
#define DUX_BASE_SRC_BASE_TYPE_WRAPPER_H_

#include <type_traits>

namespace dux {

template <typename T>
class BaseTypeWrapper {
 public:
  explicit BaseTypeWrapper(T value) : value_(value) {}

  constexpr BaseTypeWrapper operator+(const BaseTypeWrapper& o) const {
    BaseTypeWrapper v;
    v.value_ = o.value_ + value_;
    return v;
  }
  constexpr BaseTypeWrapper operator-(const BaseTypeWrapper& o) const {
    return BaseTypeWrapper(value_ - o.value_);
  }
  BaseTypeWrapper operator++() {
    value_++;
    return *this;
  }
  BaseTypeWrapper operator--() {
    value_--;
    return *this;
  }

  constexpr bool operator!=(const BaseTypeWrapper& o) const {
    return value_ != o.value_;
  }

  constexpr bool operator==(const BaseTypeWrapper& o) const {
    return value_ == o.value_;
  }
  constexpr bool operator<(const BaseTypeWrapper& o) const {
    return value_ < o.value_;
  }
  constexpr bool operator<=(const BaseTypeWrapper& o) const {
    return value_ <= o.value_;
  }
  constexpr bool operator>(const BaseTypeWrapper& o) const {
    return value_ > o.value_;
  }
  constexpr bool operator>=(const BaseTypeWrapper& o) const {
    return value_ >= o.value_;
  }

  template <typename U>
  constexpr BaseTypeWrapper operator+(const U v) const {
    static_assert(std::is_integral<U>::value, "Integer required.");
    return BaseTypeWrapper(value_ + v);
  }

  template <typename U>
  constexpr BaseTypeWrapper operator-(const U v) const {
    static_assert(std::is_integral<U>::value, "Integer required.");
    return BaseTypeWrapper(value_ - v);
  }

  template <typename U>
  void operator+=(const U v) {
    static_assert(std::is_integral<U>::value, "Integer required.");
    value_ += v;
  }

  template <typename U>
  void operator-=(const U v) {
    static_assert(std::is_integral<U>::value, "Integer required.");
    value_ -= v;
  }

  T value_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_BASE_TYPE_WRAPPER_H_
