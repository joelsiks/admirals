#include "objects/Ship.hpp"
#include "GameData.hpp"
#include "PathFinding.hpp"

#include <sstream>

using namespace admirals;
using namespace admirals::mvp;
using namespace admirals::mvp::objects;

const Vector2 HalfCellSize = Vector2(mvp::GameData::CellSize / 2.f);
const float SameCellDistance = 1.75f;

Ship::Ship(const ShipData &data, const Vector2 &size, const Texture &source)
    : Sprite("ship-" + std::to_string(data.id), source, 3,
             Rect(data.location.x, data.location.y, size.x(), size.y()),
             Ship::ShipTypeToTexOffset(data.type)),
      m_data(data) {}

Ship::~Ship() {
    if (IsOwned()) {
        GameData::engine->onMouseClick -= BIND_EVENT_HANDLER(Ship::HandleClick);
    }
}

void Ship::HandleAction(const std::shared_ptr<Ship> &target) {
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

        const Vector2 moveLocation = m_path.front();
        if (GetPosition() == moveLocation) {
            m_path.pop_front();
            HandleAction();
            events::EventArgs e;
            onChanged.Invoke(this, e);
            break;
        }

        const Vector2 gridPosition =
            GridObject::ConvertPositionWorldToGrid(moveLocation);
        const auto x = static_cast<uint8_t>(gridPosition.x());
        const auto y = static_cast<uint8_t>(gridPosition.y());
        if (x != GetActionX() || y != GetActionY()) {
            SetActionX(x);
            SetActionY(y);
            events::EventArgs e;
            onChanged.Invoke(this, e);
            break;
        }
    } break;
    case ShipAction::Attack: {
        if (!IsSelected()) {
            m_path.clear();
        }

        if (target == nullptr || m_target.empty()) {
            SetAction(ShipAction::None);
            events::EventArgs e;
            onChanged.Invoke(this, e);
            break;
        }

        if (GetAttackID() != target->GetID()) {
            SetAttackID(target->GetID());
            events::EventArgs e;
            onChanged.Invoke(this, e);
            break;
        }
    } break;
    default:
        break;
    }
}

bool PathValidator(
    const Rect &bounds,
    const std::unordered_set<std::shared_ptr<IInteractiveDisplayable>>
        &objects) {
    for (const auto &object : objects) {
        if ((object->order() == 3.f || object->order() == 2.f) &&
            bounds.Overlaps(object->GetBoundingBox())) {
            return false;
        }
    }
    return true;
}

void Ship::UpdateNavMesh() {
    const Vector2 p1 = GridObject::ConvertPositionGridToWorld(0);
    const Vector2 p2 =
        GridObject::ConvertPositionGridToWorld(GameData::GridCells);
    const Rect boardBounds = Rect(p1, p2 - p1);

    GameData::navMesh = GameData::engine->GetScene()->BuildNavMesh(
        boardBounds, GetSize(), GameData::CellSize * GridObject::GetGridScale(),
        PathValidator);
}

bool IsValidNavLocation(const Vector2 &location, std::string &target) {
    const Vector2 centeredLocation =
        location + HalfCellSize * GridObject::GetGridScale();
    const auto destObjects =
        GameData::engine->GetScene()->GetQuadTree().GetObjectsAtPosition(
            centeredLocation);

    target.clear();
    bool isValid = true;
    for (const auto &object : destObjects) {
        if (!object->GetBoundingBox().Contains(centeredLocation)) {
            continue;
        }

        if (const auto ship = dynamic_pointer_cast<Ship>(object);
            ship != nullptr && !ship->IsOwned()) {
            target = ship->identifier();
            return true;
        }

        if (object->order() > 1) {
            isValid = false;
        }
    }

    return isValid;
}

void Ship::ValidateNavPath() {
    if (m_path.empty()) {
        return;
    }

    if (!IsValidNavLocation(m_path.back(), m_target)) {
        m_path.clear();
    }
}

std::shared_ptr<Ship> Ship::ValidateTarget() {
    if (GameData::navMesh == nullptr) {
        UpdateNavMesh();
    }

    auto target = std::dynamic_pointer_cast<Ship>(
        GameData::engine->GetScene()->FindDisplayable(m_target));

    if (target == nullptr) {
        m_target.clear();
        m_path.clear();
        return nullptr;
    }

    const Vector2 position = GetPosition();
    const Vector2 targetPosition = target->GetPosition();
    m_path = PathFinding::FindPath(position, targetPosition, *GameData::navMesh,
                                   false, false);

    // Is adjacent to target
    if (GetAction() == ShipAction::Move && m_path.size() <= 1 &&
        targetPosition.MaxDistance(position) <
            GameData::CellSize * GridObject::GetGridScale()) {
        SetAction(ShipAction::Attack);
    } else if (GetAction() == ShipAction::Attack) {
        SetAction(ShipAction::Move);
    }

    return target;
}

