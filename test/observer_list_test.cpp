#include "observer_list_test.h"

#include <cassert>

#include "observer_list.h"

class TestObserver {
 public:
  TestObserver() : value_(0) {}
  virtual void SetValue(int value) { value_ = value; }
  int value_;
};

class TestObserverWithLambda : public TestObserver {
 public:
  TestObserverWithLambda(std::function<void()> lambda)
      : value_(0), lambda_(lambda) {}
  void SetValue(int value) override {
    value_ = value;
    lambda_();
  }
  int value_;
  std::function<void()> lambda_;
};

#define COMPATIBLE_FOR_EACH(a, b, c)                                \
  if constexpr (std::is_same_v<T, dux::ObserverList<TestObserver>>) \
    FOR_EACH_OBSERVER(a, b, c);                                     \
  else                                                              \
    FOR_EACH_OBSERVER_TS(a, b, c);

template <class T>
void TemplatedTestObserverList() {
  T observer_list;
  TestObserver observer_1;
  TestObserver observer_2;
  assert(observer_list.ObserverCount() == 0);

  observer_list.AddObserver(&observer_1);
  assert(observer_list.ObserverCount() == 1);

  observer_list.AddObserver(&observer_2);
  assert(observer_list.ObserverCount() == 2);

  COMPATIBLE_FOR_EACH(TestObserver, observer_list, SetValue(10));

  assert(observer_1.value_ == 10);
  assert(observer_2.value_ == 10);

  COMPATIBLE_FOR_EACH(TestObserver, observer_list, SetValue(100));
  assert(observer_1.value_ == 100);
  assert(observer_2.value_ == 100);

  assert(observer_list.ObserverCount() == 2);
  observer_list.RemoveObserver(&observer_1);
  assert(observer_list.ObserverCount() == 1);

  COMPATIBLE_FOR_EACH(TestObserver, observer_list, SetValue(1000));
  assert(observer_1.value_ == 100);
  assert(observer_2.value_ == 1000);

  observer_list.RemoveObserver(&observer_2);
  assert(observer_list.ObserverCount() == 0);

  // ThreadSafeObserverList deadlocks in case of re-entrancy.
  if constexpr (std::is_same_v<T, dux::ThreadSafeObserverList<TestObserver>>)
    return;

  // Test that modifying the observer list while iterating over it works.
  TestObserver observer_3;
  TestObserverWithLambda observer_4(
      [&] { observer_list.RemoveObserver(&observer_3); });

  assert(observer_3.value_ == 0);
  assert(observer_4.value_ == 0);

  observer_list.AddObserver(&observer_4);
  observer_list.AddObserver(&observer_3);
  observer_list.AddObserver(&observer_2);
  observer_list.AddObserver(&observer_1);

  assert(observer_list.ObserverCount() == 4);
  COMPATIBLE_FOR_EACH(TestObserver, observer_list, SetValue(10000));
  assert(observer_list.ObserverCount() == 3);

  assert(observer_1.value_ == 10000);
  assert(observer_2.value_ == 10000);
  assert(observer_3.value_ == 0);
  assert(observer_4.value_ == 10000);
}

void TestObserverList() {
  TemplatedTestObserverList<dux::ObserverList<TestObserver>>();
  TemplatedTestObserverList<dux::ThreadSafeObserverList<TestObserver>>();
}
