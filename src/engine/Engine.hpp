#pragma once

#include <memory>

#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/Element.hpp"
#include "UI/Menu.hpp"
#include "events/EventSystem.hpp"
#include "events/KeyPressEvent.hpp"
#include "events/MouseClickEvent.hpp"

namespace admirals {

class Engine {
public:
    events::EventSystem<events::MouseCLickEventArgs> onMouseClick;
    events::EventSystem<events::KeyPressEventArgs> onKeyPress;

    Engine(const std::string &gameName, int windowWidth, int windowHeight,
           bool debug);

    /// @brief Adds a menu to the list of menus inside the Engine. If
    /// toggleKeyCode is set to something other than SDLK_IGNORE, a global
    /// listener for that key activates the menu if the key is pressed.
    /// @param menuName name of the menu, can be used in @ref ActivateMenu to
    /// activate it manually.
    /// @param menu a pointer to the menu instance
    /// @param toggleKeyCode should be set to one of SDL's SDLK_* keycodes. Use
    /// SDLK_IGNORE if you wish to not enable a  global listener for the menu.
    void AddMenu(const std::string &menuName,
                 const std::shared_ptr<UI::Menu> &menu,
                 SDL_Keycode toggleKeyCode);

    void ActivateMenu(const std::string &menuName);

    inline void AddUIElement(std::shared_ptr<UI::Element> element) {
        m_displayLayout->AddElement(std::move(element));
    }

    inline void AddGameObject(std::shared_ptr<scene::GameObject> object) {
        m_scene->AddObject(std::move(object));
    }

    inline void ToggleDebugRendering() { m_renderer->ToggleDebugRendering(); }

    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeUIElement(_Args &&..._args) {
        auto object = std::make_shared<T>(_args...);
        AddUIElement(object);
        return object;
    }

    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeGameObject(_Args &&..._args) {
        auto object = std::make_shared<T>(std::forward<_Args>(_args)...);
        AddGameObject(object);
        return object;
    }

    void StartGameLoop();
    inline void StopGameLoop() { m_running = false; }

private:
    bool PollAndHandleEvent();

    bool m_running;

    std::string m_gameName;

    // The engine keeps track of a number of different menus. Only one menu is
    // active at a time, indicated by m_activeMenu pointing to the active menu.
    // If m_activeMenu is nullptr, there is no active menu.
    std::unordered_map<std::string, std::shared_ptr<UI::Menu>> m_menus;
    std::shared_ptr<UI::Menu> m_activeMenu;

    inline bool hasActiveMenu() { return m_activeMenu != nullptr; }

    // Drawables
    std::shared_ptr<UI::DisplayLayout> m_displayLayout;
    std::shared_ptr<scene::Scene> m_scene;

    std::shared_ptr<renderer::Renderer> m_renderer;
};

} // namespace admirals