#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>
#include <thread>

#include "Connection.hpp"
#include "Message.hpp"
#include "MessageQueue.hpp"

namespace admirals {
namespace net {

template <typename T> class Client {

public:
    Client();
    virtual ~Client();

    // Connects to the server
    bool Connect(const std::string &host, const std::string &port);

    // Disconnect from the server
    void Disconnect();

    // Returns true if connected to the server
    bool IsConnected() const;

    // Sends a message to the server
    void Send(const Message<T> &message);

    // Returns the incoming message queue
    MessageQueue<OwnedMessage<T>> &Incoming();

private:
    asio::io_context m_io_context;
    std::thread m_context_thread;
    std::unique_ptr<Connection<T>> m_connection;
    MessageQueue<OwnedMessage<T>> m_incoming_messages;
};

} // namespace net
} // namespace admirals

#include "Client.tpp"
