#include "Engine.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/TextElement.hpp"
#include "UI/menu/Menu.hpp"
#include "UI/menu/MenuOption.hpp"

#include "GameData.hpp"
#include "objects/Animator.hpp"
#include "objects/Background.hpp"
#include "objects/GameManager.hpp"
#include "objects/Grid.hpp"
#include "objects/IconifiedButton.hpp"
#include "objects/MenuMovingShip.hpp"
#include "objects/Quad.hpp"
#include "objects/SelectionManager.hpp"
#include "objects/Ship.hpp"
#include "objects/TreasureIsland.hpp"
#include "objects/UIManager.hpp"

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
const Color gold = Color::FromHEX("#FFD700");

void SwapEngineScene(const size_t menuUIIdx = GameData::StartMenuIdx) {
    GameData::SceneStore =
        GameData::engine->SetAndGetScene(GameData::SceneStore);
    GameData::engine->ToggleLayer(menuUIIdx);
    GameData::engine->ToggleLayer(GameData::GameUIIdx);
}

void ToggleConnectMenu() {
    GameData::engine->ToggleLayer(GameData::StartMenuIdx);
    GameData::engine->ToggleLayer(GameData::ConnectMenuIdx);
}

void ToggleHelpMenu() {
    GameData::engine->ToggleLayer(GameData::StartMenuIdx);
    GameData::engine->ToggleLayer(GameData::HelpMenuIdx);
}

void CreateGameBoard(const Texture &atlas) {
    GameData::engine->MakeGameObject<Background>("background", blue,
                                                 Color::BLACK);
    GameData::engine->MakeGameObject<Grid>("grid", Color::BLACK);
    GameData::engine->MakeGameObject<Quad>(
        "islandLeft", 2, Rect(0, 0, GameData::CellSize, GameData::CellSize * 3),
        green);
    GameData::engine->MakeGameObject<Quad>(
        "islandRight", 2,
        Rect(static_cast<float>(GameData::GridCells) - 1,
             static_cast<float>(GameData::GridCells) - 3, GameData::CellSize,
             GameData::CellSize * 3),
        green);
    int index = 0;
    for (const auto &islandLocation : IslandLocations) {
        GameData::engine->MakeGameObject<TreasureIsland>(
            "treasureIsland" + std::to_string(index++), atlas, 2,
            Rect(static_cast<float>(islandLocation.x),
                 static_cast<float>(islandLocation.y), GameData::CellSize,
                 GameData::CellSize));
    }
}

void CreateGameUI(const Texture &atlas,
                  const std::shared_ptr<GameManager> &gameManager) {
    auto gameUI = std::make_shared<UI::DisplayLayout>();
    GameData::engine->AddLayer(GameData::GameUIIdx, gameUI);

    auto idText = std::make_shared<UI::TextElement>(
        "idText", 0, "Player: X", Vector2(200, 40), Color::WHITE);
    idText->SetDisplayOrientation(UI::DisplayOrientation::UpperRight);
    gameManager->onPlayerIdChanged += [idText](void *, auto e) {
        GameData::PlayerId = e.playerId;
        idText->SetText("Player: " + std::to_string(e.playerId));
    };
    gameUI->AddDisplayable(idText);

    auto coinText = std::make_shared<UI::TextElement>(
        "coinText", 0, "Coins: 0", Vector2(100, 20), Color::WHITE);
    gameManager->onCoinsChanged.Subscribe([coinText](void *, auto e) {
        coinText->SetText("Coins: " + std::to_string(e.coins));
        auto ctx = GameData::engine->GetContext();
        admirals::renderer::Renderer::TextFontSize(coinText->GetText(),
                                                   ctx.fontSize);
    });
    gameUI->AddDisplayable(coinText);

    for (auto &[shipType, ship] : ShipInfoMap) {
        if (ShipInfoMap[shipType].Cost == 0) {
            continue;
        }

        auto buyShipButton = std::make_shared<objects::IconifiedButton>(
            "buyShip" + std::to_string(shipType), 0, std::to_string(ship.Cost),
            Vector2(GameData::CellSize), Color::WHITE, Color::BLACK, atlas,
            Ship::ShipTypeToTexOffset(shipType, GameData::PlayerId)[0]);

        buyShipButton->SetDisplayOrientation(UI::DisplayOrientation::LowerLeft);
        buyShipButton->onClick.Subscribe(
            [gameManager, shipType](void *, auto args) {
                if (args.pressed && args.button == events::MouseButton::Left) {
                    gameManager->BuyShip(shipType);
                }
            });

        gameUI->AddDisplayable(buyShipButton);
    }
}

