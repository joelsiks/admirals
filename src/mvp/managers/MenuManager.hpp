#include "GameData.hpp"
#include "GameObject.hpp"
#include "UI/TextElement.hpp"
#include "UI/menu/Menu.hpp"

namespace admirals::mvp::objects {

class GameManager;

class MenuManager : public GameObject {

public:
    MenuManager(const std::string &name, GameManager &gameManager);
    ~MenuManager();

    void OnStart(const EngineContext &) override;
    void OnUpdate(const EngineContext &) override;
    void Render(const EngineContext &) const override {}

    void ShowOpponentDisconnectMenu() const;
    void ShowServerDisconnectMenu() const;
    void ShowWaitingMenu() const;
    void ShowGameEndMenu(bool won = false);
    void ShowGameMenu() const;
    void ReturnToMenu() const;

private:
    void ClearActiveMenus() const;

    GameManager &m_gameManager;
    std::shared_ptr<admirals::UI::menu::Menu> m_opponentDisconnectMenu;
    std::shared_ptr<admirals::UI::menu::Menu> m_serverDisconnectMenu;
    std::shared_ptr<admirals::UI::menu::Menu> m_endGameMenu;
    std::shared_ptr<admirals::UI::menu::Menu> m_waitingMenu;
    static const size_t OpponentDisconnectMenuIdx = 11;
    static const size_t ServerDisconnectMenuIdx = 12;
    static const size_t EndGameMenuIdx = 13;
    static const size_t WaitingMenuMenuIdx = 14;
    bool m_debug = false;
};

} // namespace admirals::mvp::objects