//
// Created by Arthur Motelevicz on 07/10/24.
//
#include <thread>
#include <catch2/catch.hpp>
#include "mgutils/Scheduler.h"

using namespace mgutils;

TEST_CASE("Scheduler executes task once in OneShot mode", "[scheduler]")
{
  Scheduler scheduler;
  std::atomic<int> counter(0);

  scheduler.start([&]() {
    counter++;
  }, std::chrono::milliseconds(100), Scheduler::Mode::OneShot);

  // Espera um tempo maior que o intervalo para garantir que a tarefa foi executada
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  REQUIRE(counter == 1);
}

TEST_CASE("Scheduler executes task repeatedly in Repeat mode", "[scheduler]")
{
  Scheduler scheduler;
  std::atomic<int> counter(0);

  scheduler.start([&]() {
    counter++;
  }, std::chrono::milliseconds(100), Scheduler::Mode::Repeat);

  // Espera o suficiente para que a tarefa seja executada algumas vezes
  std::this_thread::sleep_for(std::chrono::milliseconds(450));

  // Como esperamos 450ms e a tarefa roda a cada 100ms, esperamos pelo menos 4 execuções
  REQUIRE(counter >= 4);

  scheduler.stop();
}

TEST_CASE("Scheduler can be stopped in Repeat mode", "[scheduler]")
{
  Scheduler scheduler;
  std::atomic<int> counter(0);

  scheduler.start([&]() {
    counter++;
  }, std::chrono::milliseconds(100), Scheduler::Mode::Repeat);

  // Espera um pouco e depois cancela
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  scheduler.stop();

  int counterAtStop = counter.load();

  // Espera mais um tempo para garantir que a tarefa não será mais executada
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  // O contador não deve ter mudado após o cancelamento
  REQUIRE(counter == counterAtStop);
}

TEST_CASE("Scheduler restarts correctly when start is called again", "[scheduler]")
{
  Scheduler scheduler;
  std::atomic<int> counter(0);

  scheduler.start([&]() {
    counter++;
  }, std::chrono::milliseconds(100), Scheduler::Mode::Repeat);

  // Espera por um tempo para o scheduler rodar algumas vezes
  std::this_thread::sleep_for(std::chrono::milliseconds(250));

  // Reinicia o scheduler (o stop é chamado implicitamente)
  scheduler.start([&]() {
    counter++;
  }, std::chrono::milliseconds(100), Scheduler::Mode::Repeat);

  // Espera mais um pouco
  std::this_thread::sleep_for(std::chrono::milliseconds(250));

  // Esperamos pelo menos algumas execuções em ambas as fases
  REQUIRE(counter >= 4);

  scheduler.stop();
}


TEST_CASE("Scheduler stops correctly on destruction", "[scheduler]")
{
  std::atomic<int> counter(0);

  {
    Scheduler scheduler;
    scheduler.start([&]() {
      counter++;
    }, std::chrono::milliseconds(100), Scheduler::Mode::Repeat);

    // Espera por um tempo enquanto o scheduler está ativo
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }

  // Após a destruição do scheduler, o contador não deve mais aumentar
  int counterAfterDestruction = counter.load();

  std::this_thread::sleep_for(std::chrono::milliseconds(250));

  REQUIRE(counter == counterAfterDestruction);
}
