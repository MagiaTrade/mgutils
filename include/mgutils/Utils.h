//
// Created by Arthur Motelevicz on 21/08/24.
//

#ifndef MGUTILS_UTILS_H
#define MGUTILS_UTILS_H

#include <string>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cstdint>

#define fNaN (std::numeric_limits<float>::quiet_NaN())
#define dNaN (std::numeric_limits<double>::quiet_NaN())

#define INVALID_INT (std::numeric_limits<int>::min())
#define INVALID_INT8 (std::numeric_limits<int8_t>::min())
#define INVALID_INT16 (std::numeric_limits<int16_t>::min())
#define INVALID_INT32 (std::numeric_limits<int32_t>::min())
#define INVALID_INT64 (std::numeric_limits<int64_t>::min())

namespace mgutils
{
  inline std::string generateUUID()
  {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis;

    std::stringstream ss;
    ss << std::hex << std::setfill('0')
       << std::setw(8) << dis(gen) << "-"
       << std::setw(4) << (dis(gen) & 0xFFFF) << "-"
       << std::setw(4) << ((dis(gen) & 0x0FFF) | 0x4000) << "-"
       << std::setw(4) << ((dis(gen) & 0x3FFF) | 0x8000) << "-"
       << std::setw(12) << dis(gen) << dis(gen);

    return ss.str();
  }

  inline std::string generateID(std::size_t length)
  {
    const char characters[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    const std::size_t charactersCount = sizeof(characters) - 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, charactersCount - 1);

    std::string result;
    result.reserve(length);

    for (std::size_t i = 0; i < length; ++i)
    {
      result += characters[dis(gen)];
    }

    return result;
  }

  namespace string
  {

    inline std::string toLower(const std::string& str) {
      std::string result;
      result.reserve(str.size());
      for (char ch : str) {
        result += std::tolower(static_cast<unsigned char>(ch));
      }
      return result;
    }

    inline std::string toUpper(const std::string& str) {
      std::string result;
      result.reserve(str.size());
      for (char ch : str) {
        result += std::toupper(static_cast<unsigned char>(ch));
      }
      return result;
    }

    inline char randomChar()
    {
      const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
      const size_t max_index = (sizeof(charset) - 1);
      static std::default_random_engine rng(std::random_device{}());
      std::uniform_int_distribution<> dist(0, max_index - 1);
      return charset[dist(rng)];
    }

  }// namespace string

} // namespace mgutils

#endif // MGUTILS_UTILS_H
