//
// Created by Arthur Motelevicz on 21/08/24.
//
#include <catch2/catch.hpp>
#include <mgutils/Logger.h>
#include <fstream>
#include <sstream>

TEST_CASE("Logger Singleton Instance")
{
  // Ensure that the Logger instance is always the same (singleton pattern)
  auto& logger1 = mgutils::Logger::instance();
  auto& logger2 = mgutils::Logger::instance();

  REQUIRE(&logger1 == &logger2); // Both should point to the same instance
}

TEST_CASE("Setting Log Levels")
{
  auto& logger = mgutils::Logger::instance();

#ifdef DEBUG
  logger.setLogLevel(mgutils::LogLevel::Trace);
  REQUIRE(spdlog::get_level() == spdlog::level::trace);
#else
  logger.setLogLevel(mgutils::LogLevel::Trace);
  REQUIRE(spdlog::get_level() == spdlog::level::info);
#endif

#ifdef DEBUG
  logger.setLogLevel(mgutils::LogLevel::Debug);
  REQUIRE(spdlog::get_level() == spdlog::level::debug);
#else
  logger.setLogLevel(mgutils::LogLevel::Debug);
  REQUIRE(spdlog::get_level() == spdlog::level::info);
#endif

  // Set log level to Info and check that the correct level is set
  logger.setLogLevel(mgutils::LogLevel::Info);
  REQUIRE(spdlog::get_level() == spdlog::level::info);

  // Set log level to Error and check that the correct level is set
  logger.setLogLevel(mgutils::LogLevel::Error);
  REQUIRE(spdlog::get_level() == spdlog::level::err);
}

TEST_CASE("Logger logs messages at all levels", "[logger]")
{
  auto& logger = mgutils::Logger::instance();
  std::string logFilename = "test_log.txt";

  // Remove the file if it exists already
  std::remove(logFilename.c_str());

  logger.addFileSink(logFilename);

  auto loggAll = [&]()
  {
    logger.log(mgutils::LogLevel::Trace, "Trace message");
    logger.log(mgutils::LogLevel::Debug, "Debug message");
    logger.log(mgutils::LogLevel::Info, "Info message");
    logger.log(mgutils::LogLevel::Warning, "Warning message");
    logger.log(mgutils::LogLevel::Error, "Error message");
    logger.log(mgutils::LogLevel::Critical, "Critical message");

    logger.flush();
  };

  auto getFileContent = [&]() -> std::string
  {
    // Verifica se as mensagens foram gravadas no arquivo de log
    std::ifstream logFile(logFilename);
    std::string line;
    std::stringstream logContents;

    REQUIRE(logFile.is_open());

    while (std::getline(logFile, line)) {
      logContents << line << "\n";
    }

    logFile.close();
    return logContents.str();
  };

  SECTION("All levels")
  {
    logger.setLogLevel(mgutils::LogLevel::Trace);

    loggAll();
    auto logContents  = getFileContent();

#ifdef DEBUG
    REQUIRE(logContents.find("Trace message") != std::string::npos);
    REQUIRE(logContents.find("Debug message") != std::string::npos);
    REQUIRE(logContents.find("Info message") != std::string::npos);
    REQUIRE(logContents.find("Warning message") != std::string::npos);
    REQUIRE(logContents.find("Error message") != std::string::npos);
    REQUIRE(logContents.find("Critical message") != std::string::npos);
#else
    REQUIRE(logContents.find("Trace message") == std::string::npos);
    REQUIRE(logContents.find("Debug message") == std::string::npos);
    REQUIRE(logContents.find("Info message") != std::string::npos);
    REQUIRE(logContents.find("Warning message") != std::string::npos);
    REQUIRE(logContents.find("Error message") != std::string::npos);
    REQUIRE(logContents.find("Critical message") != std::string::npos);
#endif

  }

  SECTION("Debug and above")
  {
    logger.setLogLevel(mgutils::LogLevel::Debug);

    loggAll();
    auto logContents  = getFileContent();

#ifdef DEBUG
    REQUIRE(logContents.find("Trace message") == std::string::npos);
    REQUIRE(logContents.find("Debug message") != std::string::npos);
    REQUIRE(logContents.find("Info message") != std::string::npos);
    REQUIRE(logContents.find("Warning message") != std::string::npos);
    REQUIRE(logContents.find("Error message") != std::string::npos);
    REQUIRE(logContents.find("Critical message") != std::string::npos);
#else
    REQUIRE(logContents.find("Trace message") == std::string::npos);
    REQUIRE(logContents.find("Debug message") == std::string::npos);
    REQUIRE(logContents.find("Info message") != std::string::npos);
    REQUIRE(logContents.find("Warning message") != std::string::npos);
    REQUIRE(logContents.find("Error message") != std::string::npos);
    REQUIRE(logContents.find("Critical message") != std::string::npos);
#endif
  }

  SECTION("Info and above")
  {
    logger.setLogLevel(mgutils::LogLevel::Info);

    loggAll();
    auto logContents  = getFileContent();

    REQUIRE(logContents.find("Trace message") == std::string::npos);
    REQUIRE(logContents.find("Debug message") == std::string::npos);
    REQUIRE(logContents.find("Info message") != std::string::npos);
    REQUIRE(logContents.find("Warning message") != std::string::npos);
    REQUIRE(logContents.find("Error message") != std::string::npos);
    REQUIRE(logContents.find("Critical message") != std::string::npos);
  }

  SECTION("Warning and above")
  {
    logger.setLogLevel(mgutils::LogLevel::Warning);

    loggAll();
    auto logContents  = getFileContent();

    REQUIRE(logContents.find("Trace message") == std::string::npos);
    REQUIRE(logContents.find("Debug message") == std::string::npos);
    REQUIRE(logContents.find("Info message") == std::string::npos);
    REQUIRE(logContents.find("Warning message") != std::string::npos);
    REQUIRE(logContents.find("Error message") != std::string::npos);
    REQUIRE(logContents.find("Critical message") != std::string::npos);
  }


  SECTION("Error and above")
  {
    logger.setLogLevel(mgutils::LogLevel::Error);

    loggAll();
    auto logContents  = getFileContent();

    REQUIRE(logContents.find("Trace message") == std::string::npos);
    REQUIRE(logContents.find("Debug message") == std::string::npos);
    REQUIRE(logContents.find("Info message") == std::string::npos);
    REQUIRE(logContents.find("Warning message") == std::string::npos);
    REQUIRE(logContents.find("Error message") != std::string::npos);
    REQUIRE(logContents.find("Critical message") != std::string::npos);
  }

  SECTION("Critical only")
  {
    logger.setLogLevel(mgutils::LogLevel::Critical);

    loggAll();
    auto logContents  = getFileContent();

    REQUIRE(logContents.find("Trace message") == std::string::npos);
    REQUIRE(logContents.find("Debug message") == std::string::npos);
    REQUIRE(logContents.find("Info message") == std::string::npos);
    REQUIRE(logContents.find("Warning message") == std::string::npos);
    REQUIRE(logContents.find("Error message") == std::string::npos);
    REQUIRE(logContents.find("Critical message") != std::string::npos);
  }
}