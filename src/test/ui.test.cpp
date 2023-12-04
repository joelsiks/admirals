#include <cmath>
#include <memory>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "Engine.hpp"
#include "GameObject.hpp"
#include "UI/Button.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/TextElement.hpp"
#include "UI/menu/Menu.hpp"
#include "UI/menu/MenuOption.hpp"
#include "events/EventSystem.hpp"

using namespace admirals;
using namespace admirals::UI;

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

class TextureObject : public GameObject {
public:
    TextureObject(const std::string &name, const char *texturePath,
                  bool keepAspectRatio = true)
        : GameObject(name), m_keepAspectRatio(keepAspectRatio),
          m_texture(Texture::LoadFromPath(texturePath)) {}

    void ButtonClickHandler(void *sender, events::MouseClickEventArgs &args) {
        if (args.pressed) {
            return;
        }

        auto *button = static_cast<Button *>(sender);
        if (button->identifier() == "btn1") {
            SetPosition(Vector2(GetPosition().x() - 100, 0));
        } else if (button->identifier() == "btn2") {
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

std::shared_ptr<menu::Menu> CreateEscapeMenu(Engine &engine,
                                             bool initialDebugValue) {
    auto escapeMenu = std::make_shared<menu::Menu>(
        "Pause Menu", Color::BLACK, Color::FromRGBA(50, 50, 50, 100));

    const auto exitOption =
        std::make_shared<menu::ClickOption>("exitOption", 1.0, "Exit...");
    exitOption->onClick.Subscribe(
        [&engine](void *, events::MouseClickEventArgs &args) {
            if (!args.pressed)
                return;

            engine.StopGameLoop();
        });
    escapeMenu->AddDisplayable(exitOption);

    const auto inputOption = std::make_shared<menu::InputOption>(
        "inputOption", 1.0, engine.onKeyPress);
    inputOption->onInput.Subscribe([](void *, events::TextEventArgs &args) {
        printf("Input was changed: %s\n", args.text.c_str());
    });
    escapeMenu->AddDisplayable(inputOption);

    const auto toggleDebugOption = std::make_shared<menu::ToggleOption>(
        "toggleDebugRenderingOption", 1.0, "Debug Rendering",
        initialDebugValue);
    toggleDebugOption->onClick.Subscribe(
        [&engine](void *, events::MouseClickEventArgs &args) {
            if (args.pressed)
                return;

            engine.ToggleDebugRendering();
        });
    escapeMenu->AddDisplayable(toggleDebugOption);

    const std::vector<Color> cycleColors = {Color::RED, Color::BLACK,
                                            Color::WHITE};
    const std::vector<std::string> cycleColorsNames = {"Red", "Black", "White"};

    auto cycleColorOption = std::make_shared<menu::CycleOption>(
        "cycleMenuColorOption", 1.0, "Menu Color", cycleColorsNames, 1);
    cycleColorOption->onClick.Subscribe([&escapeMenu, cycleColors](
                                            void *sender,
                                            events::MouseClickEventArgs &args) {
        if (args.pressed)
            return;

        auto *cycleOption = static_cast<menu::CycleOption *>(sender);
        cycleOption->Cycle();
        const size_t idx = (cycleOption->CurrentIndex()) % cycleColors.size();
        escapeMenu->SetTextColor(cycleColors[idx]);
    });
    escapeMenu->AddDisplayable(cycleColorOption);

    return escapeMenu;
}

std::shared_ptr<DisplayLayout>
CreateTestUI(std::shared_ptr<TextureObject> textureObj) {
    auto testUI = std::make_shared<DisplayLayout>();

    const Vector2 elementSize = Vector2(300, 40);

    auto btn1 = std::make_shared<Button>(
        "btn1", 0, "Move Image Left", elementSize, Color::BLACK, Color::WHITE);
    btn1->onClick.Subscribe(
        BIND_EVENT_HANDLER_FROM(TextureObject::ButtonClickHandler, textureObj));
    testUI->AddDisplayable(btn1);

    auto btn2 = std::make_shared<Button>(
        "btn2", 0, "Move Image Right", elementSize, Color::BLACK, Color::WHITE);
    btn2->onClick.Subscribe(
        BIND_EVENT_HANDLER_FROM(TextureObject::ButtonClickHandler, textureObj));
    testUI->AddDisplayable(btn2);

    auto testText1 = std::make_shared<TextElement>(
        "text1", 0, "Le", Vector2(32, 40), Color::BLACK);
    testText1->SetDisplayOrientation(DisplayOrientation::LowerLeft);
    testUI->AddDisplayable(testText1);
    auto testText2 = std::make_shared<TextElement>(
        "text2", 0, "Right aligned", Vector2(220, 40), Color::BLACK);
    testText2->SetDisplayOrientation(DisplayOrientation::LowerRight);
    testUI->AddDisplayable(testText2);

    return testUI;
}

int main(int, char **) {

    const bool debug = true;
    Engine engine("UI Test", WINDOW_WIDTH, WINDOW_HEIGHT, debug);

    const size_t escapeMenuIdx = 0;
    const size_t testUIIdx = 1;

    auto escapeMenu = CreateEscapeMenu(engine, debug);
    engine.AddLayer(escapeMenuIdx,
                    std::dynamic_pointer_cast<IDisplayLayer>(escapeMenu),
                    false);

    auto texture =
        engine.MakeGameObject<TextureObject>("image", "assets/admirals.png");
    auto testUI = CreateTestUI(texture);
    engine.AddLayer(testUIIdx,
                    std::dynamic_pointer_cast<IDisplayLayer>(testUI));

    engine.onKeyPress.Subscribe(
        [&engine, escapeMenuIdx, testUIIdx](void *sender,
                                            events::KeyPressEventArgs &args) {
            auto *engine = static_cast<Engine *>(sender);

            if (args.key == SDLK_ESCAPE && args.isKeyUp) {
                engine->ToggleLayer(escapeMenuIdx);
                engine->ToggleLayer(testUIIdx);
            }
        });

    engine.StartGameLoop();

    return EXIT_SUCCESS;
}