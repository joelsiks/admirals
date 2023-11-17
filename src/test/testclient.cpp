#include <iostream>
#include <string>

#include "Client.hpp"
#include "NetworkTestData.hpp"

using namespace admirals::net;

class TestClient : public admirals::net::Client {
private:
    GameState state;

public:
    void SpawnShip(uint8_t x, uint8_t y) {
        admirals::net::Message msg;
        msg.header.id = TestEnum::SPAWN_SHIP;
        msg << x << y;
        Send(msg);
    }

    void MoveShip(uint8_t x, uint8_t y, uint8_t new_x, uint8_t new_y) {
        admirals::net::Message msg;
        msg.header.id = TestEnum::MOVE_SHIP;
        msg << x << y << new_x << new_y;
        Send(msg);
    }

    void PrintBoard() {
        system("cls");
        std::cout << "Turn: " << state.turn << std::endl;
        for (int i = 0; i < 8; i++) {
            std::cout << "[";
            for (int j = 0; j < 8; j++) {
                std::cout << static_cast<int>(state.board[i][j]) << ", ";
            }
            std::cout << "]" << std::endl;
        }
    }

    void UpdateBoard(admirals::net::Message message) { message >> state; }
};

int main() {
    srand(time(NULL));
    TestClient client;
    client.Connect("127.0.0.1", "1234");

    while (!client.IsConnected()) {
        std::cout << "Waiting for connection" << std::endl;
    }

    bool game_started = false;
    while (!game_started) {
        std::cout << "Waiting for game to start" << std::endl;
        size_t size = client.Incoming().Size();
        for (int i = 0; i < size; i++) {
            auto msg = client.Incoming().Front().message;
            client.Incoming().PopFront();

            if (msg.header.id == TestEnum::GAME_START) {
                std::cout << "Game starting" << std::endl;
                game_started = true;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    while (true) {
        if (!client.IsConnected()) {
            std::cout << "Disconnected from server" << std::endl;
            break;
        }

#ifdef _WIN32
        if (GetAsyncKeyState('Q') & 0x8000) {
            std::cout << "Spawning ship" << std::endl;
            client.SpawnShip(0, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (GetAsyncKeyState('W') & 0x8000) {
            std::cout << "Moving ship" << std::endl;
            uint8_t new_x = rand() % 8;
            uint8_t new_y = rand() % 8;
            client.MoveShip(0, 0, new_x, new_y);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (GetAsyncKeyState('E') & 0x8000) {
            std::cout << "Disconnecting" << std::endl;
            break;
        }
#endif

        size_t size = client.Incoming().Size();
        for (int i = 0; i < size; i++) {
            auto msg = client.Incoming().Front().message;
            client.Incoming().PopFront();

            switch (msg.header.id) {
            case TestEnum::BOARD_UPDATE: {
                client.UpdateBoard(msg);
                break;
            }
            default: {
                std::cout << "Unknown message id: "
                          << static_cast<int>(msg.header.id) << std::endl;
                break;
            }
            }
        }

        client.PrintBoard();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    client.Disconnect();
}
