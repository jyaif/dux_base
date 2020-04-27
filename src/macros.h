#ifndef DUX_BASE_SRC_MACROS_H_
#define DUX_BASE_SRC_MACROS_H_

// Put this in the declarations for a class to be uncopyable.
#define DISALLOW_COPY(TypeName) TypeName(const TypeName&) = delete
// Put this in the declarations for a class to be unassignable.
#define DISALLOW_ASSIGN(TypeName) TypeName& operator=(const TypeName&) = delete

// Put this in the declarations for a class to be uncopyable and unassignable.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  DISALLOW_COPY(TypeName);                 \
  DISALLOW_ASSIGN(TypeName)

// To use, define _LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS at the project level,
// and compile with -Wthread-safety
#if _LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS
#define GUARDED_BY(guard) __attribute__((guarded_by(guard)))
#define EXCLUSIVE_LOCKS_REQUIRED(...) __attribute__((exclusive_locks_required(__VA_ARGS__)))
#else
#define GUARDED_BY(guard)
#define EXCLUSIVE_LOCKS_REQUIRED(...)
#endif

#endif  // DUX_BASE_SRC_MACROS_H_
