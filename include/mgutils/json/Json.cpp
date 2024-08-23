#include "Json.h"
#include "JsonDocument.h"
#include "rapidjson/filereadstream.h"
#include <cstdio>

namespace mgutils
{
  std::shared_ptr<JsonDocument> Json::createDocument(JsonRootType type)
  {
    std::shared_ptr<JsonDocument> document(new JsonDocument());
    switch (type)
    {
      case JsonRootType::OBJECT:
      {
        document->setObjet();
        break;
      }
      case JsonRootType::ARRAY:
      {
        document->setArray();
        break;
      }
      default:
        break;
    }
    return document;
  }

  std::shared_ptr<JsonDocument> Json::parse(const std::string& json)
  {
    std::shared_ptr<JsonDocument> document(new JsonDocument());
    document->_document.Parse(json.c_str(), json.length());
    return document;
  }

  std::shared_ptr<JsonDocument> Json::parseFile(const std::string& filePath)
  {
    std::shared_ptr<JsonDocument> document(new JsonDocument());

    FILE* fp = fopen(filePath.c_str(), "r");
    if (!fp) {
      return nullptr;
    }

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    document->_document.ParseStream(is);
    fclose(fp);
  }

  bool Json::save(const std::string& strJson, const std::string& file)
  {
    if(auto doc = parse(strJson))
      return doc->save(file);

    return false;
  }
}
