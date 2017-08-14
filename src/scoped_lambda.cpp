#include "scoped_lambda.h"

namespace dux {

ScopedLambda::ScopedLambda(std::function<void()> const& lambda)
    : lambda_(lambda) {}

ScopedLambda::~ScopedLambda() {
  lambda_();
}

}  // namespace dux
