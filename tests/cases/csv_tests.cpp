//
// Created by Arthur Motelevicz on 26/08/24.
//
#include <catch2/catch.hpp>
#include <mgutils/CsvLoader.h>
#include <string>
#include <vector>

using namespace mgutils;

// Test case for basic operations of CSVLoader
TEST_CASE("CSV basic tests", "[csv_loader]")
{
  const std::string filename = "resources/test1.csv";  // Assume this file exists with appropriate content
  const std::string customDelimiterFilename = "resources/test_semicolon.csv";  // File with semicolon delimiter

  SECTION("Loading a CSV file with default delimiter") {
    CsvLoader loader(filename);

    REQUIRE_NOTHROW(loader.getColumn<std::string>("Column1"));
    REQUIRE_NOTHROW(loader.getRow<std::string>(0));
    REQUIRE_NOTHROW(loader.getCell<std::string>("Column1", 0));

    std::vector<std::string> columnData = loader.getColumn<std::string>("Column1");
    std::vector<std::string> rowData = loader.getRow<std::string>(0);
    std::string cellData = loader.getCell<std::string>("Column1", 0);

    REQUIRE(!columnData.empty());
    REQUIRE(!rowData.empty());
    REQUIRE(cellData == "Value1");  // Adjust according to actual expected value in test.csv
  }

  SECTION("Loading a CSV file with custom delimiter") {
    CsvLoader loader(customDelimiterFilename, ';');

    REQUIRE_NOTHROW(loader.getColumn<std::string>("Column1"));
    REQUIRE_NOTHROW(loader.getRow<std::string>(0));
    REQUIRE_NOTHROW(loader.getCell<std::string>("Column1", 0));

    std::vector<std::string> columnData = loader.getColumn<std::string>("Column1");
    std::vector<std::string> rowData = loader.getRow<std::string>(0);
    auto cellData = loader.getCell<std::string>("Column1", 0);

    REQUIRE(!columnData.empty());
    REQUIRE(!rowData.empty());
    REQUIRE(cellData == "ValueA");  // Adjust according to actual expected value in test_semicolon.csv
  }

  SECTION("Handling exceptions on invalid operations") {
    CsvLoader loader(filename);

    REQUIRE_THROWS_AS(loader.getColumn<std::string>("InvalidColumn"), CsvUsageException);
    REQUIRE_THROWS_AS(loader.getRow<std::string>(9999), CsvUsageException);
    REQUIRE_THROWS_AS(loader.getCell<std::string>("Column1", 9999), CsvUsageException);
  }

  SECTION("Saving a CSV file") {
    CsvLoader loader(filename);

    REQUIRE_NOTHROW(loader.save("output.csv"));

    // Optionally, load the saved file to verify its content
    CsvLoader savedLoader("output.csv");
    std::vector<std::string> savedColumnData = savedLoader.getColumn<std::string>("Column1");

    REQUIRE(savedColumnData.size() > 0);  // Should match the original loaded data
  }

  SECTION("Getting all data from the CSV") {
    CsvLoader loader(filename);

    REQUIRE_NOTHROW(loader.getAllData());

    auto allData = loader.getAllData();
    REQUIRE(allData.size() > 0);
    REQUIRE(allData[0].size() > 0);
  }
}
