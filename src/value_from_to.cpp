#include "value_from_to.h"

namespace dux {

[[nodiscard]] bool FromValue(dux::Value const* value, int64_t& dest) {
  if (value == nullptr) {
    dest = 0;
    return true;
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

[[nodiscard]] bool FromValue(dux::Value const* value, std::string& dest) {
  if (value == nullptr) {
    dest = "";
    return true;
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
