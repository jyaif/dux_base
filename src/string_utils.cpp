#include "string_utils.h"

#include <cstdarg>
#include <cstring>
#include <sstream>

namespace dux {

std::string TrimSpaces(std::string const& s) {
  if (s.empty()) {
    return s;
  }
  size_t start = 0;
  while (start < s.size() && s[start] == ' ') {
    start++;
  }
  size_t end = s.size() - 1;
  while (end >= start && s[end] == ' ') {
    end--;
  }
  return s.substr(start, end - start + 1);
}

std::vector<std::string> SplitNewLines(std::string const& string) {
  std::vector<std::string> lines;

  std::stringstream ss(string);
  std::string line;

  while (std::getline(ss, line, '\n')) {
    lines.push_back(line);
  }

  return lines;
}

std::vector<std::string> SplitString(std::string const& string,
                                     std::string const& delim) {
  std::vector<std::string> pieces;
  std::size_t current, previous = 0;
  current = string.find(delim);
  while (current != std::string::npos) {
    pieces.push_back(string.substr(previous, current - previous));
    previous = current + delim.length();
    current = string.find(delim, previous);
  }
  pieces.push_back(string.substr(previous, current - previous));
  return pieces;
}

bool HasEnding(std::string const& full_string, std::string const& ending) {
  if (full_string.length() >= ending.length()) {
    return (0 == full_string.compare(full_string.length() - ending.length(),
                                     ending.length(), ending));
  } else {
    return false;
  }
}

bool StartsWith(std::string_view full_string, std::string_view prefix) {
  if (full_string.size() < prefix.size()) {
    return false;
  }
  return memcmp(full_string.data(), prefix.data(), prefix.size()) == 0;
}

std::string StringFormatImpl(const char* format, va_list args) {
  va_list args2;
  va_copy(args2, args);

#if defined(_WIN32)
#define POSITIONAL_VSPRINTF _vsnprintf
#else
#define POSITIONAL_VSPRINTF vsnprintf
#endif
  size_t size = POSITIONAL_VSPRINTF(nullptr, 0, format, args);
  size++;  // Extra space for '\0'
  if (size == 0) {
    va_end(args2);
    return "";
  }

  std::unique_ptr<char[]> buf(new char[size]);
  POSITIONAL_VSPRINTF(buf.get(), size, format, args2);
  va_end(args2);
  return std::string(buf.get(),
                     buf.get() + size - 1  // We don't want the '\0' inside
  );
}

std::string StringFormat(const char* format, ...) {
  va_list args;
  va_start(args, format);
  auto s = StringFormatImpl(format, args);
  va_end(args);
  return s;
}

std::string StringFormat(const std::string_view format, ...) {
  va_list args;
  va_start(args, format);
  auto s = StringFormatImpl(format.data(), args);
  va_end(args);
  return s;
}

}  // namespace dux
