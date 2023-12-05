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
        "dwa", Color::BLACK,
        Color::FromRGBA(50, 50, 50, 240),
        GameData::engine->GetWindowSize().y() / 3.0);

    menu::ClickOption playAgainOption("playAgainOption", 1.0, "Play again");
    playAgainOption.onClick.Subscribe(
        [this](void *, events::MouseClickEventArgs &args) {
            if (!args.pressed)
                return;

            // TODO: implement restart
            //m_gameManager.PlayAgain();
        });

    menu::ClickOption exitOption("exitOption", 1.0, "Exit game");
    exitOption.onClick.Subscribe(
        [](void *, events::MouseClickEventArgs &args) {
            if (!args.pressed)
                return;

            GameData::engine->StopGameLoop();
        });

    m_disconnectMenu->AddMenuOption(
        menu::MenuOption::CreateFromDerived(exitOption));

    m_endGameMenu->AddMenuOption(
        menu::MenuOption::CreateFromDerived(exitOption));
    m_endGameMenu->AddMenuOption(
        menu::MenuOption::CreateFromDerived(playAgainOption));
}

void MenuManager::OnUpdate(const EngineContext &) {}

void MenuManager::ToggleDisconnectMenu() {
    if (GameData::engine->GetDisplayLayout() == m_disconnectMenu) {
        GameData::engine->SetAndGetDisplayLayout(m_displayLayoutStore);
    } else {
        m_displayLayoutStore =
            GameData::engine->SetAndGetDisplayLayout(m_disconnectMenu);
    }
}

void MenuManager::ToggleEndGameMenu(bool won) {
    m_endGameMenu->SetMenuTitle(won ? "You won!" : "You lost!");
    if (GameData::engine->GetDisplayLayout() == m_endGameMenu) {
        GameData::engine->SetAndGetDisplayLayout(m_displayLayoutStore);
    } else {
        m_displayLayoutStore =
            GameData::engine->SetAndGetDisplayLayout(m_endGameMenu);
    }
}