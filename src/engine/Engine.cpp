
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

Engine::~Engine(){};

void Engine::AddUIElement(std::shared_ptr<UI::Element> element) {
    m_displayLayout->AddElement(element);
}

void Engine::AddGameObject(std::shared_ptr<scene::GameObject> object) {
    m_scene->AddObject(object);
}

bool Engine::CheckQuit() {
    bool quit = false;
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }

        m_displayLayout->HandleEvent(e);
    }

    SDL_PumpEvents();
    return quit;
}

void Engine::StartGameLoop() {
    std::vector<std::shared_ptr<renderer::IDrawable>> layers;
    layers.emplace_back(m_scene);
    layers.emplace_back(m_displayLayout);

    // Start render loop
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        quit = CheckQuit();
        m_renderer->Render(layers);
    }
}