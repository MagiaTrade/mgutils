#ifndef MGUTILS_JSON_H
#define MGUTILS_JSON_H

#include <memory>
#include "JsonValue.h"
#include "JsonDocument.h"

namespace mgutils
{
  enum class JsonRootType
  {
    OBJECT,
    ARRAY
  };

  class Json
  {
  public:
    static std::shared_ptr<JsonDocument> createDocument(JsonRootType type = JsonRootType::OBJECT);
    static std::shared_ptr<JsonDocument> parse(const std::string& json);
    static std::shared_ptr<JsonDocument> parseFile(const std::string& filePath);
    static bool save(const std::string& strJson, const std::string& file);
  };
}

#endif //MGUTILS_JSON_H
