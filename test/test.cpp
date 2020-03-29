#include <cassert>
#include <cstdio>
#include <thread>

#include "background_task_runner_test.h"
#include "base_type_wrapper.h"
#include "copyable_lambda_test.h"
#include "crc32.h"
#include "fps_counter.h"
#include "mix.h"
#include "obfuscated.h"
#include "observer_list.h"
#include "observer_list_test.h"
#include "scope_guard.h"
#include "scoped_callback.h"
#include "stream.h"
#include "task_runner.h"
#include "thread_checker.h"
#include "value_test.h"
#include "weak_ptr_factory.h"

void TestBaseTypeWrapper();
void TestMix();
void TestScopeGuard();
void TestStream();
void SomeThread(std::pair<dux::ThreadChecker*, bool*> params);
void TestThreadChecker();
void TestWeakPtrFactory();

void TestBaseTypeWrapper() {
  dux::BaseTypeWrapper<int64_t> v(11);
  assert(v.value_ == 11);
  ++v;
  assert(v.value_ == 12);
  --v;
  assert(v.value_ == 11);
  v += 10;
  assert(v.value_ == 21);
  v -= 20;
  assert(v.value_ == 1);
}

void TestCRC32() {
  assert(dux::ComputeCRC32("", 0) == 0);
  assert(dux::ComputeCRC32("a", 1) == 3904355907);
  assert(dux::ComputeCRC32("abc", 3) == 891568578);
}

void TestMix() {
  assert(dux::Mix(100, 110, 0) == 100);
  assert(dux::Mix(100, 110, 1) == 110);
  assert(dux::Mix(100.0, 110.0, 0.5) == 105.0);
}

void TestScopeGuard() {
  int a = 0;
  {
    dux::ScopeGuard lambda([&a] { a = 10; });
    assert(a == 0);
  }
  assert(a == 10);
}

void TestScopedCallback() {
  int a = 213;
  bool first = false;
  bool second = false;
  auto weak_factory = std::make_unique<dux::WeakPtrFactory<int>>(&a);
  auto first_callback = dux::ScopedCallback(weak_factory->GetWeak(),
                                            [&first]() { first = true; });
  auto second_callback = dux::ScopedCallback(weak_factory->GetWeak(),
                                             [&second]() { second = true; });

  first_callback();
  weak_factory.reset();
  second_callback();
  assert(first == true);
  assert(second == false);
}

void TestStream() {
  dux::OStream ostream;
  {
    int8_t a = 0x12;
    int16_t b = 0x3456;
    int64_t c = static_cast<int64_t>(0xABCDABCDABCDABCD);
    int8_t d = 0x78;
    std::string s1 = "foo";
    std::string s2 = "bar";
    ostream.Write(a);
    ostream.Write(b);
    ostream.Write(c);
    ostream.Write(d);
    ostream.Write(s1);
    ostream.Write(s2);
  }
  dux::IStream istream(ostream.Data());
  {
    int8_t a;
    int16_t b;
    int64_t c;
    int8_t d;
    std::string s1;
    std::string s2;
    bool success = true;
    auto v = istream.SkipBytes(1);
    assert(v);
    assert(!istream.IsAtEnd());
    success &= istream.Read(b);
    success &= istream.Read(c);
    success &= istream.Read(d);
    success &= istream.Read(s1);
    success &= istream.Read(s2);
    assert(istream.IsAtEnd());
    assert(success);
    assert(b == 0x3456);
    assert(c == static_cast<int64_t>(0xABCDABCDABCDABCD));
    assert(d == 0x78);
    assert(s1 == "foo");
    assert(s2 == "bar");
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

void TestTaskRunner() {
  int task1_ran_count = 0;
  int task2_ran_count = 0;
  int task3_ran_count = 0;
  dux::TaskRunner task_runner;
  task_runner.RunLoop();
  assert(task1_ran_count == 0 && task2_ran_count == 0 && task3_ran_count == 0);
  task_runner.PostTask([&task1_ran_count]() { task1_ran_count++; });
  task_runner.PostTask([&task2_ran_count]() { task2_ran_count++; }, 100000);
  task_runner.PostTask([&task_runner, &task3_ran_count]() {
    task_runner.PostTask([&task3_ran_count]() { task3_ran_count++; });
  });
  assert(task1_ran_count == 0 && task2_ran_count == 0 && task3_ran_count == 0);
  task_runner.RunLoop();
  assert(task1_ran_count == 1 && task2_ran_count == 0 && task3_ran_count == 0);
  task_runner.RunLoop();
  assert(task1_ran_count == 1 && task2_ran_count == 0 && task3_ran_count == 1);
  task_runner.RunLoop();
  assert(task1_ran_count == 1 && task2_ran_count == 0 && task3_ran_count == 1);
}

void TestWeakPtrFactory() {
  int a = 43;

  auto f1 = std::make_unique<dux::WeakPtrFactory<int>>(&a);
  dux::WeakPtr<int> w1 = f1->GetWeak();
  assert(w1.Get());
  assert(*w1.Get() == 43);
  a = 44;
  assert(*w1.Get() == 44);
  dux::WeakPtr<int> w2 = f1->GetWeak();
  assert(*w2.Get() == 44);

  f1.reset();
  assert(w1.Get() == nullptr);
  assert(w2.Get() == nullptr);
}

void TestFPSCounter() {
  dux::FPSCounter f;
  assert(f.GetFPS() == 0);
  f.Update();
  f.Update();
  f.Update();
  assert(f.GetFPS() == 0);
}

void TestObfuscated() {
  dux::Obfuscated<int> a;
  assert(a.GetValue() == 0);
  a.SetValue(10);
  assert(a.GetValue() == 10);
  dux::Obfuscated<int> b(33);
  assert(b.GetValue() == 33);
}

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  TestBackgroundTaskRunner();
  TestBaseTypeWrapper();
  TestCRC32();
  TestMix();
  TestObserverList();
  TestObfuscated();
  TestScopeGuard();
  TestScopedCallback();
  TestStream();
  TestThreadChecker();
  TestTaskRunner();
  TestWeakPtrFactory();
  TestFPSCounter();
  TestValue();
  TestCopyableLambda();
  printf("tests successfully passed\n");
  return EXIT_SUCCESS;
}
