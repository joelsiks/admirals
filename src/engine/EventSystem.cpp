#include "EventSystem.hpp"

using namespace admirals::events;

EventArgs::EventArgs(const std::string &type) : m_type(type) {}

const std::string &EventArgs::type() const { return m_type; }

void EventSystem::Invoke(const std::string &type, void *sender,
                         EventArgs &args) {
    for (const auto &handler : m_handlers[type]) {
        if (args.handled) {
            return;
        }
        handler(sender, args);
    }
}

void EventSystem::Subscribe(const std::string &type,
                            const EventHandler &handler) {
    if (auto match = m_handlers.find(type); match != m_handlers.end()) {
        auto handlers = match->second;
        handlers.insert(handler);
    } else {
        std::set<EventHandler, Comparator> handlers = {handler};
        m_handlers.emplace(type, handlers);
    }
}

void EventSystem::Unsubscribe(const std::string &type,
                              const EventHandler &handler) {
    if (auto match = m_handlers.find(type); match != m_handlers.end()) {
        auto handlers = match->second;
        handlers.erase(handler);
        if (handlers.empty()) {
            m_handlers.erase(type);
        }
    }
}