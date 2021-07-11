#include <cassert>
#include <cstdio>
#include <map>
#include <string>

#include "value_test.h"

#include "value.h"
#include "value_from_to.h"
#include "value_serialization.h"

void TestToFromValueMap() {
  std::map<std::string, int64_t> m, m2;
  m["foo"] = 42;
  m["bar"] = 420;
  auto value = dux::ToValue(m);
  assert(FromValue(value.get(), m2));
  assert(m2.size() == 2);
  assert(m2["foo"] == 42);
  assert(m2["bar"] == 420);
}

void TestToFromValueVector() {
  std::vector<int64_t> v, v2;
  v.push_back(19);
  v.push_back(86);
  auto value = dux::ToValue(v);
  assert(FromValue(value.get(), v2));
  assert(v2.size() == 2);
  assert(v2[0] == 19);
  assert(v2[1] == 86);
}

void TestAssignValueFromKeyIfAvailable() {
  std::map<std::string, int64_t> m, m2;
  m["foo"] = 42;
  m["bar"] = 420;
  auto value = dux::ToValue(m);
  int64_t foo = -1;
  int64_t qux = -1;
  std::string baz;
  value->AssignValueFromKeyIfAvailable("foo", foo);
  value->AssignValueFromKeyIfAvailable("qux", qux);
  value->AssignValueFromKeyIfAvailable("baz", baz);
  assert(foo == 42);
  assert(qux == -1);
  assert(baz.empty());
}

void TestValue() {
  TestToFromValueMap();
  TestToFromValueVector();

  dux::Value v0(true);
  assert(v0.IsBool());
  assert(v0.GetBool() == true);

  dux::Value v1(static_cast<int64_t>(32));
  assert(!v1.IsBool());
  assert(v1.IsInt64());
  assert(v1.GetInt64() == 32);

  dux::Value v2(dux::Value::Type::STRING);
  assert(v2.IsString());
  assert(v2.GetString() == "");

  dux::Value v3("foo");
  assert(v3.IsString());
  assert(v3.GetString() == "foo");

  dux::Value v4(dux::Value::Type::DICTIONARY);
  assert(v4.RemoveKey("") == false);
  assert(v4.RemoveKey("key1") == false);
  assert(v4.FindKeyOfType("key1", dux::Value::Type::STRING) == nullptr);
  v4.SetKey("key1", std::make_unique<dux::Value>("value1"));
  assert(v4.FindKeyOfType("key1", dux::Value::Type::BOOLEAN) == nullptr);
  assert(v4.FindKeyOfType("key1", dux::Value::Type::STRING)->GetString() ==
         "value1");

  auto serialization1 = dux::Serialize(v4);
  auto v5 = dux::Deserialize(serialization1);
  assert(v5);
  assert(v5->FindKeyOfType("key1", dux::Value::Type::STRING)->GetString() ==
         "value1");

  dux::Value v6(dux::Value::Type::LIST);
  auto& l = v6.GetList();
  l.push_back(std::make_unique<dux::Value>(43));
  l.push_back(std::move(v5));
  auto serialization2 = dux::Serialize(v6);
  auto v7 = dux::Deserialize(serialization2);
  auto& l2 = v7->GetList();
  assert(l2.size() == 2);
  assert(l2[0]->IsInt64());
  assert(l2[0]->GetInt64() == 43);
  assert(l2[1]->IsDictionary());
  assert(l2[1]->FindKeyOfType("key1", dux::Value::Type::STRING)->GetString() ==
         "value1");
}
