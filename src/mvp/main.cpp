#include "Engine.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/TextElement.hpp"
#include "UI/menu/Menu.hpp"
#include "UI/menu/MenuOption.hpp"

#include "objects/Background.hpp"
#include "objects/GameManager.hpp"
#include "objects/Grid.hpp"
#include "objects/IconifiedButton.hpp"
#include "objects/MenuMovingShip.hpp"
#include "objects/Quad.hpp"
#include "objects/Ship.hpp"
#include "shared.hpp"

// Undefine macro from msys
#undef TRANSPARENT

using namespace admirals;
using namespace admirals::mvp;
using namespace admirals::mvp::objects;

const float GridWidth = GameData::GridSize;
const float GridHeight = GameData::GridSize + 2 * GameData::CellSize;

const Vector2 cellSize = Vector2(GameData::CellSize);
const Color blue = Color::FromHEX("#3283cf");
const Color green = Color::FromHEX("#087311");

void SwapEngineScene() {
    GameData::g_sceneStore =
        GameData::engine->SetAndGetScene(GameData::g_sceneStore);
    GameData::engine->ToggleLayer(GameData::startMenuIdx);
    GameData::engine->ToggleLayer(GameData::gameUIIdx);
}

void CreateGameBoard() {
    GameData::engine->MakeGameObject<Background>("background", blue);
    GameData::engine->MakeGameObject<Grid>("grid", Color::BLACK);
    GameData::engine->MakeGameObject<Quad>(
        "overlayTop", 1, Rect(0, -1, GridWidth, GameData::CellSize),
        Color::BLACK);
    GameData::engine->MakeGameObject<Quad>(
        "overlayBottom", 1,
        Rect(0, static_cast<float>(GameData::GridCells), GridWidth,
             GameData::CellSize),
        Color::BLACK);
    GameData::engine->MakeGameObject<Quad>(
        "islandLeft", 1, Rect(0, 0, GameData::CellSize, GameData::CellSize * 3),
        green);
    GameData::engine->MakeGameObject<Quad>(
        "islandRight", 1,
        Rect(static_cast<float>(GameData::GridCells) - 1,
             static_cast<float>(GameData::GridCells) - 3, GameData::CellSize,
             GameData::CellSize * 3),
        green);
}

void CreateBases(const Texture &atlas,
                 const std::shared_ptr<GameManager> &gameManager) {
    const Vector2 cellSize = Vector2(GameData::CellSize);
    const auto baseTop = GameData::engine->MakeGameObject<Base>(
        "baseTop", atlas, 2,
        Rect(0, 1, GameData::CellSize, GameData::CellSize));

    const auto baseBottom = GameData::engine->MakeGameObject<Base>(
        "baseBottom", atlas, 2,
        Rect(static_cast<float>(GameData::GridCells) - 1,
             static_cast<float>(GameData::GridCells) - 2, GameData::CellSize,
             GameData::CellSize));
    gameManager->SetBases(baseTop, baseBottom);
}

void CreateGameUI(const Texture &atlas,
                  const std::shared_ptr<GameManager> &gameManager) {
    auto gameUI = std::make_shared<UI::DisplayLayout>();
    GameData::engine->AddLayer(GameData::gameUIIdx, gameUI);

    auto idText = std::make_shared<UI::TextElement>(
        "idText", 0, "Player: X", Vector2(200, 40), Color::WHITE);
    idText->SetDisplayOrientation(UI::DisplayOrientation::UpperRight);
    gameManager->onPlayerIdChanged += [idText](void *, auto e) {
        GameData::playerId = e.playerId;
        idText->SetText("Player: " + std::to_string(e.playerId));
    };
    gameUI->AddDisplayable(idText);

    auto coinText = std::make_shared<UI::TextElement>(
        "coinText", 0, "Coins: 0", Vector2(100, 20), Color::WHITE);
    gameManager->onCoinsChanged.Subscribe([coinText](void *, auto e) {
        coinText->SetText("Coins: " + std::to_string(e.coins));
    });
    gameUI->AddDisplayable(coinText);

    for (auto &[shipType, ship] : ShipInfoMap) {
        if (shipType == ShipType::None) {
            continue;
        }

        auto buyShipButton = std::make_shared<objects::IconifiedButton>(
            "buyShip" + std::to_string(shipType), 0, std::to_string(ship.Cost),
            Vector2(GameData::CellSize), Color::WHITE, Color::BLACK, atlas,
            Vector2(Ship::ShipTypeToTexOffset(shipType)));

        buyShipButton->SetDisplayOrientation(UI::DisplayOrientation::LowerLeft);
        buyShipButton->onClick.Subscribe([gameManager, shipType](void *, auto) {
            gameManager->BuyShip(shipType);
        });

        gameUI->AddDisplayable(buyShipButton);
    }
}

