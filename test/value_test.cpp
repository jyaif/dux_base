#include <cassert>
#include <cstdio>

#include "value.h"
#include "value_serialization.h"

#include "value_test.h"

void TestValue() {
  dux::Value v0(true);
  assert(v0.IsBool());
  assert(!v0.IsInt());
  assert(v0.GetBool() == true);

  dux::Value v1(static_cast<int64_t>(32));
  assert(v1.IsInt());
  assert(!v1.IsBool());
  assert(!v1.IsDouble());
  assert(v1.GetInt() == 32);
  assert(v1.GetDouble() == 32);

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
  assert(serialization1);
  auto v5 = dux::Deserialize(std::move(serialization1));
  assert(v5);
  assert(v5->FindKeyOfType("key1", dux::Value::Type::STRING)->GetString() ==
         "value1");

  dux::Value v6(dux::Value::Type::LIST);
  auto& l = v6.GetList();
  l.push_back(std::make_unique<dux::Value>(43.32));
  l.push_back(std::move(v5));
  auto serialization2 = dux::Serialize(v6);
  assert(serialization2);
  auto v7 = dux::Deserialize(std::move(serialization2));
  auto& l2 = v7->GetList();
  assert(l2.size() == 2);
  assert(l2[0]->IsDouble());
  assert(l2[0]->GetDouble() == 43.32);
  assert(l2[1]->IsDictionary());
  assert(l2[1]->FindKeyOfType("key1", dux::Value::Type::STRING)->GetString() ==
         "value1");
}
