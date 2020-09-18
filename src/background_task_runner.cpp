#include "background_task_runner.h"

#include <cassert>
#include <optional>

namespace dux {

namespace {

#if !DUX_BASE_DISABLE_THREADING

void RunBackgroundThread(BackgroundTaskRunner* btr) {
  while (true) {
    // Look at the state of the btr:
    // If the btr says the thread should exit, the thread exits.
    // If the btr has no task to run, wait.
    // If the btr has a task to run, proceed.
    {
      std::unique_lock<std::mutex> lock(btr->tasks_mutex_);
      if (btr->exit_) {
        return;
      }
      if (btr->tasks_to_run_.empty()) {
        btr->cv_.wait(lock);
      }
    }

    // If the btr has a task to run, run it.
    std::optional<BackgroundTask> optional_task;
    {
      std::lock_guard<std::mutex> lock(btr->tasks_mutex_);
      if (!btr->tasks_to_run_.empty()) {
        optional_task = btr->tasks_to_run_.front();
        btr->tasks_to_run_.pop();
      }
    }
    if (optional_task.has_value()) {
      optional_task.value()();
    }
  }
}

#endif

}  // namespace

#if DUX_BASE_DISABLE_THREADING

BackgroundTaskRunner::BackgroundTaskRunner(int number_of_threads) {}

BackgroundTaskRunner::~BackgroundTaskRunner() {}

void BackgroundTaskRunner::PostTask(BackgroundTask const& task) {
  task();
}

void BackgroundTaskRunner::Stop() {}

#else
BackgroundTaskRunner::BackgroundTaskRunner(int number_of_threads) {
  assert(number_of_threads > 0);
  for (int i = 0; i < number_of_threads; i++) {
    std::thread t(RunBackgroundThread, this);
    background_threads_.emplace_back(std::move(t));
  }
}

BackgroundTaskRunner::~BackgroundTaskRunner() {
  Stop();
}

void BackgroundTaskRunner::PostTask(BackgroundTask const& task) {
  {
    std::lock_guard<std::mutex> guard(tasks_mutex_);
    if (exit_) {
      return;
    }
    tasks_to_run_.push(task);
  }
  cv_.notify_all();
}

void BackgroundTaskRunner::Stop() {
  {
    std::lock_guard<std::mutex> guard(tasks_mutex_);
    if (exit_) {
      return;
    }
    exit_ = true;
  }
  cv_.notify_all();
  for (auto& thread : background_threads_) {
    thread.join();
  }
}

#endif

}  // namespace dux
