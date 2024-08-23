#include <catch2/catch.hpp>
#include "mgutils/Json.h"

using namespace mgutils;

TEST_CASE("JSON parse array content", "[parse, array]")
{
  const std::string jsonArrayString = R"([
    "oi",
    10,
    true,
    {
      "key1": 10
    },
    30.5,
    {},
    [],
    null
    ])";

  auto arrDoc = Json::parse(jsonArrayString);

  REQUIRE(arrDoc->isArray());

  JsonValue root = arrDoc->getRoot();
  std::vector<JsonValue> items = root.getArray();

  REQUIRE(items.size() == 8);
  REQUIRE(items[0].asString() == std::optional<std::string>("oi"));
  REQUIRE(items[1].asInt() == std::optional<int>(10));
  REQUIRE(items[2].asBool() == std::optional<bool>(true));
  REQUIRE(items[3].getInt("key1") == std::optional<int>(10));
  REQUIRE(items[4].asDouble() == std::optional<double>(30.5));
  REQUIRE(items[5].isEmpty());
  REQUIRE(items[6].isEmpty());
  REQUIRE(items[7].isNull());
}

TEST_CASE("JsonValue set transform")
{
  auto doc =  Json::createDocument();
  JsonValue jsonValue = JsonValue("testValue", doc);

  REQUIRE(jsonValue.asString() == std::optional<std::string>("testValue"));

  jsonValue.set("testkey", "New valor");

  REQUIRE(jsonValue.getString("testkey") == std::optional<std::string>("New valor"));
}

TEST_CASE("JSON parsing and manipulation", "[JsonDocument]")
{
  // Test JSON string
  const std::string jsonString = R"({
        "name": "Test Name",
        "age": 30,
        "isActive": true,
        "height": 1.75,
        "weight": 70.5,
        "id": 1234567890123456789,
        "unsignedId": 987654321,
        "attributes": {
            "strength": 85,
            "dexterity": 90
        },
        "items": ["sword", "shield", "potion"]
    })";

  // Create a JsonDocument
  auto document = Json::parse(jsonString);

  SECTION("Parsing")
  {
    REQUIRE(document != nullptr);
  }

  SECTION("Valid JSON parsing with extended types")
  {
    // Get the root object
    JsonValue root = document->getRoot();

    // Verify the contents of the JSON
    REQUIRE(root.getString("name") == std::optional<std::string>("Test Name"));
    REQUIRE(root.getInt("age") == std::optional<int>(30));
    REQUIRE(root.getBool("isActive") == std::optional<bool>(true));
    REQUIRE(root.getFloat("height") == std::optional<float>(1.75f));
    REQUIRE(root.getDouble("weight") == std::optional<double>(70.5));
    REQUIRE(root.getInt64("id") == std::optional<int64_t>(1234567890123456789));
    REQUIRE(root.getUint("unsignedId") == std::optional<unsigned int>(987654321));
  }


  SECTION("Accessing subobjects")
  {
    // Get the root object
    JsonValue root = document->getRoot();

    // Access subobject
    JsonValue attributes = root.getObject("attributes");

    // Verify subobject contents
    REQUIRE(root.getString("name") == std::optional<std::string>("Test Name"));
    REQUIRE(attributes.getInt("strength") == std::optional<int>(85));
    REQUIRE(attributes.getInt("dexterity") == std::optional<int>(90));
  }

  SECTION("Accessing arrays")
  {
    // Get the root object
    JsonValue root = document->getRoot();

    // Access array
    std::vector<JsonValue> items = root.getArray("items");

    // Verify array contents
    REQUIRE(items.size() == 3);
    REQUIRE(items[0].asString() == std::optional<std::string>("sword"));
    REQUIRE(items[1].asString() == std::optional<std::string>("shield"));
    REQUIRE(items[2].asString() == std::optional<std::string>("potion"));
  }

  SECTION("Setting values in JSON")
  {
    // Get the root object
    JsonValue root = document->getRoot();

    // Set new values or replace them
    root.set("name", "New Name");
    root.set("age", 25);
    root.set("isActive", false);
    root.set("height", 1.85f);
    root.set("weight", 75.0);
    root.set("unsignedId", 123456789u);

    // Verify that the values were set correctly
    REQUIRE(root.getString("name") == std::optional<std::string>("New Name"));
    REQUIRE(root.getInt("age") == std::optional<int>(25));
    REQUIRE(root.getBool("isActive") == std::optional<bool>(false));
    REQUIRE(root.getFloat("height") == std::optional<float>(1.85f));
    REQUIRE(root.getDouble("weight") == std::optional<double>(75.0));
    REQUIRE(root.getUint("unsignedId") == std::optional<unsigned int>(123456789u));
  }

  SECTION("Setting and replacing values in JSON without duplication")
  {
    // Get the root object
    JsonValue root = document->getRoot();

    // Capture the initial member count
    size_t initialMemberCount = root.size();

    // Set new values or replace them
    root.set("name", "New Name");
    root.set("age", 25);
    root.set("isActive", false);
    root.set("height", 1.85f);
    root.set("weight", 75.0);
    root.set("unsignedId", 123456789u);

    // Capture the member count after setting new values
    size_t finalMemberCount = root.size();

    // Verify that the member count has not changed
    REQUIRE(finalMemberCount == initialMemberCount);

    // Verify that the values were set correctly
    REQUIRE(root.getString("name") == std::optional<std::string>("New Name"));
    REQUIRE(root.getInt("age") == std::optional<int>(25));
    REQUIRE(root.getBool("isActive") == std::optional<bool>(false));
    REQUIRE(root.getFloat("height") == std::optional<float>(1.85f));
    REQUIRE(root.getDouble("weight") == std::optional<double>(75.0));
    REQUIRE(root.getUint("unsignedId") == std::optional<unsigned int>(123456789u));
  }

  SECTION("Creating and manipulating arrays in JSON")
  {
    // Create a new empty JSON document
    auto doc =  Json::createDocument();
    JsonValue root = doc->getRoot();

    // Define an empty array
    root.set("items", std::vector<JsonValue>());

    // Verify that the array was created and is empty
    std::vector<JsonValue> items = root.getArray("items");
    REQUIRE(items.empty());

    // Add items to the array
    items.emplace_back("sword", doc);
    items.emplace_back("shield", doc);
    items.emplace_back("potion", doc);

    // Update
    root.set("items", items);

    // Verify that the items were added correctly
    items = root.getArray("items");
    REQUIRE(items.size() == 3);
    REQUIRE(items[0].asString() == std::optional<std::string>("sword"));
    REQUIRE(items[1].asString() == std::optional<std::string>("shield"));
    REQUIRE(items[2].asString() == std::optional<std::string>("potion"));

    // Modify an item within the array
    items[1] = JsonValue("armor", doc);
    root.set("items", items);

    // Verify that the item was modified correctly
    items = root.getArray("items");
    REQUIRE(items.size() == 3);
    REQUIRE(items[1].asString() == std::optional<std::string>("armor"));

    // Test adding a new array as a value (nested array)
    std::vector<JsonValue> nestedArray;
    nestedArray.push_back(JsonValue("nested1", doc));
    nestedArray.push_back(JsonValue("nested2", doc));
    root.set("nestedArray", nestedArray);

    // Verify the contents of the nested array
    auto nestedItems = root.getArray("nestedArray");
    REQUIRE(nestedItems.size() == 2);
    REQUIRE(nestedItems[0].asString() == std::optional<std::string>("nested1"));
    REQUIRE(nestedItems[1].asString() == std::optional<std::string>("nested2"));
  }
}

