#include "Engine.hpp"

using namespace admirals;
using namespace admirals::events;

Engine::Engine(const std::string &gameName, int windowWidth, int windowHeight,
               bool debug) {
    m_renderer = std::make_shared<renderer::Renderer>(gameName, windowWidth,
                                                      windowHeight, debug);
    // Initialize the renderer right after creating it. Necessary in cases where
    // DisplayLayout requires vk2d to be initialized.
    m_renderer->Init(debug);

    m_displayLayout = std::make_shared<UI::DisplayLayout>();
    m_scene = std::make_shared<scene::Scene>();
}

bool Engine::PollAndHandleEvent() {
    bool quit = false;
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
        default:
            break;
        }

        m_displayLayout->HandleEvent(e);
    }

    SDL_PumpEvents();
    return quit;
}

void Engine::StartGameLoop() {
    m_scene->OnStart();

    std::vector<std::shared_ptr<renderer::IDrawable>> layers;
    layers.emplace_back(m_scene);
    layers.emplace_back(m_displayLayout);

    // Start render loop
    bool quit = false;
    while (!quit) {
        quit = PollAndHandleEvent();
        m_scene->OnUpdate();
        m_renderer->Render(layers);
    }
}