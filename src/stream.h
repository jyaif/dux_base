#ifndef DUX_BASE_SRC_STREAM_H_
#define DUX_BASE_SRC_STREAM_H_

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace dux {

class OStream {
 public:
  void Write(void const* ptr, size_t byte_count);
  void Write(int8_t const* ptr, size_t size);
  void Write(bool value);
  void Write(int8_t value);
  void Write(uint8_t value);
  void Write(int16_t value);
  void Write(uint16_t value);
  void Write(int32_t value);
  void Write(uint32_t value);
  void Write(int64_t value);
  void Write(uint64_t value);
  void Write(double value);
  void Write(std::string const& string);

  std::vector<int8_t> const& Data() const;

 private:
  std::vector<int8_t> data_;
};

class IStream {
 public:
  IStream(std::vector<int8_t> const& data);
  IStream(int8_t const* ptr, size_t size);

  [[nodiscard]] bool Read(void* ptr, size_t byte_count);
  [[nodiscard]] bool Read(bool& value);
  [[nodiscard]] bool Read(int8_t& value);
  [[nodiscard]] bool Read(uint8_t& value);
  [[nodiscard]] bool Read(int16_t& value);
  [[nodiscard]] bool Read(uint16_t& value);
  [[nodiscard]] bool Read(int32_t& value);
  [[nodiscard]] bool Read(uint32_t& value);
  [[nodiscard]] bool Read(int64_t& value);
  [[nodiscard]] bool Read(uint64_t& value);
  [[nodiscard]] bool Read(double& value);
  [[nodiscard]] bool Read(std::string& value);

  [[nodiscard]] bool SkipBytes(size_t byte_count);

  [[nodiscard]] bool IsAtEnd();

 private:
  int8_t const* ptr_;
  size_t size_;
  size_t current_index_;
};

}  // namespace dux

#endif
