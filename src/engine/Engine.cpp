#include "Engine.hpp"

using namespace admirals;
using namespace admirals::events;

Engine::Engine(const std::string &gameName, int windowWidth, int windowHeight,
               bool debug)
    : m_context() {
    m_context.windowWidth = windowWidth;
    m_context.windowHeight = windowHeight;
    m_context.renderDebugOutlines = debug;
    m_renderer = std::make_shared<renderer::Renderer>(gameName, windowWidth,
                                                      windowHeight, debug);
    // Initialize the renderer right after creating it. Necessary in cases where
    // DisplayLayout requires vk2d to be initialized.
    m_renderer->Init(windowWidth, windowHeight, debug);

    m_displayLayout = std::make_shared<UI::DisplayLayout>();

    m_scene = std::make_shared<scene::Scene>();
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
            auto args = MouseCLickEventArgs(e.button);
            onMouseClick.Invoke(this, args);
        } break;
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            auto args = KeyPressEventArgs(e.key);
            onKeyPress.Invoke(this, args);
        } break;
        default:
            break;
        }

        if (hasDisplayLayout()) {
            m_displayLayout->HandleEvent(e);
        }
    }

    SDL_PumpEvents();
    return quit;
}

void Engine::StartGameLoop() {
    m_running = true;

    if (hasScene()) {
        m_scene->OnStart(GetContext());
    }

    std::vector<std::shared_ptr<renderer::IDrawable>> layers(2);

    // Start render loop
    bool quit = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime =
        std::chrono::high_resolution_clock::now();
    while (!quit) {
        layers[0] = m_scene;
        layers[1] = m_displayLayout;

        std::chrono::time_point<std::chrono::high_resolution_clock> now =
            std::chrono::high_resolution_clock::now();
        m_context.deltaTime =
            std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        quit = PollAndHandleEvent();

        if (hasScene()) {
            m_scene->OnUpdate(GetContext());
        }

        int windowWidth, windowHeight;
        m_renderer->GetWindowSize(&windowWidth, &windowHeight);
        m_context.windowWidth = windowWidth;
        m_context.windowHeight = windowHeight;

        m_renderer->Render(GetContext(), layers);
    }
}