void CreateStartMenu(const std::shared_ptr<GameManager> &gameManager) {
    auto startMenu = std::make_shared<UI::menu::Menu>(
        "Admirals Conquest (MVP)", Color::BLACK,
        Color::FromRGBA(20, 20, 20, 140), 100);
    GameData::engine->AddLayer(GameData::StartMenuIdx, startMenu, false);

    auto exitOption =
        std::make_shared<UI::menu::ClickOption>("exitOption", 1.0, "Exit...");
    exitOption->onClick.Subscribe(
        [](void *, events::MouseClickEventArgs &args) {
            if (args.pressed && args.button == events::MouseButton::Left) {
                GameData::engine->StopGameLoop();
            }
        });
    startMenu->AddDisplayable(exitOption);

    auto helpOption = std::make_shared<UI::menu::ClickOption>("helpOption", 1.0,
                                                              "How to play");
    helpOption->onClick.Subscribe(
        [gameManager](void *, events::MouseClickEventArgs &args) {
            if (args.pressed && args.button == events::MouseButton::Left) {
                args.handled = true;
                ToggleHelpMenu();
            }
        });
    startMenu->AddDisplayable(helpOption);

    auto connectOption = std::make_shared<UI::menu::ClickOption>(
        "connectOption", 1.0, "Connect to server");
    connectOption->onClick.Subscribe(
        [gameManager](void *, events::MouseClickEventArgs &args) {
            if (args.pressed && args.button == events::MouseButton::Left) {
                args.handled = true;
                ToggleConnectMenu();
            }
        });
    startMenu->AddDisplayable(connectOption);

    auto startOption = std::make_shared<UI::menu::ClickOption>(
        "startOption", 1.0, "Start server");
    startOption->onClick.Subscribe(
        [gameManager](void *, events::MouseClickEventArgs &args) {
            if (args.pressed && args.button == events::MouseButton::Left) {
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

    startMenuScene->AddDisplayable(std::make_shared<UIManager>("uiManager"));

    startMenuScene->AddDisplayable(
        std::make_shared<Background>("background", blue, blue));

    startMenuScene->AddDisplayable(std::make_shared<MenuMovingShip>(
        "movingShip1", atlas, 1, Rect(0, -1, 80, 80), ShipType::Cruiser, 1));

    startMenuScene->AddDisplayable(std::make_shared<MenuMovingShip>(
        "movingShip2", atlas, 1, Rect(3, 3, 80, 80), ShipType::Destroyer, 1.7));

    startMenuScene->AddDisplayable(std::make_shared<MenuMovingShip>(
        "movingShip3", atlas, 1, Rect(5, 7, 80, 80), ShipType::Cruiser, 0.9));

    GameData::StartMenuScene = startMenuScene;
}

void CreateConnectMenu(const std::shared_ptr<GameManager> &gameManager) {
    auto connectMenu = std::make_shared<UI::menu::Menu>(
        "Enter the IP of the server", Color::BLACK,
        Color::FromRGBA(20, 20, 20, 140), 100);
    GameData::engine->AddLayer(GameData::ConnectMenuIdx, connectMenu, false);

    const auto inputOption = std::make_shared<UI::menu::InputOption>(
        "ipInput", 1.0, GameData::engine->onKeyPress, "127.0.0.1");
    inputOption->onClick.Subscribe(
        [inputOption](void *, events::MouseClickEventArgs &args) {
            if (args.pressed && args.button == events::MouseButton::Left &&
                !inputOption->IsActive()) {
                inputOption->SetInputText("");
            }
        });

    auto connectOption = std::make_shared<UI::menu::ClickOption>(
        "connectOption", 1.0, "Connect");
    connectOption->onClick.Subscribe(
        [gameManager, connectMenu,
         inputOption](void *, events::MouseClickEventArgs &args) {
            if (args.pressed && args.button == events::MouseButton::Left) {
                connectMenu->SetTitle("Connecting...");
                const bool connected = gameManager->ConnectToServer(
                    inputOption->GetOptionText().c_str());
                if (connected) {
                    SwapEngineScene(GameData::ConnectMenuIdx);
                } else {
                    connectMenu->SetTitle("Connection failed");
                }
            }
        });

    auto returnOption =
        std::make_shared<UI::menu::ClickOption>("returnOption", 1.0, "Return");
    returnOption->onClick.Subscribe(
        [](void *, events::MouseClickEventArgs &args) {
            if (args.pressed && args.button == events::MouseButton::Left) {
                ToggleConnectMenu();
            }
        });

    connectMenu->AddDisplayable(returnOption);
    connectMenu->AddDisplayable(connectOption);
    connectMenu->AddDisplayable(inputOption);
}

void CreateHelpMenu() {
    auto helpMenu = std::make_shared<UI::menu::Menu>(
        "Welcome to Admirals Conquest!", Color::BLACK,
        Color::FromRGBA(70, 70, 70, 140), 100);
    GameData::engine->AddLayer(GameData::HelpMenuIdx, helpMenu, false);

    std::vector<std::string> sentences = {
        "Win by destroying the enemy base.",
        "Spend gold to buy ships.",
        "Buy ships by clicking the ship icons.",
        "Gold generates passively.",
        "Place ships by islands for bonus gold.\ntest123",
        "Select ships by clicking on them.",
        "You can also drag to select multiple.",
        "Click on the grid to move ships.",
        "Click on enemy ships to attack them.",
        "Click on the enemy base to attack it.",
        ""};

    float order = sentences.size() + 1;
    for (const auto &sentence : sentences) {
        auto text = std::make_shared<UI::menu::TextOption>(
            "helpText" + std::to_string(order), order--, sentence);
        helpMenu->AddDisplayable(text);
    }

    auto returnOption = std::make_shared<UI::menu::ClickOption>(
        "returnOption", order--, "Return");
    returnOption->onClick.Subscribe(
        [](void *, events::MouseClickEventArgs &args) {
            if (args.pressed && args.button == events::MouseButton::Left) {
                ToggleHelpMenu();
            }
        });

    helpMenu->AddDisplayable(returnOption);
}

int main(int, char *[]) {
    GameData::engine = std::make_unique<Engine>(
        "Admirals", GridWidth, GridHeight,
        EngineDebugMode::DebugDisabled | EngineDebugMode::DebugToggleHotkey |
            EngineDebugMode::DebugPathfinding |
            EngineDebugMode::DebugRendering);

    const Texture atlas =
        Texture::LoadFromPath("assets/admirals_texture_atlas.png");
    auto gameManager =
        GameData::engine->MakeGameObject<GameManager>("gameManager", atlas);
    GameData::Selection =
        GameData::engine->MakeGameObject<SelectionManager>("selectionMananger");
    GameData::Animator = GameData::engine->MakeGameObject<Animator>("animator");
    GameData::engine->MakeGameObject<UIManager>("uiManager");

    CreateGameBoard(atlas);
    CreateGameUI(atlas, gameManager);

    CreateStartMenu(gameManager);
    CreateStartMenuScene(atlas);
    CreateConnectMenu(gameManager);
    CreateHelpMenu();

    GameData::SceneStore = GameData::StartMenuScene;
    SwapEngineScene();

    Vector2 mouseDownLocation;
    bool mouseIsDown;
    GameData::engine->onMouseClick += [&mouseIsDown, &mouseDownLocation](
                                          void *,
                                          events::MouseClickEventArgs args) {
        mouseIsDown = args.button == events::MouseButton::Left && args.pressed;
        if (mouseIsDown) {
            mouseDownLocation = args.Location();
        } else if (args.button == events::MouseButton::Left) {
            const float selectionMax =
                std::max(GameData::Selection->GetSelectionPreview().Width(),
                         GameData::Selection->GetSelectionPreview().Height());
            if (selectionMax > 10.f) {
                GameData::Selection->SetSelectionToPreviewArea();
            }
            args.handled = true;
        } else {
            GameData::Selection->ClearSelectionPreview();
        }
    };

    GameData::engine->onMouseMove += [&mouseIsDown, &mouseDownLocation](
                                         void *,
                                         events::MouseMotionEventArgs args) {
        GameData::MousePosition = args.Location();
        if (mouseIsDown) {
            const Vector2 topLeft =
                Vector2(std::min(mouseDownLocation.x(), args.Location().x()),
                        std::min(mouseDownLocation.y(), args.Location().y()));
            const Vector2 size =
                Vector2(std::abs(mouseDownLocation.x() - args.Location().x()),
                        std::abs(mouseDownLocation.y() - args.Location().y()));
            GameData::Selection->SetSelectionPreview(Rect(topLeft, size));
        }
    };

    GameData::engine->StartGameLoop();
    GameData::engine.release();

    return EXIT_SUCCESS;
}