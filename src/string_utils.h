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

bool StringContainsString(std::string_view main_str, std::string_view substr);

// In `str`, substitutes `find` by `replace`
std::string StringReplaceAll(const std::string& str,
                             const std::string& find,
                             const std::string& replace);

std::string StringFormat(const char* format, ...)
    __attribute__((__format__(__printf__, 1, 2)));
std::string StringFormat(const std::string_view format, ...);

}  // namespace dux

#endif  // DUX_BASE_SRC_STRING_UTILS_H_
