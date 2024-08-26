//
// Created by Arthur Motelevicz on 23/08/24.
//

#include "JsonDocument.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "JsonValue.h"

namespace mgutils
{
  JsonDocument::JsonDocument():
      _document(),
      _allocator(_document.GetAllocator())
  {}

  rapidjson::Document::AllocatorType& JsonDocument::getAllocator()
  {
    return _allocator;
  }

  std::string JsonDocument::toString(bool pretty) const
  {
    rapidjson::StringBuffer buffer;
    if (pretty) {
      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
      _document.Accept(writer);
    } else {
      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
      _document.Accept(writer);
    }
    return buffer.GetString();
  }

  bool JsonDocument::save(const std::string& file)
  {
    FILE* fp = fopen(file.c_str(), "w");
    if (!fp)
      return false;

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    _document.Accept(writer);
    fclose(fp);

    return true;
  }

  JsonValue JsonDocument::getRoot()
  {
    return {_document, shared_from_this()};
  }

  void JsonDocument::setObjet()
  {
    _document.SetObject();
  }

  void JsonDocument::setArray()
  {
    _document.SetArray();
  }

  bool  JsonDocument::isArray()
  {
    return _document.IsArray();
  }

  bool  JsonDocument::isObject()
  {
    return _document.IsObject();
  }
}
