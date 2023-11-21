#include <cmath>
#include <memory>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "Engine.hpp"
#include "EventSystem.hpp"
#include "GameObject.hpp"
#include "UI/Button.hpp"
#include "UI/Menu.hpp"
#include "UI/MenuOption.hpp"
#include "UI/TextElement.hpp"

using namespace admirals;

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

    void ButtonClickHandler(void *sender, UI::ButtonClickEventArgs &e) {
        if (e.m_data.type != SDL_MOUSEBUTTONDOWN)
            return;
        auto button = static_cast<UI::Button *>(sender);
        if (button->name() == "btn1") {
            SetPosition(Vector2(GetPosition().x() - 100, 0));
        } else if (button->name() == "btn2") {
            SetPosition(Vector2(GetPosition().x() + 100, 0));
        }
    }

    void Render(const renderer::RendererContext &r) const override {
        float x = r.windowWidth / static_cast<float>(m_texture.Width());
        float y = r.windowHeight / static_cast<float>(m_texture.Height());
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

void CreateEscapeMenuOptions(std::shared_ptr<UI::Menu> escapeMenu,
                             Engine &engine, bool initialDebugValue) {
    UI::ClickOption exitOption("exitOption", 1.0, "Exit...");
    exitOption.onClick.Subscribe(
        [&engine](void *, UI::OptionClickEventArgs &args) {
            if (args.m_data.type != SDL_MOUSEBUTTONUP)
                return;

            engine.StopGameLoop();
        });
    escapeMenu->AddMenuOption(UI::MenuOption::CreateFromDerived(exitOption));

    UI::ToggleOption toggleDebugOption("toggleDebugRenderingOption", 1.0,
                                       "Debug Rendering", initialDebugValue);
    toggleDebugOption.onClick.Subscribe(
        [&engine](void *, UI::OptionClickEventArgs &args) {
            if (args.m_data.type != SDL_MOUSEBUTTONUP)
                return;

            engine.ToggleDebugRendering();
        });
    escapeMenu->AddMenuOption(
        UI::MenuOption::CreateFromDerived(toggleDebugOption));

    const std::vector<Color> cycleColors = {Color::RED, Color::BLACK,
                                            Color::WHITE};
    UI::CycleOption cycleColorOption("cycleMenuColorOption", 1.0, "Menu Color",
                                     {"Red", "Black", "White"}, 1);
    cycleColorOption.onClick.Subscribe(
        [&escapeMenu, cycleColors](void *sender,
                                   UI::OptionClickEventArgs &args) {
            if (args.m_data.type != SDL_MOUSEBUTTONUP)
                return;

            auto *cycleOption = static_cast<UI::CycleOption *>(sender);
            const size_t idx =
                (cycleOption->CurrentIndex() + 1) % cycleColors.size();
            escapeMenu->SetTextColor(cycleColors[idx]);
        });
    escapeMenu->AddMenuOption(
        UI::MenuOption::CreateFromDerived(cycleColorOption));
}

void OnButtonClick(void *object, UI::ButtonClickEventArgs &event) {
    auto button = static_cast<UI::Button *>(object);
    if (event.m_data.type == SDL_MOUSEBUTTONUP) {
        button->SetBackgroundColor(Color::BLACK);
    } else if (event.m_data.type == SDL_MOUSEBUTTONDOWN) {
        Color grey = Color::FromRGBA(50, 50, 50, 255);
        button->SetBackgroundColor(grey);
    }
}

void CreateUIElements(Engine &engine,
                      std::shared_ptr<TextureObject> textureObj) {
    const Vector2 elementSize = Vector2(300, 40);

    auto btn1 = engine.MakeUIElement<UI::Button>(
        "btn1", 0, "Move Image Left", elementSize, Color::BLACK, Color::WHITE);
    btn1->onClick.Subscribe(OnButtonClick);
    btn1->onClick.Subscribe(
        BIND_EVENT_HANDLER_FROM(TextureObject::ButtonClickHandler, textureObj));

    auto btn2 = engine.MakeUIElement<UI::Button>(
        "btn2", 0, "Move Image Right", elementSize, Color::BLACK, Color::WHITE);
    btn2->onClick.Subscribe(OnButtonClick);
    btn2->onClick.Subscribe(
        BIND_EVENT_HANDLER_FROM(TextureObject::ButtonClickHandler, textureObj));

    auto testText1 = engine.MakeUIElement<UI::TextElement>(
        "text1", 0, "Le", Vector2(32, 40), Color::BLACK);
    auto testText2 = engine.MakeUIElement<UI::TextElement>(
        "text2", 0, "Right aligned", Vector2(220, 40), Color::BLACK);
    testText1->SetDisplayPosition(UI::DisplayPosition::LowerLeft);
    testText2->SetDisplayPosition(UI::DisplayPosition::LowerRight);
}

int main(int argc, char *argv[]) {

    const bool debug = true;
    Engine engine("Renderer Test", WINDOW_WIDTH, WINDOW_HEIGHT, debug);

    auto escapeMenu =
        engine.MakeAndSetEscapeMenu<UI::Menu>("Pause Menu", Color::BLACK);
    CreateEscapeMenuOptions(escapeMenu, engine, debug);

    auto texture = engine.MakeGameObject<TextureObject>(
        "image", Vector3(0, 0, 0), "assets/admirals.png");
    CreateUIElements(engine, texture);

    engine.StartGameLoop();

    return EXIT_SUCCESS;
}