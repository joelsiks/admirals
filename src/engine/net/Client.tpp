#pragma once

namespace admirals {
namespace net {
template <typename T> Client<T>::Client() {}

template <typename T> Client<T>::~Client() { Disconnect(); }

template <typename T>
bool Client<T>::Connect(const std::string &host, const std::string &port) {
    try {
        asio::ip::tcp::resolver resolver(m_io_context);
        asio::ip::tcp::resolver::results_type endpoints =
            resolver.resolve(host, port);

        m_connection = std::make_unique<Connection<T>>(
            Connection<T>::Owner::CLIENT, m_io_context,
            asio::ip::tcp::socket(m_io_context), m_incoming_messages);
        m_connection->ConnectToServer(endpoints);

        m_context_thread = std::thread([this]() { m_io_context.run(); });

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

template <typename T> void Client<T>::Disconnect() {
    if (IsConnected()) {
        m_connection->Disconnect();
    }

    m_io_context.stop();
    if (m_context_thread.joinable())
        m_context_thread.join();
    m_connection.release();
}

template <typename T> bool Client<T>::IsConnected() const {
    if (m_connection) {
        return m_connection->IsConnected();
    } else {
        return false;
    }
}

template <typename T> void Client<T>::Send(const Message<T> &message) {
    if (IsConnected()) {
        m_connection->Send(message);
    }
}

template <typename T> MessageQueue<OwnedMessage<T>> &Client<T>::Incoming() {
    return m_incoming_messages;
}

} // namespace net
} // namespace admirals
