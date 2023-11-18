#pragma once

#include <functional>
#include <set>

namespace admirals::events {

// These macros are for easily binding class methods or functions to an event
// handler.
#define BIND_EVENT_HANDLER_FROM(handler, source)                               \
    std::bind(std::mem_fn(&handler), source, std::placeholders::_1,            \
              std::placeholders::_2)
#define BIND_EVENT_HANDLER(handler) BIND_EVENT_HANDLER_FROM(handler, this)

class EventArgs {
public:
    bool handled = false;
};

template <typename T = EventArgs> class EventSystem {
public:
    typedef std::function<void(void *, T &)> EventHandler;

    void Invoke(void *sender, T &event) const {
        for (const auto &handler : m_handlers) {
            if (event.handled) {
                return;
            }
            handler(sender, event);
        }
    }

    inline void Subscribe(const EventHandler &handler) {
        m_handlers.insert(handler);
    }

    inline void Unsubscribe(const EventHandler &handler) {
        m_handlers.erase(handler);
    }

    inline void operator+=(const EventHandler &handler) {
        m_handlers.insert(handler);
    }

    inline void operator-=(const EventHandler &handler) {
        m_handlers.erase(handler);
    }

private:
    inline void operator=(const EventSystem<T> e) = delete;

    struct Comparator {
        bool operator()(const EventHandler &l, const EventHandler &r) const {
            return l.target_type().hash_code() < r.target_type().hash_code();
        }
    };

    std::set<EventHandler, Comparator> m_handlers;
};

} // namespace admirals::events