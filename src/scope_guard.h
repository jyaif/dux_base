#ifndef DUX_BASE_SRC_SCOPE_GUARD_H_
#define DUX_BASE_SRC_SCOPE_GUARD_H_

#include <functional>

namespace dux {

class ScopeGuard {
 public:
  ScopeGuard(std::function<void()> const& lambda);
  ~ScopeGuard();

 private:
  std::function<void()> lambda_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_SCOPE_GUARD_H_
