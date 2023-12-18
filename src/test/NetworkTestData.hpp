#pragma once
#include <cstdint>

struct TestEnum {
    static const uint32_t GAME_START = 0;
    static const uint32_t BOARD_UPDATE = 1;
    static const uint32_t SPAWN_SHIP = 2;
    static const uint32_t MOVE_SHIP = 3;
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