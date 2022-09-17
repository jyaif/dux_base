#ifndef DUX_BASE_SRC_POOL_H_
#define DUX_BASE_SRC_POOL_H_

#include <memory>
#include <vector>

#include "ring_buffer.h"

namespace dux {

// Returns objects that are valid until `Reset` is called.
template <class T>
class Pool {
  public:
    Pool() {
        for (int i = 0; i < usage_history_.GetRawBuffer().size(); i++) {
            usage_history_.SaveToBuffer(0);
        }
    }

    template <typename... Arguments>
    T* New(Arguments&&... arguments) {
        // Here `new` does not actually allocate anything,
        // it constructs the object in the location returned by `GetNewPtr`.
        return new(GetNewPtr()) T(arguments...);
    }

    T* GetNewPtr() {
        if (used_ < regular_pool_.size()) {
            return &regular_pool_[used_++];
        } else {
            std::unique_ptr<T> smart_ptr = std::make_unique<T>();
            T* dumb_ptr = smart_ptr.get();
            overflow_.push_back(std::move(smart_ptr));
            used_++;
            return dumb_ptr;
        }
    }

    void Reset() {
        overflow_.clear();

        usage_history_.SaveToBuffer(used_);
        used_ = 0;

        size_t historical_max = std::numeric_limits<size_t>::min();
        for (auto v : usage_history_.GetRawBuffer()) {
            if (historical_max < v) {
                historical_max = v;
            }
        }

        const size_t lower_bound = historical_max + 4;
        const size_t upper_bound = (historical_max * 2) + 4;

        // If we are reserving less than `lower_bound`, increase reserve.
        // If we are reserving more than `upper_bound`, decrease reserve.
        if (regular_pool_.size() < lower_bound) {
            const size_t middle_ground = (upper_bound + lower_bound) / 2;
            regular_pool_.resize(middle_ground);
        } else {
            if (regular_pool_.size() > upper_bound) {
                std::vector<T> temp;
                // Only way to free memory in a std::vector is to swap it
                // with one that is empty.
                std::swap(temp, regular_pool_);
                const size_t middle_ground = (upper_bound + lower_bound) / 2;
                temp.resize(middle_ground);
            }
        }
    }
  private:
    size_t used_ = 0;
    std::vector<T> regular_pool_;
    std::vector<std::unique_ptr<T>> overflow_;
    RingBuffer<size_t, 16> usage_history_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_POOL_H_
