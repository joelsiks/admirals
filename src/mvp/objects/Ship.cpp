#include "Ship.hpp"
#include "shared.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

const Vector2 HalfCellSize = Vector2(mvp::GameData::CellSize / 2.f);
const float SameCellDistance = 1.75f;

Ship::Ship(const ShipData &data, const Vector2 &size, const Texture &source)
    : Sprite("ship-" + std::to_string(data.id), source, 3,
             Rect(data.x, data.y, size.x(), size.y()),
             Ship::ShipTypeToTexOffset(data.type)),
      m_data(data) {}

Ship::~Ship() {
    GameData::engine->onMouseClick -=
        BIND_EVENT_HANDLER_FROM(Ship::HandleClick, this);
}

void Ship::HandleAction() {
    switch (GetAction()) {
    case ShipAction::None:
        if (GameData::selectedShip != m_name) {
            m_path.clear();
        }
        break;
    case ShipAction::Move: {
        if (m_path.empty()) {
            SetAction(ShipAction::None);
            events::EventArgs e;
            onChanged.Invoke(this, e);
            break;
        }
        const Vector2 target = m_path.front();
        if (GetPosition() == target) {
            m_path.pop_front();
            HandleAction();
            events::EventArgs e;
            onChanged.Invoke(this, e);
            break;
        }

        const Vector2 gridPosition =
            GridObject::ConvertPositionWorldToGrid(target);
        const auto x = static_cast<uint8_t>(gridPosition.x());
        const auto y = static_cast<uint8_t>(gridPosition.y());
        if (x != GetActionX() || y != GetActionY()) {
            SetActionX(x);
            SetActionY(y);
            events::EventArgs e;
            onChanged.Invoke(this, e);
        }
    } break;
    default:
        break;
    }
}

void Ship::OnUpdate(const EngineContext &) {
    if (GameData::selectedShip == m_name) {
        m_path = GameData::engine->GetScene()->FindPath(
            GetPosition(), GameData::mousePosition, GameData::CellSize,
            {1, 2, 3}, GameData::CellSize);
    }

    HandleAction();
}

void Ship::OnClick(events::MouseClickEventArgs &) {
    printf("My player id = %d\n", GetID());
    if (GameData::selectedShip != m_name && GameData::playerId == GetID()) {
        GameData::selectedShip = m_name;
        GameData::engine->onMouseClick += BIND_EVENT_HANDLER(Ship::HandleClick);
    }
}

void Ship::HandleClick(void *, events::MouseClickEventArgs &args) {
    if (args.button == events::MouseButton::Left && args.pressed) {
        if (!GetBoundingBox().Contains(args.Location())) {
            GameData::selectedShip = "";
            if (!m_path.empty() &&
                args.Location().Distance(m_path.back() + HalfCellSize) <
                    GameData::CellSize / SameCellDistance) {
                SetAction(ShipAction::Move);
                // HandleAction();
            }
        }
    }
}

void Ship::OnMouseEnter(events::MouseMotionEventArgs &) {
    m_drawOutline = true;
}

void Ship::OnMouseLeave(events::MouseMotionEventArgs &) {
    m_drawOutline = false;
}

void Ship::Render(const EngineContext &ctx) const {
    Sprite::Render(ctx);
    if (GameData::selectedShip == m_name) {
        renderer::Renderer::DrawRectangleOutline(GetBoundingBox(), 3.f,
                                                 Color::BLUE);

    } else if (m_drawOutline) {
        renderer::Renderer::DrawRectangleOutline(GetBoundingBox(), 3.f,
                                                 Color::WHITE);
    }

    if (!m_path.empty()) {
        const Vector2 origin = GetPosition() + HalfCellSize;
        Vector2 position = origin;

        for (const Vector2 &part : m_path) {
            const Vector2 newPosition = part + GameData::CellSize / 2;
            renderer::Renderer::DrawLine(position, newPosition, Color::BLUE);
            position = newPosition;
        }
        // Draw square at destination
        renderer::Renderer::DrawRectangle(position - Vector2(10), Vector2(20),
                                          Color::BLUE);
        // Draw square at origin
        renderer::Renderer::DrawRectangle(origin - Vector2(10), Vector2(20),
                                          Color::BLUE);
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
