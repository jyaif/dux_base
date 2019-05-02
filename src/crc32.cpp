#include "crc32.h"

#include <array>
// #include <cmath>
// #include <cstdio>

namespace dux {

std::array<uint32_t, 256> GenerateTable() {
  std::array<uint32_t, 256> table;
  uint32_t polynomial = 0xEDB88320;
  for (uint32_t i = 0; i < 256; i++) {
    uint32_t c = i;
    for (size_t j = 0; j < 8; j++) {
      if (c & 1) {
        c = polynomial ^ (c >> 1);
      } else {
        c >>= 1;
      }
    }
    table[i] = c;
  }
  return table;
}

uint32_t ComputeCRC32(const void* buffer, size_t length) {
  static auto table = GenerateTable();
  uint32_t c = 0xFFFFFFFF;
  const uint8_t* u = static_cast<const uint8_t*>(buffer);
  for (size_t i = 0; i < length; ++i) {
    c = table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
  }
  return c ^ 0xFFFFFFFF;
}

}  // namespace dux
