#ifndef DUX_BASE_SRC_RING_BUFFER_H_
#define DUX_BASE_SRC_RING_BUFFER_H_

#include <array>
#include <cstddef>

namespace dux {

template <typename T, size_t kSize>
class RingBuffer {
 public:
  RingBuffer() : current_index_(0) {}

  void SaveToBuffer(const T& value) {
    buffer_[current_index_ % kSize] = value;
    current_index_++;
  }

  std::array<T, kSize> const& GetRawBuffer() const { return buffer_; }

 private:
  std::array<T, kSize> buffer_;
  size_t current_index_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_RING_BUFFER_H_
