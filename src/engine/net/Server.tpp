#pragma once

namespace admirals {
namespace net {

template <typename T>
Server<T>::Server(uint16_t port)
    : acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
}

template <typename T> Server<T>::~Server() { Stop(); }

template <typename T> bool Server<T>::Start() {
    try {
        // Start accepting connections
        WaitForClientConnection();
        context_thread = std::thread([this]() { io_context.run(); });
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    std::cout << "Server started" << std::endl;
    return true;
}

template <typename T> void Server<T>::Stop() {
    // Stop accepting connections
    io_context.stop();
    if (context_thread.joinable())
        context_thread.join();
    std::cout << "Server stopped" << std::endl;
}

template <typename T> void Server<T>::WaitForClientConnection() {
    acceptor.async_accept([this](std::error_code ec,
                                 asio::ip::tcp::socket socket) {
        if (!ec) {
            std::cout << "New connection: " << socket.remote_endpoint()
                      << std::endl;

            // Create a new connection object for the client
            std::shared_ptr<Connection<T>> new_connection =
                std::make_shared<Connection<T>>(
                    Connection<T>::Owner::SERVER, io_context, std::move(socket),
                    incoming_messages, this, &Server<T>::OnClientValidated);
            // Verify that the client is allowed to connect
            if (OnClientConnect(new_connection)) {
                // Add the new connection to the list of connections
                connections.push_back(std::move(new_connection));
                connections.back()->ConnectToClient(this, id_counter++);
                std::cout << "[" << connections.back()->GetID()
                          << "] Connection approved" << std::endl;
            } else {
                std::cout << "Connection denied" << std::endl;
            }
        } else {
            std::cout << "Connection error: " << ec.message() << std::endl;
        }
        WaitForClientConnection();
    });
}

template <typename T>
void Server<T>::MessageClient(std::shared_ptr<Connection<T>> client,
                              const Message<T> &message) {
    if (client && client->IsConnected()) {
        // If the client is connected, send the message
        client->Send(message);
    } else {
        // Otherwise, remove the client
        OnClientDisconnect(client);
        client.reset();
        connections.erase(
            std::remove(connections.begin(), connections.end(), client),
            connections.end());
    }
}

template <typename T>
void Server<T>::MessageAllClients(
    const Message<T> &message, std::shared_ptr<Connection<T>> ignore_client) {
    bool client_disconnected = false;
    for (auto &connection : connections) {
        // Send the message to all connected clients
        if (connection && connection->IsConnected()) {
            if (connection != ignore_client) {
                connection->Send(message);
            }
        } else {
            // If the client is not connected, flag it for removal
            OnClientDisconnect(connection);
            connection.reset();
            client_disconnected = true;
        }
    }

    if (client_disconnected) {
        // Remove all disconnected clients
        // Reorders the vector so that all bad clients are at the end, then
        // erases them
        connections.erase(
            std::remove(connections.begin(), connections.end(), nullptr),
            connections.end());
    }
}

template <typename T> void Server<T>::Update(size_t max_messages) {
    size_t message_count = 0;
    while (message_count < max_messages && !incoming_messages.Empty()) {
        // Process all messages in the queue
        auto message = incoming_messages.Front();
        incoming_messages.PopFront();
        OnMessage(message.remote, message.message);
        message_count++;
    }
}
} // namespace net
} // namespace admirals
