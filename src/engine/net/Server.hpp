#pragma once
#include <thread>
#include <vector>

#include "Connection.hpp"

namespace admirals::net {

class Server {
public:
    Server(uint16_t port);
    virtual ~Server();

    // Starts the server
    bool Start();

    // Stops the server
    void Stop();

    // Continuously accepts new connections
    void WaitForClientConnection();

    // Sends a message to a client
    void MessageClient(std::shared_ptr<Connection> client,
                       const Message &message);

    // Sends a message to all clients, except the one specified
    void MessageAllClients(const Message &message,
                           std::shared_ptr<Connection> ignore_client = nullptr);

    // Empties the incoming message queue, calling OnMessage on each message
    void Update(size_t max_messages = -1);

protected:
    // Called when a client connects
    virtual bool OnClientConnect(std::shared_ptr<Connection> client) {
        return false;
    }
    // Called when a client disconnects
    virtual void OnClientDisconnect(std::shared_ptr<Connection> client) = 0;
    // Called when a message is received
    virtual void OnMessage(std::shared_ptr<Connection> client,
                           Message &message) = 0;

public:
    // Called when a client is validated
    virtual void OnClientValidated(std::shared_ptr<Connection> client) = 0;

private:
    void HandleAcceptedConnection(std::error_code ec,
                                  asio::ip::tcp::socket socket);

public:
    MessageQueue<OwnedMessage> m_incomingMessages;
    std::vector<std::shared_ptr<Connection>> m_connections;

private:
    asio::io_context m_ioContext;
    std::thread m_contextThread;

    asio::ip::tcp::acceptor m_acceptor;

    // Start ID
    uint32_t m_idCounter = 1;
};

} // namespace admirals::net