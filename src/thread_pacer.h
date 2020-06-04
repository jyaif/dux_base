#ifndef DUX_BASE_SRC_THREAD_PACER_H_
#define DUX_BASE_SRC_THREAD_PACER_H_

#include <chrono>

namespace dux {

class ThreadPacer {
 public:
  ThreadPacer(int wake_up_frequency);
  void Sleep();

 private:
  std::chrono::steady_clock::time_point start_;
  double update_ = 0;
  int wake_up_frequency_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_THREAD_PACER_H_
