#include <iostream>

#include "Server.hpp"

enum class TestEnum : uint32_t {
    GAME_START,
    BOARD_UPDATE,
    SPAWN_SHIP,
    MOVE_SHIP
};

struct GameState {
    uint32_t turn;
    uint8_t board[8][8];

    GameState() {
        turn = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = 0;
            }
        }
    }
};

class TestServer : public admirals::net::Server<TestEnum> {
private:
    GameState state;

public:
    bool game_started = false;

public:
    TestServer(uint16_t port) : admirals::net::Server<TestEnum>(port) {}

    void BroadcastState() {
        admirals::net::Message<TestEnum> msg;
        msg.header.id = TestEnum::BOARD_UPDATE;
        msg << state;
        MessageAllClients(msg);
        state.turn++;
    }

protected:
    virtual bool OnClientConnect(
        std::shared_ptr<admirals::net::Connection<TestEnum>> client) override {
        return true;
    }

    virtual void OnClientDisconnect(
        std::shared_ptr<admirals::net::Connection<TestEnum>> client) override {}

    virtual void
    OnMessage(std::shared_ptr<admirals::net::Connection<TestEnum>> client,
              admirals::net::Message<TestEnum> &message) override {
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

    virtual void OnClientValidated(
        std::shared_ptr<admirals::net::Connection<TestEnum>> client) override {
        uint32_t players = 0;
        for (auto &client : connections) {
            if (client && client->IsConnected()) {
                players++;
            }
        }
        if (players == 2 && !game_started) {
            std::cout << "Starting game" << std::endl;
            admirals::net::Message<TestEnum> msg;
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
