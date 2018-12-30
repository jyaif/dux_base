#include <cassert>
#include <functional>
#include <memory>

#include "copyable_lambda_test.h"

#include "copyable_lambda.h"

void TestCopyableLambda() {
  std::function<void()> void_func = dux::MakeCopyable([]() {});
  void_func();

  std::function<int()> int_func = dux::MakeCopyable([]() { return 5; });
  assert(5 == int_func());

  std::unique_ptr<int> int_ptr(new int);
  *int_ptr = 42;
  std::function<int()> int_ptr_func =
      dux::MakeCopyable([p = std::move(int_ptr)]() { return *p; });
  assert(int_ptr == nullptr);
  assert(42 == int_ptr_func());

  std::unique_ptr<int> src(new int);
  *src = 42;
  std::unique_ptr<int> dest;
  std::function<void()> mutable_func = dux::MakeCopyable(
      [&dest, src = std::move(src)]() mutable { dest = std::move(src); });
  mutable_func();
  assert(42 == *dest);
}
