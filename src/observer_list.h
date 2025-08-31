#ifndef DUX_BASE_SRC_OBSERVER_LIST_H_
#define DUX_BASE_SRC_OBSERVER_LIST_H_

#include <cassert>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <vector>

#include "macros.h"
#include "thread_checker.h"

namespace dux {

template <class ObserverType>
// Class is not thread safe.
// Class supports some reentrancy:
// * You can add and remove observers while the `ForEachObserver` is running.
// * You must not call `ForEachObserver` from within a call of
// `ForEachObserver`.
class ObserverList {
 public:
  // Adds an observer to the list.
  // An observer must not be added to the same list more than once.
  void AddObserver(ObserverType* observer) {
#if !defined(NDEBUG)
    assert(thread_checker_.IsCreationThreadCurrent());
    for (ObserverType*& temp_observer : observers_) {
      assert(observer != temp_observer);
    }
#endif
    observers_.push_back(observer);
  }

  // Removes an observer from the list.
  // The observer must be present in the list.
  void RemoveObserver(ObserverType* observer) {
#if !defined(NDEBUG)
    assert(thread_checker_.IsCreationThreadCurrent());
#endif
    for (ObserverType*& temp_observer : observers_) {
      if (observer == temp_observer) {
        temp_observer = nullptr;
        return;
      }
    }
    assert(false);
  }

  void ForEachObserver(std::function<void(ObserverType&)> callback) {
#if !defined(NDEBUG)
    assert(thread_checker_.IsCreationThreadCurrent());
    assert(!for_each_is_running_);
    for_each_is_running_ = true;
#endif

    for (size_t i = 0; i < observers_.size(); i++) {
      ObserverType* observer = observers_[i];
      if (observer) {
        callback(*observer);
      }
    }
    DefragmentList();
#if !defined(NDEBUG)
    for_each_is_running_ = false;
#endif
  }

#if !defined(NDEBUG)
  // For tests only.
  int ObserverCount() const {
    int count = 0;
    for (auto const& observer : observers_) {
      if (observer != nullptr) {
        count++;
      }
    }
    return count;
  }
#endif

  // Removes holes in the list caused by the removal of observers.
  void DefragmentList() {
    // Iterate over the observers, filling the holes by swapping with the
    // last element of the array.
    int32_t count = (int32_t)observers_.size();
    // `i` and `count` need to be int32s because `i` can become negative.
    for (int32_t i = 0; i < count; i++) {
      if (observers_[i] == nullptr) {
        observers_[i] = observers_.back();
        count--;
        observers_.pop_back();
        // This is to handle the case where the last element of the vector was
        // nullptr.
        i--;
      }
    }
  }

  // A vector is used to keep the order of the observers deterministic.
  // Do not change to std::set.
  std::vector<ObserverType*> observers_;
#if !defined(NDEBUG)
  ThreadChecker thread_checker_;
  bool for_each_is_running_ = false;
#endif
};

}  // namespace dux

#endif  // DUX_BASE_SRC_OBSERVER_LIST_H_
