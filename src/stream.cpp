#include "stream.h"

#include <cassert>
#include <cstddef>

namespace dux {

void OStream::Write(void const* ptr, size_t byte_count) {
  assert(ptr);
  Write(static_cast<uint8_t const*>(ptr), byte_count);
}

void OStream::Write(uint8_t const* ptr, size_t size) {
  assert(ptr);
  while (size) {
    data_.push_back(*ptr);
    ptr++;
    size--;
  }
}

void OStream::Write(bool value) {
  static_assert(sizeof(bool) == 1, "");
  Write(&value, sizeof(value));
}

void OStream::Write(int8_t value) {
  Write(&value, sizeof(value));
}

void OStream::Write(uint8_t value) {
  Write(&value, sizeof(value));
}

void OStream::Write(int16_t value) {
  Write(&value, sizeof(value));
}

void OStream::Write(uint16_t value) {
  Write(&value, sizeof(value));
}

void OStream::Write(int32_t value) {
  Write(&value, sizeof(value));
}

void OStream::Write(uint32_t value) {
  Write(&value, sizeof(value));
}

void OStream::Write(int64_t value) {
  Write(&value, sizeof(value));
}

void OStream::Write(uint64_t value) {
  Write(&value, sizeof(value));
}

void OStream::Write(std::string const& string) {
  uint32_t size = string.size();
  Write(size);
  Write(string.data(), string.length());
}

std::vector<uint8_t> const& OStream::Data() const {
  return data_;
}

IStream::IStream(std::unique_ptr<std::vector<uint8_t>> data)
    : data_(std::move(data)), current_index_(0) {
  assert(data_);
}

IStream::IStream(std::vector<uint8_t> const& data) : current_index_(0) {
  data_ = std::make_unique<std::vector<uint8_t>>();
  *data_ = data;
}

IStream::IStream(uint8_t const* ptr, size_t size) : current_index_(0) {
  assert(ptr);
  data_ = std::make_unique<std::vector<uint8_t>>(size);
  for (size_t i = 0; i < size; i++) {
    (*data_)[i] = ptr[i];
  }
}

bool IStream::Read(void* ptr, size_t byte_count) {
  assert(ptr);
  if (current_index_ + byte_count > data_->size()) {
    memset(ptr, 0, byte_count);
    return false;
  } else {
    memcpy(ptr, &((*data_)[current_index_]), byte_count);
    current_index_ += byte_count;
    return true;
  }
}

bool IStream::Read(bool& value) {
  static_assert(sizeof(bool) == 1, "");
  return Read(&value, sizeof(bool));
}

bool IStream::Read(int8_t& value) {
  return Read(&value, sizeof(int8_t));
}

bool IStream::Read(uint8_t& value) {
  return Read(&value, sizeof(uint8_t));
}
bool IStream::Read(int16_t& value) {
  return Read(&value, sizeof(int16_t));
}

bool IStream::Read(uint16_t& value) {
  return Read(&value, sizeof(uint16_t));
}

bool IStream::Read(int32_t& value) {
  return Read(&value, sizeof(int32_t));
}

bool IStream::Read(uint32_t& value) {
  return Read(&value, sizeof(uint32_t));
}

bool IStream::Read(int64_t& value) {
  return Read(&value, sizeof(int64_t));
}

bool IStream::Read(uint64_t& value) {
  return Read(&value, sizeof(uint64_t));
}

bool IStream::Read(std::string& value) {
  uint32_t size;
  if (Read(size)) {
    if (size > 32000) {
      return false;
    }
    uint8_t buffer[size];
    if (Read(buffer, size)) {
      value = std::string(buffer, buffer + size);
      return true;
    }
  }
  return false;
}

bool IStream::SkipBytes(size_t byte_count) {
  if (current_index_ + byte_count > data_->size()) {
    return false;
  }
  current_index_ += byte_count;
  return true;
}

}  // namespace dux
