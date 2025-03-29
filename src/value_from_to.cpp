#include "value_from_to.h"

namespace dux {

[[nodiscard]] bool FromValue(dux::Value const* value, int64_t& dest) {
  if (value == nullptr) {
    return false;
  }
  if (!value->IsInt64()) {
    return false;
  }
  dest = value->GetInt64();
  return true;
}

std::unique_ptr<dux::Value> ToValue(int64_t const& src) {
  return std::make_unique<dux::Value>(src);
}

[[nodiscard]] bool FromValue(dux::Value const* value, int32_t& dest) {
  int64_t dest2;
  bool v = FromValue(value, dest2);
  if (v) {
    dest = static_cast<int32_t>(dest2);
    return true;
  }
  return false;
}

std::unique_ptr<dux::Value> ToValue(int32_t const& src) {
  return std::make_unique<dux::Value>(src);
}

[[nodiscard]] bool FromValue(dux::Value const* value, bool& dest) {
  if (value == nullptr) {
    return false;
  }
  if (!value->IsBool()) {
    return false;
  }
  dest = value->GetBool();
  return true;
}

std::unique_ptr<dux::Value> ToValue(bool const& src) {
  return std::make_unique<dux::Value>(src);
}

[[nodiscard]] bool FromValue(dux::Value const* value, std::string& dest) {
  if (value == nullptr) {
    return false;
  }
  if (!value->IsString()) {
    return false;
  }
  dest = value->GetString();
  return true;
}

std::unique_ptr<dux::Value> ToValue(std::string const& src) {
  return std::make_unique<dux::Value>(src);
}

}  // namespace dux
