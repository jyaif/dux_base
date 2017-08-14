#ifndef DUX_BASE_THREAD_CHECKER_H_
#define DUX_BASE_THREAD_CHECKER_H_

#include <pthread.h>

namespace dux {

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

}  // namespace dux
#endif  // DUX_BASE_THREAD_CHECKER_H_
