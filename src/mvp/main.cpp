#include "Engine.hpp"
#include "UI/TextElement.hpp"
#include "objects/Background.hpp"
#include "objects/GameManager.hpp"
#include "objects/Grid.hpp"
#include "objects/IconifiedButton.hpp"
#include "objects/Quad.hpp"
#include "objects/Ship.hpp"
#include "shared.hpp"
#include <SDL_vulkan.h>
#include <VK2D/Constants.h>
#include <VK2D/VK2D.h>

#undef TRANSPARENT

using namespace admirals;
using namespace admirals::mvp;
using namespace admirals::mvp::objects;

int main(int argc, char *argv[]) {
    const float GridWidth = GameData::GridSize;
    const float GridHeight = GameData::GridSize + 2 * GameData::CellSize;
    GameData::engine =
        std::make_unique<Engine>("Admirals", GridWidth, GridHeight, false);
    const Color blue = Color::FromHEX("#3283cf");
    const Color green = Color::FromHEX("#087311");
    const Vector2 cellSize = Vector2(GameData::CellSize);
    const Texture atlas =
        Texture::LoadFromPath("assets/admirals_texture_atlas.png");
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
        Vector2(GameData::SpriteSize, 0));
    GameData::engine->MakeGameObject<Sprite>(
        "base1",
        Vector3(GridWidth - GameData::CellSize,
                GridHeight - 3 * GameData::CellSize, 2),
        cellSize, atlas, Vector2(GameData::SpriteSize, 0));
    auto gameManager =
        GameData::engine->MakeGameObject<GameManager>("gameManager");

    auto coinText = GameData::engine->MakeUIElement<UI::TextElement>(
        "coinText", 0, "Coins: 0", Vector2(100, 20), Color::WHITE);

    gameManager->onCoinsChanged += [coinText](void *, auto e) {
        coinText->SetText("Coins: " + std::to_string(e.coins));
    };

    for (auto &[shipType, ship] : ShipInfoMap) {
        auto buyShipButton =
            GameData::engine->MakeUIElement<objects::IconifiedButton>(
                "buyShip" + std::to_string(shipType), 0,
                std::to_string(ship.Cost), Vector2(GameData::CellSize),
                Color::WHITE, Color::BLACK, atlas,
                Vector2(ship.textureOffsetX, ship.textureOffsetY));
        buyShipButton->SetDisplayPosition(UI::DisplayPosition::LowerLeft);
        buyShipButton->onClick += [gameManager, shipType](void *, auto) {
            gameManager->BuyShip(shipType);
        };
    }

    GameData::engine->StartGameLoop();

    return EXIT_SUCCESS;
}

// #define TRANSPARENT _TRANSPARENT