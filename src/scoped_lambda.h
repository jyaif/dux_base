#ifndef DUX_BASE_SRC_SCOPED_LAMBDA_H_
#define DUX_BASE_SRC_SCOPED_LAMBDA_H_

#include <functional>

namespace dux {

class ScopedLambda {
 public:
  ScopedLambda(std::function<void()> const& lambda);
  ~ScopedLambda();

 private:
  std::function<void()> lambda_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_SCOPED_LAMBDA_H_
