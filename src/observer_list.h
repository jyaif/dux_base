#ifndef DUX_BASE_SRC_OBSERVER_LIST_H_
#define DUX_BASE_SRC_OBSERVER_LIST_H_

#include <cassert>
#include <mutex>
#include <vector>

namespace dux {

template <class ObserverType>
// Class is not thread safe.
// TODO: add debug checks that all methods are called from the same thread.
class ObserverList {
 public:
  // Add an observer to the list.  An observer should not be added to
  // the same list more than once.
  void AddObserver(ObserverType* observer) { observers_.push_back(observer); }

  // Remove an observer from the list if it is in the list.
  void RemoveObserver(ObserverType* observer) {
    for (ObserverType*& tempObserver : observers_) {
      if (observer == tempObserver) {
        tempObserver = observers_.back();
        observers_.pop_back();
        return;
      }
    }
    assert(false);
  }

  bool IsEmpty() { return observers_.empty(); }

  // A vector is used to keep the order of the observers deterministic.
  // Do not change to std::set.
  std::vector<ObserverType*> observers_;
};

#define FOR_EACH_OBSERVER(ObserverType, observerList, func)   \
  do {                                                        \
    for (ObserverType * observer : observerList.observers_) { \
      observer->func;                                         \
    }                                                         \
  } while (0)

template <class ObserverType>
class ThreadSafeObserverList {
 public:
  // Add an observer to the list.  An observer should not be added to
  // the same list more than once.
  void AddObserver(ObserverType* observer) {
    std::lock_guard<std::mutex> guard(list_mutex_);
    observers_.push_back(observer);
  }

  // Remove an observer from the list if it is in the list.
  void RemoveObserver(ObserverType* observer) {
    std::lock_guard<std::mutex> guard(list_mutex_);
    for (ObserverType*& tempObserver : observers_) {
      if (observer == tempObserver) {
        tempObserver = observers_.back();
        observers_.pop_back();
        return;
      }
    }
    assert(false);
  }

  bool IsEmpty() { return observers_.empty(); }

  // A vector is used to keep the order of the observers deterministic.
  // Do not change to std::set.
  std::vector<ObserverType*> observers_;
  std::mutex list_mutex_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_OBSERVER_LIST_H_
