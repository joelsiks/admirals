#pragma once

namespace admirals {
namespace net {

template <typename T>
Connection<T>::Connection(Owner parent, asio::io_context& io_context, asio::ip::tcp::socket socket, MessageQueue<OwnedMessage<T>>& incoming_messages, Server<T>* server, void (Server<T>::*validation_function)(std::shared_ptr<Connection<T>>))
    : m_io_context(io_context), m_socket(std::move(socket)), m_incoming_messages(incoming_messages), m_owner_type(parent), m_server(server), m_validation_function(validation_function) {
    if (m_owner_type == Owner::SERVER) {
        // Uses the current time as the validation number
        m_validation_out = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
    }
}

template <typename T>
Connection<T>::~Connection() {}

template <typename T>
void Connection<T>::ConnectToClient(admirals::net::Server<T>* server, uint32_t uid) {
    if (m_socket.is_open()) {
        m_id = uid;
        // Send the validation number to the client and wait for the client to send it back hashed
        WriteValidation();
        ReadValidation(server);
    }
}

template <typename T>
void Connection<T>::ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
    asio::async_connect(m_socket, endpoints, [this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
        if (!ec) {
            // Read the validation number from the server and send it back hashed
            ReadValidation();
        } else {
            std::cout << "Connect failed: " << ec.message() << std::endl;
        }
    });
}

template <typename T>
bool Connection<T>::Disconnect() {
    if (IsConnected()) {
        asio::post(m_io_context,
                   [this]() {
                       m_socket.close();
                   });
        return true;
    }
    return false;
}

template <typename T>
bool Connection<T>::IsConnected() const {
    return m_socket.is_open();
}

template <typename T>
void Connection<T>::Send(const Message<T>& message) {
    asio::post(m_io_context,
               [this, message]() {
                   // Check if asio is already writing messages
                   bool writing_message = !m_outgoing_messages.Empty();
                   m_outgoing_messages.PushBack(message);
                   // If asio is not writing messages, start writing
                   if (!writing_message) {
                       WriteHeader();
                   }
               });
}

template <typename T>
uint32_t Connection<T>::GetID() const {
    return m_id;
}

// Reads the header of the message to determine type and the size of the body
template <typename T>
void Connection<T>::ReadHeader() {
    asio::async_read(m_socket, asio::buffer(&m_message_buffer.header, sizeof(MessageHeader<T>)),
                     [this](std::error_code ec, std::size_t length) {
                         if (!ec) {
                             if (m_message_buffer.header.size > 0) {
                                 // If the message has a body, resize the buffer and read the body
                                 m_message_buffer.body.resize(m_message_buffer.header.size);
                                 ReadBody();
                             } else {
                                 // If the message does not have a body, add the message to the incoming queue
                                 AddToIncomingMessageQueue();
                             }
                         } else {
                             std::cout << "[" << m_id << "] Read header failed: " << ec.message() << std::endl;
                             m_socket.close();
                         }
                     });
}

// Reads the full body of the message
template <typename T>
void Connection<T>::ReadBody() {
    asio::async_read(m_socket, asio::buffer(m_message_buffer.body.data(), m_message_buffer.body.size()),
                     [this](std::error_code ec, std::size_t length) {
                         if (!ec) {
                             AddToIncomingMessageQueue();
                         } else {
                             std::cout << "[" << m_id << "] Read body failed: " << ec.message() << std::endl;
                             m_socket.close();
                         }
                     });
}

// Adds incoming messages to the queue to be processed later
template <typename T>
void Connection<T>::AddToIncomingMessageQueue() {
    // For the server, tag the message with the connection to distinguish between clients
    if (m_owner_type == Owner::SERVER) {
        m_incoming_messages.PushBack({this->shared_from_this(), m_message_buffer});
    } else {
        m_incoming_messages.PushBack({nullptr, m_message_buffer});
    }
    // Return to reading the next header
    ReadHeader();
}

// Writes the header of the first message in the outgoing queue
template <typename T>
void Connection<T>::WriteHeader() {
    asio::async_write(m_socket, asio::buffer(&m_outgoing_messages.Front().header, sizeof(MessageHeader<T>)),
                      [this](std::error_code ec, std::size_t length) {
                          if (!ec) {
                              if (m_outgoing_messages.Front().body.size() > 0) {
                                  // If the message has a body, write the body
                                  WriteBody();
                              } else {
                                  // If the message does not have a body, pop the message and write the next header
                                  m_outgoing_messages.PopFront();
                                  if (!m_outgoing_messages.Empty()) {
                                      WriteHeader();
                                  }
                              }
                          } else {
                              std::cout << "[" << m_id << "] Write header failed: " << ec.message() << std::endl;
                              m_socket.close();
                          }
                      });
}

// Writes the body of the first message in the outgoing queue
template <typename T>
void Connection<T>::WriteBody() {
    asio::async_write(m_socket, asio::buffer(m_outgoing_messages.Front().body.data(), m_outgoing_messages.Front().body.size()),
                      [this](std::error_code ec, std::size_t length) {
                          if (!ec) {
                              // Pop the current message and continue writing if there are more messages
                              m_outgoing_messages.PopFront();
                              if (!m_outgoing_messages.Empty()) {
                                  WriteHeader();
                              }
                          } else {
                              std::cout << "[" << m_id << "] Write body failed: " << ec.message() << std::endl;
                              m_socket.close();
                          }
                      });
}

// Hashes the validation number with "random" numbers and operations
// Used to determine if a connection is valid and from a client
template <typename T>
uint64_t Connection<T>::HashValidation(uint64_t unhashed) {
    uint64_t hash_with_version = unhashed ^ CONNECTION_VERSON;
    // Hash with random numbers and operations
    hash_with_version += 0x1d2b711cd3e5ad3d;
    hash_with_version = (hash_with_version >> 0x20) | (hash_with_version << 0x20);
    hash_with_version *= 0x6d2ba92bd3e5ad3d;
    hash_with_version ^= 0x8d2b79f09010ad3d;
    return hash_with_version;
}

// Writes the (either generated or received) validation number
template <typename T>
void Connection<T>::WriteValidation() {
    asio::async_write(m_socket, asio::buffer(&m_validation_out, sizeof(uint64_t)),
                      [this](std::error_code ec, std::size_t length) {
                          if (!ec) {
                              // If the connection is a client, continue to normal message reading
                              if (m_owner_type == Owner::CLIENT) {
                                  ReadHeader();
                              }
                          } else {
                              m_socket.close();
                          }
                      });
}

// Reads the (either hashed or generated) validation number and validates it (if the connection is a server)
template <typename T>
void Connection<T>::ReadValidation(admirals::net::Server<T>* server) {
    asio::async_read(m_socket, asio::buffer(&m_validation_in, sizeof(uint64_t)),
                     [this, server](std::error_code ec, std::size_t length) {
                         if (!ec) {
                             uint64_t validation_hash = HashValidation(m_validation_in);
                             if (m_owner_type == Owner::CLIENT) {
                                 // If the connection is a client, send the validation number back to the server
                                 m_validation_out = HashValidation(m_validation_in);
                                 WriteValidation();
                             } else {
                                 // If the connection is a server, check if the validation number is correct
                                 if (m_validation_in == HashValidation(m_validation_out)) {
                                     std::cout << "[" << m_id << "] Client validated" << std::endl;
                                     // If the validation number is correct, continue to normal message reading
                                     (m_server->*m_validation_function)(this->shared_from_this());
                                     ReadHeader();
                                 } else {
                                     std::cout << "Client validation failed" << std::endl;
                                     m_socket.close();
                                 }
                             }
                         } else {
                             std::cout << "[" << m_id << "] Read validation failed: " << ec.message() << std::endl;
                             m_socket.close();
                         }
                     });
}

}  // namespace net
}  // namespace admirals
