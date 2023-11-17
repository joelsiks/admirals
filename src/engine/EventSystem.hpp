#pragma once
#include <functional>
#include <map>
#include <set>

namespace admirals::events {

class Event 
{
public:
    Event(const char *type);
    ~Event();
public:
    bool handled = false;
    const char *m_type;
};

typedef std::function<void(void *, Event &)> EventHandler;

class EventSystem
{
public:
    void Invoke(const char *type, void *sender, Event &event);
    void Subscribe(const char *type, const EventHandler &handler);
    void Unsubscribe(const char *type, const EventHandler &handler);

private:
    std::map<const char *, std::set<size_t>> m_handlers;
};

}