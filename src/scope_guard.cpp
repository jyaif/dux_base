#include "scope_guard.h"

namespace dux {

ScopeGuard::ScopeGuard(std::function<void()> const& lambda) : lambda_(lambda) {}

ScopeGuard::~ScopeGuard() {
  lambda_();
}

}  // namespace dux
