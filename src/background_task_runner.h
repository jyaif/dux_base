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
  BackgroundTaskRunner(int number_of_threads = 1);
  ~BackgroundTaskRunner();
  void PostTask(BackgroundTask const& task);
  // Blocks until all on-going tasks have executed.
  void Stop();
  int TasksToRun();

  std::queue<BackgroundTask> tasks_to_run_;
  std::mutex tasks_mutex_;
  std::condition_variable cv_;
  bool exit_ = false;
  // Needs to be constructed *after* most of the other members.
  std::vector<std::thread> background_threads_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_TASK_RUNNER_H_
