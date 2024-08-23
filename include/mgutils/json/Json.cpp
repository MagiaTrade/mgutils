#include "Json.h"
#include "JsonDocument.h"
#include "rapidjson/filereadstream.h"
#include <cstdio>

namespace mgutils
{
  std::shared_ptr<JsonDocument> Json::createDocument()
  {
    return std::shared_ptr<JsonDocument>(new JsonDocument());
  }

  std::shared_ptr<JsonDocument> Json::parse(const std::string& json)
  {
    std::shared_ptr<JsonDocument> document(new JsonDocument());
    document->getDocument().Parse(json.c_str(), json.length());
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
    document->getDocument().ParseStream(is);
    fclose(fp);
  }

  bool Json::save(const std::string& strJson, const std::string& file)
  {
    if(auto doc = parse(strJson))
      return doc->save(file);

    return false;
  }
}
