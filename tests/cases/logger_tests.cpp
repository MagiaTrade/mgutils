//
// Created by Arthur Motelevicz on 21/08/24.
//
#include <catch2/catch.hpp>
#include <mgutils/Logger.h>
#include <mgutils/Files.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

using namespace mgutils;

std::size_t getFileSize(const std::string& filename)
{
  struct stat stat_buf;
  int rc = stat(filename.c_str(), &stat_buf);
  return rc == 0 ? stat_buf.st_size : 0;
}

TEST_CASE("Logger Singleton Instance", "[logger]")
{
  // Ensure that the Logger instance is always the same (singleton pattern)
  auto& logger1 = mgutils::Logger::instance();
  auto& logger2 = mgutils::Logger::instance();

  REQUIRE(&logger1 == &logger2); // Both should point to the same instance
}

TEST_CASE("Setting Log Levels", "[logger]")
{
  auto& logger = Logger::instance();

#ifdef DEBUG
  logger.setLogLevel(Trace);
  REQUIRE(spdlog::get_level() == spdlog::level::trace);
#else
  logger.setLogLevel(Trace);
  REQUIRE(spdlog::get_level() == spdlog::level::info);
#endif

#ifdef DEBUG
  logger.setLogLevel(Debug);
  REQUIRE(spdlog::get_level() == spdlog::level::debug);
#else
  logger.setLogLevel(Debug);
  REQUIRE(spdlog::get_level() == spdlog::level::info);
#endif

  // Set log level to Info and check that the correct level is set
  logger.setLogLevel(Info);
  REQUIRE(spdlog::get_level() == spdlog::level::info);

  // Set log level to Error and check that the correct level is set
  logger.setLogLevel(Error);
  REQUIRE(spdlog::get_level() == spdlog::level::err);
}

