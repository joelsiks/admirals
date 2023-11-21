#pragma once
#include "EventArgs.hpp"

#include <functional>
#include <unordered_set>

namespace admirals::events {

// These macros are for easily binding class methods or functions to an event
// handler.
#define BIND_EVENT_HANDLER_FROM(handler, source)                               \
    std::bind(std::mem_fn(&handler), source, std::placeholders::_1,            \
              std::placeholders::_2)
#define BIND_EVENT_HANDLER(handler) BIND_EVENT_HANDLER_FROM(handler, this)

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

    struct Hash {
        size_t operator()(const EventHandler &e) const {
            // Hash the type information
            const size_t typeHash = typeid(EventHandler).hash_code();

            // target function pointer
            const size_t *functor = reinterpret_cast<const size_t *>(&e);
            const size_t functionHash = functor[0];

            // Combine the hash values
            return typeHash ^ functionHash;
        }
    };

    struct Equals {
        bool operator()(const EventHandler &l, const EventHandler &r) const {
            return Hash{}(l) == Hash{}(r);
        }
    };

    std::unordered_set<EventHandler, Hash, Equals> m_handlers;
};

} // namespace admirals::events