TEST_CASE("JSON create different roots", "[object, array]")
{
  auto doc = Json::createDocument(JsonRootType::OBJECT);
  REQUIRE(doc->isObject());

  auto arrDoc = Json::createDocument(JsonRootType::ARRAY);
  REQUIRE(arrDoc->isArray());
}

TEST_CASE("JSON parse object content", "[parse, object]")
{
  const std::string jsonString = R"({
        "name": "John Doe",
        "age": 30,
        "isActive": true
    })";

  auto doc = Json::parse(jsonString);

  REQUIRE(doc->isObject());

  JsonValue root = doc->getRoot();

  REQUIRE(root.getString("name") == std::optional<std::string>("John Doe"));
  REQUIRE(root.getInt("age") == std::optional<int>(30));
  REQUIRE(root.getBool("isActive") == std::optional<bool>(true));
}

TEST_CASE("JSON parse and insert nested object", "[parse, object, nested]")
{
  const std::string jsonString = R"({
        "name": "John Doe",
        "age": 30,
        "isActive": true
    })";

  auto doc = Json::parse(jsonString);
  REQUIRE(doc->isObject());

  JsonValue root = doc->getRoot();

  REQUIRE(root.getString("name") == std::optional<std::string>("John Doe"));
  REQUIRE(root.getInt("age") == std::optional<int>(30));
  REQUIRE(root.getBool("isActive") == std::optional<bool>(true));

  JsonValue newObject(doc);
  newObject.set("key1", "value1");
  newObject.set("key2", 42);

  root.set("newObject", newObject);

  JsonValue nestedObject = root.getObject("newObject");
  REQUIRE(nestedObject.getString("key1") == std::optional<std::string>("value1"));
  REQUIRE(nestedObject.getInt("key2") == std::optional<int>(42));
}

