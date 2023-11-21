
#include "Engine.hpp"

using namespace admirals;

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
    bool quit = !m_running;
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }

        // Toggle escape menu.
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            m_showingMenus = !m_showingMenus;
        }

        if (m_showingMenus) {
            m_escapeMenu->HandleEvent(e);
        } else {
            m_displayLayout->HandleEvent(e);
        }
    }

    SDL_PumpEvents();
    return quit;
}

void Engine::StartGameLoop() {
    m_running = true;
    m_scene->OnStart();

    std::vector<std::shared_ptr<renderer::IDrawable>> menuLayers;
    menuLayers.emplace_back(m_escapeMenu);

    std::vector<std::shared_ptr<renderer::IDrawable>> layers;
    layers.emplace_back(m_scene);
    layers.emplace_back(m_displayLayout);

    // Start render loop
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        quit = PollAndHandleEvent();
        m_scene->OnUpdate();
        m_renderer->Render(m_showingMenus ? menuLayers : layers);
    }
}