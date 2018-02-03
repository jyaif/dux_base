#ifndef DUX_BASE_SRC_VALUE_H_
#define DUX_BASE_SRC_VALUE_H_

#include <stdint.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dux {

class Value {
 public:
  using DictionaryStorage = std::map<std::string, std::unique_ptr<Value>>;
  using ListStorage = std::vector<std::unique_ptr<Value>>;

  enum class Type : uint8_t {
    NONE = 0,
    BOOLEAN,
    INTEGER,
    DOUBLE,
    STRING,
    DICTIONARY,
    LIST
  };

  explicit Value(Type type);
  explicit Value(bool in_bool);
  explicit Value(int64_t in_int);
  explicit Value(double in_double);
  explicit Value(std::string const& in_string);
  explicit Value(char const* in_string);
  Value(Value&& that);
  ~Value();

  Type GetType() const { return type_; }

  bool IsNone() const { return GetType() == Type::NONE; }
  bool IsBool() const { return GetType() == Type::BOOLEAN; }
  bool IsInt() const { return GetType() == Type::INTEGER; }
  bool IsDouble() const { return GetType() == Type::DOUBLE; }
  bool IsString() const { return GetType() == Type::STRING; }
  bool IsDictionary() const { return GetType() == Type::DICTIONARY; }
  bool IsList() const { return GetType() == Type::LIST; }

  bool GetBool() const;
  int64_t GetInt() const;
  double GetDouble() const;
  const std::string& GetString() const;
  ListStorage& GetList();
  const ListStorage& GetList() const;
  DictionaryStorage& GetDictionary();
  const DictionaryStorage& GetDictionary() const;

  Value* FindKeyOfType(std::string const& key, Type type);
  const Value* FindKeyOfType(std::string const& key, Type type) const;

  void SetKey(std::string const& key, std::unique_ptr<Value> value);
  bool RemoveKey(std::string const& key);

 private:
  Type type_;
  union {
    bool bool_value_;
    int64_t int_value_;
    double double_value_;
    std::string string_value_;
    DictionaryStorage dictionary_;
    ListStorage list_;
  };
};

}  // namespace dux

#endif  // DUX_BASE_SRC_VALUE_H_
