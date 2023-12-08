#pragma once

#include <memory>

#include "DeferedAction.hpp"
#include "EngineContext.hpp"
#include "GameObject.hpp"
#include "IDisplayLayer.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "UI/Element.hpp"
#include "events/EventSystem.hpp"
#include "events/KeyPressEvent.hpp"
#include "events/MouseClickEvent.hpp"
#include "events/MouseMotionEvent.hpp"

namespace admirals {

class Engine {
public:
    events::EventSystem<events::MouseClickEventArgs> onMouseClick;
    events::EventSystem<events::KeyPressEventArgs> onKeyPress;
    events::EventSystem<events::MouseMotionEventArgs> onMouseMove;

    Engine(const std::string &gameName, int windowWidth, int windowHeight,
           bool debug);

    inline const EngineContext &GetContext() const { return m_context; }

    inline bool AddLayer(size_t idx, std::shared_ptr<IDisplayLayer> layer,
                         bool active = true) {
        const bool wasInserted = m_layers.emplace(idx, std::move(layer)).second;

        if (wasInserted && active) {
            m_deferredToggleLayers.push_back({idx, DeferType::Add});
        }

        return wasInserted;
    }

    inline void DeleteLayer(size_t idx) {
        m_deferredToggleLayers.push_back({idx, DeferType::Delete});
    }

    inline std::shared_ptr<IDisplayLayer> GetLayer(size_t idx) {
        return m_layers[idx];
    }

    inline bool LayerIsActive(size_t idx) {
        if (!m_layers.contains(idx)) {
            return false;
        }

        return m_activeLayers.find(idx) != m_activeLayers.end();
    }

    inline void ActivateLayer(size_t idx) {
        m_deferredToggleLayers.push_back({idx, DeferType::Activate});
    }

    inline void DeactivateLayer(size_t idx) {
        m_deferredToggleLayers.push_back({idx, DeferType::Deactivate});
    }

    inline void ToggleLayer(size_t idx) {
        m_deferredToggleLayers.push_back({idx, DeferType::ToggleActive});
    }

    inline void AddUIElement(std::shared_ptr<UI::Element> element,
                             size_t layerIdx) {
        if (m_layers.find(layerIdx) != m_layers.end()) {
            m_layers[layerIdx]->AddDisplayable(std::move(element));
        }
    }

    std::shared_ptr<Scene> SetAndGetScene(const std::shared_ptr<Scene> &scene);

    inline std::shared_ptr<Scene> GetScene() { return m_scene; }

    inline void AddGameObject(std::shared_ptr<GameObject> object) {
        if (hasScene()) {
            m_scene->AddDisplayable(std::move(object));
        }
    }

    inline void ToggleDebugRendering() { m_context.debug = !m_context.debug; }

    // TODO: This is broken.
    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeUIElement(size_t layerIdx, _Args &&..._args) {
        auto object = std::make_shared<T>(_args...);
        AddUIElement(object, layerIdx);
        return object;
    }

    // TODO: This is broken.
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

    void HandleDeferredToggleLayers();

    bool m_running;

    std::string m_gameName;

    inline bool hasScene() { return m_scene != nullptr; }

    inline bool hasLayers() { return !m_layers.empty(); }
    inline bool hasActiveLayers() { return !m_activeLayers.empty(); }

    std::shared_ptr<Scene> m_scene;
    std::map<size_t, std::shared_ptr<IDisplayLayer>> m_layers;
    std::unordered_set<size_t> m_activeLayers;
    std::vector<std::pair<size_t, DeferType>> m_deferredToggleLayers;

    std::shared_ptr<renderer::Renderer> m_renderer;
    EngineContext m_context;
};

} // namespace admirals