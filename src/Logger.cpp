//
// Created by Arthur Motelevicz on 21/08/24.
//
#include "Logger.h"
#include "Utils.h"
#include <iostream>

namespace mgutils
{

Logger::Logger(const std::string& logFilename, bool enableConsoleLogging):
_logFileName(logFilename)
{
  _instanceId = generateID(10);
  try {
    // Default log pattern
    _cachedPattern = "[%Y-%m-%d %H:%M:%S.%f] [thread %t] %v";

    //Workround to turn off console logs
    if(!enableConsoleLogging)
    {
      std::vector<spdlog::sink_ptr> console_sinks;
      _traceLogger = std::make_shared<spdlog::logger>(_instanceId + "_trace_logger", console_sinks.begin(), console_sinks.end());
      _debugLogger = std::make_shared<spdlog::logger>(_instanceId +"debug_logger", console_sinks.begin(), console_sinks.end());
      _infoLogger = std::make_shared<spdlog::logger>(_instanceId + "info_logger", console_sinks.begin(), console_sinks.end());
      _warningLogger = std::make_shared<spdlog::logger>(_instanceId + "warning_logger", console_sinks.begin(), console_sinks.end());
      _errorLogger = std::make_shared<spdlog::logger>(_instanceId + "error_logger", console_sinks.begin(), console_sinks.end());
      _criticalLogger = std::make_shared<spdlog::logger>(_instanceId + "critical_logger", console_sinks.begin(), console_sinks.end());
      _customLogger = std::make_shared<spdlog::logger>(_instanceId + "custom_logger", console_sinks.begin(), console_sinks.end());
    }
    else
    {
      _traceLogger = spdlog::stdout_color_mt(_instanceId + "trace_logger");
      _debugLogger = spdlog::stdout_color_mt(_instanceId +"debug_logger");
      _infoLogger = spdlog::stdout_color_mt(_instanceId + "info_logger");
      _warningLogger = spdlog::stdout_color_mt(_instanceId + "warning_logger");
      _errorLogger = spdlog::stdout_color_mt(_instanceId + "error_logger");
      _criticalLogger = spdlog::stdout_color_mt(_instanceId + "critical_logger");
      _customLogger = spdlog::stdout_color_mt(_instanceId + "custom_logger");
    }

    if(!_logFileName.empty())
      addFileSink(_logFileName);

    setPattern(_cachedPattern);

    setLogLevel(LogLevel::Trace);

  } catch (const spdlog::spdlog_ex& ex) {
    throw std::runtime_error("Failed to initialize logger: " + std::string(ex.what()));
  } catch (const std::exception& ex) {
    throw std::runtime_error("Unexpected error during logger initialization: " + std::string(ex.what()));
  }
}

Logger& Logger::instance()
{
  static Logger instance;
  return instance;
}

std::string Logger::getLogFilename() const
{
  return _logFileName;
}

LogMessage Logger::log(LogLevel level) {
  return {*this, level};
}

void Logger::logStream(LogLevel level, const std::string& message)
{
  log(level, message);
}


void Logger::flush() const
{
  _traceLogger->flush();
  _debugLogger->flush();
  _infoLogger->flush();
  _warningLogger->flush();
  _errorLogger->flush();
  _criticalLogger->flush();
  _customLogger->flush();
  if(_fileLogger)
    _fileLogger->flush();
}

// Set the global log level for the logger
void Logger::setLogLevel(LogLevel level)
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
      return;
  }

  _currentLevel = level;
}

// Cache the logging pattern and prepare preformatted patterns for each log level
void Logger::setPattern(const std::string& pattern, bool usesConsoleTag)
{
  if(usesConsoleTag)
    _cachedPattern = "%L: " + pattern;
  else
    _cachedPattern = pattern;

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


  if(_fileLogger)
    _fileLogger->set_pattern(_cachedPattern);
}

// Add a file sink for logging to a file
  void Logger::addFileSink(const std::string& filename)
  {
    // Se o _fileLogger ainda não foi inicializado, inicialize-o como basic_logger_mt
    if(!_fileLogger)
    {
      _fileLogger = spdlog::basic_logger_mt(_instanceId + "file_logger", filename);
      setLogLevel(_currentLevel);
      _fileLogger->set_pattern(_cachedPattern);  // Definir o padrão
    }
    else
    {
      // Se já existe, adicione o novo sink
      auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);
      _fileLogger->sinks().push_back(file_sink);
    }

    _fileLogger->flush_on(spdlog::level::err);  // Exemplo: Forçar flush em erros
  }

// Add a rotating file sink for logging to files with rotation based on size
  void Logger::addRotatingFileSink(const std::string& filename, std::size_t max_size, std::size_t max_files)
  {
    // Se o _fileLogger ainda não foi inicializado, inicialize-o como basic_logger_mt
    if(!_fileLogger)
    {
      auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, max_size, max_files);
      _fileLogger = std::make_shared<spdlog::logger>(_instanceId + "file_logger", rotating_sink);
      setLogLevel(_currentLevel);
      _fileLogger->set_pattern(_cachedPattern);  // Definir o padrão
    }
    else
    {
      // Se já existe, adicione o novo sink rotativo
      auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, max_size, max_files);
      _fileLogger->sinks().push_back(rotating_sink);
    }
  }

LogMessage::LogMessage(Logger& logger, LogLevel level):
_logger(logger), _level(level)
{}

LogMessage::~LogMessage()
{
  _logger.logStream(_level, _stream.str());
  _logger.flush();
}


} // namespace mgutils
