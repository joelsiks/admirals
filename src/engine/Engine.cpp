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
void Engine::AddMenu(const std::string &menuName,
                     std::shared_ptr<UI::Menu> menu,
                     SDL_Keycode toggleKeyCode) {
    m_menus[menuName] = menu;

    if (toggleKeyCode != SDLK_IGNORE) {
        onKeyPress.Subscribe([this, menu, toggleKeyCode](
                                 void *, events::KeyPressEventArgs &args) {
            if (args.key == toggleKeyCode && args.isKeyUp) {
                if (this->m_activeMenu == menu) {
                    this->m_activeMenu = nullptr;
                } else {
                    this->m_activeMenu = menu;
                }
            }
        });
    }
}

void Engine::ActivateMenu(const std::string &menuName) {
    if (m_menus.contains(menuName)) {
        m_activeMenu = m_menus[menuName];
    }
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

        if (hasActiveMenu()) {
            m_activeMenu->HandleEvent(e);
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

    std::vector<std::shared_ptr<renderer::IDrawable>> menuLayer;

    std::vector<std::shared_ptr<renderer::IDrawable>> layers;
    layers.emplace_back(m_scene);
    layers.emplace_back(m_displayLayout);

    // Start render loop
    bool quit = false;
    while (!quit) {
        if (hasActiveMenu()) {
            menuLayer = {m_activeMenu};
        }

        quit = PollAndHandleEvent();
        m_scene->OnUpdate();
        m_renderer->Render(hasActiveMenu() ? menuLayer : layers);
    }
}