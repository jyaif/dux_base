#ifndef DUX_BASE_SRC_VALUE_SERIALIZATION_H_
#define DUX_BASE_SRC_VALUE_SERIALIZATION_H_

#include "value.h"

namespace dux {

std::vector<int8_t> Serialize(Value const& value);

std::unique_ptr<Value> Deserialize(std::vector<int8_t> data);

}  // namespace dux

#endif  // DUX_BASE_SRC_VALUE_SERIALIZATION_H_
