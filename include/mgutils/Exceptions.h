//
// Created by Arthur Motelevicz on 26/08/24.
//

#ifndef MGUTILS_EXCEPTIONS_H
#define MGUTILS_EXCEPTIONS_H

#include <stdexcept>
#include "Logger.h"

namespace mgutils
{
  class Exception : public std::runtime_error
  {
  public:
    explicit Exception(const std::string& message)
        : std::runtime_error(message)
    {
      NOTIFY_ERROR(-1,message);
    }

    Exception()
        : std::runtime_error("Exception with no message!")
    {
      NOTIFY_ERROR(-1,"Exception with no message!");
    }
  };

  class FilesException : Exception {
  public:
   using Exception::Exception;
  };

  class JsonParseException : Exception {
  public:
    using Exception::Exception;
  };

  class JsonUsageException : Exception {
  public:
    using Exception::Exception;
  };

  class CsvWriteException : Exception {
  public:
    using Exception::Exception;
  };

  class CsvReadException : Exception {
  public:
    using Exception::Exception;
  };

  class CsvUsageException : Exception {
  public:
    using Exception::Exception;
  };
} // namespace mgutils

#endif //MGUTILS_EXCEPTIONS_H
