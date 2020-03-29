#include "background_task_runner.h"

#include <optional>

namespace dux {

namespace {

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
      std::unique_lock<std::mutex> lock(btr->tasks_mutex_);
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

}  // namespace

BackgroundTaskRunner::BackgroundTaskRunner()
    : background_thread_(RunBackgroundThread, this) {}

BackgroundTaskRunner::~BackgroundTaskRunner() {
  Stop();
}

void BackgroundTaskRunner::PostTask(BackgroundTask const& task) {
  {
    std::lock_guard<std::mutex> guard(tasks_mutex_);
    tasks_to_run_.push(task);
  }
  cv_.notify_one();
}

void BackgroundTaskRunner::Stop() {
  if (exit_) {
    return;
  }
  {
    std::lock_guard<std::mutex> guard(tasks_mutex_);
    exit_ = true;
  }
  cv_.notify_one();
  background_thread_.join();
}

}  // namespace dux
