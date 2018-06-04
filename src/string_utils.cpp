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

}  // namespace dux
