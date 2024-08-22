#ifndef MGUTILS_ERRORMANAGER_H
#define MGUTILS_ERRORMANAGER_H

#include <boost/signals2.hpp>
#include <functional>
#include <string>

namespace mgutils
{
  struct ErrorInfo
  {
    int errorCode;
    std::string errorMessage;
    std::string file;
    int line;

    ErrorInfo(int code, const std::string &message, const std::string &file, int line)
        : errorCode(code), errorMessage(message), file(file), line(line) {}
  };

  using ErrorSignal = boost::signals2::signal<void(const ErrorInfo& info)>;

  class ErrorManager {
  public:

    static ErrorManager& instance()
    {
      static ErrorManager instance;
      return instance;
    }

    boost::signals2::connection subscribe(const std::function<void(const ErrorInfo& info)>& subscriber)
    {
      return _errorSignal.connect(subscriber);
    }

    void notify(const ErrorInfo& info)
    {
      _errorSignal(info);
    }

    ErrorManager(const ErrorManager&) = delete;
    ErrorManager& operator=(const ErrorManager&) = delete;

  private:
    ErrorManager() = default;
    ErrorSignal _errorSignal;
  };
}

#endif //MGUTILS_ERRORMANAGER_H
