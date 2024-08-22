//
// Created by Arthur Motelevicz on 22/08/24.
//
#include <catch2/catch.hpp>
#include <mgutils/JobPool.h>
#include <atomic>

TEST_CASE("JobPool basic job execution", "[JobPool]") {
  mgutils::JobPool pool;
  std::atomic<int> counter(0);

  pool.addJob([&counter]() {
    counter++;
  });

  pool.wait();

  REQUIRE(counter == 1);
}

TEST_CASE("JobPool multiple jobs execution", "[JobPool]") {
  mgutils::JobPool pool;
  std::atomic<int> counter(0);
  const int numJobs = 10;

  for (int i = 0; i < numJobs; ++i) {
    pool.addJob([&counter]() {
      counter++;
    });
  }

  pool.wait();

  REQUIRE(counter == numJobs);
}

TEST_CASE("JobPool wait for jobs completion", "[JobPool]") {
  mgutils::JobPool pool;
  std::atomic<int> counter(0);

  pool.addJob([&counter]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    counter++;
  });

  pool.addJob([&counter]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    counter++;
  });

  pool.wait();

  REQUIRE(counter == 2);
}