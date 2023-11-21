#pragma once
#include <map>
#include <stdint.h>

const int GridCells = 10;

struct NetworkMessageTypes {
    static const uint16_t PlayerReady = 0;
    static const uint16_t ReadyConfirmation = 1;
    static const uint16_t GameStart = 2;
    static const uint16_t GameStop = 3;
    static const uint16_t BuyShip = 4;
    static const uint16_t MoveShip = 5;
    static const uint16_t AttackShip = 6;
    static const uint16_t BoardUpdate = 7;
};

struct ShipType {
    static const uint8_t None = 0;
    static const uint8_t Cruiser = 1;
    static const uint8_t Destroyer = 2;
};

struct ShipAction {
    static const uint8_t None = 0;
    static const uint8_t Move = 1;
    static const uint8_t Attack = 2;
};

struct ShipInfo {
    uint16_t Damage;
    uint16_t Health;
    uint16_t Cost;
    uint16_t textureOffsetX;
    uint16_t textureOffsetY;
};

inline std::map<uint8_t, ShipInfo> ShipInfoMap = {
    // ShipType, {damage, health, cost}
    {ShipType::Cruiser, {90, 500, 10, 0, 0}},
    {ShipType::Destroyer, {180, 300, 12, 0, 0}},
};

struct ShipData {
    ShipData() {}
    ShipData(uint16_t id, uint8_t type, uint8_t x, uint8_t y, uint16_t health,
             uint8_t owner)
        : id(id), type(type), x(x), y(y), health(health), owner(owner) {}

    uint16_t id = 0;
    uint16_t health = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t type = ShipType::None;
    uint8_t owner = -1;
    uint8_t action = ShipAction::None;
    union {
        struct {
            uint8_t actionX;
            uint8_t actionY;
        } moveData;
        uint16_t attackTargetID;
    };
};
