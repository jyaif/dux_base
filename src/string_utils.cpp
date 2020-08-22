#include "string_utils.h"

#include <sstream>

namespace dux {

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

}  // namespace dux
