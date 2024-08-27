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
#include "ErrorManager.h"

#define NOTIFY_ERROR(code, message)                                        \
    do {                                                                   \
        std::ostringstream oss;                                            \
        oss << message << " (Error code: " << code                         \
            << ", File: " << __FILE__ << ", Line: " << __LINE__ << ")";    \
        mgutils::Logger::instance().log(mgutils::LogLevel::Error, oss.str());       \
        mgutils::ErrorInfo errorInfo(code, message, __FILE__, __LINE__);   \
        mgutils::ErrorManager::instance().notify(errorInfo);               \
    } while (0)

#define NOTIFY_CRITICAL(code, message)                                     \
    do {                                                                   \
        std::ostringstream oss;                                            \
        oss << message << " (Critical code: " << code                      \
            << ", File: " << __FILE__ << ", Line: " << __LINE__ << ")";    \
        Logger::instance().log(mgutils::LogLevel::Critical, oss.str());    \
        mgutils::ErrorInfo errorInfo(code, message, __FILE__, __LINE__);   \
        mgutils::ErrorManager::instance().notify(errorInfo);               \
    } while (0)

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

    std::string getLogFilename() const;
    void logStream(LogLevel level, const std::string& message);

    void flush() const;

    template <typename... Args>
    void log(LogLevel level, const std::string& format, Args&&... args)
    {
      switch (level) {
        case LogLevel::Trace:
          logInternal(_traceLogger, level, format, std::forward<Args>(args)...);
          break;
        case LogLevel::Debug:
          logInternal(_debugLogger, level, format, std::forward<Args>(args)...);
          break;
        case LogLevel::Info:
          logInternal(_infoLogger, level, format, std::forward<Args>(args)...);
          break;
        case LogLevel::Warning:
          logInternal(_warningLogger, level, format, std::forward<Args>(args)...);
          break;
        case LogLevel::Error:
          logInternal(_errorLogger, level, format, std::forward<Args>(args)...);
          break;
        case LogLevel::Critical:
          logInternal(_criticalLogger, level, format, std::forward<Args>(args)...);
          break;
      }
    }

    template <typename... Args>
    void logCustom(LogLevel level, const std::string& color_code, const std::string& format, Args&&... args)
    {
      std::string customPattern = color_code + _cachedPattern + RESET;
      _customLogger->set_pattern(customPattern);
      logInternal(_customLogger, level, format, std::forward<Args>(args)...);
    }

    // Set the global log level for the logger
    void setLogLevel(LogLevel level);

    // Cache the logging pattern and prepare preformatted patterns for each log level
    void setPattern(const std::string& pattern, bool usesConsoleTag = true);

    // Add a file sink for logging to a file
    void addFileSink(const std::string& filename);

    // Add a rotating file sink for logging to files with rotation based on size
    void addRotatingFileSink(const std::string& filename, std::size_t max_size, std::size_t max_files);

    explicit Logger(const std::string& logFilename = "");
    ~Logger() = default;

    // Disable copy
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Enable move
    Logger(Logger&& other) noexcept
    {
      // Move the members from other to this
      _traceLogger = std::move(other._traceLogger);
      _debugLogger = std::move(other._debugLogger);
      _infoLogger = std::move(other._infoLogger);
      _warningLogger = std::move(other._warningLogger);
      _errorLogger = std::move(other._errorLogger);
      _criticalLogger = std::move(other._criticalLogger);
      _customLogger = std::move(other._customLogger);
      _fileLogger = std::move(other._fileLogger);
      _logFileName = std::move(other._logFileName);
      _cachedPattern = std::move(other._cachedPattern);

      // Reset the state of the moved-from object
      other._traceLogger = nullptr;
      other._debugLogger = nullptr;
      other._infoLogger = nullptr;
      other._warningLogger = nullptr;
      other._errorLogger = nullptr;
      other._criticalLogger = nullptr;
      other._customLogger = nullptr;
      other._fileLogger = nullptr;
      other._logFileName.clear();
      other._cachedPattern.clear();
    }

    Logger& operator=(Logger&& other) noexcept
        {
      if (this != &other)
      {
        // Move the members from other to this
        _traceLogger = std::move(other._traceLogger);
        _debugLogger = std::move(other._debugLogger);
        _infoLogger = std::move(other._infoLogger);
        _warningLogger = std::move(other._warningLogger);
        _errorLogger = std::move(other._errorLogger);
        _criticalLogger = std::move(other._criticalLogger);
        _customLogger = std::move(other._customLogger);
        _fileLogger = std::move(other._fileLogger);
        _logFileName = std::move(other._logFileName);
        _cachedPattern = std::move(other._cachedPattern);

        // Reset the state of the moved-from object
        other._traceLogger = nullptr;
        other._debugLogger = nullptr;
        other._infoLogger = nullptr;
        other._warningLogger = nullptr;
        other._errorLogger = nullptr;
        other._criticalLogger = nullptr;
        other._customLogger = nullptr;
        other._fileLogger = nullptr;
        other._logFileName.clear();
        other._cachedPattern.clear();
      }
      return *this;
    }

  private:
    std::string _logFileName;

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

    template <typename... Args>
    void logInternal(const std::shared_ptr<spdlog::logger>& logger, LogLevel level, const std::string& format, Args&&... args) {
      if constexpr (sizeof...(args) > 0) {
        // Use fmt::format only when there are arguments provided
        std::string formattedMessage = fmt::format(format, std::forward<Args>(args)...);
        switch (level) {
          case LogLevel::Trace:
            logger->trace(formattedMessage);
            _fileLogger->trace(formattedMessage);
            break;
          case LogLevel::Debug:
            logger->debug(formattedMessage);
            _fileLogger->debug(formattedMessage);
            break;
          case LogLevel::Info:
            logger->info(formattedMessage);
            _fileLogger->info(formattedMessage);
            break;
          case LogLevel::Warning:
            logger->warn(formattedMessage);
            _fileLogger->warn(formattedMessage);
            break;
          case LogLevel::Error:
            logger->error(formattedMessage);
            _fileLogger->error(formattedMessage);
            break;
          case LogLevel::Critical:
            logger->critical(formattedMessage);
            _fileLogger->critical(formattedMessage);
            break;
        }
      } else {
        // No arguments, log the raw string
        switch (level) {
          case LogLevel::Trace:
            logger->trace(format);
            _fileLogger->trace(format);
            break;
          case LogLevel::Debug:
            logger->debug(format);
            _fileLogger->debug(format);
            break;
          case LogLevel::Info:
            logger->info(format);
            _fileLogger->info(format);
            break;
          case LogLevel::Warning:
            logger->warn(format);
            _fileLogger->warn(format);
            break;
          case LogLevel::Error:
            logger->error(format);
            _fileLogger->error(format);
            break;
          case LogLevel::Critical:
            logger->critical(format);
            _fileLogger->critical(format);
            break;
        }
      }
    }
  };
} // namespace mgutils

#endif //MGUTILS_LOGGER_H
