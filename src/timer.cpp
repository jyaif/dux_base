#include "timer.h"

#include <sstream>

namespace dux {

timespec TimespecDiff(timespec const &a, timespec const &b) {
  timespec result;
  result.tv_sec  = a.tv_sec  - b.tv_sec;
  result.tv_nsec = a.tv_nsec - b.tv_nsec;
  if (result.tv_nsec < 0) {
    --result.tv_sec;
    result.tv_nsec += 1000000000L;
  }
  return result;
}

Timer::Timer() : time_elapsed_(0), previous_lap_time_(0), is_running_(false) {}

Timer::~Timer() = default;

void Timer::Start() {
  if (is_running_) {
    return;
  }
  is_running_ = true;
  gettimeofday(&start_time_, nullptr);
}

void Timer::Stop() {
  if (!is_running_) {
    return;
  }
  time_elapsed_ = GetTimeElapsed();

  is_running_ = false;
}

double Timer::GetTimeElapsed() const {
  if (!is_running_) {
    return time_elapsed_;
  }

  struct timeval end_time;
  gettimeofday(&end_time, nullptr);

  return static_cast<double>((end_time.tv_sec - start_time_.tv_sec) * 1000000 +
                             end_time.tv_usec - start_time_.tv_usec) /
         1000.0;
}

double Timer::GetLapTime() {
  double current_lap_time = GetTimeElapsed();
  double delta = current_lap_time - previous_lap_time_;
  previous_lap_time_ = current_lap_time;
  return delta;
}

bool Timer::GetIsRunning() {
  return is_running_;
}

std::ostream& operator<<(std::ostream& stream, const Timer& timer) {
  stream << timer.GetTimeElapsed() << "ms";
  return stream;
}

AverageTimer::AverageTimer(std::string const& name, size_t iterations)
    : name_(name), index_(0), iterations_(iterations) {
  for (size_t i = 0; i < iterations_; i++) {
    times_.push_back(0);
  }
}
AverageTimer::~AverageTimer() = default;

void AverageTimer::Start() {
  timer_.Start();
}

void AverageTimer::Stop() {
  timer_.Stop();
  times_[index_] = timer_.GetTimeElapsed();
  if (index_ == 0) {
    float sum = 0;
    for (size_t i = 0; i < iterations_; i++) {
      sum += times_[i];
    }
    printf("time elapsed for %s: %f\n", name_.c_str(),
           static_cast<double>(sum / iterations_));
  }
  index_++;
  if (index_ >= iterations_) {
    index_ = 0;
  }
}
}  // namespace dux
