#include "background_task_runner_test.h"

#include "background_task_runner.h"

#include <cassert>
#include <thread>

void TestBackgroundTaskRunner() {
  int v = 0;
  {
    dux::BackgroundTaskRunner btr;
    btr.PostTask([&v]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      v += 1;
    });
    btr.PostTask([&v]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      v += 2;
    });
    // This last task will not be executed because the btr will be destroyed
    // while the 2nd task is running.
    btr.PostTask([&v]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      v += 4;
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    assert(v == 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(v == 1);
  }
  assert(v == 3);
}
