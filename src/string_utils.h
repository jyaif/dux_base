#ifndef DUX_BASE_SRC_STRING_UTILS_H_
#define DUX_BASE_SRC_STRING_UTILS_H_

#include <memory>
#include <string>
#include <vector>

namespace dux {

std::vector<std::string> SplitNewLines(std::string const& string);
std::vector<std::string> SplitString(std::string const& string,
                                     std::string const& delim);
bool HasEnding(std::string const& full_string, std::string const& ending);
bool StartsWith(std::string_view full_string, std::string_view prefix);

template <typename... Args>
std::string StringFormat(const std::string& format, Args... args) {
  size_t size = snprintf(nullptr, 0, format.c_str(), args...) +
                1;  // Extra space for '\0'
  if (size <= 0) {
    return "";
  }
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(),
                     buf.get() + size - 1);  // We don't want the '\0' inside
}

}  // namespace dux

#endif  // DUX_BASE_SRC_STRING_UTILS_H_
