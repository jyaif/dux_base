#ifndef DUX_BASE_SRC_BACKGROUND_TASK_RUNNER_H_
#define DUX_BASE_SRC_BACKGROUND_TASK_RUNNER_H_

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace dux {

class TaskRunner;

using BackgroundTask = std::function<void(void)>;

class BackgroundTaskRunner {
 public:
  BackgroundTaskRunner();
  ~BackgroundTaskRunner();
  void PostTask(BackgroundTask const& task);
  // Blocks until all on-going tasks have executed.
  void Stop();

  std::queue<BackgroundTask> tasks_to_run_;
  std::mutex tasks_mutex_;
  std::condition_variable cv_;
  bool exit_ = false;
  // Needs to be constructed *after* most of the other members.
  std::thread background_thread_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_TASK_RUNNER_H_
