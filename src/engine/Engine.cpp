
#include "Engine.hpp"

using namespace admirals;

Engine::Engine(const std::string &gameName, int windowWidth, int windowHeight,
               bool debug) {
    std::shared_ptr<renderer::Renderer> renderer =
        std::make_shared<renderer::Renderer>(gameName, windowWidth,
                                             windowHeight);
    renderer->init(debug);
    m_renderer = renderer;

    std::shared_ptr<UI::DisplayLayout> displayLayout =
        std::make_shared<UI::DisplayLayout>(windowWidth, windowHeight);
    m_displayLayout = displayLayout;

    std::shared_ptr<scene::Scene> scene = std::make_shared<scene::Scene>();
    m_scene = scene;
}

Engine::~Engine(){};

void Engine::AddUIElement(std::shared_ptr<UI::Element> element) {
    m_displayLayout->AddElement(element);
}

void Engine::AddGameObject(std::shared_ptr<scene::GameObject> object) {
    m_scene->addObject(object);
}

bool Engine::CheckQuit() {
    bool quit = false;
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }

        m_displayLayout->handleEvent(e);
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
        m_renderer->render(layers);
    }
}