#include <iostream>

#include "NetworkTestData.hpp"
#include "Server.hpp"

using namespace admirals::net;

class TestServer : public Server {
private:
    GameState state;

public:
    bool game_started = false;

public:
    TestServer(uint16_t port) : Server(port) {}

    void BroadcastState() {
        Message msg;
        msg.header.id = TestEnum::BOARD_UPDATE;
        msg << state;
        MessageAllClients(msg);
        state.turn++;
    }

protected:
    virtual bool OnClientConnect(std::shared_ptr<Connection> client) override {
        return true;
    }

    virtual void
    OnClientDisconnect(std::shared_ptr<Connection> client) override {}

    virtual void OnMessage(std::shared_ptr<Connection> client,
                           Message &message) override {
        switch (message.header.id) {
        case TestEnum::SPAWN_SHIP: {
            uint8_t x, y;
            message >> y >> x;
            std::cout << "[" << client->GetID() << "]: Spawn ship at ("
                      << static_cast<int>(x) << ", " << static_cast<int>(y)
                      << ")" << std::endl;
            state.board[x][y] = 1;
            break;
        }

        case TestEnum::MOVE_SHIP: {
            uint8_t x, y, new_x, new_y;
            message >> new_y >> new_x >> y >> x;
            std::cout << "[" << client->GetID() << "]: Move ship from ("
                      << static_cast<int>(x) << ", " << static_cast<int>(y)
                      << ") to (" << static_cast<int>(new_x) << ", "
                      << static_cast<int>(new_y) << ")" << std::endl;
            if (state.board[x][y] == 0) {
                std::cout << "Invalid move" << std::endl;
                break;
            }
            state.board[new_x][new_y] = 1;
            state.board[x][y] = 0;
            break;
        }
        }
    }

    virtual void
    OnClientValidated(std::shared_ptr<Connection> client) override {
        uint32_t players = 0;
        for (auto &client : m_connections) {
            if (client && client->IsConnected()) {
                players++;
            }
        }
        if (players == 2 && !game_started) {
            std::cout << "Starting game" << std::endl;
            Message msg;
            msg.header.id = TestEnum::GAME_START;
            MessageAllClients(msg);
            game_started = true;
        }
    }
};

int main() {
    TestServer server(1234);
    server.Start();
    while (true) {
        server.Update();
        if (server.game_started) {
            server.BroadcastState();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
