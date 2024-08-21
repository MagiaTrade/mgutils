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

  // Enum representing log levels
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
    // Singleton pattern to ensure only one instance of Logger exists
    static Logger& instance()
    {
      static Logger instance;
      return instance;
    }

    // Log a message with a specific log level
    void log(LogLevel level, const std::string& message)
    {
      switch (level)
      {
        case LogLevel::Trace:
          spdlog::set_pattern(trace_pattern_);  // Apply the trace pattern (with TRACE_COLOR)
          spdlog::trace("TRACE: " + message);               // Log the message at the trace level
          break;
        case LogLevel::Debug:
          spdlog::set_pattern(debug_pattern_);  // Apply the debug pattern (with DEBUG_COLOR)
          spdlog::debug("DEBUG: " + message);               // Log the message at the debug level
          break;
        case LogLevel::Info:
          spdlog::set_pattern(info_pattern_);   // Apply the info pattern (with INFO_COLOR)
          spdlog::info("INFO: " + message);                // Log the message at the info level
          break;
        case LogLevel::Warning:
          spdlog::set_pattern(warning_pattern_); // Apply the warning pattern (with WARNING_COLOR)
          spdlog::warn("WARNING: " + message);                 // Log the message at the warning level
          break;
        case LogLevel::Error:
          spdlog::set_pattern(error_pattern_);  // Apply the error pattern (with ERROR_COLOR)
          spdlog::error("ERROR: " + message);               // Log the message at the error level
          break;
        case LogLevel::Critical:
          spdlog::set_pattern(critical_pattern_); // Apply the critical pattern (with CRITICAL_COLOR)
          spdlog::critical("CRITICAL: " + message);              // Log the message at the critical level
          break;
      }
    }

    // Log a custom message with a specified color and log level
    void logCustom(LogLevel level, const std::string& color_code, const std::string& message)
    {
      auto customPattern = color_code + cached_pattern_ + RESET; // Apply custom color to the cached pattern
      spdlog::set_pattern(customPattern);  // Set the custom pattern

      switch (level) {
        case LogLevel::Trace:
          spdlog::trace("{}{}{}", color_code, "TRACE: " + message, RESET); // Log custom trace message
          break;
        case LogLevel::Debug:
          spdlog::debug("{}{}{}", color_code, "DEBUG: " + message, RESET); // Log custom debug message
          break;
        case LogLevel::Info:
          spdlog::info("{}{}{}", color_code, "INFO: " + message, RESET);   // Log custom info message
          break;
        case LogLevel::Warning:
          spdlog::warn("{}{}{}", color_code,"WARNING: " + message, RESET); // Log custom warning message
          break;
        case LogLevel::Error:
          spdlog::error("{}{}{}", color_code,"ERROR: " +  message, RESET); // Log custom error message
          break;
        case LogLevel::Critical:
          spdlog::critical("{}{}{}", color_code,"CRITICAL: " +  message, RESET); // Log custom critical message
          break;
      }
    }

    // Set the global log level for the logger
    void setLogLevel(LogLevel level)
    {
      switch (level) {
        case LogLevel::Trace:
          spdlog::set_level(spdlog::level::trace);  // Enable logging at trace level and above
          break;
        case LogLevel::Debug:
          spdlog::set_level(spdlog::level::debug);  // Enable logging at debug level and above
          break;
        case LogLevel::Info:
          spdlog::set_level(spdlog::level::info);   // Enable logging at info level and above
          break;
        case LogLevel::Warning:
          spdlog::set_level(spdlog::level::warn);   // Enable logging at warning level and above
          break;
        case LogLevel::Error:
          spdlog::set_level(spdlog::level::err);    // Enable logging at error level and above
          break;
        case LogLevel::Critical:
          spdlog::set_level(spdlog::level::critical); // Enable logging at critical level only
          break;
      }
    }

    // Cache the logging pattern and prepare preformatted patterns for each log level
    void setPattern(const std::string& pattern)
    {
      cached_pattern_ = pattern; // Store the pattern for later use

      // Create color-specific patterns for each log level
      trace_pattern_ = TRACE_COLOR + cached_pattern_ + RESET;
      debug_pattern_ = DEBUG_COLOR + cached_pattern_ + RESET;
      info_pattern_ = INFO_COLOR + cached_pattern_ + RESET;
      warning_pattern_ = WARNING_COLOR + cached_pattern_ + RESET;
      error_pattern_ = ERROR_COLOR + cached_pattern_ + RESET;
      critical_pattern_ = CRITICAL_COLOR + cached_pattern_ + RESET;
    }

    // Add a file sink for logging to a file
    void addFileSink(const std::string& filename)
    {
      auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);
      spdlog::default_logger()->sinks().push_back(file_sink);
    }

    // Add a rotating file sink for logging to files with rotation based on size
    void addRotatingFileSink(const std::string& filename, std::size_t max_size, std::size_t max_files)
    {
      auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, max_size, max_files);
      spdlog::default_logger()->sinks().push_back(rotating_sink);
    }

  private:
    Logger()
    {
      try {
        // Initialize the default logger to output to the console with colors
        console_logger_ = spdlog::stdout_color_mt("console");
        spdlog::set_default_logger(console_logger_);
        spdlog::set_level(spdlog::level::debug); // Set the default log level to debug
        cached_pattern_ = "[%Y-%m-%d %H:%M:%S.%f] [thread %t] %v"; // Default log pattern
        setPattern(cached_pattern_);
        spdlog::set_pattern(cached_pattern_);  // Apply the default pattern
      } catch (const spdlog::spdlog_ex& ex) {
        throw std::runtime_error("Failed to initialize logger: " + std::string(ex.what()));
      }
    }

    ~Logger() = default;

    Logger(const Logger&) = delete; // Disable copy constructor
    Logger& operator=(const Logger&) = delete; // Disable assignment operator

    std::shared_ptr<spdlog::logger> console_logger_; // Pointer to the default console logger

    // Cached patterns and preformatted patterns for each log level
    std::string cached_pattern_;
    std::string trace_pattern_;
    std::string debug_pattern_;
    std::string info_pattern_;
    std::string warning_pattern_;
    std::string error_pattern_;
    std::string critical_pattern_;
  };

} // namespace mgutils

#endif //MGUTILS_LOGGER_H
