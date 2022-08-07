#include "thread_checker.h"

#include <windows.h>
#include "Processthreadsapi.h"

namespace dux {

#ifdef _WIN32

ThreadChecker::ThreadChecker() : thread_id_(GetCurrentThreadId()) {}

bool ThreadChecker::IsCreationThreadCurrent() const {
  return GetCurrentThreadId() == thread_id_;
}

#endif

}  // namespace dux
