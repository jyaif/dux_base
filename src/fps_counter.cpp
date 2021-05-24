
#include "fps_counter.h"

namespace dux {

void FPSCounter::Update() {
  frames_drawn_since_start_of_measurement_++;

  auto now = std::chrono::steady_clock::now();
  auto time_since_start_of_measurement = now - start_of_measurement_;
  if (std::chrono::duration_cast<std::chrono::milliseconds>(
          time_since_start_of_measurement)
          .count() > 1000) {
    last_fps_ = frames_drawn_since_start_of_measurement_ - 1;
    start_of_measurement_ = now;
    frames_drawn_since_start_of_measurement_ = 0;
  }
}

size_t FPSCounter::GetFPS() {
  return last_fps_;
}

}  // namespace dux
