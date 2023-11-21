#pragma once

#include <memory>

#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/Element.hpp"
#include "events/EventSystem.hpp"
#include "events/MouseClickEvent.hpp"

namespace admirals {

class Engine {
public:
    events::EventSystem<events::MouseCLickEventArgs> onMouseClick;

    Engine(const std::string &gameName, int windowWidth, int windowHeight,
           bool debug);

    inline void AddUIElement(std::shared_ptr<UI::Element> element) {
        m_displayLayout->AddElement(std::move(element));
    }

    inline void AddGameObject(std::shared_ptr<scene::GameObject> object) {
        m_scene->AddObject(std::move(object));
    }

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

private:
    bool PollAndHandleEvent();

    std::string m_gameName;

    // Drawables
    std::shared_ptr<UI::DisplayLayout> m_displayLayout;
    std::shared_ptr<scene::Scene> m_scene;

    std::shared_ptr<renderer::Renderer> m_renderer;
};

} // namespace admirals