#ifndef DUX_BASE_SRC_TIMER_H_
#define DUX_BASE_SRC_TIMER_H_

#include <sys/time.h>
#include <string>
#include <vector>

namespace dux {

class Timer {
 public:
  Timer();
  ~Timer();
  void Start();
  void Stop();
  double GetTimeElapsed();
  double GetLapTime();
  void Print(char const* ptr = NULL);
  bool GetIsRunning();

 private:
  double time_elapsed_;
  double previous_lap_time_;
  struct timeval start_time_;
  bool is_running_;
};

class AverageTimer {
 public:
  AverageTimer(std::string const& name, size_t iterations);
  ~AverageTimer();
  void Start();
  void Stop();

 private:
  std::string name_;
  std::vector<double> times_;
  size_t index_;
  size_t iterations_;
  Timer timer_;
};
}  // namespace dux

#endif  // DUX_BASE_SRC_TIMER_H_
