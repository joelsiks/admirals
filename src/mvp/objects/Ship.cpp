#include "objects/Ship.hpp"
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
    if (IsOwned()) {
        GameData::engine->onMouseClick -= BIND_EVENT_HANDLER(Ship::HandleClick);
    }
}

void Ship::HandleAction() {
    switch (GetAction()) {
    case ShipAction::None:
        if (!IsSelected()) {
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
    case ShipAction::Attack: {
        // TODO: Implement
        if (!IsSelected()) {
            m_path.clear();
        }
    } break;
    default:
        break;
    }
}

void Ship::OnUpdate(const EngineContext &) {
    if (IsSelected()) {
        m_path = GameData::engine->GetScene()->FindPath(
            GetPosition(), GameData::mousePosition, GameData::CellSize,
            {1, 2, 3}, GameData::CellSize);
    }
    HandleAction();
}

void Ship::OnClick(events::MouseClickEventArgs &args) {
    if (args.button == events::MouseButton::Left && args.pressed) {
        if (IsOwned()) {
            if (IsSelected()) {
                printf("Ship (%d): Deselected...\n", GetID());
                DeSelect();
                GameData::engine->onMouseClick -=
                    BIND_EVENT_HANDLER(Ship::HandleClick);
            } else {
                printf("Ship (%d): Selected...\n", GetID());
                Select();
                GameData::engine->onMouseClick +=
                    BIND_EVENT_HANDLER(Ship::HandleClick);
            }
            args.handled = true;
        }
    }
}

void Ship::HandleClick(void *, events::MouseClickEventArgs &args) {
    printf("Ship (%d): Handling click...\n", GetID());
    if (!IsSelected()) {
        GameData::engine->onMouseClick -= BIND_EVENT_HANDLER(Ship::HandleClick);
        args.handled = true;
        return;
    }

    if (args.button != events::MouseButton::Left || !args.pressed) {
        return;
    }

    const Vector2 clickLocation = args.Location();
    if (!m_path.empty() &&
        clickLocation.Distance(m_path.back() + HalfCellSize) <
            GameData::CellSize / SameCellDistance) {
        SetAction(ShipAction::Move);
        const Vector2 gridPosition =
            GridObject::ConvertPositionWorldToGrid(clickLocation);
        printf("Now moving to: (%f, %f)\n", gridPosition.x(), gridPosition.y());
        args.handled = true;
    }

    DeSelect();
}

void Ship::OnMouseEnter(events::MouseMotionEventArgs &) {
    m_drawOutline = true;
}

void Ship::OnMouseLeave(events::MouseMotionEventArgs &) {
    m_drawOutline = false;
}

void Ship::Render(const EngineContext &ctx) const {
    if (IsOwned()) {
        renderer::Renderer::DrawRectangle(GetBoundingBox(), Color::BLUE);
    } else {
        renderer::Renderer::DrawRectangle(GetBoundingBox(), Color::RED);
    }

    Sprite::Render(ctx);

    if (IsSelected()) {
        renderer::Renderer::DrawRectangleOutline(GetBoundingBox(), 3.f,
                                                 Color::BLUE);
    } else if (m_drawOutline) {
        renderer::Renderer::DrawRectangleOutline(GetBoundingBox(), 3.f,
                                                 Color::WHITE);
    }

    const auto position = GetPosition();
    const auto size = GetSize();

    // HealthBar
    const float healthPercentage =
        static_cast<float>(GetHealth()) /
        static_cast<float>(ShipInfoMap[GetType()].Health);
    const Vector2 healthOrigin =
        position + Vector2(0, size.y() - GameData::HealthBarSize);
    renderer::Renderer::DrawRectangle(
        healthOrigin, Vector2(size.x(), GameData::HealthBarSize), Color::GREY);
    renderer::Renderer::DrawRectangle(
        healthOrigin,
        Vector2(size.x() * healthPercentage, GameData::HealthBarSize),
        Color::BLACK);

    if (!m_path.empty()) {
        const Vector2 origin = position + HalfCellSize;
        Vector2 position = origin;
        // Draw path
        for (const Vector2 &part : m_path) {
            const Vector2 newPosition = part + GameData::CellSize / 2;
            renderer::Renderer::DrawLine(position, newPosition, Color::WHITE);
            position = newPosition;
        }
        // Draw square at destination
        renderer::Renderer::DrawRectangle(position - Vector2(10), Vector2(20),
                                          Color::WHITE);
        // Draw square at origin
        renderer::Renderer::DrawRectangle(origin - Vector2(10), Vector2(20),
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
