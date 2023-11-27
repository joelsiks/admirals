#pragma once

#include <memory>

#include "EngineContext.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/Element.hpp"
#include "events/EventSystem.hpp"
#include "events/KeyPressEvent.hpp"
#include "events/MouseClickEvent.hpp"

namespace admirals {

class Engine {
public:
    events::EventSystem<events::MouseClickEventArgs> onMouseClick;
    events::EventSystem<events::KeyPressEventArgs> onKeyPress;

    Engine(const std::string &gameName, int windowWidth, int windowHeight,
           bool debug);

    inline const EngineContext &GetContext() const { return m_context; }

    inline std::shared_ptr<UI::DisplayLayout>
    SetAndGetDisplayLayout(const std::shared_ptr<UI::DisplayLayout> &layout) {
        auto currentLayout = m_displayLayout;
        m_displayLayout = layout;
        return currentLayout;
    }

    inline std::shared_ptr<UI::DisplayLayout> GetDisplayLayout() {
        return m_displayLayout;
    }

    inline std::shared_ptr<scene::Scene>
    SetAndGetScene(const std::shared_ptr<scene::Scene> &scene) {
        auto currentScene = m_scene;
        m_scene = scene;

        if (hasScene() && !m_scene->IsInitialized()) {
            m_scene->OnStart(m_context);
        }

        return currentScene;
    }

    inline std::shared_ptr<scene::Scene> GetScene() { return m_scene; }

    inline void AddUIElement(std::shared_ptr<UI::Element> element) {
        if (hasDisplayLayout()) {
            m_displayLayout->AddElement(std::move(element));
        }
    }

    inline void AddGameObject(std::shared_ptr<scene::GameObject> object) {
        if (hasScene()) {
            m_scene->AddObject(std::move(object));
        }
    }

    inline void ToggleDebugRendering() { m_context.debug = !m_context.debug; }

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

    inline Vector2 GetWindowSize() const { return m_context.windowSize; }

private:
    bool PollAndHandleEvent();

    bool m_running;

    std::string m_gameName;

    inline bool hasDisplayLayout() { return m_displayLayout != nullptr; }
    inline bool hasScene() { return m_scene != nullptr; }

    // Drawables
    std::shared_ptr<UI::DisplayLayout> m_displayLayout;
    std::shared_ptr<scene::Scene> m_scene;

    std::shared_ptr<renderer::Renderer> m_renderer;
    EngineContext m_context;
};

} // namespace admirals