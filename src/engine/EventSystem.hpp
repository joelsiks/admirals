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
    EventSystem();
    ~EventSystem();

    void invoke(const char *type, void *sender, Event &event);
    void subscribe(const char *type, const EventHandler &handler);
    void unsubscribe(const char *type, const EventHandler &handler);

private:
    std::map<const char *, std::set<size_t>> m_handlers;
};

}