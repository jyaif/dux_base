#ifndef DUX_BASE_SRC_STRING_UTILS_H_
#define DUX_BASE_SRC_STRING_UTILS_H_

#include <string>
#include <vector>

namespace dux {

std::vector<std::string> SplitNewLines(std::string const& string);
std::vector<std::string> SplitString(std::string const& string,
                                     std::string const& delim);
bool HasEnding(std::string const& full_string, std::string const& ending);
bool StartsWith(std::string_view full_string, std::string_view prefix);

}  // namespace dux

#endif  // DUX_BASE_SRC_STRING_UTILS_H_
