//
// Created by Arthur Motelevicz on 21/08/24.
//
#include <mgutils/Logger.h>

int main()
{
  // Obtém a instância do logger
  auto& logger = mgutils::Logger::instance();

  // Configura o padrão de log
//  logger.setPattern("[%Y-%m-%d %H:%M:%S.%f] [thread %t] [%^%l%$] %v");
  logger.setPattern("[%Y-%m-%d %H:%M:%S.%f] %v");

  // Define o nível de log para Info
  logger.setLogLevel(mgutils::LogLevel::Debug);

  // Adiciona um sink para log em arquivo
  logger.addFileSink("app.log");

  // Adiciona um sink de arquivo rotativo (5 MB por arquivo, até 3 arquivos)
  logger.addRotatingFileSink("app_rotating.log", 1048576 * 5, 3);

  // Faz alguns logs de exemplo
  logger.log(mgutils::LogLevel::Info, "This is an info log.");
  logger.log(mgutils::LogLevel::Warning, "This is a warning log.");
  logger.log(mgutils::LogLevel::Error, "This is an error log.");

  // Log customizado com cor
  logger.logCustom(mgutils::LogLevel::Debug, mgutils::MAGENTA, "This is a custom MAGENTA log.");

  // Tentativa de log de debug (não será exibido pois o nível de log é Info)
  logger.log(mgutils::LogLevel::Debug, "This is a debug log that will not be shown.");

  return 0;
}
