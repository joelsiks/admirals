#include "Engine.hpp"
#include "UI/TextElement.hpp"
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

static std::shared_ptr<UI::DisplayLayout> g_dlStore;
static std::shared_ptr<scene::Scene> g_sStore;

void CreateGameBoard() {
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
}

void CreateBases(const Texture &atlas) {
    const Vector2 cellSize = Vector2(GameData::CellSize);
    GameData::engine->MakeGameObject<Sprite>(
        "base0", Vector3(0, 2 * GameData::CellSize, 2), cellSize, atlas,
        Vector2(0));
    GameData::engine->MakeGameObject<Sprite>(
        "base1",
        Vector3(GridWidth - GameData::CellSize,
                GridHeight - 3 * GameData::CellSize, 2),
        cellSize, atlas, Vector2(0));
}

void CreateUI(const Texture &atlas,
              const std::shared_ptr<GameManager> &gameManager) {
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
                Vector2(Ship::ShipTypeToTexOffset(shipType)));
        buyShipButton->SetDisplayPosition(UI::DisplayPosition::LowerLeft);
        buyShipButton->onClick += [gameManager, shipType](void *, auto) {
            gameManager->BuyShip(shipType);
        };
    }
}

void CreateStartMenu() {
    GameData::startMenuScene = std::make_shared<scene::Scene>();

    GameData::startMenu = std::make_shared<UI::menu::Menu>(
        "Admirals Conquest (MVP)", Color::BLACK,
        Color::FromRGBA(20, 20, 20, 140), 100);

    UI::menu::ClickOption exitOption("exitOption", 1.0, "Exit...");
    exitOption.onClick.Subscribe([](void *, events::MouseClickEventArgs &args) {
        if (!args.pressed)
            return;

        GameData::engine->StopGameLoop();
    });
    GameData::startMenu->AddMenuOption(
        UI::menu::MenuOption::CreateFromDerived(exitOption));

    UI::menu::ClickOption connectOption("connectOption", 1.0,
                                        "Connect to server");
    connectOption.onClick.Subscribe(
        [](void *, events::MouseClickEventArgs &args) {
            if (args.pressed) {
                GameData::engine->SetAndGetDisplayLayout(g_dlStore);
                GameData::engine->SetAndGetScene(g_sStore);
            }
        });
    GameData::startMenu->AddMenuOption(
        UI::menu::MenuOption::CreateFromDerived(connectOption));

    // Currently does nothing...
    const UI::menu::ClickOption startOption("startOption", 1.0, "Start server");
    GameData::startMenu->AddMenuOption(
        UI::menu::MenuOption::CreateFromDerived(startOption));
}

void SwapEngineLayers() {
    g_dlStore = GameData::engine->SetAndGetDisplayLayout(GameData::startMenu);
    g_sStore = GameData::engine->SetAndGetScene(GameData::startMenuScene);
}

void CreateStartMenuScene(const Texture &atlas) {
    GameData::engine->MakeGameObject<Background>("background", blue);
    GameData::engine->MakeGameObject<MenuMovingShip>(
        "movingShip1", Vector2(0, 0), Vector2(80, 80), atlas, ShipType::Cruiser,
        70);
    GameData::engine->MakeGameObject<MenuMovingShip>(
        "movingShip2", Vector2(500, 300), Vector2(80, 80), atlas,
        ShipType::Destroyer, 150);
    GameData::engine->MakeGameObject<MenuMovingShip>(
        "movingShip3", Vector2(200, 500), Vector2(80, 80), atlas,
        ShipType::Cruiser, 80);
}

int main(int, char *[]) {
    GameData::engine =
        std::make_unique<Engine>("Admirals", GridWidth, GridHeight, false);

    CreateGameBoard();

    const Texture atlas =
        Texture::LoadFromPath("assets/admirals_texture_atlas.png");

    CreateBases(atlas);

    auto gameManager =
        GameData::engine->MakeGameObject<GameManager>("gameManager");

    CreateUI(atlas, gameManager);

    CreateStartMenu();
    SwapEngineLayers();
    CreateStartMenuScene(atlas);

    GameData::engine->StartGameLoop();

    return EXIT_SUCCESS;
}