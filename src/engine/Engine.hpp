#pragma once

#include <cstdint>
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

enum EngineDebugMode : uint32_t {
    // First bit denotes if we're in debug mode or not.
    DebugDisabled = 0b0,
    DebugEnabled = 0b1,
    DebugToggleHotkey = 0b10,

    // Other bits denote special features.
    DebugRendering = 0b100,
    DebugQuadTree = 0b1000,
    DebugPathfinding = 0b10000,

    DebugAll = 0xFFFFFFFF,
};

class Engine {
public:
    events::EventSystem<events::MouseClickEventArgs> onMouseClick;
    events::EventSystem<events::KeyPressEventArgs> onKeyPress;
    events::EventSystem<events::MouseMotionEventArgs> onMouseMove;

    Engine(const std::string &gameName, int windowWidth, int windowHeight,
           uint32_t debugMode = EngineDebugMode::DebugDisabled);

    inline const EngineContext &GetContext() const { return m_context; }

    inline bool AddLayer(size_t idx, std::shared_ptr<IDisplayLayer> layer,
                         bool active = true) {
        const bool wasInserted = m_layers.emplace(idx, std::move(layer)).second;

        if (wasInserted && active) {
            m_deferredLayerActions.emplace_back(DeferType::Add, idx);
        }

        return wasInserted;
    }

    inline void DeleteLayer(size_t idx) {
        m_deferredLayerActions.emplace_back(DeferType::Delete, idx);
    }

    inline std::shared_ptr<IDisplayLayer> GetLayer(size_t idx) {
        return m_layers[idx];
    }

    inline std::unordered_set<size_t> GetActiveLayersIndices() {
        return m_activeLayers;
    }

    inline bool LayerIsActive(size_t idx) {
        if (!m_layers.contains(idx)) {
            return false;
        }

        return m_activeLayers.find(idx) != m_activeLayers.end();
    }

    inline void ActivateLayer(size_t idx) {
        m_deferredLayerActions.emplace_back(DeferType::Activate, idx);
    }

    inline void DeactivateLayer(size_t idx) {
        m_deferredLayerActions.emplace_back(DeferType::Deactivate, idx);
    }

    inline void ToggleLayer(size_t idx) {
        m_deferredLayerActions.emplace_back(DeferType::ToggleActive, idx);
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

    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeUIElement(size_t layerIdx, _Args &&..._args) {
        auto object = std::make_shared<T>(_args...);
        AddUIElement(object, layerIdx);
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

    inline Rect GetDisplayPort() const { return m_context.displayPort; }
    inline void SetDisplayPort(const Rect &displayPort) {
        m_context.displayPort = displayPort;
    }

    inline void ToggleDebug() {
        m_context.debug ^= EngineDebugMode::DebugEnabled;
    }

    inline void ToggleDebugMode(EngineDebugMode mode) {
        m_context.debug ^= static_cast<uint32_t>(mode);
    }

    static bool HasDebugMask(uint32_t currentDebugMode, uint32_t mask) {
        return (currentDebugMode & mask) == mask;
    }

private:
    bool PollAndHandleEvent();

    void HandleDeferredLayerActions();

    inline bool hasScene() { return m_scene != nullptr; }
    inline bool hasLayers() { return !m_layers.empty(); }
    inline bool hasActiveLayers() { return !m_activeLayers.empty(); }

    bool m_running;
    std::string m_gameName;

    std::shared_ptr<Scene> m_scene;
    std::map<size_t, std::shared_ptr<IDisplayLayer>> m_layers;
    std::unordered_set<size_t> m_activeLayers;
    std::vector<std::pair<DeferType, size_t>> m_deferredLayerActions;

    std::shared_ptr<renderer::Renderer> m_renderer;
    EngineContext m_context;
};

} // namespace admirals