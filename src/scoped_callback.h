#ifndef DUX_BASE_SRC_SCOPED_CALLBACK_H_
#define DUX_BASE_SRC_SCOPED_CALLBACK_H_

#include <functional>

#include "weak_ptr_factory.h"

namespace dux {

template <typename T, typename U>
class ScopedCallback {
 public:
  ScopedCallback(T witness, U function)
      : function_(std::move(function)), witness_(std::move(witness)) {}

  template <typename... ArgType>
  void operator()(ArgType&&... args) {
    if (witness_.Get() != nullptr) {
      function_(std::forward<ArgType>(args)...);
    }
  }

  template <typename... ArgType>
  void operator()(ArgType&&... args) const {
    if (witness_.Get() != nullptr) {
      function_(std::forward<ArgType>(args)...);
    }
  }

 private:
  U function_;
  T witness_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_SCOPED_CALLBACK_H_
