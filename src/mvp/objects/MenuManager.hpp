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

    void ToggleOpponentDisconnectMenu();
    void ToggleServerDisconnectMenu();
    void ToggleEndGameMenu(bool won = false);

private:
    void ReturnToMenu();

    GameManager &m_gameManager;
    std::shared_ptr<admirals::UI::menu::Menu> m_opponentDisconnectMenu;
    std::shared_ptr<admirals::UI::menu::Menu> m_serverDisconnectMenu;
    std::shared_ptr<admirals::UI::menu::Menu> m_endGameMenu;
    const size_t m_opponentDisconnectIdx = 11;
    const size_t m_serverDisconnectIdx = 12;
    const size_t m_endGameIdx = 13;
    bool m_debug = false;
};

} // namespace admirals::mvp::objects