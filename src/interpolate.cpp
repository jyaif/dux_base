#include "interpolate.h"

#include <cmath>

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

float QuadIn(float percentage) {
  return percentage * percentage;
}

float QuadOut(float percentage) {
  return 1 - QuadIn(1 - percentage);
}

#include "stdio.h"

float ElasticIn(float t) {
  const double pi(M_PI);
  const double v(t - 1);
  const double p(0.3);
  return -std::pow(2, 10 * v) * std::sin((v - p / 4) * 2 * pi / p);
  /*
    float a = 10 * (percentage - 1);
    float b = sin(((percentage - 1) - 0.3 / 4) * 2 * M_PI / 4);
    float c = a * b;
    float d = pow(2, c);
    printf("%f %f %f %f\n", a,b,c,d);
    return d;
    */
  // return pow(-2, 10 * (percentage - 1)) *
  //        sin(((percentage - 1) - 0.3 / 4) * 2 * M_PI / 4);
}

float ElasticOut(float percentage) {
  return 1 - ElasticIn(1 - percentage);
}

}  // namespace dux
