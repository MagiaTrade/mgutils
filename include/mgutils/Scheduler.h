//
// Created by Arthur Motelevicz on 07/10/24.
//

#ifndef MGUTILS_SCHEDULER_H
#define MGUTILS_SCHEDULER_H

#include <chrono>
#include <functional>
#include <future>
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace mgutils
{
  class Scheduler
  {
  public:
    using TaskCallback = const std::function<void()>;

    enum class Mode
    {
      OneShot,
      Repeat
    };

    Scheduler() : _running(false) {}

    void start(const TaskCallback& task, std::chrono::milliseconds interval, Mode mode = Mode::Repeat)
    {
      stop();
      _running.store(true);

      _futureTask = std::async(std::launch::async, [this, task, interval, mode]() {
        std::unique_lock<std::mutex> lock(_mutex);
        do
        {
          auto wakeUpTime = std::chrono::steady_clock::now() + interval;
          while (_running && std::chrono::steady_clock::now() < wakeUpTime)
            _condition.wait_for(lock, std::chrono::milliseconds(50), [this]() { return !_running; });

          if (_running)
            task();

        } while (mode == Mode::Repeat && _running);
      });
    }

    void stop()
    {
      {
        std::lock_guard<std::mutex> lock(_mutex);
        _running.store(false);
      }
      _condition.notify_all();
      if (_futureTask.valid())
        _futureTask.wait();
    }

    ~Scheduler()
    {
      stop();
    }

  private:
    std::atomic<bool> _running;
    std::future<void> _futureTask;
    std::condition_variable _condition;
    std::mutex _mutex;
  };
}

#endif //MGUTILS_SCHEDULER_H
