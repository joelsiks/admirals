#pragma once
#include "EventArgs.hpp"

#include <functional>
#include <unordered_set>

namespace admirals::events {

// These macros are for easily binding class methods or functions to an event
// handler.
#define BIND_EVENT_HANDLER_FROM(handler, source)                               \
    {                                                                          \
        std::move(std::bind(std::mem_fn(&handler), source,                     \
                            std::placeholders::_1, std::placeholders::_2)),    \
            source                                                             \
    }
#define BIND_EVENT_HANDLER(handler) BIND_EVENT_HANDLER_FROM(handler, this)

template <typename T = EventArgs> class EventSystem {
public:
    typedef std::function<void(void *, T &)> EventHandler;

    class EventHandlerWrapper : public EventHandler {
    public:
        EventHandlerWrapper(const EventHandler &handler, void *source) noexcept
            : EventHandler(handler), m_source(source) {}
        EventHandlerWrapper(const EventHandler &handler,
                            std::shared_ptr<void> source)
            : EventHandlerWrapper(handler, source.get()) {}
        EventHandlerWrapper(const EventHandler &handler,
                            std::unique_ptr<void> source)
            : EventHandlerWrapper(handler, source.get()) {}

        const void *m_source;
    };

    void Invoke(void *sender, T &event) {
        for (auto it = m_queued_unsubscribes.begin();
             it != m_queued_unsubscribes.end();) {
            m_handlers.erase(*it);
            it = m_queued_unsubscribes.erase(it);
        }

        for (const auto &handler : m_handlers) {
            if (event.handled) {
                return;
            }
            handler(sender, event);
        }
    }

    /// @brief Subscribes an event handler
    inline void Subscribe(const EventHandlerWrapper &handler) {
        m_handlers.insert(std::move(handler));
    }

    /// @brief Unsubscribes an event handler
    inline void Unsubscribe(const EventHandlerWrapper &handler) {
        m_queued_unsubscribes.push_back(handler);
    }

    /// @brief Subscribes an event handler
    inline void Subscribe(const EventHandler &handler) {
        Subscribe(EventHandlerWrapper(handler, nullptr));
    }

    /// @brief Unsubscribes an event handler
    inline void Unsubscribe(const EventHandler &handler) {
        Unsubscribe(EventHandlerWrapper(handler, nullptr));
    }

    /// @brief Subscribes an event handler
    inline void operator+=(const EventHandler &handler) { Subscribe(handler); }

    /// @brief Unsubscribes an event handler
    inline void operator-=(const EventHandler &handler) {
        Unsubscribe(handler);
    }

    /// @brief Subscribes an event handler
    inline void operator+=(const EventHandlerWrapper &handler) {
        Subscribe(handler);
    }

    /// @brief Unsubscribes an event handler
    inline void operator-=(const EventHandlerWrapper &handler) {
        Unsubscribe(handler);
    }

private:
    inline void operator=(const EventSystem<T> e) = delete;

    struct Hash {
        size_t operator()(const EventHandlerWrapper &e) const noexcept {

            // Target function pointer
            // const size_t *functor = reinterpret_cast<const size_t *>(&e);
            const size_t typeHash = e.target_type().hash_code();
            const size_t sourceHash =
                reinterpret_cast<const size_t>(e.m_source);
            if (sourceHash != 0) {
                return typeHash ^ sourceHash;
            }
            const size_t targetHash = reinterpret_cast<const size_t>(
                e.template target<void (*)(void *, EventArgs &)>());
            return typeHash ^ targetHash;
        }
    };

    struct Equals {
        bool operator()(const EventHandlerWrapper &l,
                        const EventHandlerWrapper &r) const noexcept {
            return Hash{}(l) == Hash{}(r);
        }
    };

    std::unordered_set<EventHandlerWrapper, Hash, Equals> m_handlers;
    std::vector<EventHandlerWrapper> m_queued_unsubscribes;
};

} // namespace admirals::events