#include "Client.hpp"
#include <iostream>

using namespace admirals::net;

Client::Client() {}

Client::~Client() { Disconnect(); }

bool Client::Connect(const std::string &host, const std::string &port) {
    try {
        asio::ip::tcp::resolver resolver(m_ioContext);
        asio::ip::tcp::resolver::results_type endpoints =
            resolver.resolve(host, port);

        m_connection = std::make_unique<Connection>(
            Connection::Owner::CLIENT, m_ioContext,
            asio::ip::tcp::socket(m_ioContext), m_incomingMessages);
        bool connected = m_connection->ConnectToServer(endpoints);

        if (!m_contextThread.joinable() && connected) {
            m_contextThread = std::thread([this]() { m_ioContext.run(); });
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

void Client::Disconnect() {
    if (IsConnected()) {
        m_connection->Disconnect();
    }

    m_ioContext.stop();
    if (m_contextThread.joinable())
        m_contextThread.join();
    m_connection.release();
}

bool Client::IsConnected() const {
    if (m_connection) {
        return m_connection->IsConnected();
    } else {
        return false;
    }
}

void Client::Send(const Message &message) {
    if (IsConnected()) {
        m_connection->Send(message);
    }
}

MessageQueue<OwnedMessage> &Client::Incoming() { return m_incomingMessages; }
