//
// Created by Arthur Motelevicz on 22/08/24.
//
#include <catch2/catch.hpp>
#include <mgutils/Logger.h>
#include <thread>
#include <atomic>

using namespace mgutils;

// Test case for basic subscription and notification of errors
TEST_CASE("ErrorManager basic subscription and notification", "[error_manager]")
{
  ErrorInfo receivedError{0, "", "", 0};

  // Subscribe to receive error notifications
  auto connection = ErrorManager::instance().subscribe(
      [&receivedError](const ErrorInfo& error) {
        receivedError = error;
      }
  );

  // Notify an error
  ErrorInfo error(404, "Not Found", "main.cpp", 42);
  ErrorManager::instance().notify(error);

  // Verify that the error was received correctly
  REQUIRE(receivedError.errorCode == 404);
  REQUIRE(receivedError.errorMessage == "Not Found");
  REQUIRE(receivedError.file == "main.cpp");
  REQUIRE(receivedError.line == 42);

  // Disconnect the subscriber
  connection.disconnect();
}

// Test case to check if multiple subscribers receive the same error notification
TEST_CASE("ErrorManager multiple subscribers", "[error_manager]")
{
  ErrorInfo receivedError1{0, "", "", 0};
  ErrorInfo receivedError2{0, "", "", 0};

  // Subscribe two different callbacks to the error manager
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

  // Notify an error
  ErrorInfo error(500, "Internal Server Error", "server.cpp", 88);
  ErrorManager::instance().notify(error);

  // Verify that both subscribers received the error correctly
  REQUIRE(receivedError1.errorCode == 500);
  REQUIRE(receivedError1.errorMessage == "Internal Server Error");
  REQUIRE(receivedError1.file == "server.cpp");
  REQUIRE(receivedError1.line == 88);

  REQUIRE(receivedError2.errorCode == 500);
  REQUIRE(receivedError2.errorMessage == "Internal Server Error");
  REQUIRE(receivedError2.file == "server.cpp");
  REQUIRE(receivedError2.line == 88);

  // Disconnect the subscribers
  connection1.disconnect();
  connection2.disconnect();
}

// Test case to verify that a subscriber is properly disconnected
TEST_CASE("ErrorManager subscriber disconnection", "[error_manager]")
{
  ErrorInfo receivedError1{0, "", "", 0};
  ErrorInfo receivedError2{0, "", "", 0};

  // Subscribe two different callbacks to the error manager
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

  // Disconnect the first subscriber
  connection1.disconnect();

  // Notify an error
  ErrorInfo error(403, "Forbidden", "access.cpp", 24);
  ErrorManager::instance().notify(error);

  // Verify that the first subscriber did not receive the error
  REQUIRE(receivedError1.errorCode == 0);
  REQUIRE(receivedError1.errorMessage.empty());

  // Verify that the second subscriber received the error correctly
  REQUIRE(receivedError2.errorCode == 403);
  REQUIRE(receivedError2.errorMessage == "Forbidden");
  REQUIRE(receivedError2.file == "access.cpp");
  REQUIRE(receivedError2.line == 24);

  // Disconnect the second subscriber
  connection2.disconnect();
}

// Test case to verify that ErrorManager handles notifications correctly in a multithreading scenario
TEST_CASE("ErrorManager multithreading", "[error_manager][multithreading]")
{
  std::atomic<int> counter(0);

  // Subscribe to receive error notifications and increment the counter
  auto connection = ErrorManager::instance().subscribe(
      [&counter](const ErrorInfo& error) {
        counter.fetch_add(1, std::memory_order_relaxed);
      }
  );

  // Function to notify errors in a loop
  auto notifyFunc = []() {
    for (int i = 0; i < 1000; ++i) {
      ErrorInfo error(500, "Internal Server Error", "server.cpp", i);
      ErrorManager::instance().notify(error);
    }
  };

  // Create two threads to notify errors concurrently
  std::thread thread1(notifyFunc);
  std::thread thread2(notifyFunc);

  // Wait for both threads to finish
  thread1.join();
  thread2.join();

  // Verify that the counter reflects the total number of notifications
  REQUIRE(counter.load() == 2000);

  // Disconnect the subscriber
  connection.disconnect();
}

// Test case to verify that logger error notifications triggers the ErrorManager correctly
TEST_CASE("ErrorManager from logger notification error", "[error_manager]")
{
  ErrorInfo receivedError{0, "", "", 0};

  // Subscribe to receive error notifications
  auto connection = ErrorManager::instance().subscribe(
      [&receivedError](const ErrorInfo& error) {
        receivedError = error;
      }
  );

  // Capture the current line number for verification
  int expectedLine = __LINE__ + 1;
  NOTIFY_ERROR(404, "Not Found");

  // Verify that the error was received correctly
  REQUIRE(receivedError.errorCode == 404);
  REQUIRE(receivedError.errorMessage == "Not Found");

  // Verify that the correct file is reported
  bool foundFile = receivedError.file.find("error_tests.cpp") != std::string::npos;
  REQUIRE(foundFile);

  // Verify that the correct line number is reported
  REQUIRE(receivedError.line == expectedLine);

  // Disconnect the subscriber
  connection.disconnect();
}

// Test case to verify that logger critical notifications triggers the ErrorManager correctly
TEST_CASE("ErrorManager from logger notification critical", "[error_manager]")
{
  ErrorInfo receivedError{0, "", "", 0};

  // Subscribe to receive error notifications
  auto connection = ErrorManager::instance().subscribe(
      [&receivedError](const ErrorInfo& error) {
        receivedError = error;
      }
  );

  // Capture the current line number for verification
  int expectedLine = __LINE__ + 1;
  NOTIFY_CRITICAL(500, "Crash!");

  // Verify that the critical error was received correctly
  REQUIRE(receivedError.errorCode == 500);
  REQUIRE(receivedError.errorMessage == "Crash!");

  // Verify that the correct file is reported
  bool foundFile = receivedError.file.find("error_tests.cpp") != std::string::npos;
  REQUIRE(foundFile);

  // Verify that the correct line number is reported
  REQUIRE(receivedError.line == expectedLine);

  // Disconnect the subscriber
  connection.disconnect();
}
