#include "Ship.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Ship::Ship(const ShipData &data, const Vector2 &size, const Texture &source)
    : Sprite("ship-" + std::to_string(data.id), 3, Vector2(data.x, data.y),
             size, source, Ship::ShipTypeToTexOffset(data.type)),
      m_data(data) {}

void Ship::OnUpdate(const EngineContext &c) {
    const Vector2 pos = GetPosition();
    Vector2 target = pos;
    if (m_data.owner % 2 == 1) {
        if (pos.y() >= GridCells - 1) {
            target[1] = 0;
        } else {
            target[1] += 1;
        }
    } else {
        if (pos.y() <= 0) {
            target[1] = GridCells - 1;
        } else {
            target[1] -= 1;
        }
    }

    if (m_data.action == ShipAction::None) {
        m_data.action = ShipAction::Move;
        m_data.moveData.actionX = static_cast<uint8_t>(target.x());
        m_data.moveData.actionY = static_cast<uint8_t>(target.y());
        events::EventArgs e;
        onChanged.Invoke(this, e);
    }
}

void Ship::OnStart(const EngineContext &c) {}

Vector2 Ship::CalcOrigin() const {
    return GetPosition() * GameData::CellSize - Vector2(0, GameData::CellSize);
}

Vector2 Ship::ShipTypeToTexOffset(uint16_t type) {
    switch (type) {
    case ShipType::Cruiser:
        return Vector2(0, GameData::SpriteSize);
    case ShipType::Destroyer:
        return Vector2(GameData::SpriteSize, GameData::SpriteSize);
    default:
        return Vector2(0, GameData::SpriteSize);
    }
}
