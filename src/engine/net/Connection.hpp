#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>

#include "Message.hpp"
#include "MessageQueue.hpp"

// Can be changed to disallow connections from different versions
#define CONNECTION_VERSON 0

namespace admirals {
namespace net {

template <typename T>
class Server;

template <typename T>
class Connection : public std::enable_shared_from_this<Connection<T>> {
   public:
   // Distinguishes between a server and a client
    enum class Owner { SERVER,
                       CLIENT };

    Connection(Owner parent, asio::io_context& io_context, asio::ip::tcp::socket socket, MessageQueue<OwnedMessage<T>>& incoming_messages, Server<T>* server = nullptr, void (Server<T>::*validation_function)(std::shared_ptr<Connection<T>>) = nullptr);
    virtual ~Connection();

    // Connects to a client from the server
    void ConnectToClient(admirals::net::Server<T>* server, uint32_t uid = 0);

    // Connects to the server from a client
    void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);

    // Disconnects the connection
    bool Disconnect();

    // Returns true if the connection is open
    bool IsConnected() const;

    // Sends a message through the connection
    void Send(const Message<T>& message);

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
    void ReadValidation(admirals::net::Server<T>* server = nullptr);

   private:
    asio::io_context& m_io_context;
    asio::ip::tcp::socket m_socket;

    MessageQueue<Message<T>> m_outgoing_messages;
    MessageQueue<OwnedMessage<T>>& m_incoming_messages;
    Message<T> m_message_buffer;

    Owner m_owner_type = Owner::SERVER;
    uint32_t m_id = 0;

    uint64_t m_validation_out = 0;
    uint64_t m_validation_in = 0;

    // Used when validating a connection
    Server<T>* m_server = nullptr;
    void (Server<T>::* m_validation_function)(std::shared_ptr<Connection<T>>) = nullptr;
};

}  // namespace net
}  // namespace admirals

#include "Connection.tpp"
