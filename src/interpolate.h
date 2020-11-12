#ifndef DUX_BASE_SRC_INTERPOLATE_H_
#define DUX_BASE_SRC_INTERPOLATE_H_

namespace dux {

float QuadIn(float percentage);
float EaseInAndOutQuad(float percentage);
float Linear(float percentage);
float QuadOut(float percentage);
float ElasticIn(float percentage);
float ElasticOut(float percentage);

float InterpolateAngle(float angle_start, float angle_end, float percentage);

}  // namespace dux

#endif  // DUX_BASE_SRC_INTERPOLATE_H_
