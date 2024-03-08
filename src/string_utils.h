#ifndef DUX_BASE_SRC_STRING_UTILS_H_
#define DUX_BASE_SRC_STRING_UTILS_H_

#include <memory>
#include <string>
#include <vector>

namespace dux {

// Removes the leading and trailing spaces.
std::string TrimSpaces(std::string const& s);

std::vector<std::string> SplitNewLines(std::string const& string);
std::vector<std::string> SplitString(std::string const& string,
                                     std::string const& delim);
bool HasEnding(std::string const& full_string, std::string const& ending);
bool StartsWith(std::string_view full_string, std::string_view prefix);

template <typename... Args>
std::string StringFormat(const char* format, Args... args) {
#if defined(_WIN32)
#define POSITIONAL_SPRINTF _sprintf_p
#else
#define POSITIONAL_SPRINTF snprintf
#endif

  size_t size = POSITIONAL_SPRINTF(nullptr, 0, format, args...) +
                1;  // Extra space for '\0'
  if (size <= 0) {
    return "";
  }

  std::unique_ptr<char[]> buf(new char[size]);
  POSITIONAL_SPRINTF(buf.get(), size, format, args...);
  return std::string(buf.get(),
                     buf.get() + size - 1);  // We don't want the '\0' inside
}

template <typename... Args>
std::string StringFormat(const std::string& format, Args... args) {
  return StringFormat(format.c_str(), args...);
}

}  // namespace dux

#endif  // DUX_BASE_SRC_STRING_UTILS_H_
