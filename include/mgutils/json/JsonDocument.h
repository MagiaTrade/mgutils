//
// Created by Arthur Motelevicz on 23/08/24.
//

#ifndef MGUTILS_JSONDOCUMENT_H
#define MGUTILS_JSONDOCUMENT_H

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
  class JsonValue; // Forward declaration

  class JsonDocument: public std::enable_shared_from_this<JsonDocument>
  {
  public:
    JsonValue getRoot();  // To get the root object
    rapidjson::Document::AllocatorType& getAllocator();
    // Serialization
    std::string toString(bool pretty = false) const;
    bool save(const std::string& file);
    bool isArray();
    bool isObject();

    friend class Json;

  private:
    JsonDocument();
    void setObjet();
    void setArray();
    rapidjson::Document _document;
    rapidjson::Document::AllocatorType& _allocator;
  };


}

#endif //MGUTILS_JSONDOCUMENT_H
