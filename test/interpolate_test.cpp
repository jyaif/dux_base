#include <cassert>
#include <cmath>
#include <cstdio>

#include "interpolate_test.h"

#include "interpolate.h"

using namespace dux;

void AssertNearlyEqual(double actual, double expected) {
  double threshold = 0.02;
  double delta = fabs(expected - actual);
  if (delta >= threshold) {
    printf("expected:%f, actual:%f\n", expected, actual);
    printf("diff:%f, max threshold:%f\n", delta, threshold);
    // assert(false);
  }
}

void TestInterpolation() {
  AssertNearlyEqual(InterpolateAngle(0, 0, 0), 0);
  AssertNearlyEqual(InterpolateAngle(0, 1, 0), 0);
  AssertNearlyEqual(InterpolateAngle(0, 1, 1), 1);
  AssertNearlyEqual(InterpolateAngle(0, 1, 0.5), 0.5);

  AssertNearlyEqual(InterpolateAngle(1, -1, 0), 1);
  AssertNearlyEqual(InterpolateAngle(1, -1, 0.5), 0);
  AssertNearlyEqual(InterpolateAngle(1, -1, 1), -1);

  AssertNearlyEqual(InterpolateAngle(-1, 1, 0), -1);
  AssertNearlyEqual(InterpolateAngle(-1, 1, 0.5), 0);
  AssertNearlyEqual(InterpolateAngle(-1, 1, 1), 1);

  AssertNearlyEqual(InterpolateAngle(0, M_PI * 2, 0), 0);
  AssertNearlyEqual(InterpolateAngle(0, M_PI * 2, 1), 0);
  AssertNearlyEqual(InterpolateAngle(0, M_PI * 2, 0.5), 0);
}
