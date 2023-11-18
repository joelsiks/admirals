#pragma once

#include <functional>
#include <map>
#include <set>
#include <string>

namespace admirals::events {

#define BIND_EVENT_HANDLER_FROM(handler, source)                               \
    std::bind((handler), (source), std::placeholders::_1, std::placeholders::_2)
#define BIND_EVENT_HANDLER(handler) BIND_EVENT_HANDLER_FROM(&(handler), this)

/// @brief Describes an event and stores related data. The `EventSystem` sends
/// `EventArgs` with every event, setting the `handled` value to true will
/// prevent the `EventSystem` from further calling event handlers. The `type`
/// value corresponds to the type of the event that was invoked.
class EventArgs {
public:
    EventArgs(const std::string &type);

public:
    bool handled = false;
    const std::string &type() const;

private:
    const std::string m_type;
};

typedef std::function<void(void *, EventArgs &)> EventHandler;

/// @brief A subscription based event handler that allows the binding of
/// `EventHandler` functions to events of a given `type`.
class EventSystem {
public:
    /// @brief Calls all event handlers bound to the given `type` and
    /// passes `sender` along with `event` to the event handlers.
    /// @param type The identifier of the type of event to call.
    /// @param sender A pointer to the class invoking the event handler.
    /// @param event The event arguments passed to the event handlers.
    void Invoke(const std::string &type, void *sender, EventArgs &args);

    /// @brief Binds an `handler` to events of the given `type`. If it is
    /// already bound, it will have no effect.
    /// @param type The identifier of the type of event to bind to.
    /// @param handler The event handler function.
    void Subscribe(const std::string &type, const EventHandler &handler);

    /// @brief Un-binds a `handler` of events of the given `type`. If it is
    /// not already bound, it will have no effect.
    /// @param type The identifier of the type of event to bind to.
    /// @param handler The event handler function.
    void Unsubscribe(const std::string &type, const EventHandler &handler);

private:
    struct Comparator {
        bool operator()(const EventHandler &l, const EventHandler &r) const {
            return l.target_type().hash_code() < r.target_type().hash_code();
        }
    };

    std::map<const std::string, std::set<EventHandler, Comparator>> m_handlers;
};

} // namespace admirals::events