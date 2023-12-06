#include "objects/MenuManager.hpp"
#include "UI/menu/Menu.hpp"
#include "UI/menu/MenuOption.hpp"
#include "commontypes.hpp"
#include "objects/GameManager.hpp"

using namespace admirals;
using namespace admirals::UI;
using namespace admirals::mvp::objects;

MenuManager::MenuManager(const std::string &name, GameManager &gameManager)
    : GameObject(name), m_gameManager(gameManager) {}

MenuManager::~MenuManager() {}

void MenuManager::OnStart(const EngineContext &) {
    m_opponentDisconnectMenu = std::make_shared<menu::Menu>(
        "Waiting for opponent to reconnect...", Color::BLACK,
        Color::FromRGBA(50, 50, 50, 240),
        GameData::engine->GetWindowSize().y() / 3.0);

    m_serverDisconnectMenu = std::make_shared<menu::Menu>(
        "Game server closed", Color::BLACK, Color::FromRGBA(50, 50, 50, 240),
        GameData::engine->GetWindowSize().y() / 3.0);

    m_endGameMenu = std::make_shared<menu::Menu>(
        "", Color::BLACK, Color::FromRGBA(50, 50, 50, 240),
        GameData::engine->GetWindowSize().y() / 3.0);

    const auto returnToMenuOption = std::make_shared<menu::ClickOption>(
        "returnToMenuOption", 1.0, "Return to menu");
    returnToMenuOption->onClick.Subscribe(
        [this](void *, events::MouseClickEventArgs &args) {
            if (!args.pressed)
                return;

            // TODO: reset game state
            // should also close the server if hosting
            // m_gameManager.Reset();

            ReturnToMenu();
        });

    const auto playAgainOption = std::make_shared<menu::ClickOption>(
        "playAgainOption", 1.0, "Play again");
    playAgainOption->onClick.Subscribe(
        [this](void *, events::MouseClickEventArgs &args) {
            if (!args.pressed)
                return;

            // TODO: implement restart
            // m_gameManager.PlayAgain();
        });

    const auto exitOption =
        std::make_shared<menu::ClickOption>("exitOption", 1.0, "Exit game");
    exitOption->onClick.Subscribe(
        [](void *, events::MouseClickEventArgs &args) {
            if (!args.pressed)
                return;

            GameData::engine->StopGameLoop();
        });

    m_opponentDisconnectMenu->AddDisplayable(exitOption);
    m_opponentDisconnectMenu->AddDisplayable(returnToMenuOption);

    m_serverDisconnectMenu->AddDisplayable(exitOption);
    m_serverDisconnectMenu->AddDisplayable(returnToMenuOption);

    m_endGameMenu->AddDisplayable(exitOption);
    m_endGameMenu->AddDisplayable(returnToMenuOption);
    m_endGameMenu->AddDisplayable(playAgainOption);

    GameData::engine->AddLayer(
        m_opponentDisconnectIdx,
        std::dynamic_pointer_cast<IDisplayLayer>(m_opponentDisconnectMenu),
        false);
    GameData::engine->AddLayer(
        m_serverDisconnectIdx,
        std::dynamic_pointer_cast<IDisplayLayer>(m_serverDisconnectMenu),
        false);
    GameData::engine->AddLayer(
        m_endGameIdx, std::dynamic_pointer_cast<IDisplayLayer>(m_endGameMenu),
        false);
}

void MenuManager::OnUpdate(const EngineContext &) {}

void MenuManager::ToggleOpponentDisconnectMenu() {
    GameData::engine->ToggleLayer(GameData::gameUIIdx);
    GameData::engine->ToggleLayer(m_opponentDisconnectIdx);
}

void MenuManager::ToggleServerDisconnectMenu() {
    GameData::engine->ToggleLayer(GameData::gameUIIdx);
    GameData::engine->ToggleLayer(m_serverDisconnectIdx);
}

void MenuManager::ToggleEndGameMenu(bool won) {
    m_endGameMenu->SetTitle(won ? "You won!" : "You lost!");
    GameData::engine->ToggleLayer(GameData::gameUIIdx);
    GameData::engine->ToggleLayer(m_endGameIdx);
}

void MenuManager::ReturnToMenu() {
    if (GameData::engine->LayerIsActive(m_opponentDisconnectIdx)) {
        GameData::engine->ToggleLayer(m_opponentDisconnectIdx);
    } else if (GameData::engine->LayerIsActive(m_serverDisconnectIdx)) {
        GameData::engine->ToggleLayer(m_serverDisconnectIdx);
    } else if (GameData::engine->LayerIsActive(m_endGameIdx)) {
        GameData::engine->ToggleLayer(m_endGameIdx);
    }

    GameData::engine->ToggleLayer(GameData::startMenuIdx);

    GameData::g_sceneStore =
        GameData::engine->SetAndGetScene(GameData::g_sceneStore);
}