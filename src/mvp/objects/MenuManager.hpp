#include "GameObject.hpp"
#include "UI/TextElement.hpp"
#include "shared.hpp"

namespace admirals::mvp::objects {

class GameManager;

class MenuManager : public GameObject {

public:
    MenuManager(const std::string &name, GameManager &gameManager);
    ~MenuManager();

    void OnStart(const EngineContext &ctx) override;
    void OnUpdate(const EngineContext &ctx) override;
    void Render(const EngineContext &ctx) const override {}

    void ToggleDisconnectMenu();
    void ToggleEndGameMenu(bool won = false);

private:
    GameManager &m_gameManager;
    std::shared_ptr<admirals::UI::menu::Menu> m_disconnectMenu;
    std::shared_ptr<admirals::UI::menu::Menu> m_endGameMenu;
    // std::shared_ptr<admirals::UI::DisplayLayout> m_displayLayoutStore;
    const size_t m_disconnectIdx = 11;
    const size_t m_endGameIdx = 12;
    bool m_debug = false;
};

} // namespace admirals::mvp::objects