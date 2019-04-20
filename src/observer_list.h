#ifndef DUX_BASE_SRC_OBSERVER_LIST_H_
#define DUX_BASE_SRC_OBSERVER_LIST_H_

#include <cassert>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <vector>

#include "thread_checker.h"

namespace dux {

template <class ObserverType>
class ObserverLink {
  ObserverType* observer_ = nullptr;
  ObserverLink<ObserverType>* next_ = nullptr;
};

template <class ObserverType>
// Class is not thread safe.
// TODO: add debug checks that all methods are called from the same thread.
class ObserverList {
 public:
  // Add an observer to the list.  An observer should not be added to
  // the same list more than once.
  void AddObserver(ObserverType* observer) {
    VerifyThread();
    new_observers_.push_back(observer);
  }

  // Remove an observer from the list if it is in the list.
  void RemoveObserver(ObserverType* observer) {
    VerifyThread();
    for (ObserverType*& temp_observer : observers_) {
      if (observer == temp_observer) {
        temp_observer = nullptr;
        deletion_count_++;
        return;
      }
    }
    for (ObserverType*& temp_observer : new_observers_) {
      if (observer == temp_observer) {
        temp_observer = new_observers_.back();
        new_observers_.pop_back();
        return;
      }
    }
    assert(false);
  }

  void ForEachObserver(std::function<void(ObserverType&)> callback) {
    VerifyThread();
    PrepareObserverListForIteration();
    for (ObserverType* observer : observers_) {
      if (observer) {
        callback(*observer);
      }
    }
  }

#ifndef NDEBUG
  // For tests only.
  int ObserverCount() {
    int count = 0;
    for (auto const& observer : observers_) {
      if (observer != nullptr) {
        count++;
      }
    }
    return count + new_observers_.size();
  }
#endif

  void PrepareObserverListForIteration() {
    if (deletion_count_ > 0) {
      for (ObserverType*& tempObserver : observers_) {
        if (tempObserver == nullptr) {
          tempObserver = observers_.back();
          observers_.pop_back();
          deletion_count_--;
          if (deletion_count_ == 0) {
            break;
          }
        }
      }
    }
    if (!new_observers_.empty()) {
      observers_.insert(std::end(observers_), std::begin(new_observers_),
                        std::end(new_observers_));
      new_observers_.clear();
    }
  }

  void VerifyThread() {
#ifndef NDEBUG
    if (!thread_checker_) {
      thread_checker_ = std::make_unique<ThreadChecker>();
    } else {
      assert(thread_checker_->IsCreationThreadCurrent());
    }
#endif
  }

  // A vector is used to keep the order of the observers deterministic.
  // Do not change to std::set.
  std::vector<ObserverType*> observers_;
  std::vector<ObserverType*> new_observers_;
  int deletion_count_ = 0;
#ifndef NDEBUG
  std::unique_ptr<ThreadChecker> thread_checker_;
#endif
};

#define FOR_EACH_OBSERVER(ObserverType, observerList, func)   \
  do {                                                        \
    observerList.VerifyThread();                              \
    observerList.PrepareObserverListForIteration();           \
    for (ObserverType * observer : observerList.observers_) { \
      if (observer) {                                         \
        observer->func;                                       \
      }                                                       \
    }                                                         \
  } while (0)

// Deadlocks in case of re-entrance.
template <class ObserverType>
class ThreadSafeObserverList {
 public:
  // Add an observer to the list.  An observer should not be added to
  // the same list more than once.
  void AddObserver(ObserverType* observer) {
    std::lock_guard<std::mutex> guard(new_observers_mutex_);
    new_observers_.push_back(observer);
  }

  // Remove an observer from the list if it is in the list.
  void RemoveObserver(ObserverType* observer) {
    std::lock_guard<std::mutex> guard(delete_observers_mutex_);
    delete_observers_.push_back(observer);
  }

  void ForEachObserver(std::function<void(ObserverType&)> callback) {
    PrepareObserverListForIteration();
    std::lock_guard<std::mutex> guard(observers_mutex_);
    for (ObserverType* observer : observers_) {
      std::lock_guard<std::mutex> guard(delete_observers_mutex_);
      if (observer) {
        if (!delete_observers_.empty()) {
          for (ObserverType* observer_to_delete : delete_observers_) {
            if (observer == observer_to_delete) {
              continue;
            }
          }
        }
        callback(*observer);
      }
    }
  }

#ifndef NDEBUG
  // For tests only.
  int ObserverCount() {
    int count = 0;
    for (auto const& observer : observers_) {
      if (observer != nullptr) {
        count++;
      }
    }
    return count + new_observers_.size() - delete_observers_.size();
  }
#endif

 private:
  void PrepareObserverListForIteration() {
    {
      std::lock_guard<std::mutex> guard(delete_observers_mutex_);
      if (!delete_observers_.empty()) {
        std::lock_guard<std::mutex> guard2(observers_mutex_);
        for (ObserverType*& observer : observers_) {
          for (auto* observer_to_delete : delete_observers_) {
            if (observer == observer_to_delete) {
              observer = observers_.back();
              observers_.pop_back();
            }
          }
        }
        delete_observers_.clear();
      }
    }
    {
      std::lock_guard<std::mutex> guard(new_observers_mutex_);
      if (!new_observers_.empty()) {
        observers_.insert(std::end(observers_), std::begin(new_observers_),
                          std::end(new_observers_));
        new_observers_.clear();
      }
    }
  }

  // A vector is used to keep the order of the observers deterministic.
  // Do not change to std::set.
  std::vector<ObserverType*> observers_;
  std::vector<ObserverType*> new_observers_;
  std::vector<ObserverType*> delete_observers_;
  std::mutex observers_mutex_;
  std::mutex new_observers_mutex_;
  std::mutex delete_observers_mutex_;
  int deletion_count_ = 0;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_OBSERVER_LIST_H_
