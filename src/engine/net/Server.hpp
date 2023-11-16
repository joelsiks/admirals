#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>
#include <thread>
#include <vector>

#include "Connection.hpp"

namespace admirals {
namespace net {

template <typename T> class Server {
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
    void MessageClient(std::shared_ptr<Connection<T>> client,
                       const Message<T> &message);

    // Sends a message to all clients, except the one specified
    void
    MessageAllClients(const Message<T> &message,
                      std::shared_ptr<Connection<T>> ignore_client = nullptr);

    // Empties the incoming message queue, calling OnMessage on each message
    void Update(size_t max_messages = -1);

protected:
    // Called when a client connects
    virtual bool OnClientConnect(std::shared_ptr<Connection<T>> client) {
        return false;
    }
    // Called when a client disconnects
    virtual void OnClientDisconnect(std::shared_ptr<Connection<T>> client) {}
    // Called when a message is received
    virtual void OnMessage(std::shared_ptr<Connection<T>> client,
                           Message<T> &message) {}

public:
    // Called when a client is validated
    virtual void OnClientValidated(std::shared_ptr<Connection<T>> client) {}

public:
    MessageQueue<OwnedMessage<T>> incoming_messages;
    std::vector<std::shared_ptr<Connection<T>>> connections;

private:
    asio::io_context io_context;
    std::thread context_thread;

    asio::ip::tcp::acceptor acceptor;

    // Start ID
    uint32_t id_counter = 1;
};
} // namespace net
} // namespace admirals

#include "Server.tpp"
