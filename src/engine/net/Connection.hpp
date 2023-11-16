#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>

#include "Message.hpp"
#include "MessageQueue.hpp"

// Can be changed to disallow connections from different versions
#define CONNECTION_VERSON 0

namespace admirals::net {

using ValidationFunction = std::function<void(std::shared_ptr<Connection>)>;

class Server;

class Connection : public std::enable_shared_from_this<Connection> {
public:
    // Distinguishes between a server and a client
    enum class Owner { SERVER, CLIENT };

    Connection(Owner parent, asio::io_context &io_context,
               asio::ip::tcp::socket socket,
               MessageQueue<OwnedMessage> &incoming_messages,
               Server *server = nullptr,
               ValidationFunction validation_function = nullptr);
    virtual ~Connection();

    // Connects to a client from the server
    void ConnectToClient(admirals::net::Server *server, uint32_t uid = 0);

    // Connects to the server from a client
    void
    ConnectToServer(const asio::ip::tcp::resolver::results_type &endpoints);

    // Disconnects the connection
    bool Disconnect();

    // Returns true if the connection is open
    bool IsConnected() const;

    // Sends a message through the connection
    void Send(const Message &message);

    // Returns the id of the connection
    // Only applicable for client ids
    uint32_t GetID() const;

private:
    void ReadHeader();
    void ReadBody();
    void AddToIncomingMessageQueue();
    void WriteHeader();
    void WriteBody();
    uint64_t HashValidation(uint64_t hash);
    void WriteValidation();
    void ReadValidation();

    void HandleReadHeader(std::error_code ec);
    void HandleReadBody(std::error_code ec);
    void HandleWriteHeader(std::error_code ec);
    void HandleWriteBody(std::error_code ec);
    void HandleWriteValidation(std::error_code ec);
    void HandleReadValidation(std::error_code ec);

private:
    asio::io_context &m_ioContext;
    asio::ip::tcp::socket m_socket;

    MessageQueue<Message> m_outgoingMessages;
    MessageQueue<OwnedMessage> &m_incomingMessages;
    Message m_messageBuffer;

    Owner m_ownerType = Owner::SERVER;
    uint32_t m_id = 0;

    uint64_t m_validationOut = 0;
    uint64_t m_validationIn = 0;

    // Used when validating a connection
    Server *m_server = nullptr;
    // void (Server::*m_validationFunction)(std::shared_ptr<Connection>) =
    // nullptr;
    ValidationFunction m_validationFunction = nullptr;
};

} // namespace admirals::net
