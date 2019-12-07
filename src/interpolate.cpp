#include "interpolate.h"

#include <cmath>
#include <cstdio>

namespace dux {

float EaseInAndOutQuad(float percentage) {
  if (percentage < 0.5) {
    return 2 * percentage * percentage;
  }
  percentage = 1 - percentage;
  return 1 - 2 * percentage * percentage;
}

float Linear(float percentage) {
  return percentage;
}

//                   *
//                  *
//                **
//           *****
//  *********
float QuadIn(float percentage) {
  return percentage * percentage;
}

//            *********
//       *****
//     **
//    *
//   *
float QuadOut(float percentage) {
  return 1 - QuadIn(1 - percentage);
}

}  // namespace dux
