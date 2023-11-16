#include "EventSystem.hpp"

using namespace admirals::events;

Event::Event(const char *type) : m_type(type) {}
Event::~Event() {}

EventSystem::EventSystem() {}
EventSystem::~EventSystem() {}

void EventSystem::invoke(const char *type, void *sender, Event &event) {
    for (const auto &h : m_handlers[type]) {
        if (event.handled) { return; }
        ((const EventHandler &) h)(sender, event);
    }
}

void EventSystem::subscribe(const char *type, const EventHandler &handler) {
    size_t value = (size_t) handler.target<void(*)>();
    auto match = m_handlers.find(type);
    if (match != m_handlers.end()) {
        auto handlers = match->second;
        handlers.insert(value);
    } else {
        std::set<size_t> handlers = { value };
        m_handlers.emplace(type, handlers);
    }
}

void EventSystem::unsubscribe(const char *type, const EventHandler &handler) {
    size_t value = (size_t) handler.target<void(*)>();
    auto match = m_handlers.find(type);
    if (match != m_handlers.end()) {
        auto handlers = match->second;
        handlers.erase(value);
        if (handlers.empty()) {
            m_handlers.erase(type);
        }
    }
}