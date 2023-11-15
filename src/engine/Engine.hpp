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
    ~Engine();

    void AddUIElement(std::shared_ptr<UI::Element> element);
    void AddGameObject(std::shared_ptr<scene::GameObject> object);

    void StartGameLoop();

private:
    bool CheckQuit();

    std::string m_gameName;

    // Drawables
    std::shared_ptr<UI::DisplayLayout> m_displayLayout;
    std::shared_ptr<scene::Scene> m_scene;

    std::shared_ptr<renderer::Renderer> m_renderer;
};

} // namespace admirals