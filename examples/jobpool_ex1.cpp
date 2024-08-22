//
// Created by Arthur Motelevicz on 22/08/24.
//

#include <mgutils/JobPool.h>
#include <iostream>

int main() {
  mgutils::JobPool pool;

  for (int i = 0; i < 100; ++i)
  {
    pool.addJob([i](){
      std::cout << "Executing job " << i << std::endl;
    });
  }

  pool.wait();

  std::cout << "All jobs completed." << std::endl;

  return 0;
}
