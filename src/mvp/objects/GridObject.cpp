#include "objects/GridObject.hpp"
#include "GameData.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

float GridObject::GetGridScale() {
    const Vector2 windowSize = GameData::engine->GetWindowSize();
    return GetGridScale(windowSize);
}

Vector2 GridObject::GetGridOffset() {
    const Vector2 windowSize = GameData::engine->GetWindowSize();
    const float scale = GetGridScale(windowSize);
    return GetGridOffset(windowSize, scale);
}

Vector2 GridObject::ConvertPositionGridToWorld(const Vector2 &gridPosition) {
    const Vector2 windowSize = GameData::engine->GetWindowSize();
    const float scale = GetGridScale(windowSize);
    const Vector2 offset = GetGridOffset(windowSize, scale);
    return offset +
           (GameData::GridArea.Position() + gridPosition * GameData::CellSize) *
               scale;
}

Vector2 GridObject::ConvertPositionWorldToGrid(const Vector2 &worldPosition) {
    const Vector2 windowSize = GameData::engine->GetWindowSize();
    const float scale = GetGridScale(windowSize);
    const Vector2 offset = GetGridOffset(windowSize, scale);
    const Vector2 res =
        (((worldPosition - offset) / scale) - GameData::GridArea.Position()) /
        GameData::CellSize;
    return res;
}

float GridObject::GetGridScale(const Vector2 &windowSize) {
    const float dim =
        std::min(windowSize.x() + GameData::CellSize * 2, windowSize.y());
    return std::max<float>(0, dim) / GameData::TotalHeight;
}

Vector2 GridObject::GetGridOffset(const Vector2 &windowSize, float scale) {
    return (windowSize -
            Vector2(GameData::GridSize, GameData::TotalHeight) * scale) /
           2.f;
}