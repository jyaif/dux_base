#ifndef DUX_BASE_SRC_WEAK_PTR_FACTORY_H_
#define DUX_BASE_SRC_WEAK_PTR_FACTORY_H_

#include <memory>

namespace dux {

template <class T>
class WeakPtr {
 public:
  WeakPtr(std::weak_ptr<std::unique_ptr<T>> ptr) : ptr_(ptr) {}

  T* Get() const {
    auto v = ptr_.lock();
    if (v)
      return v->get();
    return nullptr;
  }

 private:
  std::weak_ptr<std::unique_ptr<T>> ptr_;
};

template <class T>
class WeakPtrFactory {
 public:
  WeakPtrFactory(T* object) {
    std::unique_ptr<T> temp(object);
    object_ = std::make_shared<std::unique_ptr<T>>(std::move(temp));
  }
  ~WeakPtrFactory() { object_.get()->release(); }
  WeakPtr<T> GetWeak() { return WeakPtr<T>(object_); }

 private:
  std::shared_ptr<std::unique_ptr<T>> object_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_WEAK_PTR_FACTORY_H_