void Ship::OnUpdate(const EngineContext &) {
    std::shared_ptr<Ship> target = nullptr;
    if (IsSelected()) {
        if (GameData::navMesh == nullptr) {
            UpdateNavMesh();
        }

        m_path = PathFinding::FindPath(GetPosition(), GameData::mousePosition,
                                       *GameData::navMesh, false, false);
        ValidateNavPath();
    } else if (!m_target.empty()) {
        target = ValidateTarget();
        printf("Validated target = %s\n",
               target == nullptr ? "none" : target->identifier().c_str());
    }
    HandleAction(target);
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
            m_target.clear();
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

    if (!m_path.empty()) {
        const Rect clickBounds =
            Rect(m_path.back(), GameData::navMesh->GetLevelOfDetail());
        const Vector2 clickLocation =
            args.Location() - m_boundingBox.Position();
        if (clickBounds.Contains(clickLocation)) {
            SetAction(ShipAction::Move);
            args.handled = true;
        }
    }

    DeSelect();
}

void Ship::OnMouseEnter(events::MouseMotionEventArgs &) {
    m_drawOutline = true;
}

void Ship::OnMouseLeave(events::MouseMotionEventArgs &) {
    m_drawOutline = false;
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}

void Ship::Render(const EngineContext &ctx) const {
    if (IsOwned()) {
        renderer::Renderer::DrawRectangle(GetBoundingBox(), Color::BLUE);
    } else {
        renderer::Renderer::DrawRectangle(GetBoundingBox(), Color::RED);
    }

    Sprite::Render(ctx);

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

    if (IsSelected()) {
        renderer::Renderer::DrawRectangleOutline(GetBoundingBox(), 3.f,
                                                 Color::BLUE);
    } else if (m_drawOutline) {
        renderer::Renderer::DrawRectangleOutline(GetBoundingBox(), 3.f,
                                                 Color::WHITE);
    }

    // Nav nav-path
    if (!m_path.empty()) {
        const float scale = GetGridScale(ctx.windowSize);
        const Vector2 origin = GetPosition() + HalfCellSize * scale;
        const Color color = m_target.empty() ? Color::WHITE : Color::BLACK;
        Vector2 position = origin;
        // Draw path
        for (const Vector2 &part : m_path) {
            const Vector2 newPosition = part + HalfCellSize * scale;
            renderer::Renderer::DrawLine(position, newPosition, color);
            position = newPosition;
        }
        // Draw square at destination
        renderer::Renderer::DrawRectangle(position - Vector2(10) * scale,
                                          Vector2(20) * scale, color);
        // Draw square at origin
        renderer::Renderer::DrawRectangle(origin - Vector2(10) * scale,
                                          Vector2(20) * scale, color);
    }

    // Debug draw navmesh
    if (GameData::navMesh != nullptr && IsSelected()) { //  && ctx.debug
        renderer::Renderer::DrawRectangleOutline(GameData::navMesh->GetBounds(),
                                                 5, Color::RED);
        const size_t count = GameData::navMesh->GetGridWidth() *
                             GameData::navMesh->GetGridHeight();
        const Color tRed = Color(0.8, 0.1, 0.1, 0.3f);
        for (size_t i = 0; i < count; i++) {
            const auto pos = PathFinding::ConvertNodeIndexToVector(
                                 i, GameData::navMesh->GetGridWidth(),
                                 GameData::navMesh->GetLevelOfDetail()) +
                             GameData::navMesh->GetBounds().Position();
            const float cost = GameData::navMesh->GetCostAt(i);
            if (cost < 0) {
                renderer::Renderer::DrawRectangle(
                    pos, GameData::navMesh->GetLevelOfDetail(), tRed);
            }

            std::string target;
            if (IsValidNavLocation(pos, target)) {
                renderer::Renderer::DrawRectangle(pos, 10.f, Color::GREEN);
            } else {
                renderer::Renderer::DrawRectangle(pos, 10.f, Color::RED);
            }

            renderer::Renderer::DrawText(*ctx.fontTexture, pos, Color::RED,
                                         to_string_with_precision(cost, 3));
            renderer::Renderer::DrawRectangleOutline(
                pos, GameData::navMesh->GetLevelOfDetail(), 2, Color::RED);
        }
    }
}

Vector2 Ship::ShipTypeToTexOffset(uint16_t type) {
    switch (type) {
    case ShipType::Cruiser:
        return Vector2(0, GameData::SpriteSize);
    case ShipType::Destroyer:
        return Vector2(GameData::SpriteSize, GameData::SpriteSize);
    case ShipType::Base:
        return Vector2(0);
    default:
        return Vector2(0, GameData::SpriteSize);
    }
}
