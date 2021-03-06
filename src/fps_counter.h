#ifndef DUX_BASE_SRC_FPS_COUNTER_H_
#define DUX_BASE_SRC_FPS_COUNTER_H_

#include <chrono>

namespace dux {

class FPSCounter {
 public:
  // Call whenever a frame has finished rendering.
  void Update();

  // Returns the last measured FPS.
  size_t GetFPS();

 private:
  std::chrono::steady_clock::time_point start_of_measurement_;
  size_t last_fps_ = 0;
  size_t frames_drawn_since_start_of_measurement_ = 0;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_FPS_COUNTER_H_
