//
// Created by Arthur Motelevicz on 22/08/24.
//

#ifndef MGUTILS_JOBPOOL_H
#define MGUTILS_JOBPOOL_H

#include <tbb/tbb.h>
#include <functional>
#include <vector>

namespace mgutils
{
  class JobPool
  {
  public:
    JobPool() : _taskGroup(std::make_shared<tbb::task_group>()) {}

    ~JobPool() {
      wait();
    }

    void addJob(const std::function<void()>& job) {
      _taskGroup->run(job);
    }

    void wait() {
      _taskGroup->wait();
    }

  private:
    std::shared_ptr<tbb::task_group> _taskGroup;
  };

} // namespace mgutils

#endif //MGUTILS_JOBPOOL_H
