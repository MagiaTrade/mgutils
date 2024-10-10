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
#include <cmath>  // Para isnan()
#include <cstdint>
#include <cctype>
#include <chrono>
#include <iostream>
#include <ctime>

#define fNaN (std::numeric_limits<float>::quiet_NaN())

#define dNaN (std::numeric_limits<double>::quiet_NaN())
#define INVALID_INT (std::numeric_limits<int>::min())
#define INVALID_INT8 (std::numeric_limits<int8_t>::min())
#define INVALID_INT16 (std::numeric_limits<int16_t>::min())
#define INVALID_INT32 (std::numeric_limits<int32_t>::min())

#define INVALID_INT64 (std::numeric_limits<int64_t>::min())
#define INVALID_UINT32 (std::numeric_limits<uint32_t>::max())

#define INVALID_UINT64 (std::numeric_limits<uint64_t>::max())

#define INVALID_CHAR '\0'


namespace mgutils
{
  inline int64_t parseDateTimeWithFormat(const std::string& dateTimeStr, const std::string& format)
  {
    std::tm tm = {};
    int32_t milliseconds = 0;

    // Primeiro, faz o parse da parte da data e hora
    size_t dotPos = dateTimeStr.find('.'); // Encontra a posição do ponto (separador de milissegundos)
    std::string dateTimePart = dateTimeStr.substr(0, dotPos); // Pega só a parte da data e hora

    std::istringstream ss(dateTimePart);
    ss >> std::get_time(&tm, format.c_str());

    // Verifica se a parte da data e hora foi completamente lida
    if (ss.fail() || !ss.eof())
    {
      throw std::invalid_argument("Failed to parse date and time with the given format.");
    }

    // Se houver ponto, tenta extrair a parte dos milissegundos
    if (dotPos != std::string::npos)
    {
      std::string millisPart = dateTimeStr.substr(dotPos + 1);
      try
      {
        milliseconds = std::stoi(millisPart);
        if (milliseconds < 0 || milliseconds > 999)
        {
          throw std::out_of_range("Milliseconds out of range!");
        }
      }
      catch (const std::exception&)
      {
        throw std::invalid_argument("Invalid milliseconds part.");
      }
    }

    // Converte a data/hora para tempo desde o epoch
    auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    auto timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint.time_since_epoch()).count();

