//
// Created by Arthur Motelevicz on 21/08/24.
//

#ifndef MGUTILS_ERRORMANAGER_H
#define MGUTILS_ERRORMANAGER_H

#include <boost/signals2.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <functional>
#include <string>
#include <mutex>
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
      std::lock_guard<std::mutex> lock(_mutex);
      return _errorSignal.connect(subscriber);
    }

    void notify(const ErrorInfo& info)
    {
      std::lock_guard<std::mutex> lock(_mutex);
      _errorSignal(info);
    }

    ErrorManager(const ErrorManager&) = delete;
    ErrorManager& operator=(const ErrorManager&) = delete;

  private:
    ErrorManager() = default;
    ErrorSignal _errorSignal;
    std::mutex _mutex;
  };
}

#endif //MGUTILS_ERRORMANAGER_H
