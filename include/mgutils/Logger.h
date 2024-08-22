//
// Created by Arthur Motelevicz on 21/08/24.
//

#ifndef MGUTILS_LOGGER_H
#define MGUTILS_LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>
#include <stdexcept>
#include <string>

namespace mgutils
{
  // ANSI color codes for formatting log output
  constexpr const char* RESET = "\033[0m";        // Reset color to default
  constexpr const char* RED = "\033[31m";         // Red
  constexpr const char* GREEN = "\033[32m";       // Green
  constexpr const char* YELLOW = "\033[33m";      // Yellow
  constexpr const char* BLUE = "\033[34m";        // Blue
  constexpr const char* MAGENTA = "\033[35m";     // Magenta
  constexpr const char* CYAN = "\033[36m";        // Cyan
  constexpr const char* WHITE = "\033[39m";       // Default white (usually light gray)
  constexpr const char* DARK_GRAY = "\033[38;5;8m"; // Dark gray
  constexpr const char* LIGHT_GRAY = "\033[37m"; // Light gray
  constexpr const char* DARK_YELLOW = "\033[33m\033[2m"; // Dark yellow (dimmed)
  constexpr const char* BRIGHT_RED = "\033[91m";  // Bright red
  constexpr const char* BLACK_TEXT_RED_BG = "\033[30;101m";  // Black text with bright red background

  // Define color variables for each log level
  constexpr const char* TRACE_COLOR = DARK_GRAY;
  constexpr const char* DEBUG_COLOR = LIGHT_GRAY;
  constexpr const char* INFO_COLOR = BLUE;
  constexpr const char* WARNING_COLOR = DARK_YELLOW;
  constexpr const char* ERROR_COLOR = RED;
  constexpr const char* CRITICAL_COLOR = BLACK_TEXT_RED_BG;

  enum LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
  };


  class Logger;

  class LogMessage
  {
  public:
    LogMessage(Logger& logger, LogLevel level);
    ~LogMessage();

    template <typename T>
    LogMessage& operator<<(const T& value)
    {
      _stream << value;
      return *this;
    }

  private:
    Logger& _logger;
    LogLevel _level;
    std::ostringstream _stream;
  };

  class Logger
  {
  public:

    static Logger& instance();

    LogMessage log(LogLevel level) ;

    void logStream(LogLevel level, const std::string& message);

    void flush() const;

    template<typename... Args>
    void log(LogLevel level, const std::string& format, Args&&... args)
    {
      switch (level)
      {
        case LogLevel::Trace:
          _traceLogger->trace(fmt::format(format, std::forward<Args>(args)...));
          _fileLogger->trace(fmt::format(format, std::forward<Args>(args)...));
          break;
        case LogLevel::Debug:
          _debugLogger->debug(fmt::format(format, std::forward<Args>(args)...));
          _fileLogger->debug(fmt::format(format, std::forward<Args>(args)...));
          break;
        case LogLevel::Info:
          _infoLogger->info(fmt::format(format, std::forward<Args>(args)...));
          _fileLogger->info(fmt::format(format, std::forward<Args>(args)...));
          break;
        case LogLevel::Warning:
          _warningLogger->warn(fmt::format(format, std::forward<Args>(args)...));
          _fileLogger->warn(fmt::format(format, std::forward<Args>(args)...));
          break;
        case LogLevel::Error:
          _errorLogger->error(fmt::format(format, std::forward<Args>(args)...));
          _fileLogger->error(fmt::format(format, std::forward<Args>(args)...));
          break;
        case LogLevel::Critical:
          _criticalLogger->critical(fmt::format(format, std::forward<Args>(args)...));
          _fileLogger->critical(fmt::format(format, std::forward<Args>(args)...));
          break;
      }
    }

    // Log a custom message with a specified color and log level
    template<typename... Args>
    void logCustom(LogLevel level, const std::string& color_code, const std::string& message, Args&&... args)
    {
      std::string customPattern = color_code + _cachedPattern + RESET;
      _customLogger->set_pattern(customPattern);

      switch (level) {
        case LogLevel::Trace:
          _customLogger->trace(fmt::format(message, std::forward<Args>(args)...));
          _fileLogger->trace(fmt::format(message, std::forward<Args>(args)...));
          break;
        case LogLevel::Debug:
          _customLogger->debug(fmt::format(message, std::forward<Args>(args)...));
          _fileLogger->debug(fmt::format(message, std::forward<Args>(args)...));
          break;
        case LogLevel::Info:
          _customLogger->info(fmt::format(message, std::forward<Args>(args)...));
          _fileLogger->info(fmt::format(message, std::forward<Args>(args)...));
          break;
        case LogLevel::Warning:
          _customLogger->warn(fmt::format(message, std::forward<Args>(args)...));
          _fileLogger->warn(fmt::format(message, std::forward<Args>(args)...));

          break;
        case LogLevel::Error:
          _customLogger->error(fmt::format(message, std::forward<Args>(args)...));
          _fileLogger->error(fmt::format(message, std::forward<Args>(args)...));
          break;
        case LogLevel::Critical:
          _customLogger->critical(fmt::format(message, std::forward<Args>(args)...));
          _fileLogger->critical(fmt::format(message, std::forward<Args>(args)...));
          break;
      }
    }

    // Set the global log level for the logger
    void setLogLevel(LogLevel level);

    // Cache the logging pattern and prepare preformatted patterns for each log level
    void setPattern(const std::string& pattern, bool usesConsoleTag = true);

    // Add a file sink for logging to a file
    void addFileSink(const std::string& filename);

    // Add a rotating file sink for logging to files with rotation based on size
    void addRotatingFileSink(const std::string& filename, std::size_t max_size, std::size_t max_files);

    Logger();
    ~Logger() = default;
  private:

    Logger(const Logger&) = delete; // Disable copy constructor
    Logger& operator=(const Logger&) = delete; // Disable assignment operator

    std::shared_ptr<spdlog::logger> _traceLogger;
    std::shared_ptr<spdlog::logger> _debugLogger;
    std::shared_ptr<spdlog::logger> _infoLogger;
    std::shared_ptr<spdlog::logger> _warningLogger;
    std::shared_ptr<spdlog::logger> _errorLogger;
    std::shared_ptr<spdlog::logger> _criticalLogger;
    std::shared_ptr<spdlog::logger> _customLogger;

    std::shared_ptr<spdlog::logger> _fileLogger;

    std::string _cachedPattern;
    std::string _tracePattern;
    std::string _debugPattern;
    std::string _infoPattern;
    std::string _warningPattern;
    std::string _errorPattern;
    std::string _criticalPattern;

    std::string _instanceId;
  };
} // namespace mgutils

#endif //MGUTILS_LOGGER_H
