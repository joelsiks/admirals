#include "Connection.hpp"
#include <iostream>

using namespace admirals::net;

Connection::Connection(Owner parent, asio::io_context &io_context,
                       asio::ip::tcp::socket socket,
                       MessageQueue<OwnedMessage> &incoming_messages,
                       Server *server, ValidationFunction validation_function)
    : m_ioContext(io_context), m_socket(std::move(socket)),
      m_incomingMessages(incoming_messages) {

    m_ownerType = parent;
    m_server = server;
    m_validationFunction = validation_function;

    if (m_ownerType == Owner::SERVER) {
        // Uses the current time as the validation number
        m_validationOut = uint64_t(
            std::chrono::system_clock::now().time_since_epoch().count());
    }
}

Connection::~Connection() {}

void Connection::ConnectToClient(admirals::net::Server *server, uint32_t uid) {
    if (m_socket.is_open()) {
        m_id = uid;
        // Send the validation number to the client and wait for the client to
        // send it back hashed
        WriteValidation();
        ReadValidation();
    }
}

void Connection::ConnectToServer(
    const asio::ip::tcp::resolver::results_type &endpoints) {
    asio::async_connect(
        m_socket, endpoints,
        [this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
            if (!ec) {
                // Read the validation number from the server and send it back
                // hashed
                ReadValidation();
            } else {
                std::cout << "Connect failed: " << ec.message() << std::endl;
            }
        });
}

bool Connection::Disconnect() {
    if (IsConnected()) {
        asio::post(m_ioContext, [this]() { m_socket.close(); });
        return true;
    }
    return false;
}

bool Connection::IsConnected() const { return m_socket.is_open(); }

void Connection::Send(const Message &message) {
    asio::post(m_ioContext, [this, message]() {
        // Check if asio is already writing messages
        bool writing_message = !m_outgoingMessages.Empty();
        m_outgoingMessages.PushBack(message);
        // If asio is not writing messages, start writing
        if (!writing_message) {
            WriteHeader();
        }
    });
}

uint32_t Connection::GetID() const { return m_id; }

// Handles reading the header of the message
void Connection::HandleReadHeader(std::error_code ec) {
    if (!ec) {
        if (m_messageBuffer.header.size > 0) {
            // If the message has a body, resize the buffer and read the
            // body
            m_messageBuffer.body.resize(m_messageBuffer.header.size);
            ReadBody();
        } else {
            // If the message does not have a body, add the message to
            // the incoming queue
            AddToIncomingMessageQueue();
        }
    } else {
        std::cout << "[" << m_id << "] Read header failed: " << ec.message()
                  << std::endl;
        m_socket.close();
    }
}

// Reads the header of the message to determine type and the size of the body
void Connection::ReadHeader() {
    asio::async_read(
        m_socket, asio::buffer(&m_messageBuffer.header, sizeof(MessageHeader)),
        [this](std::error_code ec, std::size_t length) {
            HandleReadHeader(ec);
        });
}

// Handles reading the body of the message
void Connection::HandleReadBody(std::error_code ec) {
    if (!ec) {
        AddToIncomingMessageQueue();
    } else {
        std::cout << "[" << m_id << "] Read body failed: " << ec.message()
                  << std::endl;
        m_socket.close();
    }
}

// Reads the full body of the message
void Connection::ReadBody() {
    asio::async_read(
        m_socket,
        asio::buffer(m_messageBuffer.body.data(), m_messageBuffer.body.size()),
        [this](std::error_code ec, std::size_t length) { HandleReadBody(ec); });
}

// Adds incoming messages to the queue to be processed later
void Connection::AddToIncomingMessageQueue() {
    // For the server, tag the message with the connection to distinguish
    // between clients
    if (m_ownerType == Owner::SERVER) {
        m_incomingMessages.PushBack(
            {this->shared_from_this(), m_messageBuffer});
    } else {
        m_incomingMessages.PushBack({nullptr, m_messageBuffer});
    }
    // Return to reading the next header
    ReadHeader();
}

