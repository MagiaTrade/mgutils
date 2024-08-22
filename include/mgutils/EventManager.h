//
// Created by Arthur Motelevicz on 22/08/24.
//

#ifndef MGUTILS_EVENTMANAGER_H
#define MGUTILS_EVENTMANAGER_H

#include <boost/signals2.hpp>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <memory>
#include <vector>

namespace mgutils
{
  class Event {
  public:
    virtual ~Event() = default;
  };

  class EventManager {
  public:
    static EventManager& instance() {
      static EventManager instance;
      return instance;
    }

    // Método para se inscrever em eventos de um tipo específico
    template <typename EventType>
    boost::signals2::connection subscribe(std::function<void(const EventType&)> subscriber) {
      auto key = std::type_index(typeid(EventType));
      auto& signal = _signals[key];
      if (!signal) {
        signal = std::make_shared<boost::signals2::signal<void(const EventType&)>>();
      }
      return std::static_pointer_cast<boost::signals2::signal<void(const EventType&)>>(signal)->connect(subscriber);
    }

    // Método para notificar todos os inscritos sobre um evento
    template <typename EventType>
    void notify(const EventType& event) {
      auto key = std::type_index(typeid(EventType));
      if (_signals.find(key) != _signals.end()) {
        (*std::static_pointer_cast<boost::signals2::signal<void(const EventType&)>>(_signals[key]))(event);
      }
    }

  private:
    EventManager() = default;

    // Mapa de sinais organizados por tipo de evento
    std::unordered_map<std::type_index, std::shared_ptr<void>> _signals;
  };

}

#endif //MGUTILS_EVENTMANAGER_H
