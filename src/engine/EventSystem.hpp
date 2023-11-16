#pragma once
#include <functional>
#include <map>

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

    void invoke(const char *type, void *sender);
    void subscribe(const char *type, const EventHandler &handler);
    void unsubscribe(const char *type, const EventHandler &handler);

private:
    std::multimap<const char *, const EventHandler &> m_handlers;
};

}