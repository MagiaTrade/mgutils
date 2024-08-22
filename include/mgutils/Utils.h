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

  }// namespace string

} // namespace mgutils

#endif // MGUTILS_UTILS_H
