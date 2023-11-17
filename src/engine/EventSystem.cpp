#include "EventSystem.hpp"
#include <cstring>

using namespace admirals::events;

Event::Event(const char *type) : m_type(type) {}
Event::~Event() {}

void EventSystem::Invoke(const char *type, void *sender, Event &event) {
    for (const auto &handler : m_handlers[type]) {
        if (event.handled) {
            return;
        }
        handler(sender, event);
    }
}

void EventSystem::Subscribe(const char *type, const EventHandler &handler) {
    if (auto match = m_handlers.find(type); match != m_handlers.end()) {
        auto handlers = match->second;
        handlers.insert(handler);
    } else {
        std::set<EventHandler, Comparator> handlers = {handler};
        m_handlers.emplace(type, handlers);
    }
}

void EventSystem::Unsubscribe(const char *type, const EventHandler &handler) {
    if (auto match = m_handlers.find(type); match != m_handlers.end()) {
        auto handlers = match->second;
        handlers.erase(handler);
        if (handlers.empty()) {
            m_handlers.erase(type);
        }
    }
}