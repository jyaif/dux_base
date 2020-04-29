#include "thread_pacer.h"

#include <thread>

namespace dux {

ThreadPacer::ThreadPacer(int wake_up_frequency)
    : wake_up_frequency_(wake_up_frequency) {
  start_ = std::chrono::system_clock::now();
}

void ThreadPacer::Sleep() {
  update_++;
  int64_t us_since_start = (update_ / wake_up_frequency_) * 1000000;
  auto duration_since_start = std::chrono::microseconds(us_since_start);
  auto sleep_end_timepoint = start_ + duration_since_start;
  std::this_thread::sleep_until(sleep_end_timepoint);
}

}  // namespace dux
