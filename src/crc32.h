#ifndef DUX_BASE_SRC_CRC32_H_
#define DUX_BASE_SRC_CRC32_H_

#include <cstddef>
#include <ctype.h>
#include <cstdint>

namespace dux {

uint32_t ComputeCRC32(const void* buffer, size_t length);

}  // namespace dux

#endif  // DUX_BASE_SRC_CRC32_H_
