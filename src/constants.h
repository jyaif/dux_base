#ifndef DUX_BASE_SRC_BASE_CONSTANTS_H_
#define DUX_BASE_SRC_BASE_CONSTANTS_H_

namespace dux {

// M_PI is not part of the standard, and when it is present it is a double and
// not a float.
constexpr float kPi = 3.14159265359f;
constexpr float kTau = 6.28318530718f;

}  // namespace dux

#endif  // DUX_BASE_SRC_BASE_CONSTANTS_H_
