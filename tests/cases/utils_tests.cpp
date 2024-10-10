//
// Created by Arthur Motelevicz on 10/10/24.
//
#include <catch2/catch.hpp>
#include <mgutils>
#include <chrono>
#include <string>

using namespace mgutils;

TEST_CASE("DateTime utils", "[utils]")
{
  SECTION("Valid date-time with format YYYY-MM-DD HH:MM:SS")
  {
    std::string dateTimeStr = "2023-08-01 11:34:45";
    std::string format = "%Y-%m-%d %H:%M:%S";
    int64_t expectedMillis = 1690900485000; // Corresponds to 2023-08-01 11:34:45 in milliseconds since epoch
    int64_t result = parseDateTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }

  SECTION("Valid date-time with milliseconds")
  {
    std::string dateTimeStr = "2023-08-01 11:34:45.123";
    std::string format = "%Y-%m-%d %H:%M:%S";
    int64_t expectedMillis = 1690900485123; // Adding 123 milliseconds

    int64_t result = parseDateTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }

  SECTION("Valid date-time with format DD/MM/YYYY HH:MM")
  {
    std::string dateTimeStr = "01/08/2023 10:54";
    std::string format = "%d/%m/%Y %H:%M";
    int64_t expectedMillis = 1690898040000; // Corresponds to 2023-08-01 11:34:00 in milliseconds since epoch

    int64_t result = parseDateTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }

  SECTION("Incomplete date-time format - missing seconds should work as well")
  {
    std::string dateTimeStr = "2023-08-01 10:54";
    std::string format = "%Y-%m-%d %H:%M:%S"; // Expecting seconds but they are missing

    int64_t expectedMillis = 1690898040000; // Corresponds to 2023-08-01 11:34:00 in milliseconds since epoch

    int64_t result = parseDateTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }

  SECTION("Invalid date-time format - wrong delimiter")
  {
    std::string dateTimeStr = "2023/08/01 11:34:45"; // Using '/' instead of '-'
    std::string format = "%Y-%m-%d %H:%M:%S"; // Expecting '-'

    REQUIRE_THROWS_AS(parseDateTimeWithFormat(dateTimeStr, format), std::invalid_argument);
  }

  SECTION("Invalid milliseconds value")
  {
    std::string dateTimeStr = "2023-08-01 11:34:45.1234"; // Invalid milliseconds (4 digits)
    std::string format = "%Y-%m-%d %H:%M:%S";

    REQUIRE_THROWS_AS(parseDateTimeWithFormat(dateTimeStr, format), std::invalid_argument);
  }

  SECTION("Valid date-time without milliseconds")
  {
    std::string dateTimeStr = "2023-08-01 10:54:45"; // No milliseconds
    std::string format = "%Y-%m-%d %H:%M:%S";
    int64_t expectedMillis = 1690898085000; // Corresponds to 2023-08-01 11:34:45 in milliseconds since epoch

    int64_t result = parseDateTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }

  SECTION("Valid date-time with custom format MM-DD-YYYY HH:MM:SS")
  {
    std::string dateTimeStr = "08-01-2023 10:54:45";
    std::string format = "%m-%d-%Y %H:%M:%S";
    int64_t expectedMillis = 1690898085000; // Corresponds to 2023-08-01 11:34:45 in milliseconds since epoch

    REQUIRE(parseDateTimeWithFormat(dateTimeStr, format) == expectedMillis);
  }
}

TEST_CASE("Time utils", "[utils]")
{
  SECTION("Valid time with format HH:MM:SS")
  {
    std::string dateTimeStr = "11:34:45";
    std::string format = "%H:%M:%S";
    int64_t expectedMillis = ((60*11 + 34)*60 + 45)*1000;
    int64_t result = parseTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }

  SECTION("Valid time with milliseconds")
  {
    std::string dateTimeStr = "11:34:45.123";
    std::string format = "%H:%M:%S.";
    int64_t expectedMillis = ((60*11 + 34)*60 + 45)*1000 + 123;
    int64_t result = parseTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }

  SECTION("Valid time with custom format")
  {
    std::string dateTimeStr = "113445";
    std::string format = "%H%M%S";
    int64_t expectedMillis = ((60*11 + 34)*60 + 45)*1000;
    int64_t result = parseTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }

  SECTION("Valid time with custom format 2")
  {
    std::string dateTimeStr = "453411";
    std::string format = "%S%M%H";
    int64_t expectedMillis = ((60*11 + 34)*60 + 45)*1000;
    int64_t result = parseTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }

  SECTION("Valid time with custom format in millis")
  {
    std::string dateTimeStr = "113445123";
    std::string format = "%H%M%S";
    int64_t expectedMillis = ((60*11 + 34)*60 + 45)*1000 + 123;
    int64_t result = parseTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }

  SECTION("Valid time with custom format in dot millis")
  {
    std::string dateTimeStr = "113445.123";
    std::string format = "%H%M%S.";
    int64_t expectedMillis = ((60*11 + 34)*60 + 45)*1000 + 123;
    int64_t result = parseTimeWithFormat(dateTimeStr, format);
    REQUIRE(result == expectedMillis);
  }
}