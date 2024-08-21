//
// Created by Arthur Motelevicz on 21/08/24.
//
#include <mgutils/Logger.h>

int main()
{
  auto& logger = mgutils::Logger::instance();

  logger.setPattern("[%Y-%m-%d %H:%M:%S.%f] %v");

  logger.setLogLevel(mgutils::LogLevel::Trace);

  logger.log(mgutils::LogLevel::Trace, "This is a trace log.");
  logger.log(mgutils::LogLevel::Debug, "This is a debug log.");
  logger.log(mgutils::LogLevel::Info, "This is an info log.");
  logger.log(mgutils::LogLevel::Warning, "This is a warning log.");
  logger.log(mgutils::LogLevel::Error, "This is an error log.");
  logger.log(mgutils::LogLevel::Critical, "This is a critical log.");

  return 0;
}
