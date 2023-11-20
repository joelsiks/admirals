#include "Ship.hpp"
#include "shared.hpp"

using namespace admirals::mvp::objects;

Ship::Ship(const std::string &name, const Vector3 position, const Vector2 &size,
           const Texture &source, const Vector2 &texOffset)
    : Sprite(name, position, size, source, texOffset) {}

void Ship::OnUpdate() {}

void Ship::OnStart() {}

void Ship::Move(int x, int y) {
    SetPosition(Vector2(x * GameData::CellSize, y * GameData::CellSize));
}