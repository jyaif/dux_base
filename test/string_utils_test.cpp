#include "string_utils_test.h"

#include <cassert>

#include "string_utils.h"

void VecEq(std::vector<std::string> v1, std::vector<std::string> v2) {
  assert(v1.size() == v2.size());
  for (int i = 0; i < v1.size(); i++) {
    assert(v1[i] == v2[i]);
  }
}

void TestStringUtils() {
  VecEq(dux::SplitNewLines("A"), {"A"});
  VecEq(dux::SplitNewLines("\n"), {""});
  VecEq(dux::SplitNewLines("A\nA"), {"A", "A"});
  VecEq(dux::SplitNewLines("A\n\nA"), {"A", "", "A"});

  VecEq(dux::SplitString("AA", "XY"), {"AA"});
  VecEq(dux::SplitString("AXA", "XY"), {"AXA"});
  VecEq(dux::SplitString("AYA", "XY"), {"AYA"});
  VecEq(dux::SplitString("AXYA", "XY"), {"A", "A"});
  VecEq(dux::SplitString("AXYXYA", "XY"), {"A", "", "A"});
  VecEq(dux::SplitString("AVWXYZA", "VWXYZ"), {"A", "A"});

  assert(dux::HasEnding("", "XYZ") == false);
  assert(dux::HasEnding("A", "XYZ") == false);
  assert(dux::HasEnding("ABC", "XYZ") == false);
  assert(dux::HasEnding("XYZ.ABC", "XYZ") == false);
  assert(dux::HasEnding("XYZ", "") == true);
  assert(dux::HasEnding("XYZ", "XYZ") == true);
  assert(dux::HasEnding("ABC.XYZ", "XYZ") == true);
  assert(dux::HasEnding("XYZ.XYZ", "XYZ") == true);

  assert(dux::StartsWith("", "XYZ") == false);
  assert(dux::StartsWith("A", "XYZ") == false);
  assert(dux::StartsWith("ABC.XYZ", "XYZ") == false);
  assert(dux::StartsWith("XYZ", "XYZ") == true);
  assert(dux::StartsWith("XYZ.ABC", "XYZ") == true);

  assert(dux::StringFormat("a %s a", "bb") == "a bb a");
  assert(dux::StringFormat("%1$s %2$s", "aa", "bb") == "aa bb");
  assert(dux::StringFormat("%2$s %1$s", "aa", "bb") == "bb aa");
}
