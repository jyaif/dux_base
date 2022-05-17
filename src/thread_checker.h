#ifndef DUX_BASE_THREAD_CHECKER_H_
#define DUX_BASE_THREAD_CHECKER_H_

namespace dux {

#ifndef _WIN32
#include <pthread.h>

class ThreadChecker final {
 public:
  ThreadChecker() : self_(pthread_self()) {}
  ~ThreadChecker() {}
  // Returns true if the current thread is the thread this object was created
  // on and false otherwise.
  bool IsCreationThreadCurrent() const {
    return !!pthread_equal(pthread_self(), self_);
  }

 private:
  const pthread_t self_;
};

#else

class ThreadChecker final {
 public:
  ThreadChecker() {}
  ~ThreadChecker() {}
  bool IsCreationThreadCurrent() const {
    return true;
  }
};

#endif

}  // namespace dux
#endif  // DUX_BASE_THREAD_CHECKER_H_
