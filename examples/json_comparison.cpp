//
// Created by Arthur Motelevicz on 23/08/24.
//

#include <iostream>
#include "rapidjson/document.h"
#include <mgutils/Json.h>

using namespace std::chrono;

// Função para medir o tempo de parsing com rapidjson direto
void benchmarkRapidJson(const std::string& jsonString)
{
  auto start = high_resolution_clock::now();

  for (int i = 0; i < 100000; ++i) {
    rapidjson::Document document;
    document.Parse(jsonString.c_str());
    auto name = document["name"].GetString();
    auto age = document["age"].GetInt();
    auto active = document["isActive"].GetBool();
    auto height = document["height"].GetDouble();
  }

  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start);
  std::cout << "RapidJSON parsing: " << duration.count() << "ms\n";
}

// Função para medir o tempo de parsing com seu wrapper
void benchmarkJsonWrapper(const std::string& jsonString)
{
  auto start = high_resolution_clock::now();

  for (int i = 0; i < 100000; ++i) {
    auto document = mgutils::Json::parse(jsonString);
    auto root = document->getRoot();
    auto name = root.getString("name");
    auto age = root.getInt("age");
    auto active = root.getBool("isActive");
    auto height = root.getDouble("height");
  }

  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start);
  std::cout << "Wrapper parsing: " << duration.count() << "ms\n";
}

int main()
{
  const std::string jsonString = R"({
        "name": "John Doe",
        "age": 30,
        "isActive": true,
        "height": 1.75,
        "weight": 70.5
    })";

  benchmarkRapidJson(jsonString);
  benchmarkJsonWrapper(jsonString);

  return 0;
}
