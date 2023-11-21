#pragma once

#include <memory>

#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/Element.hpp"
#include "UI/Menu.hpp"

namespace admirals {

class Engine {
public:
    Engine(const std::string &gameName, int windowWidth, int windowHeight,
           bool debug);

    inline void SetEscapeMenu(std::shared_ptr<UI::Menu> menu) {
        m_escapeMenu = menu;
    }

    inline void AddUIElement(std::shared_ptr<UI::Element> element) {
        m_displayLayout->AddElement(element);
    }

    inline void AddGameObject(std::shared_ptr<scene::GameObject> object) {
        m_scene->AddObject(object);
    }

    inline void ToggleDebugRendering() { m_renderer->ToggleDebugRendering(); }

    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeAndSetEscapeMenu(_Args &&..._args) {
        auto object = std::make_shared<T>(_args...);
        SetEscapeMenu(object);
        return object;
    }

    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeUIElement(_Args &&..._args) {
        auto object = std::make_shared<T>(_args...);
        AddUIElement(object);
        return object;
    }

    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeGameObject(_Args &&..._args) {
        auto object = std::make_shared<T>(_args...);
        AddGameObject(object);
        return object;
    }

    void StartGameLoop();
    inline void StopGameLoop() { m_running = false; }

private:
    bool PollAndHandleEvent();

    bool m_running;

    std::string m_gameName;

    bool m_showingMenus = false;

    // Drawables
    std::shared_ptr<UI::Menu> m_escapeMenu;
    std::shared_ptr<UI::DisplayLayout> m_displayLayout;
    std::shared_ptr<scene::Scene> m_scene;

    std::shared_ptr<renderer::Renderer> m_renderer;
};

} // namespace admirals