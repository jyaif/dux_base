#ifndef DUX_BASE_SRC_VALUE_FROM_TO_H_
#define DUX_BASE_SRC_VALUE_FROM_TO_H_

#include <string>

#include "value.h"

namespace dux {

// int64_t
[[nodiscard]] bool FromValue(dux::Value const* value, int64_t& dest);
std::unique_ptr<dux::Value> ToValue(int64_t const& src);

// int32_t
[[nodiscard]] bool FromValue(dux::Value const* value, int32_t& dest);
std::unique_ptr<dux::Value> ToValue(int32_t const& src);

// std::string
[[nodiscard]] bool FromValue(dux::Value const* value, std::string& dest);
std::unique_ptr<dux::Value> ToValue(std::string const& src);

// std::vector
template <typename T>
[[nodiscard]] bool FromValue(dux::Value const* value, std::vector<T>& dest) {
  dest.clear();
  if (!value) {
    return false;
  }
  if (!value->IsList()) {
    return false;
  }

  auto& vec_of_values = value->GetList();
  for (auto& v : vec_of_values) {
    T o;
    if (!dux::FromValue(v.get(), o)) {
      return false;
    }
    dest.push_back(o);
  }
  return true;
}

template <typename T>
std::unique_ptr<dux::Value> ToValue(std::vector<T> const& elements) {
  auto value = std::make_unique<dux::Value>(dux::Value::Type::LIST);
  auto& vec = value->GetList();
  for (auto const& element : elements) {
    vec.push_back(dux::ToValue(element));
  }
  return value;
}

// std::map
template <typename T>
[[nodiscard]] bool FromValue(dux::Value const* value,
                             std::map<std::string, T>& dest) {
  dest.clear();
  if (!value) {
    return false;
  }
  if (!value->IsDictionary()) {
    return false;
  }

  auto& map_of_values = value->GetDictionary();
  for (auto& v : map_of_values) {
    T o;
    if (!dux::FromValue(v.second.get(), o)) {
      return false;
    }
    dest[v.first] = o;
  }
  return true;
}

template <typename T>
std::unique_ptr<dux::Value> ToValue(std::map<std::string, T> const& map) {
  auto value = std::make_unique<dux::Value>(dux::Value::Type::DICTIONARY);
  auto& dic = value->GetDictionary();
  for (auto const& it : map) {
    dic[it.first] = dux::ToValue(it.second);
  }
  return value;
}

}  // namespace dux

#endif  // DUX_BASE_SRC_VALUE_FROM_TO_H_
