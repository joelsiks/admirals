#pragma once
#include <deque>
#include <mutex>

namespace admirals::net {

template <typename T> class MessageQueue {
private:
    std::deque<T> m_queue;
    std::mutex m_mutex;

public:
    MessageQueue() = default;
    MessageQueue(const MessageQueue<T> &) = delete;
    virtual ~MessageQueue() = default;

    const T &Front() {
        std::scoped_lock lock(m_mutex);
        return m_queue.front();
    }

    // Returns and maintains item at back of queue
    const T &Back() {
        std::scoped_lock lock(m_mutex);
        return m_queue.back();
    }

    // Adds an item to the back of the queue
    void PushBack(const T &msg) {
        std::scoped_lock lock(m_mutex);
        m_queue.emplace_back(std::move(msg));
    }

    // Removes an item from the front of the queue
    void PopFront() {
        std::scoped_lock lock(m_mutex);
        m_queue.pop_front();
    }

    // Returns true if queue is empty
    bool Empty() {
        std::scoped_lock lock(m_mutex);
        return m_queue.empty();
    }

    // Returns number of items in queue
    size_t Count() {
        std::scoped_lock lock(m_mutex);
        return m_queue.size();
    }

    // Clears the queue
    void Clear() {
        std::scoped_lock lock(m_mutex);
        m_queue.clear();
    }

    size_t Size() {
        std::scoped_lock lock(m_mutex);
        return m_queue.size();
    }
};

} // namespace admirals::net