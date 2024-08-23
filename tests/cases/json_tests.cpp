#include <catch2/catch.hpp>
#include "mgutils/json/Json.h"

using namespace mgutils;

TEST_CASE("JsonDocument extended JSON parsing and manipulation", "[JsonDocument]") {
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

  SECTION("Creating and manipulating arrays in JSON") {
    // Create a new empty JSON document
    JsonDocument doc;
    JsonValue root = doc.getRoot();

    // Define an empty array
    root.set("items", std::vector<JsonValue>());

    // Verify that the array was created and is empty
    std::vector<JsonValue> items = root.getArray("items");
    REQUIRE(items.empty());

    // Add items to the array
    items.emplace_back("sword", doc.getAllocator());
    items.emplace_back("shield", doc.getAllocator());
    items.emplace_back("potion", doc.getAllocator());

    // Update the array in the document
    root.set("items", items);

    // Verify that the items were added correctly
    items = root.getArray("items");
    REQUIRE(items.size() == 3);
    REQUIRE(items[0].asString() == std::optional<std::string>("sword"));
    REQUIRE(items[1].asString() == std::optional<std::string>("shield"));
    REQUIRE(items[2].asString() == std::optional<std::string>("potion"));

    // Modify an item within the array
    items[1] = JsonValue("armor", doc.getAllocator());
    root.set("items", items);

    // Verify that the item was modified correctly
    items = root.getArray("items");
    REQUIRE(items.size() == 3);
    REQUIRE(items[1].asString() == std::optional<std::string>("armor"));

    // Test adding a new array as a value (nested array)
    std::vector<JsonValue> nestedArray;
    nestedArray.push_back(JsonValue("nested1", doc.getAllocator()));
    nestedArray.push_back(JsonValue("nested2", doc.getAllocator()));
    root.set("nestedArray", nestedArray);

    // Verify the contents of the nested array
    auto nestedItems = root.getArray("nestedArray");
    REQUIRE(nestedItems.size() == 2);
    REQUIRE(nestedItems[0].asString() == std::optional<std::string>("nested1"));
    REQUIRE(nestedItems[1].asString() == std::optional<std::string>("nested2"));
  }
}

TEST_CASE("JsonValue set transform")
{
  JsonDocument document;
  JsonValue jsonValue = JsonValue("testValue", document.getAllocator());

  REQUIRE(jsonValue.asString() == std::optional<std::string>("testValue"));

  jsonValue.set("testkey", "New valor");

  REQUIRE(jsonValue.getString("testkey") == std::optional<std::string>("New valor"));
}