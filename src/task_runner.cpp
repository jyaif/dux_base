#include "task_runner.h"

namespace dux {

TaskWithTimeStamp::TaskWithTimeStamp(Task const& task, TimeStamp time_stamp)
    : task_(task), time_stamp_(time_stamp) {}

void TaskRunner::RunLoop() {
  auto now = std::chrono::steady_clock::now();

  std::vector<Task> tasks_to_run;

  {
    std::lock_guard<std::mutex> guard(tasks_mutex_);
    for (size_t i = 0; i < timed_tasks_.size(); i++) {
      auto& timed_task = timed_tasks_[i];
      if (timed_task.time_stamp_ <= now) {
        tasks_to_run.push_back(std::move(timed_task.task_));
        timed_task = timed_tasks_.back();
        timed_tasks_.pop_back();
        i--;
      }
    }
  }

  for (auto& t : tasks_to_run) {
    t();
  }
}

void TaskRunner::PostTask(Task const& task) {
  TimeStamp time = std::chrono::steady_clock::now();
  std::lock_guard<std::mutex> guard(tasks_mutex_);
  timed_tasks_.emplace_back(TaskWithTimeStamp(task, time));
}

void TaskRunner::PostTask(Task const& task, int64_t ms) {
  auto time = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
  std::lock_guard<std::mutex> guard(tasks_mutex_);
  timed_tasks_.emplace_back(task, time);
}

}  // namespace dux
