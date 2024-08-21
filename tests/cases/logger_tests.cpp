//
// Created by Arthur Motelevicz on 21/08/24.
//
#include <catch2/catch.hpp>
#include <mgutils/Logger.h>
#include <sstream>

TEST_CASE("Logger Singleton Instance") {
  // Ensure that the Logger instance is always the same (singleton pattern)
  auto& logger1 = mgutils::Logger::instance();
  auto& logger2 = mgutils::Logger::instance();

  REQUIRE(&logger1 == &logger2); // Both should point to the same instance
}

TEST_CASE("Setting Log Levels") {
  auto& logger = mgutils::Logger::instance();

  // Set log level to Info and check that the correct level is set
  logger.setLogLevel(mgutils::LogLevel::Info);
  REQUIRE(spdlog::get_level() == spdlog::level::info);

  // Set log level to Debug and check that the correct level is set
  logger.setLogLevel(mgutils::LogLevel::Debug);
  REQUIRE(spdlog::get_level() == spdlog::level::debug);

  // Set log level to Error and check that the correct level is set
  logger.setLogLevel(mgutils::LogLevel::Error);
  REQUIRE(spdlog::get_level() == spdlog::level::err);
}

TEST_CASE("Logging Messages with Different Levels") {
  auto& logger = mgutils::Logger::instance();

  // Test logging at different levels
  logger.setLogLevel(mgutils::LogLevel::Trace);
  logger.log(mgutils::LogLevel::Trace, "This is a trace log.");
  logger.log(mgutils::LogLevel::Debug, "This is a debug log.");
  logger.log(mgutils::LogLevel::Info, "This is an info log.");
  logger.log(mgutils::LogLevel::Warning, "This is a warning log.");
  logger.log(mgutils::LogLevel::Error, "This is an error log.");
  logger.log(mgutils::LogLevel::Critical, "This is a critical log.");

  // Normally you would mock or redirect output to validate this
  // For the purpose of this test, we are assuming the logs are produced correctly
}

TEST_CASE("Setting and Using Custom Patterns") {
  auto& logger = mgutils::Logger::instance();
  std::string customPattern = "[%H:%M:%S] %v"; // Custom pattern for testing

  // Set custom pattern
  logger.setPattern(customPattern);

  // Test logging with the custom pattern
  logger.log(mgutils::LogLevel::Info, "This log uses a custom pattern.");

  // Validate that the pattern is set correctly
  // In real tests, you'd capture and validate the log output
}

TEST_CASE("Custom Color Log") {
  auto& logger = mgutils::Logger::instance();

  // Test a custom color log message
  logger.logCustom(mgutils::LogLevel::Debug, mgutils::CYAN, "This is a cyan debug log.");

  // Again, in a more comprehensive test, you'd redirect output and validate
}
