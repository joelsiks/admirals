#pragma once

namespace admirals {
namespace net {

template <typename T> const T &MessageQueue<T>::Front() {
    std::scoped_lock lock(m_mutex);
    return m_queue.front();
}

template <typename T> const T &MessageQueue<T>::Back() {
    std::scoped_lock lock(m_mutex);
    return m_queue.back();
}

template <typename T> void MessageQueue<T>::PushBack(const T &msg) {
    std::scoped_lock lock(m_mutex);
    m_queue.emplace_back(std::move(msg));
}

template <typename T> void MessageQueue<T>::PopFront() {
    std::scoped_lock lock(m_mutex);
    m_queue.pop_front();
}

template <typename T> bool MessageQueue<T>::Empty() {
    std::scoped_lock lock(m_mutex);
    return m_queue.empty();
}

template <typename T> size_t MessageQueue<T>::Count() {
    std::scoped_lock lock(m_mutex);
    return m_queue.size();
}

template <typename T> void MessageQueue<T>::Clear() {
    std::scoped_lock lock(m_mutex);
    m_queue.clear();
}

template <typename T> size_t MessageQueue<T>::Size() {
    std::scoped_lock lock(m_mutex);
    return m_queue.size();
}

} // namespace net
} // namespace admirals
