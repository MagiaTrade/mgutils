//
// Created by Arthur Motelevicz on 23/08/24.
//

#ifndef MGUTILS_JSONVALUE_H
#define MGUTILS_JSONVALUE_H

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <fstream>
#include <optional>
#include <string>
#include <vector>
#include <type_traits>

namespace mgutils
{
  class JsonDocument;

  class JsonValue
  {
  public:
    JsonValue(const rapidjson::Value& value, const std::shared_ptr<JsonDocument>& doc);
    JsonValue(JsonValue&& other) noexcept;
    JsonValue& operator=(JsonValue&& other) noexcept;

    explicit JsonValue(const std::shared_ptr<JsonDocument>& doc);
    JsonValue(const std::string& value, const std::shared_ptr<JsonDocument>& doc);
    JsonValue(const char* value, const std::shared_ptr<JsonDocument>& doc);
    JsonValue(int value, const std::shared_ptr<JsonDocument>& doc);
    JsonValue(bool value, const std::shared_ptr<JsonDocument>& doc);
    JsonValue(int64_t value, const std::shared_ptr<JsonDocument>& doc);
    JsonValue(uint value, const std::shared_ptr<JsonDocument>& doc);
    JsonValue(uint64_t value, const std::shared_ptr<JsonDocument>& doc);
    JsonValue(float value, const std::shared_ptr<JsonDocument>& doc);
    JsonValue(double value, const std::shared_ptr<JsonDocument>& doc);

    bool hasBool(const std::string& memberName) const;
    bool hasNumber(const std::string& memberName) const;
    bool hasString(const std::string& memberName) const;
    bool hasObject(const std::string& memberName) const;
    bool hasArray(const std::string& memberName) const;
    bool exists(const std::string& memberName) const;

    std::optional<std::string> getString(const std::string& key) const;
    std::optional<int> getInt(const std::string& key) const;
    std::optional<unsigned> getUint(const std::string& key) const;
    std::optional<int64_t> getInt64(const std::string& key) const;
    std::optional<uint64_t> getUint64(const std::string& key) const;
    std::optional<bool> getBool(const std::string& key) const;
    std::optional<float> getFloat(const std::string& key) const;
    std::optional<double> getDouble(const std::string& key) const;

    std::optional<std::string> asString() const;

    std::optional<int> asInt() const;
    std::optional<bool> asBool() const;
    std::optional<float> asFloat() const;
    std::optional<double> asDouble() const;
    std::optional<double> asUint() const;
    std::optional<double> asUint64() const;

    bool isNull();
    bool isEmpty();

    JsonValue getObject(const std::string& key) const;
    std::vector<JsonValue> getArray(const std::string& key) const;
    std::vector<JsonValue> getArray() const;

    template <typename T>
    JsonValue& set(const std::string& key, T& value)
    {
      using DecayedT = typename std::decay<T>::type;

      static_assert(std::is_same<T, bool>::value ||
                    std::is_same<T, int>::value ||
                    std::is_same<T, unsigned int>::value ||
                    std::is_same<T, int64_t>::value ||
                    std::is_same<T, uint64_t>::value ||
                    std::is_same<T, float>::value ||
                    std::is_same<T, double>::value ||
                    std::is_same<T, std::string>::value ||
                    std::is_same<T, std::vector<JsonValue>>::value ||
                    std::is_same<T, JsonValue>::value ||
                    std::is_same<DecayedT, const char*>::value,
                    "Invalid type for set function");

      if (!_value.IsObject()) {
        _value.SetObject();
      }

      if constexpr (std::is_same<T, bool>::value) {
        return setBool(key, value);
      } else if constexpr (std::is_same<T, int>::value) {
        return setInt(key, value);
      } else if constexpr (std::is_same<T, unsigned int>::value) {
        return setUint(key, value);
      } else if constexpr (std::is_same<T, int64_t>::value) {
        return setInt64(key, value);
      } else if constexpr (std::is_same<T, uint64_t>::value) {
        return setUint64(key, value);
      } else if constexpr (std::is_same<T, float>::value) {
        return setFloat(key, value);
      } else if constexpr (std::is_same<T, double>::value) {
        return setDouble(key, value);
      }else if constexpr (std::is_same<T, JsonValue>::value) {
        return setObject(key, value);
      }else if constexpr (std::is_same<T, std::vector<JsonValue>>::value) {
        return setVector(key, value);
      } else if constexpr ( (std::is_same<DecayedT, const char*>::value) || (std::is_same<T, std::string>::value) ) {
        return setString(key, value);
      }
    }

    template <typename T>
    JsonValue& set(const std::string& key, T&& value)
    {
      using DecayedT = typename std::decay<T>::type;

      static_assert(std::is_same<T, bool>::value ||
                    std::is_same<T, int>::value ||
                    std::is_same<T, unsigned int>::value ||
                    std::is_same<T, int64_t>::value ||
                    std::is_same<T, uint64_t>::value ||
                    std::is_same<T, float>::value ||
                    std::is_same<T, double>::value ||
                    std::is_same<T, std::string>::value ||
                    std::is_same<T, std::vector<JsonValue>>::value ||
                    std::is_same<T, JsonValue>::value ||
                    std::is_same<DecayedT, const char*>::value,
                    "Invalid type for set function");

      if (!_value.IsObject()) {
        _value.SetObject();
      }

      if constexpr (std::is_same<T, bool>::value) {
        return setBool(key, value);
      } else if constexpr (std::is_same<T, int>::value) {
        return setInt(key, value);
      } else if constexpr (std::is_same<T, unsigned int>::value) {
        return setUint(key, value);
      } else if constexpr (std::is_same<T, int64_t>::value) {
        return setInt64(key, value);
      } else if constexpr (std::is_same<T, uint64_t>::value) {
        return setUint64(key, value);
      } else if constexpr (std::is_same<T, float>::value) {
        return setFloat(key, value);
      } else if constexpr (std::is_same<T, double>::value) {
        return setDouble(key, value);
      } else if constexpr (std::is_same<T, JsonValue>::value) {
        return setObject(key, value);
      }else if constexpr (std::is_same<T, std::vector<JsonValue>>::value) {
        return setVector(key, std::forward<T>(value));
      } else if constexpr ( (std::is_same<DecayedT, const char*>::value) || (std::is_same<T, std::string>::value) ) {
        return setString(key, value);
      }
    }

    size_t size() const;

  private:

    JsonValue(const rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator);

    JsonValue& setBool(const std::string& key, bool boolValue);
    JsonValue& setInt(const std::string& key, int intValue);
    JsonValue& setUint(const std::string& key, unsigned int unsignedValue);
    JsonValue& setInt64(const std::string& key, int64_t int64Value);
    JsonValue& setUint64(const std::string& key, uint64_t uint64Value);
    JsonValue& setFloat(const std::string& key, float floatValue);
    JsonValue& setDouble(const std::string& key, double doubleValue);
    JsonValue& setString(const std::string& key, const std::string& stringValue);
    JsonValue& setObject(const std::string& key, const JsonValue& objectValue);
    JsonValue& setVector(const std::string& key, std::vector<JsonValue>& values);
    JsonValue& setVector(const std::string& key, std::vector<JsonValue>&& values);

    rapidjson::Value _value;
    rapidjson::Document::AllocatorType& _allocator;
  };

}
#endif //MGUTILS_JSONVALUE_H