// Handles writing the header of the first message in the outgoing queue
void Connection::HandleWriteHeader(std::error_code ec) {
    if (!ec) {
        if (m_outgoingMessages.Front().body.size() > 0) {
            // If the message has a body, write the body
            WriteBody();
        } else {
            // If the message does not have a body, pop
            // the message and write the next header
            m_outgoingMessages.PopFront();
            if (!m_outgoingMessages.Empty()) {
                WriteHeader();
            }
        }
    } else {
        std::cout << "[" << m_id << "] Write header failed: " << ec.message()
                  << std::endl;
        m_socket.close();
    }
}

// Writes the header of the first message in the outgoing queue
void Connection::WriteHeader() {
    asio::async_write(
        m_socket,
        asio::buffer(&m_outgoingMessages.Front().header, sizeof(MessageHeader)),
        [this](std::error_code ec, std::size_t length) {
            HandleWriteHeader(ec);
        });
}

// Handles writing the body of the first message in the outgoing queue
void Connection::HandleWriteBody(std::error_code ec) {
    if (!ec) {
        // Pop the current message and continue writing if
        // there are more messages
        m_outgoingMessages.PopFront();
        if (!m_outgoingMessages.Empty()) {
            WriteHeader();
        }
    } else {
        std::cout << "[" << m_id << "] Write body failed: " << ec.message()
                  << std::endl;
        m_socket.close();
    }
}

// Writes the body of the first message in the outgoing queue
void Connection::WriteBody() {
    asio::async_write(m_socket,
                      asio::buffer(m_outgoingMessages.Front().body.data(),
                                   m_outgoingMessages.Front().body.size()),
                      [this](std::error_code ec, std::size_t length) {
                          HandleWriteBody(ec);
                      });
}

// Hashes the validation number with "random" numbers and operations
// Used to determine if a connection is valid and from a client
uint64_t Connection::HashValidation(uint64_t unhashed) {
    uint64_t hash_with_version = unhashed ^ CONNECTION_VERSON;
    // Hash with random numbers and operations
    hash_with_version += 0x1d2b711cd3e5ad3d;
    hash_with_version =
        (hash_with_version >> 0x20) | (hash_with_version << 0x20);
    hash_with_version *= 0x6d2ba92bd3e5ad3d;
    hash_with_version ^= 0x8d2b79f09010ad3d;
    return hash_with_version;
}

// Handles writing the validation number
void Connection::HandleWriteValidation(std::error_code ec) {
    if (!ec) {
        // If the connection is a client, continue to
        // normal message reading
        if (m_ownerType == Owner::CLIENT) {
            ReadHeader();
        }
    } else {
        m_socket.close();
    }
}

// Writes the (either generated or received) validation number
void Connection::WriteValidation() {
    asio::async_write(m_socket,
                      asio::buffer(&m_validationOut, sizeof(uint64_t)),
                      [this](std::error_code ec, std::size_t length) {
                          HandleWriteValidation(ec);
                      });
}

// Handles reading the validation number
void Connection::HandleReadValidation(std::error_code ec) {
    if (!ec) {
        uint64_t validation_hash = HashValidation(m_validationIn);
        if (m_ownerType == Owner::CLIENT) {
            // If the connection is a client, send the validation number
            // back to the server
            m_validationOut = HashValidation(m_validationIn);
            WriteValidation();
        } else {
            // If the connection is a server, check if the validation
            // number is correct
            if (m_validationIn == HashValidation(m_validationOut)) {
                std::cout << "[" << m_id << "] Client validated" << std::endl;
                // If the validation number is correct, continue to
                // normal message reading
                m_validationFunction(this->shared_from_this());
                ReadHeader();
            } else {
                std::cout << "Client validation failed" << std::endl;
                m_socket.close();
            }
        }
    } else {
        std::cout << "[" << m_id << "] Read validation failed: " << ec.message()
                  << std::endl;
        m_socket.close();
    }
}

// Reads the (either hashed or generated) validation number and validates it (if
// the connection is a server)
void Connection::ReadValidation() {
    asio::async_read(m_socket, asio::buffer(&m_validationIn, sizeof(uint64_t)),
                     [this](std::error_code ec, std::size_t length) {
                         HandleReadValidation(ec);
                     });
}
