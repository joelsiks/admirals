#include "Engine.hpp"
#include "objects/Background.hpp"
#include "objects/Grid.hpp"
#include "objects/Quad.hpp"
#include "objects/Ship.hpp"
#include "shared.hpp"
#include <SDL_vulkan.h>
#include <VK2D/Constants.h>
#include <VK2D/VK2D.h>

using namespace admirals;
using namespace admirals::mvp;
using namespace admirals::mvp::objects;

int main(int argc, char *argv[]) {
    const float GridWidth = GameData::GridSize;
    const float GridHeight = GameData::GridSize + 2 * GameData::CellSize;
    GameData::engine =
        std::make_unique<Engine>("Admirals", GridWidth, GridHeight, true);
    Color blue = Color::FromHEX("#3283cf");
    Color green = Color::FromHEX("#087311");
    Vector2 cellSize = Vector2(GameData::CellSize);
    Texture atlas = Texture::LoadFromPath("assets/admirals_texture_atlas.png");
    GameData::engine->MakeGameObject<Background>("background", blue);
    GameData::engine->MakeGameObject<Grid>("grid", Color::BLACK);
    GameData::engine->MakeGameObject<Quad>(
        "overlayTop", Vector3(0, 0, 1), Vector2(GridWidth, GameData::CellSize),
        Color::BLACK);
    GameData::engine->MakeGameObject<Quad>(
        "overlayBottom", Vector3(0, GridHeight - GameData::CellSize, 1),
        Vector2(GridWidth, GameData::CellSize), Color::BLACK);

    GameData::engine->MakeGameObject<Quad>(
        "islandLeft0", Vector3(0, GameData::CellSize, 1), cellSize, green);
    GameData::engine->MakeGameObject<Quad>(
        "islandLeft1", Vector3(0, 2 * GameData::CellSize, 1), cellSize, green);
    GameData::engine->MakeGameObject<Quad>(
        "islandLeft2", Vector3(0, 3 * GameData::CellSize, 1), cellSize, green);
    GameData::engine->MakeGameObject<Quad>(
        "islandRight0",
        Vector3(GridWidth - GameData::CellSize,
                GridHeight - 2 * GameData::CellSize, 1),
        cellSize, green);
    GameData::engine->MakeGameObject<Quad>(
        "islandRight1",
        Vector3(GridWidth - GameData::CellSize,
                GridHeight - 3 * GameData::CellSize, 1),
        cellSize, green);
    GameData::engine->MakeGameObject<Quad>(
        "islandRight2",
        Vector3(GridWidth - GameData::CellSize,
                GridHeight - 4 * GameData::CellSize, 1),
        cellSize, green);
    GameData::engine->MakeGameObject<Sprite>(
        "base0", Vector3(0, 2 * GameData::CellSize, 2), cellSize, atlas,
        Vector2(64, 0));
    GameData::engine->MakeGameObject<Sprite>(
        "base1",
        Vector3(GridWidth - GameData::CellSize,
                GridHeight - 3 * GameData::CellSize, 2),
        cellSize, atlas, Vector2(64, 0));
    GameData::engine->MakeGameObject<Ship>(
        "ship0", Vector3(GameData::CellSize, 5 * GameData::CellSize, 2),
        cellSize, atlas);
    GameData::engine->StartGameLoop();

    return EXIT_SUCCESS;
}