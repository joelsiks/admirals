#pragma once
#include <thread>

#include "Connection.hpp"
#include "Message.hpp"
#include "MessageQueue.hpp"

namespace admirals::net {

class Client {

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
    void Send(const Message &message);

    // Returns the incoming message queue
    MessageQueue<OwnedMessage> &Incoming();

private:
    asio::io_context m_ioContext;
    std::thread m_contextThread;
    std::unique_ptr<Connection> m_connection;
    MessageQueue<OwnedMessage> m_incomingMessages;
};

} // namespace admirals::net