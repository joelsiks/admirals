#include <cmath>
#include <memory>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "Engine.hpp"
#include "GameObject.hpp"
#include "UI/Button.hpp"
#include "UI/TextElement.hpp"
#include "events/EventSystem.hpp"

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

    void ButtonClickHandler(void *sender, events::ButtonClickEventArgs &e) {
        if (e.m_data.type != SDL_MOUSEBUTTONDOWN)
            return;
        auto *button = static_cast<UI::Button *>(sender);
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

void OnButtonClick(void *object, events::ButtonClickEventArgs &event) {
    auto *button = static_cast<UI::Button *>(object);
    if (event.m_data.type == SDL_MOUSEBUTTONUP) {
        button->SetBackgroundColor(Color::BLACK);
    } else if (event.m_data.type == SDL_MOUSEBUTTONDOWN) {
        const Color grey = Color::FromRGBA(50, 50, 50, 255);
        button->SetBackgroundColor(grey);
    }
}

int main(int argc, char *argv[]) {

    Engine engine("Renderer Test", WINDOW_WIDTH, WINDOW_HEIGHT, true);

    // Create texture object.
    auto texture = engine.MakeGameObject<TextureObject>(
        "image", Vector3(0, 0, 0), "assets/admirals.png");

    const Vector2 elementSize = Vector2(300, 40);

    auto btn1 = engine.MakeUIElement<UI::Button>(
        "btn1", 0, "Move Image Left", elementSize, Color::BLACK, Color::WHITE);
    btn1->onClick.Subscribe(OnButtonClick);
    btn1->onClick.Subscribe(
        BIND_EVENT_HANDLER_FROM(TextureObject::ButtonClickHandler, texture));

    auto btn2 = engine.MakeUIElement<UI::Button>(
        "btn2", 0, "Move Image Right", elementSize, Color::BLACK, Color::WHITE);
    btn2->onClick.Subscribe(OnButtonClick);
    btn2->onClick.Subscribe(
        BIND_EVENT_HANDLER_FROM(TextureObject::ButtonClickHandler, texture));

    auto testText1 = engine.MakeUIElement<UI::TextElement>(
        "text1", 0, "Left aligned", Vector2(220, 40), Color::BLACK);
    auto testText2 = engine.MakeUIElement<UI::TextElement>(
        "text2", 0, "Right aligned", Vector2(220, 40), Color::BLACK);
    testText1->SetDisplayPosition(UI::DisplayPosition::LowerLeft);
    testText2->SetDisplayPosition(UI::DisplayPosition::LowerRight);

    engine.StartGameLoop();

    return EXIT_SUCCESS;
}