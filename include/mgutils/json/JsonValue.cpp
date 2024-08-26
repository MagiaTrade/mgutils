//
// Created by Arthur Motelevicz on 23/08/24.
//

#include "JsonValue.h"
#include "JsonDocument.h"
#include "Exceptions.h"

namespace mgutils
{

  //private
  JsonValue::JsonValue(const rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator):
      _value(value, allocator),_allocator(allocator) {}

  // Movement constructor
  JsonValue::JsonValue(JsonValue&& other) noexcept:
      _value(std::move(other._value)), _allocator(other._allocator){}

  JsonValue& JsonValue::operator=(JsonValue&& other) noexcept
  {
    if (this != &other) {
      _value = std::move(other._value);  // Moves rapidjson::Value
      _allocator = other._allocator;  // Copies allocator
    }
    return *this;
  }

  JsonValue::JsonValue(const rapidjson::Value& value, const std::shared_ptr<JsonDocument>& doc):
      _value(value, doc->getAllocator()),_allocator(doc->getAllocator()) {}

  JsonValue::JsonValue(const std::shared_ptr<JsonDocument>& doc):
      _value(rapidjson::kNullType), _allocator(doc->getAllocator()) {}

  JsonValue::JsonValue(const std::string& value, const std::shared_ptr<JsonDocument>& doc):
      _value(rapidjson::kStringType), _allocator(doc->getAllocator())
  {
    _value.SetString(value.c_str(), doc->getAllocator());
  }

  JsonValue::JsonValue(const char* value, const std::shared_ptr<JsonDocument>& doc):
      _value(rapidjson::kStringType), _allocator(doc->getAllocator())
  {
    _value.SetString(value, doc->getAllocator());
  }

  JsonValue::JsonValue(bool value, const std::shared_ptr<JsonDocument>& doc):
      _value(rapidjson::kFalseType), _allocator(doc->getAllocator())
  {
    _value.SetBool(value);
  }

  JsonValue::JsonValue(int value, const std::shared_ptr<JsonDocument>& doc):
      _value(rapidjson::kNumberType), _allocator(doc->getAllocator())
  {
    _value.SetInt(value);
  }

  JsonValue::JsonValue(int64_t value, const std::shared_ptr<JsonDocument>& doc):
      _value(rapidjson::kNumberType), _allocator(doc->getAllocator())
  {
    _value.SetInt64(value);
  }

  JsonValue::JsonValue(uint value, const std::shared_ptr<JsonDocument>& doc):
      _value(rapidjson::kNumberType), _allocator(doc->getAllocator())
  {
    _value.SetUint(value);
  }

  JsonValue::JsonValue(uint64_t value, const std::shared_ptr<JsonDocument>& doc):
      _value(rapidjson::kNumberType), _allocator(doc->getAllocator())
  {
    _value.SetUint64(value);
  }

  JsonValue::JsonValue(float value, const std::shared_ptr<JsonDocument>& doc)
      : _value(rapidjson::kNumberType), _allocator(doc->getAllocator())
  {
    _value.SetFloat(value);
  }

  JsonValue::JsonValue(double value, const std::shared_ptr<JsonDocument>& doc):
      _value(rapidjson::kNumberType), _allocator(doc->getAllocator())
  {
    _value.SetDouble(value);
  }

  bool JsonValue::hasBool(const std::string& memberName) const {
    return _value.HasMember(memberName.c_str()) && _value[memberName.c_str()].IsBool();
  }

  bool JsonValue::hasNumber(const std::string& memberName) const {
    return _value.HasMember(memberName.c_str()) && _value[memberName.c_str()].IsNumber();
  }

  bool JsonValue::hasString(const std::string& memberName) const {
    return _value.HasMember(memberName.c_str()) && _value[memberName.c_str()].IsString();
  }

  bool JsonValue::hasObject(const std::string& memberName) const {
    return _value.HasMember(memberName.c_str()) && _value[memberName.c_str()].IsObject();
  }

  bool JsonValue::hasArray(const std::string& memberName) const {
    return _value.HasMember(memberName.c_str()) && _value[memberName.c_str()].IsArray();
  }

  bool JsonValue::exists(const std::string& memberName) const {
    return _value.HasMember(memberName.c_str());
  }

  std::optional<std::string> JsonValue::getString(const std::string& key) const {
    if (_value.HasMember(key.c_str()) && _value[key.c_str()].IsString()) {
      return _value[key.c_str()].GetString();
    }
    return std::nullopt;
  }

