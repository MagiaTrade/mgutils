//
// Created by Arthur Motelevicz on 22/08/24.
//
#include <catch2/catch.hpp>
#include <mgutils/Logger.h>
#include <thread>
#include <atomic>

using namespace mgutils;

TEST_CASE("ErrorManager basic subscription and notification", "[ErrorManager]")
{
  ErrorInfo receivedError{0, "", "", 0};

  auto connection = ErrorManager::instance().subscribe(
    [&receivedError](const ErrorInfo& error) {
      receivedError = error;
    }
  );

  ErrorInfo error(404, "Not Found", "main.cpp", 42);
  ErrorManager::instance().notify(error);

  REQUIRE(receivedError.errorCode == 404);
  REQUIRE(receivedError.errorMessage == "Not Found");
  REQUIRE(receivedError.file == "main.cpp");
  REQUIRE(receivedError.line == 42);

  connection.disconnect();
}

TEST_CASE("ErrorManager multiple subscribers", "[ErrorManager]")
{
  ErrorInfo receivedError1{0, "", "", 0};
  ErrorInfo receivedError2{0, "", "", 0};

  auto connection1 = ErrorManager::instance().subscribe(
    [&receivedError1](const ErrorInfo& error) {
      receivedError1 = error;
    }
  );

  auto connection2 = ErrorManager::instance().subscribe(
      [&receivedError2](const ErrorInfo& error) {
        receivedError2 = error;
      }
  );

  ErrorInfo error(500, "Internal Server Error", "server.cpp", 88);
  ErrorManager::instance().notify(error);

  REQUIRE(receivedError1.errorCode == 500);
  REQUIRE(receivedError1.errorMessage == "Internal Server Error");
  REQUIRE(receivedError1.file == "server.cpp");
  REQUIRE(receivedError1.line == 88);

  REQUIRE(receivedError2.errorCode == 500);
  REQUIRE(receivedError2.errorMessage == "Internal Server Error");
  REQUIRE(receivedError2.file == "server.cpp");
  REQUIRE(receivedError2.line == 88);

  connection1.disconnect();
  connection2.disconnect();
}

TEST_CASE("ErrorManager subscriber disconnection", "[ErrorManager]")
{
  ErrorInfo receivedError1{0, "", "", 0};
  ErrorInfo receivedError2{0, "", "", 0};

  auto connection1 = ErrorManager::instance().subscribe(
    [&receivedError1](const ErrorInfo& error) {
      receivedError1 = error;
    }
  );

  auto connection2 = ErrorManager::instance().subscribe(
    [&receivedError2](const ErrorInfo& error) {
      receivedError2 = error;
    }
  );

  connection1.disconnect();

  ErrorInfo error(403, "Forbidden", "access.cpp", 24);
  ErrorManager::instance().notify(error);

  REQUIRE(receivedError1.errorCode == 0);
  REQUIRE(receivedError1.errorMessage.empty());

  REQUIRE(receivedError2.errorCode == 403);
  REQUIRE(receivedError2.errorMessage == "Forbidden");
  REQUIRE(receivedError2.file == "access.cpp");
  REQUIRE(receivedError2.line == 24);

  connection2.disconnect();
}

TEST_CASE("ErrorManager multithreading", "[ErrorManager][multithreading]")
{
  std::atomic<int> counter(0);

  auto connection = ErrorManager::instance().subscribe(
      [&counter](const ErrorInfo& error) {
        counter.fetch_add(1, std::memory_order_relaxed);
      }
  );

  auto notifyFunc = []() {
    for (int i = 0; i < 1000; ++i) {
      ErrorInfo error(500, "Internal Server Error", "server.cpp", i);
      ErrorManager::instance().notify(error);
    }
  };

  std::thread thread1(notifyFunc);
  std::thread thread2(notifyFunc);

  thread1.join();
  thread2.join();

  REQUIRE(counter.load() == 2000);

  connection.disconnect();
}

TEST_CASE("ErrorManager from logger notification error", "[ErrorManager]")
{
  ErrorInfo receivedError{0, "", "", 0};

  auto connection = ErrorManager::instance().subscribe(
      [&receivedError](const ErrorInfo& error) {
        receivedError = error;
      }
  );

  int expectedLine = __LINE__ + 1;
  NOTIFY_ERROR(404, "Not Found");

  REQUIRE(receivedError.errorCode == 404);
  REQUIRE(receivedError.errorMessage == "Not Found");

  bool foundFile = receivedError.file.find("error_tests.cpp") != std::string::npos;
  REQUIRE(foundFile);

  REQUIRE(receivedError.line == expectedLine);

  connection.disconnect();
}

TEST_CASE("ErrorManager from logger notification critical", "[ErrorManager]")
{
  ErrorInfo receivedError{0, "", "", 0};

  auto connection = ErrorManager::instance().subscribe(
    [&receivedError](const ErrorInfo& error) {
      receivedError = error;
    }
  );

  int expectedLine = __LINE__ + 1;
  NOTIFY_CRITICAL(500, "Crash!");

  REQUIRE(receivedError.errorCode == 500);
  REQUIRE(receivedError.errorMessage == "Crash!");

  bool foundFile = receivedError.file.find("error_tests.cpp") != std::string::npos;
  REQUIRE(foundFile);

  REQUIRE(receivedError.line == expectedLine);

  connection.disconnect();
}