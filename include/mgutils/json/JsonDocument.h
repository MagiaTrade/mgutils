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

  class JsonDocument
  {
  public:
    JsonDocument();

    rapidjson::Document& getDocument();  // Non-const, allowing modifications
    rapidjson::Document::AllocatorType& getAllocator();

    JsonValue getRoot();  // To get the root object

    // Serialization
    std::string toString(bool pretty = false);
    bool save(const std::string& file);

  private:
    rapidjson::Document _document;
    rapidjson::Document::AllocatorType& _allocator;
  };


}

#endif //MGUTILS_JSONDOCUMENT_H
