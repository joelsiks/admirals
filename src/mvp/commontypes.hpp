#pragma once
#include <map>
#include <stdint.h>

namespace admirals::mvp {

const int GridCells = 10;

namespace NetworkMessageTypes {
enum NetworkMessageTypes : uint16_t {
    PlayerReady,
    ReadyConfirmation,
    GameStart,
    GameStop,
    BuyShip,
    MoveShip,
    AttackShip,
    BoardUpdate
};
}

namespace ShipType {
enum ShipType : uint16_t { None, Cruiser, Destroyer };
}

namespace ShipAction {
enum ShipAction : uint16_t { None, Move, Attack };
}

struct ShipInfo {
    uint16_t Damage;
    uint16_t Health;
    uint16_t Cost;
};

inline std::map<uint8_t, ShipInfo> ShipInfoMap = {
    // ShipType, {damage, health, cost}
    {ShipType::Cruiser, {90, 500, 10}},
    {ShipType::Destroyer, {180, 300, 12}},
};

struct ShipData {
    ShipData() {}
    ShipData(uint16_t id, uint8_t type, uint8_t x, uint8_t y, uint16_t health,
             uint16_t damage, uint8_t owner)
        : id(id), type(type), x(x), y(y), health(health), damage(damage),
          owner(owner) {}

    uint16_t id = 0;
    uint16_t health = 0;
    uint16_t damage = 0;
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

} // namespace admirals::mvp