void CreateStartMenu(const std::shared_ptr<GameManager> &gameManager) {
    auto startMenu = std::make_shared<UI::menu::Menu>(
        "Admirals Conquest (MVP)", Color::BLACK,
        Color::FromRGBA(20, 20, 20, 140), 100);
    GameData::engine->AddLayer(GameData::startMenuIdx, startMenu, false);

    auto exitOption =
        std::make_shared<UI::menu::ClickOption>("exitOption", 1.0, "Exit...");
    exitOption->onClick.Subscribe(
        [](void *, events::MouseClickEventArgs &args) {
            if (!args.pressed)
                return;

            GameData::engine->StopGameLoop();
        });
    startMenu->AddDisplayable(exitOption);

    auto connectOption = std::make_shared<UI::menu::ClickOption>(
        "connectOption", 1.0, "Connect to server");
    connectOption->onClick.Subscribe(
        [gameManager](void *, events::MouseClickEventArgs &args) {
            if (args.pressed) {
                const bool connected = gameManager->ConnectToServer();
                if (connected) {
                    SwapEngineScene();
                }
            }
        });
    startMenu->AddDisplayable(connectOption);

    auto startOption = std::make_shared<UI::menu::ClickOption>(
        "startOption", 1.0, "Start server");
    startOption->onClick.Subscribe(
        [gameManager](void *, events::MouseClickEventArgs &args) {
            if (args.pressed) {
                const bool connected = gameManager->StartAndConnectToServer();
                if (connected) {
                    SwapEngineScene();
                }
            }
        });
    startMenu->AddDisplayable(startOption);
}

void CreateStartMenuScene(const Texture &atlas) {
    auto startMenuScene = std::make_shared<Scene>();

    startMenuScene->AddDisplayable(
        std::make_shared<Background>("background", blue));

    startMenuScene->AddDisplayable(std::make_shared<MenuMovingShip>(
        "movingShip1", atlas, 1, Rect(0, -1, 80, 80), ShipType::Cruiser, 1));

    startMenuScene->AddDisplayable(std::make_shared<MenuMovingShip>(
        "movingShip2", atlas, 1, Rect(3, 3, 80, 80), ShipType::Destroyer, 1.7));

    startMenuScene->AddDisplayable(std::make_shared<MenuMovingShip>(
        "movingShip3", atlas, 1, Rect(5, 7, 80, 80), ShipType::Cruiser, 0.9));

    GameData::startMenuScene = startMenuScene;
}

int main(int, char *[]) {
    GameData::engine =
        std::make_unique<Engine>("Admirals", GridWidth, GridHeight, false);

    CreateGameBoard();

    const Texture atlas =
        Texture::LoadFromPath("assets/admirals_texture_atlas.png");
    auto gameManager =
        GameData::engine->MakeGameObject<GameManager>("gameManager", atlas);

    CreateBases(atlas, gameManager);

    CreateGameUI(atlas, gameManager);

    CreateStartMenu(gameManager);
    CreateStartMenuScene(atlas);

    GameData::g_sceneStore = GameData::startMenuScene;
    SwapEngineScene();

    GameData::engine->onMouseMove +=
        [](void *, events::MouseMotionEventArgs args) {
            GameData::mousePosition = args.Location();
        };

    GameData::engine->StartGameLoop();
    GameData::engine.release();

    return EXIT_SUCCESS;
}