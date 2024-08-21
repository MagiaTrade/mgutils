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

  // Define color variables for each log level
  constexpr const char* TRACE_COLOR = DARK_GRAY;
  constexpr const char* DEBUG_COLOR = LIGHT_GRAY;
  constexpr const char* INFO_COLOR = BLUE;
  constexpr const char* WARNING_COLOR = DARK_YELLOW;
  constexpr const char* ERROR_COLOR = RED;
  constexpr const char* CRITICAL_COLOR = BRIGHT_RED;

  enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
  };

  class Logger {
  public:

    static Logger& instance()
    {
      static Logger instance;
      return instance;
    }

    void flush() const
    {
      _traceLogger->flush();
      _debugLogger->flush();
      _infoLogger->flush();
      _warningLogger->flush();
      _errorLogger->flush();
      _criticalLogger->flush();
      _customLogger->flush();
      _fileLogger->flush();
    }

    void log(LogLevel level, const std::string& message)
    {
      switch (level)
      {
        case LogLevel::Trace:
          _traceLogger->trace("TRACE: " + message);
          _fileLogger->trace("TRACE: " + message);
          break;
        case LogLevel::Debug:
          _debugLogger->debug("DEBUG: " + message);
          _fileLogger->debug("DEBUG: " + message);
          break;
        case LogLevel::Info:
          _infoLogger->info("INFO: " + message);
          _fileLogger->info("INFO: " + message);
          break;
        case LogLevel::Warning:
          _warningLogger->warn("WARNING: " + message);
          _fileLogger->warn("WARNING: " + message);
          break;
        case LogLevel::Error:
          _errorLogger->error("ERROR: " + message);
          _fileLogger->error("ERROR: " + message);
          break;
        case LogLevel::Critical:
          _criticalLogger->critical("CRITICAL: " + message);
          _fileLogger->critical("CRITICAL: " + message);
          break;
      }
    }

    // Log a custom message with a specified color and log level
    void logCustom(LogLevel level, const std::string& color_code, const std::string& message)
    {
      std::string customPattern = color_code + _cachedPattern + RESET;
      _customLogger->set_pattern(customPattern);

      switch (level) {
        case LogLevel::Trace:
          _customLogger->trace("TRACE: " + message);
          _fileLogger->trace("TRACE: " + message);
          break;
        case LogLevel::Debug:
          _customLogger->debug("DEBUG: " + message);
          _fileLogger->debug("DEBUG: " + message);
          break;
        case LogLevel::Info:
          _customLogger->info("INFO: " + message);
          _fileLogger->info("INFO: " + message);
          break;
        case LogLevel::Warning:
          _customLogger->warn("WARNING: " + message);
          _fileLogger->warn("WARNING: " + message);

          break;
        case LogLevel::Error:
          _customLogger->error("ERROR: " + message);
          _fileLogger->error("ERROR: " + message);
          break;
        case LogLevel::Critical:
          _customLogger->critical("CRITICAL: " + message);
          _fileLogger->critical("CRITICAL: " + message);
          break;
      }
    }

    // Set the global log level for the logger
    void setLogLevel(LogLevel level)
    {
      switch (level)
      {
        case LogLevel::Trace:
#ifdef DEBUG
          spdlog::set_level(spdlog::level::trace);
#else
          spdlog::set_level(spdlog::level::info);
#endif
          break;
        case LogLevel::Debug:
#ifdef DEBUG
          spdlog::set_level(spdlog::level::debug);
#else
          spdlog::set_level(spdlog::level::info);
#endif
          break;
        case LogLevel::Info:
          spdlog::set_level(spdlog::level::info);   // Enable logging at info level and above for all loggers
          break;
        case LogLevel::Warning:
          spdlog::set_level(spdlog::level::warn);   // Enable logging at warning level and above for all loggers
          break;
        case LogLevel::Error:
          spdlog::set_level(spdlog::level::err);    // Enable logging at error level and above for all loggers
          break;
        case LogLevel::Critical:
          spdlog::set_level(spdlog::level::critical); // Enable logging at critical level only
          break;
        default:
          break;
      }
    }

    // Cache the logging pattern and prepare preformatted patterns for each log level
    void setPattern(const std::string& pattern)
    {
      _cachedPattern = pattern; // Store the pattern for later use on log custom

      _tracePattern = TRACE_COLOR + _cachedPattern + RESET;
      _debugPattern = DEBUG_COLOR + _cachedPattern + RESET;
      _infoPattern = INFO_COLOR + _cachedPattern + RESET;
      _warningPattern = WARNING_COLOR + _cachedPattern + RESET;
      _errorPattern = ERROR_COLOR + _cachedPattern + RESET;
      _criticalPattern = CRITICAL_COLOR + _cachedPattern + RESET;

      _traceLogger->set_pattern(_tracePattern);
      _debugLogger->set_pattern(_debugPattern);
      _infoLogger->set_pattern(_infoPattern);
      _warningLogger->set_pattern(_warningPattern);
      _errorLogger->set_pattern(_errorPattern);
      _criticalLogger->set_pattern(_criticalPattern);

      _fileLogger->set_pattern(_cachedPattern);
    }

    // Add a file sink for logging to a file
    void addFileSink(const std::string& filename)
    {
      auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);
      _fileLogger->sinks().push_back(file_sink);
      _fileLogger->set_pattern(_cachedPattern);
    }

    // Add a rotating file sink for logging to files with rotation based on size
    void addRotatingFileSink(const std::string& filename, std::size_t max_size, std::size_t max_files)
    {
      auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, max_size, max_files);
      _fileLogger->sinks().push_back(rotating_sink);
      _fileLogger->set_pattern(_cachedPattern);
    }

  private:
    Logger()
    {
      try {
        _cachedPattern = "[%Y-%m-%d %H:%M:%S.%f] [thread %t] %v"; // Default log pattern

        // Inicializa os loggers com cores específicas para o console
        _traceLogger = spdlog::stdout_color_mt("trace_logger");
        _traceLogger->set_pattern(TRACE_COLOR + _cachedPattern + RESET);

        _debugLogger = spdlog::stdout_color_mt("debug_logger");
        _debugLogger->set_pattern(DEBUG_COLOR + _cachedPattern + RESET);

        _infoLogger = spdlog::stdout_color_mt("info_logger");
        _infoLogger->set_pattern(INFO_COLOR + _cachedPattern + RESET);

        _warningLogger = spdlog::stdout_color_mt("warning_logger");
        _warningLogger->set_pattern(WARNING_COLOR + _cachedPattern + RESET);

        _errorLogger = spdlog::stdout_color_mt("error_logger");
        _errorLogger->set_pattern(ERROR_COLOR + _cachedPattern + RESET);

        _criticalLogger = spdlog::stdout_color_mt("critical_logger");
        _criticalLogger->set_pattern(CRITICAL_COLOR + _cachedPattern + RESET);

        _customLogger = spdlog::stdout_color_mt("custom_logger");
        _customLogger->set_pattern(_cachedPattern);

        // Logger para arquivo, sem cores, mas com rotação de arquivos
        _fileLogger = spdlog::basic_logger_mt("file_logger", "logs.txt");
        _fileLogger->set_pattern(_cachedPattern);

#ifdef DEBUG
        spdlog::set_level(spdlog::level::trace);
#else
        spdlog::set_level(spdlog::level::info);
#endif

      } catch (const spdlog::spdlog_ex& ex) {
        throw std::runtime_error("Failed to initialize logger: " + std::string(ex.what()));
      } catch (const std::exception& ex) {
        throw std::runtime_error("Unexpected error during logger initialization: " + std::string(ex.what()));
      }
    }

    ~Logger() = default;

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
  };

} // namespace mgutils

#endif //MGUTILS_LOGGER_H