  std::optional<int> JsonValue::getInt(const std::string& key) const {
    if (_value.HasMember(key.c_str()) && _value[key.c_str()].IsInt()) {
      return _value[key.c_str()].GetInt();
    }
    return std::nullopt;
  }

  std::optional<unsigned> JsonValue::getUint(const std::string& key) const {
    if (_value.HasMember(key.c_str()) && _value[key.c_str()].IsUint()) {
      return _value[key.c_str()].GetUint();
    }
    return std::nullopt;
  }

  std::optional<int64_t> JsonValue::getInt64(const std::string& key) const {
    if (_value.HasMember(key.c_str()) && _value[key.c_str()].IsInt64()) {
      return _value[key.c_str()].GetInt64();
    }
    return std::nullopt;
  }

  std::optional<uint64_t> JsonValue::getUint64(const std::string& key) const {
    if (_value.HasMember(key.c_str()) && _value[key.c_str()].IsUint64()) {
      return _value[key.c_str()].GetUint64();
    }
    return std::nullopt;
  }

  std::optional<bool> JsonValue::getBool(const std::string& key) const {
    if (_value.HasMember(key.c_str()) && _value[key.c_str()].IsBool()) {
      return _value[key.c_str()].GetBool();
    }
    return std::nullopt;
  }

  std::optional<float> JsonValue::getFloat(const std::string& key) const {
    if (_value.HasMember(key.c_str()) && _value[key.c_str()].IsFloat()) {
      return _value[key.c_str()].GetFloat();
    }
    return std::nullopt;
  }

  std::optional<double> JsonValue::getDouble(const std::string& key) const {
    if (_value.HasMember(key.c_str()) && _value[key.c_str()].IsDouble()) {
      return _value[key.c_str()].GetDouble();
    }
    return std::nullopt;
  }

  bool JsonValue::isNull()
  {
    return _value.IsNull();
  }

  bool JsonValue::isEmpty()
  {
    if (_value.IsObject()) {
      return _value.ObjectEmpty();
    } else if (_value.IsArray()) {
      return _value.Empty();
    }

    return false;
  }

  JsonValue JsonValue::getObject(const std::string& key) const
  {
    if (_value[key.c_str()].IsObject() && _value.HasMember(key.c_str()))
    {
      return {_value[key.c_str()], _allocator};
    }

    throw JsonParseException("Key not found or not an object");

    // return to avoid warning only
    return {_value,_allocator};
  }



  std::vector<JsonValue> JsonValue::getArray() const
  {
    std::vector<JsonValue> values;
    if (_value.IsArray())
    {
      for (auto& v : _value.GetArray())
      {
        values.push_back(JsonValue(v, _allocator));
      }
      return values;
    }
    throw JsonUsageException("JsonValue is not an array: ");
  }

  std::vector<JsonValue> JsonValue::getArray(const std::string& key) const
  {
    std::vector<JsonValue> values;
    if ( _value[key.c_str()].IsArray() && _value.HasMember(key.c_str()))
    {
      for (auto& v : _value[key.c_str()].GetArray())
      {
        values.push_back(JsonValue(v, _allocator));
      }
      return values;
    }
    throw JsonUsageException("Key not found or not an array: " + key);
  }

  JsonValue& JsonValue::setObject(const std::string& key, const JsonValue& objectValue)
  {
    rapidjson::Value obj(objectValue._value, _allocator);

    if (_value.HasMember(key.c_str())) {
      _value[key.c_str()] = obj;
    } else {
      rapidjson::Value name(key.c_str(), _allocator);
      _value.AddMember(name, obj, _allocator);
    }

    return *this;
  }

  JsonValue& JsonValue::setVector(const std::string& key, std::vector<JsonValue>& values)
  {
    rapidjson::Value array(rapidjson::kArrayType);
    for (auto& val : values) {
      array.PushBack(val._value, _allocator);
    }

    if (_value.HasMember(key.c_str())) {
      _value[key.c_str()] = array;
    } else {
      rapidjson::Value name(key.c_str(), _allocator);
      _value.AddMember(name, array, _allocator);
    }

    return *this;
  }

  JsonValue& JsonValue::setVector(const std::string& key, std::vector<JsonValue>&& values)
  {
    rapidjson::Value array(rapidjson::kArrayType);

    for (auto& val : values) {
      array.PushBack(val._value, _allocator);
    }

    if (_value.HasMember(key.c_str())) {
      _value[key.c_str()] = array;
    } else {
      rapidjson::Value name(key.c_str(), _allocator);
      _value.AddMember(name, array, _allocator);
    }

    return *this;
  }

