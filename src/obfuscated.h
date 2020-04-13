#ifndef DUX_BASE_SRC_OBFUSCATED_H_
#define DUX_BASE_SRC_OBFUSCATED_H_

#include <cstdlib>

namespace dux {

template <class T>
class Obfuscated {
 public:
  Obfuscated(Obfuscated<T> const& other)
      : obfuscated_value_(other.obfuscated_value_),
        obfuscator_(other.obfuscator_) {}
  Obfuscated(T initial_value) {
    InitializeObfuscator();
    SetValue(initial_value);
  }

  Obfuscated() {
    InitializeObfuscator();
    SetValue(0);
  }

  T GetValue() { return obfuscated_value_ ^ obfuscator_; }
  void SetValue(T new_value) { obfuscated_value_ = new_value ^ obfuscator_; }

 private:
  void InitializeObfuscator() {
    do {
      obfuscator_ = rand();
    } while (obfuscator_ == 0 || obfuscator_ == 1);
  }
  T obfuscated_value_;
  T obfuscator_;
};

}  // namespace dux

#endif  // DUX_BASE_SRC_OBFUSCATED_H_
