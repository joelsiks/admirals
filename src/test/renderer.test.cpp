#include <cmath>
#include <memory>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "Engine.hpp"
#include "GameObject.hpp"
#include "UI/Button.hpp"
#include "UI/TextElement.hpp"
#include "UI/menu/Menu.hpp"
#include "UI/menu/MenuOption.hpp"
#include "events/EventSystem.hpp"

using namespace admirals;
using namespace admirals::UI;

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

class TextureObject : public scene::GameObject {
public:
    TextureObject(const std::string &name, const Vector3 &pos,
                  const char *texturePath, bool keepAspectRatio = true)
        : scene::GameObject(name, pos), m_keepAspectRatio(keepAspectRatio),
          m_texture(Texture::LoadFromPath(texturePath)) {}

    void OnStart() override {}

    void OnUpdate() override {}

    void ButtonClickHandler(void *sender, events::ButtonClickEventArgs &e) {
        if (e.m_data.type != SDL_MOUSEBUTTONDOWN)
            return;
        auto *button = static_cast<Button *>(sender);
        if (button->name() == "btn1") {
            SetPosition(Vector2(GetPosition().x() - 100, 0));
        } else if (button->name() == "btn2") {
            SetPosition(Vector2(GetPosition().x() + 100, 0));
        }
    }

    void Render(const renderer::RendererContext &r) const override {
        float x = static_cast<float>(r.windowWidth) /
                  static_cast<float>(m_texture.Width());
        float y = static_cast<float>(r.windowHeight) /
                  static_cast<float>(m_texture.Height());
        if (m_keepAspectRatio) {
            x = std::min(x, y);
            y = x;
        }

        renderer::Renderer::DrawTexture(m_texture, GetPosition(),
                                        Vector2(x, y));
    }

private:
    Texture m_texture;
    bool m_keepAspectRatio;
};

void CreateEscapeMenuOptions(std::shared_ptr<menu::Menu> escapeMenu,
                             Engine &engine, bool initialDebugValue) {
    menu::ClickOption exitOption("exitOption", 1.0, "Exit...");
    exitOption.onClick.Subscribe(
        [&engine](void *, menu::OptionClickEventArgs &args) {
            if (args.m_data.type != SDL_MOUSEBUTTONUP)
                return;

            engine.StopGameLoop();
        });
    escapeMenu->AddMenuOption(menu::MenuOption::CreateFromDerived(exitOption));

    menu::ToggleOption toggleDebugOption("toggleDebugRenderingOption", 1.0,
                                         "Debug Rendering", initialDebugValue);
    toggleDebugOption.onClick.Subscribe(
        [&engine](void *, menu::OptionClickEventArgs &args) {
            if (args.m_data.type != SDL_MOUSEBUTTONUP)
                return;

            engine.ToggleDebugRendering();
        });
    escapeMenu->AddMenuOption(
        menu::MenuOption::CreateFromDerived(toggleDebugOption));

    const std::vector<Color> cycleColors = {Color::RED, Color::BLACK,
                                            Color::WHITE};
    menu::CycleOption cycleColorOption("cycleMenuColorOption", 1.0,
                                       "Menu Color", {"Red", "Black", "White"},
                                       1);
    cycleColorOption.onClick.Subscribe([&escapeMenu, cycleColors](
                                           void *sender,
                                           menu::OptionClickEventArgs &args) {
        if (args.m_data.type != SDL_MOUSEBUTTONUP)
            return;

        auto *cycleOption = static_cast<menu::CycleOption *>(sender);
        cycleOption->Cycle();
        const size_t idx = (cycleOption->CurrentIndex()) % cycleColors.size();
        escapeMenu->SetTextColor(cycleColors[idx]);
    });
    escapeMenu->AddMenuOption(
        menu::MenuOption::CreateFromDerived(cycleColorOption));
}

void OnButtonClick(void *object, events::ButtonClickEventArgs &event) {
    auto button = static_cast<Button *>(object);
    if (event.m_data.type == SDL_MOUSEBUTTONUP) {
        button->SetBackgroundColor(Color::BLACK);
    } else if (event.m_data.type == SDL_MOUSEBUTTONDOWN) {
        const Color grey = Color::FromRGBA(50, 50, 50, 255);
        button->SetBackgroundColor(grey);
    }
}

void CreateUIElements(Engine &engine,
                      std::shared_ptr<TextureObject> textureObj) {
    const Vector2 elementSize = Vector2(300, 40);

    auto btn1 = engine.MakeUIElement<Button>(
        "btn1", 0, "Move Image Left", elementSize, Color::BLACK, Color::WHITE);
    btn1->onClick.Subscribe(OnButtonClick);
    btn1->onClick.Subscribe(
        BIND_EVENT_HANDLER_FROM(TextureObject::ButtonClickHandler, textureObj));

    auto btn2 = engine.MakeUIElement<Button>(
        "btn2", 0, "Move Image Right", elementSize, Color::BLACK, Color::WHITE);
    btn2->onClick.Subscribe(OnButtonClick);
    btn2->onClick.Subscribe(
        BIND_EVENT_HANDLER_FROM(TextureObject::ButtonClickHandler, textureObj));

    auto testText1 = engine.MakeUIElement<TextElement>(
        "text1", 0, "Le", Vector2(32, 40), Color::BLACK);
    auto testText2 = engine.MakeUIElement<TextElement>(
        "text2", 0, "Right aligned", Vector2(220, 40), Color::BLACK);
    testText1->SetDisplayPosition(DisplayPosition::LowerLeft);
    testText2->SetDisplayPosition(DisplayPosition::LowerRight);
}

int main(int, char **) {

    const bool debug = true;
    Engine engine("Renderer Test", WINDOW_WIDTH, WINDOW_HEIGHT, debug);

    auto escapeMenu = std::make_shared<menu::Menu>(
        "Pause Menu", Color::BLACK, Color::FromRGBA(50, 50, 50, 100));

    std::shared_ptr<DisplayLayout> displayLayoutStore;
    std::shared_ptr<scene::Scene> sceneStore;
    engine.onKeyPress.Subscribe([&escapeMenu, &displayLayoutStore,
                                 &sceneStore](void *sender,
                                              events::KeyPressEventArgs &args) {
        auto *engine = static_cast<Engine *>(sender);

        if (args.key == SDLK_ESCAPE && args.isKeyUp) {
            if (engine->GetDisplayLayout() == escapeMenu) {
                engine->SetAndGetDisplayLayout(displayLayoutStore);
                // engine->SetAndGetScene(sceneStore);
            } else {
                displayLayoutStore = engine->SetAndGetDisplayLayout(escapeMenu);
                // sceneStore = engine->SetAndGetScene(nullptr);
            }
        }
    });

    CreateEscapeMenuOptions(escapeMenu, engine, debug);

    auto texture = engine.MakeGameObject<TextureObject>(
        "image", Vector3(0, 0, 0), "assets/admirals.png");
    CreateUIElements(engine, texture);

    engine.StartGameLoop();

    return EXIT_SUCCESS;
}