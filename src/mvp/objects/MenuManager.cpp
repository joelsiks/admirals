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
    m_disconnectMenu = std::make_shared<menu::Menu>(
        "Waiting for opponent to reconnect...", Color::BLACK,
        Color::FromRGBA(50, 50, 50, 240),
        GameData::engine->GetWindowSize().y() / 3.0);

    m_endGameMenu = std::make_shared<menu::Menu>(
        "", Color::BLACK, Color::FromRGBA(50, 50, 50, 240),
        GameData::engine->GetWindowSize().y() / 3.0);

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

    m_disconnectMenu->AddDisplayable(exitOption);

    m_endGameMenu->AddDisplayable(exitOption);
    m_endGameMenu->AddDisplayable(playAgainOption);

    GameData::engine->AddLayer(
        m_disconnectIdx,
        std::dynamic_pointer_cast<IDisplayLayer>(m_disconnectMenu), false);
    GameData::engine->AddLayer(
        m_endGameIdx, std::dynamic_pointer_cast<IDisplayLayer>(m_endGameMenu),
        false);
}

void MenuManager::OnUpdate(const EngineContext &) {}

void MenuManager::ToggleDisconnectMenu() {
    GameData::engine->ToggleLayer(GameData::gameUIIdx);
    GameData::engine->ToggleLayer(m_disconnectIdx);
}

void MenuManager::ToggleEndGameMenu(bool won) {
    m_endGameMenu->SetMenuTitle(won ? "You won!" : "You lost!");
    GameData::engine->ToggleLayer(GameData::gameUIIdx);
    GameData::engine->ToggleLayer(m_endGameIdx);
}