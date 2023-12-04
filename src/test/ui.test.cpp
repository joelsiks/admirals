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
using namespace admirals::events;

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

class TextureObject : public scene::GameObject {
public:
    TextureObject(const std::string &name, const char *texturePath,
                  bool keepAspectRatio = true)
        : scene::GameObject(name), m_keepAspectRatio(keepAspectRatio),
          m_texture(Texture::LoadFromPath(texturePath)) {}

    void ButtonClickHandler(void *sender, MouseClickEventArgs &args) {
        if (args.pressed) {
            return;
        }

        auto *button = static_cast<Button *>(sender);
        if (button->name() == "btn1") {
            SetPosition(Vector2(GetPosition().x() - 100, 0));
        } else if (button->name() == "btn2") {
            SetPosition(Vector2(GetPosition().x() + 100, 0));
        }
    }

    void Render(const EngineContext &c) const override {
        Vector2 scale = c.windowSize / m_texture.Size();
        if (m_keepAspectRatio) {
            scale[0] = scale[1] = std::min(scale.x(), scale.y());
        }

        renderer::Renderer::DrawTexture(m_texture, GetPosition(), scale);
    }

private:
    Texture m_texture;
    bool m_keepAspectRatio;
};

void CreateEscapeMenuOptions(std::shared_ptr<menu::Menu> escapeMenu,
                             Engine &engine, bool initialDebugValue) {
    menu::ClickOption exitOption("exitOption", 1.0, "Exit...");
    exitOption.onClick.Subscribe([&engine](void *, MouseClickEventArgs &args) {
        if (!args.pressed)
            return;

        engine.StopGameLoop();
    });
    escapeMenu->AddMenuOption(menu::MenuOption::CreateFromDerived(exitOption));

    const std::shared_ptr<menu::InputOption> inputOption =
        std::make_shared<menu::InputOption>("inputOption", 1.0,
                                            engine.onKeyPress);
    inputOption->onInput.Subscribe([](void *, TextEventArgs &args) {
        printf("Input was changed: %s\n", args.text.c_str());
    });
    escapeMenu->AddMenuOption(inputOption);

    menu::ToggleOption toggleDebugOption("toggleDebugRenderingOption", 1.0,
                                         "Debug Rendering", initialDebugValue);
    toggleDebugOption.onClick.Subscribe(
        [&engine](void *, MouseClickEventArgs &args) {
            if (args.pressed)
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
    cycleColorOption.onClick.Subscribe(
        [&escapeMenu, cycleColors](void *sender, MouseClickEventArgs &args) {
            if (args.pressed)
                return;

            auto *cycleOption = static_cast<menu::CycleOption *>(sender);
            cycleOption->Cycle();
            const size_t idx =
                (cycleOption->CurrentIndex()) % cycleColors.size();
            escapeMenu->SetTextColor(cycleColors[idx]);
        });
    escapeMenu->AddMenuOption(
        menu::MenuOption::CreateFromDerived(cycleColorOption));
}

void CreateUIElements(Engine &engine,
                      std::shared_ptr<TextureObject> textureObj) {
    const Vector2 elementSize = Vector2(300, 40);

    auto btn1 = engine.MakeUIElement<Button>(
        "btn1", 0, "Move Image Left", elementSize, Color::BLACK, Color::WHITE);
    btn1->onClick.Subscribe(
        BIND_EVENT_HANDLER_FROM(TextureObject::ButtonClickHandler, textureObj));

    auto btn2 = engine.MakeUIElement<Button>(
        "btn2", 0, "Move Image Right", elementSize, Color::BLACK, Color::WHITE);
    btn2->onClick.Subscribe(
        BIND_EVENT_HANDLER_FROM(TextureObject::ButtonClickHandler, textureObj));

    auto testText1 = engine.MakeUIElement<TextElement>(
        "text1", 0, "Le", Vector2(32, 40), Color::BLACK);
    auto testText2 = engine.MakeUIElement<TextElement>(
        "text2", 0, "Right aligned", Vector2(220, 40), Color::BLACK);
    testText1->SetDisplayOrientation(DisplayOrientation::LowerLeft);
    testText2->SetDisplayOrientation(DisplayOrientation::LowerRight);
}

int main(int, char **) {

    const bool debug = true;
    Engine engine("UI Test", WINDOW_WIDTH, WINDOW_HEIGHT, debug);

    auto escapeMenu = std::make_shared<menu::Menu>(
        "Pause Menu", Color::BLACK, Color::FromRGBA(50, 50, 50, 100));

    std::shared_ptr<DisplayLayout> displayLayoutStore;
    std::shared_ptr<scene::Scene> sceneStore;
    engine.onKeyPress.Subscribe([&escapeMenu, &displayLayoutStore, &sceneStore](
                                    void *sender, KeyPressEventArgs &args) {
        auto *engine = static_cast<Engine *>(sender);

        if (args.key == SDLK_ESCAPE && args.isKeyUp) {
            if (engine->GetDisplayLayout() == escapeMenu) {
                engine->SetAndGetDisplayLayout(displayLayoutStore);
            } else {
                displayLayoutStore = engine->SetAndGetDisplayLayout(escapeMenu);
            }
        }
    });

    CreateEscapeMenuOptions(escapeMenu, engine, debug);

    auto texture =
        engine.MakeGameObject<TextureObject>("image", "assets/admirals.png");
    CreateUIElements(engine, texture);

    engine.StartGameLoop();

    return EXIT_SUCCESS;
}