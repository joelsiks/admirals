#include "managers/MenuManager.hpp"
#include "CommonTypes.hpp"
#include "UI/menu/Menu.hpp"
#include "UI/menu/MenuOption.hpp"
#include "managers/GameManager.hpp"

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

    m_waitingMenu = std::make_shared<UI::menu::Menu>(
        "Waiting for Opponent...", Color::BLACK,
        Color::FromRGBA(20, 20, 20, 140), 100);

    const auto returnToMenuOption = std::make_shared<menu::ClickOption>(
        "returnToMenuOption", 1.0, "Return to menu");
    returnToMenuOption->onClick.Subscribe(
        [this](void *, events::MouseClickEventArgs &args) {
            if (!args.pressed)
                return;
            m_gameManager.ExitToMenu();
        });

    const auto playAgainOption = std::make_shared<menu::ClickOption>(
        "playAgainOption", 1.0, "Play again");
    playAgainOption->onClick.Subscribe(
        [this](void *, events::MouseClickEventArgs &args) {
            if (!args.pressed)
                return;

            m_gameManager.PlayAgain();
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

    GameData::engine->AddLayer(OpponentDisconnectMenuIdx,
                               m_opponentDisconnectMenu, false);
    GameData::engine->AddLayer(ServerDisconnectMenuIdx, m_serverDisconnectMenu,
                               false);
    GameData::engine->AddLayer(EndGameMenuIdx, m_endGameMenu, false);
    GameData::engine->AddLayer(WaitingMenuMenuIdx, m_waitingMenu, false);
}

void MenuManager::OnUpdate(const EngineContext &) {}

void MenuManager::ShowOpponentDisconnectMenu() const {
    GameData::engine->ActivateLayer(OpponentDisconnectMenuIdx);
    GameData::engine->DeactivateLayer(GameData::GameUIIdx);
    GameData::engine->DeactivateLayer(ServerDisconnectMenuIdx);
}

void MenuManager::ShowServerDisconnectMenu() const {
    GameData::engine->ActivateLayer(ServerDisconnectMenuIdx);
    GameData::engine->DeactivateLayer(GameData::GameUIIdx);
    GameData::engine->DeactivateLayer(OpponentDisconnectMenuIdx);
    GameData::engine->DeactivateLayer(WaitingMenuMenuIdx);
}

void MenuManager::ShowWaitingMenu() const {
    GameData::engine->ActivateLayer(WaitingMenuMenuIdx);
    GameData::engine->DeactivateLayer(GameData::GameUIIdx);
}

void MenuManager::ShowGameEndMenu(bool won) {
    ClearActiveMenus();
    GameData::engine->ActivateLayer(EndGameMenuIdx);
    m_endGameMenu->SetTitle(won ? "You won!" : "You lost!");
}

void MenuManager::ShowGameMenu() const {
    ClearActiveMenus();
    GameData::engine->ActivateLayer(GameData::GameUIIdx);
}

void MenuManager::ClearActiveMenus() const {
    GameData::engine->DeactivateLayer(GameData::StartMenuIdx);
    GameData::engine->DeactivateLayer(GameData::GameUIIdx);
    GameData::engine->DeactivateLayer(GameData::ConnectMenuIdx);
    GameData::engine->DeactivateLayer(WaitingMenuMenuIdx);
    GameData::engine->DeactivateLayer(OpponentDisconnectMenuIdx);
    GameData::engine->DeactivateLayer(ServerDisconnectMenuIdx);
    GameData::engine->DeactivateLayer(EndGameMenuIdx);
}

void MenuManager::ReturnToMenu() const {
    ClearActiveMenus();
    GameData::engine->SetAndGetScene(GameData::StartMenuScene);
    GameData::engine->ActivateLayer(GameData::StartMenuIdx);
}