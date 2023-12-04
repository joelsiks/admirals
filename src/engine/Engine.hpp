#pragma once

#include <memory>

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

    inline void AddLayer(size_t idx,
                         std::shared_ptr<IDisplayLayer<UI::Element>> layer) {
        m_layers[idx] = std::move(layer);
        m_activeLayers.insert(idx);
    }

    inline void DeleteLayer(size_t idx) {
        if (m_layers.contains(idx)) {
            m_layers.erase(idx);
        }

        if (m_activeLayers.contains(idx)) {
            m_activeLayers.erase(idx);
        }
    }

    inline std::shared_ptr<IDisplayLayer<UI::Element>> GetLayer(size_t idx) {
        return m_layers[idx];
    }

    inline void ActivateLayer(size_t idx) {
        if (m_layers.contains(idx) && !m_activeLayers.contains(idx)) {
            m_layers[idx]->OnShown();
            m_activeLayers.insert(idx);
        }
    }

    inline void DeactivateLayer(size_t idx) {
        if (m_activeLayers.contains(idx)) {
            m_layers[idx]->OnHidden();
            m_activeLayers.insert(idx);
        }
    }

    inline bool LayerIsActive(size_t idx) {
        return m_layers.contains(idx) && m_activeLayers.contains(idx);
    }

    inline void AddUIElement(std::shared_ptr<UI::Element> element,
                             size_t layerIdx) {
        if (hasLayers() && m_layers.contains(layerIdx)) {
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

    inline bool hasScene() { return m_scene != nullptr; }

    inline bool hasLayers() { return !m_layers.empty(); }
    inline bool hasActiveLayers() { return !m_activeLayers.empty(); }

    // Drawables
    std::shared_ptr<Scene> m_scene;
    std::map<size_t, std::shared_ptr<IDisplayLayer<UI::Element>>> m_layers;
    std::unordered_set<size_t> m_activeLayers;

    std::shared_ptr<renderer::Renderer> m_renderer;
    EngineContext m_context;
};

} // namespace admirals