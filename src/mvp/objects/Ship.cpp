#include "Ship.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

Ship::Ship(const ShipData &data, const Vector2 &size, const Texture &source)
    : Sprite("ship-" + std::to_string(data.id), source, 3,
             Rect(data.x, data.y, size.x(), size.y()),
             Ship::ShipTypeToTexOffset(data.type)),
      m_data(data) {}

void Ship::OnUpdate(const EngineContext &ctx) {
    Sprite::OnUpdate(ctx);
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

void Ship::OnClick(events::MouseClickEventArgs &) {
    printf("Mouse Click\n");
    m_selected = true;
}

void Ship::OnMouseEnter(events::MouseMotionEventArgs &) {
    printf("Mouse Enter\n");
    m_drawOutline = true;
}

void Ship::OnMouseLeave(events::MouseMotionEventArgs &) {
    printf("Mouse Leave\n");
    m_drawOutline = false;
}

void Ship::Render(const EngineContext &ctx) const {
    Sprite::Render(ctx);
    if (m_drawOutline) {
        renderer::Renderer::DrawRectangleOutline(GetBoundingBox(), 3.f,
                                                 Color::WHITE);
    }
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
