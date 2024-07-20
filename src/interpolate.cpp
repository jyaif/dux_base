#include "interpolate.h"

#include <cmath>
#include <cstdio>

#include "constants.h"

namespace dux {

float EaseInAndOutQuad(float percentage) {
  if (percentage < 0.5f) {
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

float InterpolateAngle(float angle_start, float angle_end, float percentage) {
  float d_angle = angle_end - angle_start;
  if (d_angle >= -kPi && d_angle <= kPi) {
    return angle_start + d_angle * percentage;
  }
  d_angle = fmodf(angle_end - angle_start, kTau);
  float short_angle = fmodf(2.f * d_angle, kTau) - d_angle;
  return angle_start + short_angle * percentage;
}

}  // namespace dux
