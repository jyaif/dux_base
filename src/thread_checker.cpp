#include "thread_checker.h"

#ifdef _WIN32

#include <windows.h>
#include "Processthreadsapi.h"

namespace dux {

ThreadChecker::ThreadChecker() : thread_id_(GetCurrentThreadId()) {}

bool ThreadChecker::IsCreationThreadCurrent() const {
  return GetCurrentThreadId() == thread_id_;
}

}  // namespace dux

#endif
