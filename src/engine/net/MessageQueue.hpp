#pragma once
#include <deque>
#include <mutex>

namespace admirals {
namespace net {

template <typename T>
class MessageQueue {
   private:
    std::deque<T> m_queue;
    std::mutex m_mutex;

   public:
    MessageQueue() = default;
    MessageQueue(const MessageQueue<T>&) = delete;
    virtual ~MessageQueue() = default;

    const T& Front();

    // Returns and maintains item at back of queue
    const T& Back();

    // Adds an item to the back of the queue
    void PushBack(const T& msg);

    // Removes an item from the front of the queue
    void PopFront();

    // Returns true if queue is empty
    bool Empty();

    // Returns number of items in queue
    size_t Count();

    // Clears the queue
    void Clear();

    size_t Size();
};

}  // namespace net
}  // namespace admirals

#include "MessageQueue.tpp"
