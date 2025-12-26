#ifndef DUX_BASE_SRC_WEAK_PTR_FACTORY_H_
#define DUX_BASE_SRC_WEAK_PTR_FACTORY_H_

#include <cassert>
#include <memory>
#include "thread_checker.h"

namespace dux {

template <class T>
class WeakPtr {
 public:
  WeakPtr(std::weak_ptr<T> ptr) : ptr_(std::move(ptr)) {}

  WeakPtr() = default;

  template <typename U>
  friend class WeakPtr;

  template <typename U>
  WeakPtr(const WeakPtr<U>& other) : ptr_(other.ptr_) {
    static_assert(std::is_convertible<U*, T*>::value,
                  "Implicit conversion invalid: U must inherit from T");
  }

  T* Get() const {
#if !defined(NDEBUG)
    assert(thread_checker_.IsCreationThreadCurrent());
#endif
    // If expired, lock returns null shared_ptr, get returns nullptr.
    // If valid, lock returns valid shared_ptr, get returns T*.
    return ptr_.lock().get();
  }

 private:
#if !defined(NDEBUG)
  ThreadChecker thread_checker_;
#endif
  std::weak_ptr<T> ptr_;
};

template <class T>
class WeakPtrFactory {
 public:
  explicit WeakPtrFactory(T* object) {
    // Create a shared_ptr that points to 'object'.
    // We pass a no-op deleter [](T*){} so that when this shared_ptr
    // dies, it does NOT delete the underlying object.
    ptr_.reset(object, [](T*) {});
  }

  WeakPtr<T> GetWeak() { return WeakPtr<T>(ptr_); }

 private:
  std::shared_ptr<T> ptr_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_WEAK_PTR_FACTORY_H_
