#ifndef DUX_BASE_SRC_TASK_RUNNER_H_
#define DUX_BASE_SRC_TASK_RUNNER_H_

#include <chrono>
#include <functional>
#include <mutex>
#include <vector>

namespace dux {

using Task = std::function<void(void)>;
using TimeStamp = std::chrono::time_point<std::chrono::steady_clock>;

class TaskWithTimeStamp {
 public:
  TaskWithTimeStamp(Task const& task, TimeStamp time_stamp);
  Task task_;
  TimeStamp time_stamp_;
};

class TaskRunner {
 public:
  void RunLoop();
  void PostTask(Task const& task);
  void PostTask(Task const& task, int64_t ms);

 private:
  std::vector<TaskWithTimeStamp> timed_tasks_;
  std::mutex tasks_mutex_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_TASK_RUNNER_H_
