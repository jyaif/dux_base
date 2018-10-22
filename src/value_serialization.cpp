#include "value_serialization.h"

#include "stream.h"

namespace dux {

void WriteValueToStream(Value const& value, dux::OStream& s) {
  int8_t type = static_cast<int8_t>(value.GetType());
  s.Write(type);
  switch (value.GetType()) {
    case Value::Type::NONE:
      break;
    case Value::Type::BOOLEAN:
      s.Write(value.GetBool());
      break;
    case Value::Type::INT64:
      s.Write(value.GetInt64());
      break;
    case Value::Type::STRING:
      s.Write(value.GetString());
      break;
    case Value::Type::DICTIONARY: {
      auto& dict = value.GetDictionary();
      int64_t number_of_entries = dict.size();
      s.Write(number_of_entries);
      for (auto& e : dict) {
        std::string const& key = e.first;
        Value const& value = *e.second.get();
        s.Write(key);
        WriteValueToStream(value, s);
      }
    } break;
    case Value::Type::LIST: {
      auto& list = value.GetList();
      int64_t number_of_entries = list.size();
      s.Write(number_of_entries);
      for (auto const& value : list) {
        WriteValueToStream(*value.get(), s);
      }
    } break;
  }
}

std::vector<int8_t> Serialize(Value const& value) {
  dux::OStream s;
  WriteValueToStream(value, s);
  return s.Data();
}

std::unique_ptr<Value> ReadValueFromStream(dux::IStream& stream) {
  int8_t t;
  if (stream.Read(t)) {
    Value::Type type = static_cast<Value::Type>(t);
    switch (type) {
      case Value::Type::NONE:
        return std::make_unique<Value>(Value::Type::NONE);
      case Value::Type::BOOLEAN: {
        bool b;
        if (!stream.Read(b)) {
          break;
        }
        return std::make_unique<Value>(b);
      }
      case Value::Type::INT64: {
        int64_t d;
        if (!stream.Read(d)) {
          break;
        }
        return std::make_unique<Value>(d);
      }
      case Value::Type::STRING: {
        std::string s;
        if (!stream.Read(s)) {
          break;
        }
        return std::make_unique<Value>(s);
      }
      case Value::Type::DICTIONARY: {
        int64_t number_of_entries;
        if (!stream.Read(number_of_entries))
          break;
        auto dict_value = std::make_unique<Value>(Value::Type::DICTIONARY);
        Value::DictionaryStorage& dictionary = dict_value->GetDictionary();
        std::string key;
        for (int64_t i = 0; i < number_of_entries; i++) {
          if (!stream.Read(key))
            return std::unique_ptr<Value>();
          auto value = ReadValueFromStream(stream);
          if (!value.get())
            return std::unique_ptr<Value>();
          dictionary[key] = std::move(value);
        }
        return dict_value;
      }
      case Value::Type::LIST: {
        int64_t number_of_entries;
        if (!stream.Read(number_of_entries))
          break;
        auto list_value = std::make_unique<Value>(Value::Type::LIST);
        Value::ListStorage& list = list_value->GetList();
        for (int64_t i = 0; i < number_of_entries; i++) {
          auto value = ReadValueFromStream(stream);
          if (!value.get())
            return std::unique_ptr<Value>();
          list.push_back(std::move(value));
        }
        return list_value;
      }
    }
  }
  return std::unique_ptr<Value>();
}

std::unique_ptr<Value> Deserialize(std::vector<int8_t> data) {
  dux::IStream s(std::move(data));
  auto v = ReadValueFromStream(s);
  if (s.IsAtEnd())
    return v;
  return std::unique_ptr<Value>();
}

}  // namespace dux