  JsonValue& JsonValue::setBool(const std::string& key, bool boolValue)
  {
    rapidjson::Value name(key.c_str(), _allocator);

    if (_value.HasMember(key.c_str()))
      _value[key.c_str()].SetBool(boolValue);
    else
      _value.AddMember(name, boolValue, _allocator);

    return *this;
  }

  JsonValue& JsonValue::setInt(const std::string& key, int intValue) {
    rapidjson::Value name(key.c_str(), _allocator);

    if (_value.HasMember(key.c_str()))
      _value[key.c_str()].SetInt(intValue);
    else
      _value.AddMember(name, intValue, _allocator);

    return *this;
  }

  JsonValue& JsonValue::setUint(const std::string &key, unsigned int unsignedValue)
  {
    rapidjson::Value name(key.c_str(), _allocator);

    if (_value.HasMember(key.c_str()))
      _value[key.c_str()].SetUint(unsignedValue);
    else
      _value.AddMember(name, unsignedValue, _allocator);

    return *this;
  }

  JsonValue& JsonValue::setInt64(const std::string &key, int64_t int64Value)
  {
    rapidjson::Value name(key.c_str(), _allocator);

    if (_value.HasMember(key.c_str()))
      _value[key.c_str()].SetInt64(int64Value);
    else
      _value.AddMember(name, int64Value, _allocator);

    return *this;
  }

  JsonValue& JsonValue::setUint64(const std::string &key, uint64_t uint64Value)
  {
    rapidjson::Value name(key.c_str(), _allocator);

    if (_value.HasMember(key.c_str()))
      _value[key.c_str()].SetUint64(uint64Value);
    else
      _value.AddMember(name, uint64Value, _allocator);

    return *this;
  }

  JsonValue& JsonValue::setFloat(const std::string &key, float floatValue)
  {
    rapidjson::Value name(key.c_str(), _allocator);

    if (_value.HasMember(key.c_str()))
      _value[key.c_str()].SetFloat(floatValue);
    else
      _value.AddMember(name, floatValue, _allocator);

    return *this;
  }

  JsonValue& JsonValue::setDouble(const std::string &key, double doubleValue)
  {
    rapidjson::Value name(key.c_str(), _allocator);

    if (_value.HasMember(key.c_str()))
      _value[key.c_str()].SetDouble(doubleValue);
    else
      _value.AddMember(name, doubleValue, _allocator);

    return *this;
  }

  JsonValue& JsonValue::setString(const std::string &key, const std::string &stringValue)
  {
    rapidjson::Value name(key.c_str(), _allocator);
    rapidjson::Value value(stringValue.c_str(), _allocator);

    if (_value.HasMember(key.c_str())) {
      _value[key.c_str()].SetString(value.GetString(), _allocator);
    } else {
      _value.AddMember(name, value, _allocator);
    }

    return *this;
  }

  std::optional<std::string> JsonValue::asString() const
  {
    if (_value.IsString()) {
      return _value.GetString();
    }
    return std::nullopt;
  }

  std::optional<int> JsonValue::asInt() const {
    if (_value.IsInt()) {
      return _value.GetInt();
    }
    return std::nullopt;
  }

  std::optional<bool> JsonValue::asBool() const {
    if (_value.IsBool()) {
      return _value.GetBool();
    }
    return std::nullopt;
  }

  std::optional<float> JsonValue::asFloat() const {
    if (_value.IsFloat()) {
      return _value.GetFloat();
    }
    return std::nullopt;
  }

  std::optional<double> JsonValue::asDouble() const {
    if (_value.IsDouble()) {
      return _value.GetDouble();
    }
    return std::nullopt;
  }

  std::optional<double> JsonValue::asUint() const
  {
    if (_value.IsUint()) {
      return _value.GetUint();
    }
    return std::nullopt;
  }

  std::optional<double> JsonValue::asUint64() const
  {
    if (_value.IsUint64()) {
      return _value.GetUint64();
    }
    return std::nullopt;
  }

  size_t JsonValue::size() const
  {
    if (_value.IsObject()) {
      return _value.MemberCount();
    } else if (_value.IsArray()) {
      return _value.Size();
    }
    return 0;
  }


}
