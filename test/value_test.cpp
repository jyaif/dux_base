#include <cassert>
#include <cstdio>

#include "value_test.h"

#include "value.h"
#include "value_serialization.h"

void TestValue() {
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
