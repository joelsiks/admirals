#include "EventSystem.hpp"

using namespace admirals::events;

Event::Event(const char *type) : m_type(type) {}
Event::~Event() {}

EventSystem::EventSystem() {}
EventSystem::~EventSystem() {}

void EventSystem::invoke(const char *type, void *sender) {}

void EventSystem::subscribe(const char *type, const EventHandler &handler) {
    m_handlers.insert(type, handler);
}

void EventSystem::unsubscribe(const char *type, const EventHandler &handler) {

}