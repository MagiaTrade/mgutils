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
  constexpr const char* RESET = "\033[0m";
  constexpr const char* RED = "\033[31m";
  constexpr const char* GREEN = "\033[32m";
  constexpr const char* YELLOW = "\033[33m";
  constexpr const char* BLUE = "\033[34m";
  constexpr const char* MAGENTA = "\033[35m";
  constexpr const char* CYAN = "\033[36m";
  constexpr const char* WHITE = "\033[39m";
  constexpr const char* DARK_GRAY = "\033[38;5;8m";
  constexpr const char* DARK_YELLOW = "\033[33m\033[2m";

  enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
  };

  class Logger {
  public:
    static Logger& instance()
    {
      static Logger instance;
      return instance;
    }

    void log(LogLevel level, const std::string& message)
    {
      switch (level)
      {
        case LogLevel::Debug:
          logCustom(level, WHITE, message);
          break;
        case LogLevel::Info:
          logCustom(level, DARK_GRAY, message);
          break;
        case LogLevel::Warning:
          logCustom(level, DARK_YELLOW, message);
          break;
        case LogLevel::Error:
          logCustom(level, RED, message);
          break;
      }
    }

    void logCustom(LogLevel level, const std::string& color_code, const std::string& message)
    {
      switch (level) {
        case LogLevel::Debug:
          spdlog::debug("{}{}{}", color_code, "DEBUG: " + message, RESET);
          break;
        case LogLevel::Info:
          spdlog::info("{}{}{}", color_code, "INFO: " + message, RESET);
          break;
        case LogLevel::Warning:
          spdlog::warn("{}{}{}", color_code,"WARNING: " + message, RESET);
          break;
        case LogLevel::Error:
          spdlog::error("{}{}{}", color_code,"ERROR: " +  message, RESET);
          break;
      }
    }
      void setLogLevel(LogLevel level)
    {
      switch (level) {
        case LogLevel::Debug:
          spdlog::set_level(spdlog::level::debug);
          break;
        case LogLevel::Info:
          spdlog::set_level(spdlog::level::info);
          break;
        case LogLevel::Warning:
          spdlog::set_level(spdlog::level::warn);
          break;
        case LogLevel::Error:
          spdlog::set_level(spdlog::level::err);
          break;
      }
    }

    void setPattern(const std::string& pattern)
    {
      spdlog::set_pattern(pattern);
    }

    void addFileSink(const std::string& filename)
    {
      auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);
      spdlog::default_logger()->sinks().push_back(file_sink);
    }

    void addRotatingFileSink(const std::string& filename, std::size_t max_size, std::size_t max_files)
    {
      auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, max_size, max_files);
      spdlog::default_logger()->sinks().push_back(rotating_sink);
    }

  private:
    Logger()
    {
      try {
        // Configura o logger padrão para o console com cores
        console_logger_ = spdlog::stdout_color_mt("console");
        spdlog::set_default_logger(console_logger_);
        spdlog::set_level(spdlog::level::debug); // Define o nível de log inicial como debug
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%f] [thread %t] [%^%l%$] %v"); // Formato de log padrão
      } catch (const spdlog::spdlog_ex& ex) {
        throw std::runtime_error("Failed to initialize logger: " + std::string(ex.what()));
      }
    }

    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::shared_ptr<spdlog::logger> console_logger_;
  };

} // namespace mgutils

#endif //MGUTILS_LOGGER_H
