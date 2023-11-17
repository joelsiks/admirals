#pragma once
#include <functional>
#include <map>
#include <set>

namespace admirals::events {

class Event {
public:
    Event(const char *type);
    ~Event();

public:
    bool handled = false;
    const char *m_type;
};

typedef std::function<void(void *, Event &)> EventHandler;

class EventSystem {
public:
    void Invoke(const char *type, void *sender, Event &event);
    void Subscribe(const char *type, const EventHandler &handler);
    void Unsubscribe(const char *type, const EventHandler &handler);

private:
    struct Comparator {
        bool operator()(const EventHandler &l, const EventHandler &r) const {
            return l.target_type().hash_code() < r.target_type().hash_code();
        }
    };

    std::map<const char *, std::set<EventHandler, Comparator>> m_handlers;
};

} // namespace admirals::events