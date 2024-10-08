//
// Created by Arthur Motelevicz on 08/10/24.
//

#ifndef MGUTILS_HEARTBEATCHECKER_H
#define MGUTILS_HEARTBEATCHECKER_H

#include <iostream>
#include <future>
#include <chrono>
#include <atomic>
#include <functional>
#include <utility>

namespace mgutils
{
  class HeartBeatChecker
  {
  public:
    using OnTimeoutCallback = std::function<void(void)>;

    HeartBeatChecker(int intervalMillis, OnTimeoutCallback  timeoutCallback):
        _intervalMillis(intervalMillis),
        _onTimeoutCallback(std::move(timeoutCallback)),
        _active(false)
    {
      startChecking();
    }

    ~HeartBeatChecker()
    {
      stop();
    }

    void restart()
    {
      stop();
      startChecking();
    }

    void stop()
    {
      _active.store(false);
      if (_checkerThread.valid())
        _checkerThread.wait();
    }

  private:

    void startChecking()
    {
      _active.store(true);
      _lastReset = std::chrono::steady_clock::now();
      _checkerThread = std::async(std::launch::async, [this]() {
        while (_active.load()) {
          auto now = std::chrono::steady_clock::now();
          auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastReset).count();

          if (elapsed > _intervalMillis)
          {
            _onTimeoutCallback();
            _active.store(false);
          }

          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
      });
    }

    int _intervalMillis;
    std::atomic<bool> _active;
    std::chrono::steady_clock::time_point _lastReset;
    OnTimeoutCallback _onTimeoutCallback;
    std::future<void> _checkerThread;
  };

}

#endif //MGUTILS_HEARTBEATCHECKER_H
