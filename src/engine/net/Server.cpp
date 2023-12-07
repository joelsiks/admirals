#include "Server.hpp"
#include <iostream>

using namespace admirals::net;

#define BIND_HANDLER(handler) std::bind(&handler, this, std::placeholders::_1)
#define BIND_HANDLER_2ARGS(handler)                                            \
    std::bind(&handler, this, std::placeholders::_1, std::placeholders::_2)

Server::Server(uint16_t port)
    : m_acceptor(m_ioContext,
                 asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

Server::~Server() { Stop(); }

bool Server::Start() {
    try {
        // Start accepting connections
        WaitForClientConnection();
        m_contextThread = std::thread([this]() { m_ioContext.run(); });
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    std::cout << "Server started" << std::endl;
    return true;
}

void Server::Stop() {
    // Stop accepting connections
    m_ioContext.stop();
    if (m_contextThread.joinable())
        m_contextThread.join();
    std::cout << "Server stopped" << std::endl;
}

void Server::HandleAcceptedConnection(std::error_code ec,
                                      asio::ip::tcp::socket socket) {
    if (!ec) {
        std::cout << "New connection: " << socket.remote_endpoint()
                  << std::endl;

        // Create a new connection object for the client
        std::shared_ptr<Connection> new_connection =
            std::make_shared<Connection>(
                Connection::Owner::SERVER, m_ioContext, std::move(socket),
                m_incomingMessages, this,
                BIND_HANDLER(Server::OnClientValidated));
        // Verify that the client is allowed to connect
        if (OnClientConnect(new_connection)) {
            // Add the new connection to the list of connections
            m_connections.push_back(std::move(new_connection));
            m_connections.back()->ConnectToClient(this, m_idCounter++);
            std::cout << "[" << m_connections.back()->GetID()
                      << "] Connection approved" << std::endl;
        } else {
            std::cout << "Connection denied" << std::endl;
        }
    } else {
        std::cout << "Connection error: " << ec.message() << std::endl;
    }
    WaitForClientConnection();
}

void Server::WaitForClientConnection() {
    m_acceptor.async_accept(
        BIND_HANDLER_2ARGS(Server::HandleAcceptedConnection));
}

void Server::MessageClient(std::shared_ptr<Connection> client,
                           const Message &message) {
    if (client && client->IsConnected()) {
        // If the client is connected, send the message
        client->Send(message);
    } else {
        // Otherwise, remove the client
        OnClientDisconnect(client);
        client.reset();
        m_connections.erase(
            std::remove(m_connections.begin(), m_connections.end(), client),
            m_connections.end());
    }
}

void Server::MessageAllClients(const Message &message,
                               std::shared_ptr<Connection> ignore_client) {
    for (auto &connection : m_connections) {
        // Send the message to all connected clients
        if (connection && connection->IsConnected()) {
            if (connection != ignore_client) {
                connection->Send(message);
            }
        } else {
            // Extract and remove the connection
            std::shared_ptr<Connection> temp = connection;
            m_connections.erase(
                std::remove(m_connections.begin(), m_connections.end(), temp),
                m_connections.end());

            OnClientDisconnect(temp);
            temp.reset();
        }
    }
}

void Server::Update(size_t max_messages) {
    size_t message_count = 0;
    while (message_count < max_messages && !m_incomingMessages.Empty()) {
        // Process all messages in the queue
        auto message = m_incomingMessages.Front();
        m_incomingMessages.PopFront();
        OnMessage(message.remote, message.message);
        message_count++;
    }
}