    // Adiciona os milissegundos, se houver
    return timeMillis + milliseconds;
  }

  inline int64_t parseTimeWithFormat(const std::string& timeStr, const std::string& format)
  {
    std::tm tm = {};
    int32_t milliseconds = 0;

    // Verifica se o formato contém o ponto de separação para milissegundos
    bool hasMillisSeparator = format.find('.') != std::string::npos;

    std::string timePart;
    std::string millisPart = "000"; // Assumimos 0 milissegundos por padrão

    if (hasMillisSeparator)
    {
      // Encontra a posição do ponto (separador de milissegundos)
      size_t dotPos = timeStr.find('.');
      timePart = timeStr.substr(0, dotPos); // Parte de tempo sem milissegundos

      // Se houver ponto, extrai a parte dos milissegundos
      if (dotPos != std::string::npos)
      {
        millisPart = timeStr.substr(dotPos + 1); // Extrai a parte dos milissegundos
      }
    }
    else
    {
      // Caso não tenha ponto de milissegundos, assume que os últimos 3 dígitos são os milissegundos
      if (timeStr.length() > 8) // HHMMSSMMM
      {
        timePart = timeStr.substr(0, timeStr.length() - 3);  // Parte de HHMMSS
        millisPart = timeStr.substr(timeStr.length() - 3);   // Últimos 3 dígitos são os milissegundos
      }
      else
      {
        timePart = timeStr; // Sem milissegundos
      }
    }

    // Faz o parsing da parte de tempo (HHMMSS) de acordo com o formato fornecido
    std::istringstream ss(timePart);
    ss >> std::get_time(&tm, format.c_str());

    if (ss.fail() || !ss.eof())
    {
      throw std::invalid_argument("Failed to parse time with the given format.");
    }

    // Tenta converter os milissegundos (assumidos ou extraídos)
    try
    {
      milliseconds = std::stoi(millisPart);
      if (milliseconds < 0 || milliseconds > 999)
      {
        throw std::out_of_range("Milliseconds out of range!");
      }
    }
    catch (const std::exception&)
    {
      throw std::invalid_argument("Invalid milliseconds part.");
    }

    // Converte o tempo para milissegundos
    auto timeOffset = std::chrono::hours(tm.tm_hour) + std::chrono::minutes(tm.tm_min) + std::chrono::seconds(tm.tm_sec) +
                      std::chrono::milliseconds(milliseconds);

    return timeOffset.count();
  }

  [[deprecated("Use parseTimeWithFormat() instead")]] inline int64_t parseTimeToMillis(const std::string& timeStr)
  {
    // timeStr can be in format HHMMSS or HHMMSSMMM
    std::tm tm = {};
    std::string hourStr;
    std::string minuteStr;
    std::string secondStr;
    std::string millisecondsStr = "000";

    int32_t hours;
    int32_t minutes;
    int32_t seconds;
    int32_t milliseconds;

    // Check if the timeStr length is valid
    if (timeStr.length() == 6 || timeStr.length() == 9)
    {
      // Extract HH, MM, SS
      hourStr = timeStr.substr(0, 2);
      minuteStr = timeStr.substr(2, 2);
      secondStr = timeStr.substr(4, 2);

      if (timeStr.length() == 9)
        millisecondsStr = timeStr.substr(6, 3);

      try
      {
        hours = std::stoi(hourStr);
        minutes = std::stoi(minuteStr);
        seconds = std::stoi(secondStr);
        milliseconds = std::stoi(millisecondsStr);
      }
      catch (const std::exception&)
      {
        throw std::invalid_argument("Invalid time components in time string");
      }

      if (hours < 0 || hours > 23 ||
          minutes < 0 || minutes > 59 ||
          seconds < 0 || seconds > 59 ||
          milliseconds < 0 || milliseconds > 999)
      {
        throw std::out_of_range("Time values out of range!");
      }

    }
    else
    {
      throw std::invalid_argument("Invalid format. Use HHMMSSMMM or HHMMSS");
    }

    auto timeOffset = std::chrono::hours(hours) + std::chrono::minutes(minutes) + std::chrono::seconds(seconds) +
                      std::chrono::milliseconds(milliseconds);

    return timeOffset.count();
  }

  inline static std::chrono::system_clock::time_point parseDate(const std::string& date)
  {
    std::tm tm = {};
    std::stringstream ss(date);
    ss >> std::get_time(&tm, "%Y%m%d");
    auto time = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time);
  }

  inline static int64_t parseDateToMillis(const std::string& date)
  {
    auto time = parseDate(date);
    int64_t milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
    return milliseconds_since_epoch;
  }

  inline int64_t getCurrentTimestampInMillis() {
    auto now = std::chrono::system_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return millis;
  }

  inline static std::string getDateFromTimestamp(int64_t timestamp)
  {
    std::time_t timeInSeconds = timestamp / 1000;
    std::tm* tm = std::gmtime(&timeInSeconds);
    char buffer[9];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d", tm);

    return buffer;
  }


  inline std::string extractTimestampTimeWithMillis(int64_t timestampMs)
  {
    auto now_time_t = static_cast<std::time_t>(timestampMs / 1000);
    auto now_ms = timestampMs % 1000;
    std::tm now_tm = *std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%H%M%S") << std::setw(3) << std::setfill('0') << now_ms;
    return oss.str();
  }


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

  inline int generateRandomInt(int min, int max)
  {
   // Inicializa o motor de números aleatórios com um gerador de números pseudo-aleatórios
   static std::random_device rd;  // Usa um gerador de números aleatórios de hardware se disponível
   static std::mt19937 gen(rd()); // Mersenne Twister 19937 como motor padrão

   // Define a distribuição de inteiros no intervalo [min, max]
   std::uniform_int_distribution<> distrib(min, max);

   // Gera um número inteiro aleatório
   return distrib(gen);
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


    inline bool isNumber(const std::string& str) {
      return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
    }


  }// namespace string

} // namespace mgutils

#endif // MGUTILS_UTILS_H
