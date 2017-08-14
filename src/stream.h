#ifndef DUX_BASE_SRC_STREAM_H_
#define DUX_BASE_SRC_STREAM_H_

#include <cstdint>
#include <memory>
#include <vector>

namespace dux {

class OStream {
 public:
  void Write(void const* ptr, size_t byte_count);
  void Write(uint8_t const* ptr, size_t size);
  void Write(int8_t value);
  void Write(uint8_t value);
  void Write(int16_t value);
  void Write(uint16_t value);
  void Write(int32_t value);
  void Write(uint32_t value);
  void Write(int64_t value);
  void Write(uint64_t value);
  std::vector<uint8_t> const& Data() const;

 private:
  std::vector<uint8_t> data_;
};

class IStream {
 public:
  IStream(std::unique_ptr<std::vector<uint8_t>> data);
  IStream(std::vector<uint8_t> const& data);
  IStream(uint8_t const* ptr, size_t size);

  bool Read(void* ptr, size_t byte_count);
  bool Read(int8_t& value);
  bool Read(uint8_t& value);
  bool Read(int16_t& value);
  bool Read(uint16_t& value);
  bool Read(int32_t& value);
  bool Read(uint32_t& value);
  bool Read(int64_t& value);
  bool Read(uint64_t& value);

  bool SkipBytes(size_t byte_count);

 private:
  std::unique_ptr<std::vector<uint8_t>> data_;
  size_t current_index_;
};

}  // namespace dux

#endif