TEST_CASE("Logger logs messages at all levels", "[logger]")
{
  auto& logger = Logger::instance();
  std::string logFilename = "test_log.txt";

  // Remove the file if it exists already
  std::remove(logFilename.c_str());

  logger.addFileSink(logFilename);

  auto loggAll = [&]()
  {
    logger.log(Trace, "Trace message");
    logger.log(Debug, "Debug message");
    logger.log(Info, "Info message");
    logger.log(Warning, "Warning message");
    logger.log(Error, "Error message");
    logger.log(Critical, "Critical message");

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
    logger.setLogLevel(Trace);

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
    logger.setLogLevel(Debug);

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
    logger.setLogLevel(Info);

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
    logger.setLogLevel(Warning);

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
    logger.setLogLevel(Error);

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
    logger.setLogLevel(Critical);

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

TEST_CASE("Logger rotates log files based on size", "[logger][rotation]")
{
  auto& logger = Logger::instance();
  logger.setPattern("%v");
  std::string logFilename = "rotating_log";
  std::size_t maxSize = 500;  // 1 KB
  std::size_t maxFiles = 9;    // Máximo de 3 arquivos de backup, 4 no total com o ativo

  // Remove arquivos anteriores se existirem
  std::remove(logFilename.c_str());
  std::remove((logFilename + ".1").c_str());
  std::remove((logFilename + ".2").c_str());
  std::remove((logFilename + ".3").c_str());
  std::remove((logFilename + ".4").c_str());
  std::remove((logFilename + ".5").c_str());
  std::remove((logFilename + ".6").c_str());
  std::remove((logFilename + ".7").c_str());
  std::remove((logFilename + ".8").c_str());
  std::remove((logFilename + ".9").c_str());

  logger.addRotatingFileSink(logFilename, maxSize, maxFiles);

  logger.setLogLevel(Info);

  // 75 to discount the INFO: Log entry <n>: prefix of eacho log
  // this way each line is almost 100 characters
  // 100 * 100 = 10000
  // 10000/ 500 = 20 files
  // so at the end the first file should have the Log entry  96 to 100 messages
  // the last (.9) file should have the have the Log entry 51 to 55
  std::string largeMessage = std::string(75, 'X');

  for (int i = 1; i <= 100; ++i)
  {
    logger.log(Info, "Log entry " + std::to_string(i) + ": " + largeMessage);
    logger.flush();

    std::size_t fileSize = getFileSize(logFilename);
    std::cout << "File size after log: " << i << " -> " << fileSize << " bytes" << std::endl;
  }

  REQUIRE(std::ifstream(logFilename).is_open());
  REQUIRE(std::ifstream(logFilename + ".9").is_open());

  std::ifstream logFile(logFilename);
  std::string lastLogContent;
  std::string line;

  REQUIRE(logFile.is_open());

  while (std::getline(logFile, line)) {
    lastLogContent = line;
  }

  logFile.close();

  REQUIRE(lastLogContent.find("Log entry 100") != std::string::npos);

  std::ifstream rotatedLogFile1(logFilename + ".9");
  std::string rotatedLogContent1;
  REQUIRE(rotatedLogFile1.is_open());

  while (std::getline(rotatedLogFile1, line)) {
    rotatedLogContent1 += line + "\n";
  }

  rotatedLogFile1.close();

  REQUIRE(rotatedLogContent1.find("Log entry 55") != std::string::npos);
}

TEST_CASE("Logger supports variadic formatting", "[logger][variadic]")
{
  auto& logger = Logger::instance();
  logger.setPattern("%v", false);
  std::string logFilename = "variadic_log.txt";

  std::remove(logFilename.c_str());

  logger.addFileSink(logFilename);
  logger.setLogLevel(Info);

  logger.log(Info, "User {} logged in from IP {}", "Arthur", "192.168.1.1");
  logger.flush();

  logger.log(Error, "Error {}: {}", 404, "Not Found");
  logger.flush();

  std::ifstream logFile(logFilename);
  REQUIRE(logFile.is_open());

  std::string logContents;
  std::string line;

  while (std::getline(logFile, line)) {
    logContents += line + "\n";
  }

  logFile.close();

  REQUIRE(logContents.find("User Arthur logged in from IP 192.168.1.1") != std::string::npos);
  REQUIRE(logContents.find("Error 404: Not Found") != std::string::npos);
}

TEST_CASE("Logger supports stream operator <<", "[logger][stream]")
{
  std::string logFilename = "stream_test_log.txt";
  std::remove(logFilename.c_str());

  auto logger = mgutils::Logger(logFilename);

  logger.setLogLevel(mgutils::LogLevel::Info);
  logger.log(mgutils::LogLevel::Info) << "Stream log with value: " << 100 << " and more.";

  std::ifstream logFile(logFilename);
  std::string line;
  REQUIRE(logFile.is_open());
  std::getline(logFile, line);
  REQUIRE(line.find("Stream log with value: 100 and more.") != std::string::npos);
  logFile.close();
}

TEST_CASE("Logger macros", "[logger][macros]")
{
  Logger::instance().setPattern("%v", true);
  std::ifstream logFile("logs.txt");
  std::string line;
  std::string lastLogContent;

  SECTION("ERROR")
  {
    NOTIFY_ERROR(404, "Page not found");
    Logger::instance().flush();

    REQUIRE(logFile.is_open());
    while (std::getline(logFile, line)) {
      lastLogContent = line;
    }
    REQUIRE(lastLogContent.find("Page not found") != std::string::npos);
  }

  SECTION("CRITICAL")
  {
    NOTIFY_CRITICAL(505, "Danger!");
    Logger::instance().flush();

    REQUIRE(logFile.is_open());

    while (std::getline(logFile, line)){
      lastLogContent = line;
    }

    REQUIRE(lastLogContent.find("Danger!") != std::string::npos);
  }

  logFile.close();
}

TEST_CASE("Logger handles strings with and without format placeholders correctly", "[logger][formatting]")
{
  auto& logger = Logger::instance();
  std::string logFilename = "formatting_test_log.txt";

  // Remove the file if it exists already
  std::remove(logFilename.c_str());

  logger.addFileSink(logFilename);
  logger.setLogLevel(Info);
  logger.setPattern("%v", false);

  SECTION("String without format placeholders")
  {
    logger.log(Info, "This is a simple log message without placeholders.");
    logger.flush();

    std::ifstream logFile(logFilename);
    REQUIRE(logFile.is_open());

    std::string logContents;
    std::string line;

    while (std::getline(logFile, line)) {
      logContents += line + "\n";
    }

    logFile.close();

    REQUIRE(logContents.find("This is a simple log message without placeholders.") != std::string::npos);
  }

  SECTION("String with format placeholders")
  {
    logger.log(Info, "Formatted message with placeholder: {}", 123);
    logger.flush();

    std::ifstream logFile(logFilename);
    REQUIRE(logFile.is_open());

    std::string logContents;
    std::string line;

    while (std::getline(logFile, line)) {
      logContents += line + "\n";
    }

    logFile.close();

    REQUIRE(logContents.find("Formatted message with placeholder: 123") != std::string::npos);
  }
}

TEST_CASE("Logger correctly logs messages without exceptions", "[logger][formatting]")
{
  auto& logger = Logger::instance();
  std::string logFilename = "exception_handling_test_log.txt";

  // Remove the file if it exists already
  std::remove(logFilename.c_str());

  logger.addFileSink(logFilename);
  logger.setLogLevel(Info);
  logger.setPattern("%v", true);

  SECTION("Logging string with incorrect format placeholders")
  {
    REQUIRE_NOTHROW(logger.log(Info, "Incorrect format string with placeholder {} without argument."));
    logger.flush();

    std::ifstream logFile(logFilename);
    REQUIRE(logFile.is_open());

    std::string logContents;
    std::string line;

    while (std::getline(logFile, line)) {
      logContents += line + "\n";
    }

    logFile.close();

    // Since there was a placeholder without an argument, log should not crash or throw, but handle gracefully.
    REQUIRE(logContents.find("Incorrect format string with placeholder {} without argument.") != std::string::npos);
  }
}

TEST_CASE("Logger move", "[logger][move]")
{
  Logger logger1("custom_log.txt");
  Logger logger2 = std::move(logger1);

  REQUIRE(logger2.getLogFilename() == "custom_log.txt");

  // Note: Accessing logger1 after move. This is for testing purposes only.
  REQUIRE(logger1.getLogFilename().empty());
}

TEST_CASE("Logger move 2", "[logger][move]") {
  std::string logFilename = "custom_log.txt";

  // Remove o arquivo se ele já existir
  std::remove(logFilename.c_str());

  Logger logger1(logFilename);
  Logger logger2 = std::move(logger1);

  REQUIRE(logger2.getLogFilename() == logFilename);

  // Note: Accessing logger1 after move. This is for testing purposes only.
  REQUIRE(logger1.getLogFilename().empty());

  logger2.log(Info, "Log message after move");
  logger2.flush();

  auto logContents = Files::readFile(logFilename);

  REQUIRE(logContents.find("Log message after move") != std::string::npos);
}



