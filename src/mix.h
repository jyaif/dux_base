#ifndef DUX_BASE_SRC_MIX_H_
#define DUX_BASE_SRC_MIX_H_

namespace dux {

template <class T, class S>
T Mix(T const& a, T const& b, S progress) {
  T diff = (b - a) * progress;
  return a + diff;
}

}  // namespace dux

#endif  // DUX_BASE_SRC_MIX_H_
