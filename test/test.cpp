#include <cassert>
#include <cstdio>
#include <thread>

#include "base_type_wrapper.h"
#include "mix.h"
#include "observer_list.h"
#include "scoped_lambda.h"
#include "stream.h"
#include "thread_checker.h"

void TestBaseTypeWrapper();
void TestMix();
void TestObserverList();
void TestScopedLambda();
void TestStream();
void SomeThread(std::pair<dux::ThreadChecker*, bool*> params);
void TestThreadChecker();

void TestBaseTypeWrapper() {
  dux::BaseTypeWrapper<int64_t> v(10);
  assert(v.value_ == 10);
  ++v;
  assert(v.value_ == 11);
}

void TestMix() {
  assert(dux::Mix(100, 110, 0) == 100);
  assert(dux::Mix(100, 110, 1) == 110);
  assert(dux::Mix(100.0, 110.0, 0.5) == 105.0);
}

class TestObserver {
 public:
  TestObserver() : value_(0) {}
  void SetValue(int value) { value_ = value; }
  int value_;
};

void TestObserverList() {
  dux::ObserverList<TestObserver> observer_list;
  TestObserver observer_1;
  TestObserver observer_2;
  assert(observer_list.IsEmpty());

  observer_list.AddObserver(&observer_1);
  assert(!observer_list.IsEmpty());

  observer_list.AddObserver(&observer_2);
  assert(!observer_list.IsEmpty());

  FOR_EACH_OBSERVER(TestObserver, observer_list, SetValue(10));
  assert(observer_1.value_ == 10);
  assert(observer_2.value_ == 10);

  FOR_EACH_OBSERVER(TestObserver, observer_list, SetValue(100));
  assert(observer_1.value_ == 100);
  assert(observer_2.value_ == 100);

  observer_list.RemoveObserver(&observer_1);
  FOR_EACH_OBSERVER(TestObserver, observer_list, SetValue(1000));
  assert(!observer_list.IsEmpty());
  assert(observer_1.value_ == 100);
  assert(observer_2.value_ == 1000);

  observer_list.RemoveObserver(&observer_2);
  assert(observer_list.IsEmpty());
}

void TestScopedLambda() {
  int a = 0;
  {
    dux::ScopedLambda lambda([&a] { a = 10; });
    assert(a == 0);
  }
  assert(a == 10);
}

void TestStream() {
  dux::OStream ostream;
  {
    int8_t a = 0x12;
    int16_t b = 0x3456;
    int64_t c = static_cast<int64_t>(0xABCDABCDABCDABCD);
    int8_t d = 0x78;
    ostream.Write(a);
    ostream.Write(b);
    ostream.Write(c);
    ostream.Write(d);
  }
  dux::IStream istream(ostream.Data());
  {
    int8_t a;
    int16_t b;
    int64_t c;
    int8_t d;
    bool success = true;
    istream.SkipBytes(1);
    success &= istream.Read(b);
    success &= istream.Read(c);
    success &= istream.Read(d);
    assert(success);
    assert(b == 0x3456);
    assert(c == static_cast<int64_t>(0xABCDABCDABCDABCD));
    assert(d == 0x78);
    success &= istream.Read(a);
    assert(!success);
  }
}

void SomeThread(std::pair<dux::ThreadChecker*, bool*> params) {
  dux::ThreadChecker thread_checker_2;
  assert(thread_checker_2.IsCreationThreadCurrent());
  assert(!params.first->IsCreationThreadCurrent());
  *params.second = true;
}

void TestThreadChecker() {
  bool thread_ran = false;
  dux::ThreadChecker thread_checker;
  assert(thread_checker.IsCreationThreadCurrent());
  std::thread t(SomeThread, std::pair<dux::ThreadChecker*, bool*>(
                                &thread_checker, &thread_ran));
  t.join();
  assert(thread_ran);
}

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  TestBaseTypeWrapper();
  TestMix();
  TestObserverList();
  TestScopedLambda();
  TestStream();
  TestThreadChecker();
  printf("tests successfully passed\n");
  return EXIT_SUCCESS;
}
