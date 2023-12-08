#include <chrono>

#include "Engine.hpp"

using namespace admirals;
using namespace admirals::events;

static const char *FONT_PATH = "assets/font.png";
static const float FONT_CHAR_WIDTH = 16.0;
static const float FONT_CHAR_HEIGHT = 36.0;

Engine::Engine(const std::string &gameName, int windowWidth, int windowHeight,
               bool debug)
    : m_context() {
    m_context.windowSize = Vector2(static_cast<float>(windowWidth),
                                   static_cast<float>(windowHeight));
    m_context.debug = debug;
    m_renderer = std::make_shared<renderer::Renderer>(gameName, windowWidth,
                                                      windowHeight);
    // Initialize the renderer right after creating it. Necessary in cases where
    // a layer requires vk2d to be initialized.
    m_renderer->Init(m_context);

    m_context.fontTexture = new Texture(vk2dTextureLoad(FONT_PATH));
    m_context.fontWidth = FONT_CHAR_WIDTH;
    m_context.fontHeight = FONT_CHAR_HEIGHT;

    m_scene = std::make_shared<Scene>();
}

std::shared_ptr<Scene>
Engine::SetAndGetScene(const std::shared_ptr<Scene> &scene) {
    auto currentScene = m_scene;
    currentScene->OnHidden();

    m_scene = scene;

    // Initialize the scene immediately after swapping if it isn't already.
    if (hasScene() && !m_scene->IsInitialized()) {
        m_scene->OnStart(m_context);
    }

    m_scene->OnShown();

    SDL_MouseMotionEvent motionEvent;
    motionEvent.state = SDL_GetMouseState(&motionEvent.x, &motionEvent.y);
    events::MouseMotionEventArgs args(motionEvent);
    m_scene->OnMouseMove(args);

    return currentScene;
}

bool Engine::PollAndHandleEvent() {
    bool quit = !m_running;
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            auto args = MouseClickEventArgs(e.button);

            for (const auto &activeLayerIdx : m_activeLayers) {
                m_layers[activeLayerIdx]->OnClick(args);
            }

            if (hasScene() && !args.handled) {
                m_scene->OnClick(args);
            }

            onMouseClick.Invoke(this, args);
        } break;
        case SDL_MOUSEMOTION: {
            auto args = MouseMotionEventArgs(e.motion);

            for (const auto &activeLayerIdx : m_activeLayers) {
                m_layers[activeLayerIdx]->OnMouseMove(args);
            }

            if (hasScene() && !args.handled) {
                m_scene->OnMouseMove(args);
            }

            onMouseMove.Invoke(this, args);
        } break;
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            auto args = KeyPressEventArgs(e.key);
            onKeyPress.Invoke(this, args);
        } break;
        default:
            break;
        }
    }

    SDL_PumpEvents();
    return quit;
}

void Engine::HandleDeferredToggleLayers() {
    for (const auto &deferPair : m_deferredToggleLayers) {
        const size_t layerIdx = deferPair.first;

        switch (deferPair.second) {
        case DeferType::Add:
        case DeferType::Activate:
            if (m_layers.contains(layerIdx)) {
                const bool inserted = m_activeLayers.insert(layerIdx).second;
                if (inserted) {
                    m_layers[layerIdx]->OnShown();
                }
            }
            break;
        case DeferType::Delete:
            if (m_layers.erase(layerIdx) == 1) {
                m_activeLayers.erase(layerIdx);
            }
            break;
        case DeferType::Deactivate:
            if (m_layers.contains(layerIdx) &&
                m_activeLayers.erase(layerIdx) == 1) {
                m_layers[layerIdx]->OnHidden();
            }
            break;
        default:
            break;
        }
    }

    m_deferredToggleLayers.clear();
}

void Engine::StartGameLoop() {
    m_running = true;

    if (hasScene()) {
        m_scene->OnStart(GetContext());
    }

    std::vector<std::shared_ptr<IDisplayLayer>> layers(m_activeLayers.size() +
                                                       1);

    // Start render loop
    bool quit = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime =
        std::chrono::high_resolution_clock::now();

    while (!quit) {
        layers[0] = m_scene;

        size_t i = 1;
        for (const auto &layerIdx : m_activeLayers) {
            layers[i] = m_layers[layerIdx];
            i++;
        }

        const auto now = std::chrono::high_resolution_clock::now();
        m_context.deltaTime =
            std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        quit = PollAndHandleEvent();
        m_context.windowSize = m_renderer->GetWindowSize();

        if (hasScene()) {
            m_scene->OnUpdate(GetContext());
            m_scene->RebuildQuadTree(m_context.windowSize);
        }

        for (const auto &activeLayerIdx : m_activeLayers) {
            m_layers[activeLayerIdx]->RebuildQuadTree(m_context.windowSize);
        }

        m_renderer->Render(GetContext(), layers);

        HandleDeferredToggleLayers();
        layers.resize(m_activeLayers.size() + 1);
    }
}