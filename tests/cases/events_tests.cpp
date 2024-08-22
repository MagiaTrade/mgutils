//
// Created by Arthur Motelevicz on 22/08/24.
//
#include <catch2/catch.hpp>
#include <mgutils/Logger.h>
#include <mgutils/EventManager.h>
#include <iostream>
#include <thread>
#include <atomic>

using namespace mgutils;

// Define a test event class that inherits from mgutils::Event
class TestEvent : public mgutils::Event {
public:
  std::string message;
  TestEvent(const std::string& msg) : message(msg) {}
};

// Define another test event class for testing different event types
class AnotherEvent : public mgutils::Event {
public:
  int data;
  AnotherEvent(int value) : data(value) {}
};

// Test case for basic subscription and notification functionality
TEST_CASE("EventManager basic subscription and notification", "[EventManager]")
{
  std::string receivedMessage;

  // Subscribe to TestEvent and capture the message
  auto connection = mgutils::EventManager::instance().subscribe<TestEvent>(
      [&receivedMessage](const TestEvent& event) {
        receivedMessage = event.message;
      }
  );

  // Notify subscribers with a TestEvent
  TestEvent event("Hello, World!");
  mgutils::EventManager::instance().notify(event);

  // Verify that the message was received correctly
  REQUIRE(receivedMessage == "Hello, World!");

  // Disconnect the subscriber
  connection.disconnect();
}

// Test case for multiple subscribers receiving the same event
TEST_CASE("EventManager multiple subscribers", "[EventManager]")
{
  std::string receivedMessage1;
  std::string receivedMessage2;

  // Subscribe two different callbacks to the same event type
  auto connection1 = mgutils::EventManager::instance().subscribe<TestEvent>(
      [&receivedMessage1](const TestEvent& event) {
        receivedMessage1 = event.message;
      }
  );

  auto connection2 = mgutils::EventManager::instance().subscribe<TestEvent>(
      [&receivedMessage2](const TestEvent& event) {
        receivedMessage2 = event.message;
      }
  );

  // Notify subscribers with a TestEvent
  TestEvent event("Multiple Subscribers");
  mgutils::EventManager::instance().notify(event);

  // Verify that both subscribers received the message
  REQUIRE(receivedMessage1 == "Multiple Subscribers");
  REQUIRE(receivedMessage2 == "Multiple Subscribers");

  // Section to test disconnection of a subscriber
  SECTION("Disconnection")
  {
    receivedMessage1.clear();
    receivedMessage2.clear();

    // Disconnect the first subscriber
    connection1.disconnect();

    // Notify subscribers with another TestEvent
    TestEvent event2("Disconnected Subscriber");
    mgutils::EventManager::instance().notify(event2);

    // Verify that the first subscriber did not receive the event
    // and the second one did
    REQUIRE(receivedMessage1.empty());
    REQUIRE(receivedMessage2 == "Disconnected Subscriber");
  }

  // Disconnect the subscribers to avoid interference with other tests
  connection2.disconnect();
}

// Test case for handling different event types
TEST_CASE("EventManager different event types", "[EventManager]")
{
  std::string receivedMessage;
  int receivedData = 0;

  // Subscribe to different event types: TestEvent and AnotherEvent
  auto connection1 = mgutils::EventManager::instance().subscribe<TestEvent>(
      [&receivedMessage](const TestEvent& event) {
        receivedMessage = event.message;
      }
  );

  auto connection2 = mgutils::EventManager::instance().subscribe<AnotherEvent>(
      [&receivedData](const AnotherEvent& event) {
        receivedData = event.data;
      }
  );

  // Notify subscribers with both event types
  TestEvent event1("Test Event");
  AnotherEvent event2(42);

  mgutils::EventManager::instance().notify(event1);
  mgutils::EventManager::instance().notify(event2);

  // Verify that each subscriber received the correct event
  REQUIRE(receivedMessage == "Test Event");
  REQUIRE(receivedData == 42);

  // Disconnect the subscribers to avoid interference with other tests
  connection1.disconnect();
  connection2.disconnect();
}

// Test case for ensuring multithreading safety
TEST_CASE("EventManager multithreading", "[EventManager][multithreading]")
{
  std::atomic<int> counter(0);

  // Subscribe to TestEvent and increment the counter on notification
  auto connection = mgutils::EventManager::instance().subscribe<TestEvent>(
      [&counter](const TestEvent& event) {
        counter.fetch_add(1, std::memory_order_relaxed);
      }
  );

  // Function to notify the event multiple times
  auto notifyFunc = []() {
    for (int i = 0; i < 1000; ++i) {
      TestEvent event("Multithreading Test");
      mgutils::EventManager::instance().notify(event);
    }
  };

  // Create two threads that will both notify the event
  std::thread thread1(notifyFunc);
  std::thread thread2(notifyFunc);

  // Join the threads to ensure they complete
  thread1.join();
  thread2.join();

  // Verify that the counter reflects the total number of notifications
  REQUIRE(counter.load() == 2000);

  // Disconnect the subscriber
  connection.disconnect();
}
