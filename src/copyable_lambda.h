#ifndef DUX_BASE_SRC_COPYABLE_LAMBDA_H_
#define DUX_BASE_SRC_COPYABLE_LAMBDA_H_

#include <memory>

namespace dux {

template <typename T>
class CopyableLambda {
 public:
  explicit CopyableLambda(T func)
      : impl_(std::make_shared<Impl>(std::move(func))) {}
  template <typename... ArgType>
  auto operator()(ArgType&&... args) const {
    return impl_->func_(std::forward<ArgType>(args)...);
  }

 private:
  class Impl : public std::shared_ptr<Impl> {
   public:
    explicit Impl(T func) : func_(std::move(func)) {}
    T func_;
  };
  std::shared_ptr<Impl> impl_;
};

template <typename T>
CopyableLambda<T> MakeCopyable(T lambda) {
  return CopyableLambda<T>(std::move(lambda));
}

}  // namespace dux

#endif  // DUX_BASE_SRC_COPYABLE_LAMBDA_H_
