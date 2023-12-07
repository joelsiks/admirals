#pragma once

#include <map>
#include <stdint.h>
#include <vector>

namespace admirals::mvp {

struct Location {
    uint8_t x = 0;
    uint8_t y = 0;
};

const uint16_t BoardSize = 10;
const std::vector<Location> IslandLocations = {{2, BoardSize - 3},
                                               {BoardSize - 3, 2}};

namespace NetworkMessageTypes {
enum NetworkMessageTypes : uint16_t {
    PlayerReady,
    ReadyConfirmation,
    GameStart,
    GameStop,
    GamePause,
    GameResume,
    BuyShip,
    MoveShip,
    AttackShip,
    BoardUpdate
};
}

namespace ShipType {
enum ShipType : uint16_t { None, Base, Cruiser, Destroyer };
}

namespace ShipAction {
enum ShipAction : uint16_t { None, Move, Attack };
}

struct ShipInfo {
    uint16_t Damage;
    uint16_t Health;
    uint16_t Cost;
    float MoveSpeed;
    float AttackSpeed;
    uint16_t NumberOfSpawnLocations;
    Location *TopSpawns;
    Location *BottomSpawns;
};

inline std::map<uint8_t, ShipInfo> ShipInfoMap = {
    // ShipType, {damage, health, cost}
    // Speed = number of actions per second
    {ShipType::None, {0, 0, 0, 0.f, 0.f, 0, nullptr, nullptr}},
    {ShipType::Base,
     {.Damage = 100,
      .Health = 3000,
      .Cost = 0,
      .MoveSpeed = 0.f,
      .AttackSpeed = 0.5f,
      .NumberOfSpawnLocations = 1,
      .TopSpawns = new Location[1]{{0, 1}},
      .BottomSpawns = new Location[1]{{BoardSize - 1, BoardSize - 2}}}},
    {ShipType::Cruiser,
     {.Damage = 90,
      .Health = 500,
      .Cost = 10,
      .MoveSpeed = 3.f,
      .AttackSpeed = 1.f,
      .NumberOfSpawnLocations = 3,
      .TopSpawns = new Location[3]{{1, 0}, {1, 1}, {1, 2}},
      .BottomSpawns = new Location[3]{{BoardSize - 2, BoardSize - 1},
                                      {BoardSize - 2, BoardSize - 2},
                                      {BoardSize - 2, BoardSize - 3}}}},
    {ShipType::Destroyer,
     {.Damage = 180,
      .Health = 300,
      .Cost = 12,
      .MoveSpeed = 5.f,
      .AttackSpeed = 0.7f,
      .NumberOfSpawnLocations = 3,
      .TopSpawns = new Location[3]{{1, 0}, {1, 1}, {1, 2}},
      .BottomSpawns = new Location[3]{{BoardSize - 2, BoardSize - 1},
                                      {BoardSize - 2, BoardSize - 2},
                                      {BoardSize - 2, BoardSize - 3}}}},
};

struct ShipData {
    ShipData() {}
    ShipData(uint16_t id, uint8_t type, uint8_t x, uint8_t y, uint16_t health,
             uint8_t owner)
        : id(id), type(type), location({x, y}), health(health), owner(owner) {}

    uint16_t id = 0;
    uint16_t lastActionTurn = 0;
    uint16_t health = 0;
    uint8_t type = ShipType::None;
    uint8_t owner = -1;
    uint8_t action = ShipAction::None;
    Location location = {0, 0};
    union {
        struct {
            uint8_t actionX = 0;
            uint8_t actionY = 0;
        } moveData;
        uint16_t attackTargetID = 0;
    };
};

} // namespace admirals::mvp