#pragma once

#include <memory>

#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/Element.hpp"

namespace admirals {

class Engine {
public:
    Engine(const std::string &gameName, int windowWidth, int windowHeight,
           bool debug);

    inline void AddUIElement(std::shared_ptr<UI::Element> element) {
        m_displayLayout->AddElement(element);
    }

    inline void AddGameObject(std::shared_ptr<scene::GameObject> object) {
        m_scene->AddObject(object);
    }

    template <typename T, typename... _Args>
    inline std::shared_ptr<std::_NonArray<T>>
    EmplaceUIElement(_Args &&..._args) {
        auto object = std::make_shared<T>(_args...);
        AddUIElement(object);
        return object;
    }

    template <typename T, typename... _Args>
    inline std::shared_ptr<std::_NonArray<T>>
    EmplaceGameObject(_Args &&..._args) {
        auto object = std::make_shared<T>(_args...);
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