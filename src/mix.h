#ifndef DUX_BASE_SRC_MIX_H_
#define DUX_BASE_SRC_MIX_H_

namespace dux {

template <class T, class S>
[[nodiscard]] T Mix(T const& a, T const& b, S progress) {
  T diff = (b - a) * progress;
  return a + diff;
}

template <class T, class S>
[[nodiscard]] T Mix(T const& a, T const& b, S progress, T const& thresh_hold) {
  T diff = (b - a);
  if (diff > thresh_hold || diff < -thresh_hold)
    return b;
  return a + (diff * progress);
}

}  // namespace dux

#endif  // DUX_BASE_SRC_MIX_H_
