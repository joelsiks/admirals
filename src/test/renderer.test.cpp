#include <cmath>
#include <memory>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "Engine.hpp"
#include "GameObject.hpp"
#include "UI/Button.hpp"
#include "UI/TextElement.hpp"

using namespace admirals;

const int WINDOW_WIDTH = 550;
const int WINDOW_HEIGHT = 550;

using namespace admirals;

class TextureObject : public scene::GameObject {
public:
    TextureObject(const std::string &name, const Vector3 &pos,
                  const char *texturePath, bool keepAspectRatio = true)
        : scene::GameObject(name, pos), m_keepAspectRatio(keepAspectRatio),
          m_texture(Texture::loadFromPath(texturePath)) {}

    void onStart() {}

    void onUpdate() {}

    void render(const renderer::RendererContext &r) {
        float x = r.windowWidth / static_cast<float>(m_texture.width());
        float y = r.windowHeight / static_cast<float>(m_texture.height());
        if (m_keepAspectRatio) {
            x = std::min(x, y);
            y = x;
        }

        renderer::Renderer::drawTexture(m_texture, position(), Vector2(x, y));
    }

private:
    Texture m_texture;
    bool m_keepAspectRatio;
};

void OnButtonClick(UI::Button *button, const SDL_Event &event) {
    if (event.type == SDL_MOUSEBUTTONUP) {
        button->SetBackgroundColor(Color::BLACK);
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        Color grey = Color::fromRGBA(50, 50, 50, 255);
        button->SetBackgroundColor(grey);
    }
}

int main(int argc, char *argv[]) {

    admirals::Engine engine("Renderer Test", WINDOW_WIDTH, WINDOW_HEIGHT, true);

    // Create texture object.
    TextureObject textureObject =
        TextureObject("image", Vector3(0, 0, 0), "assets/admirals.png");
    engine.AddGameObject(scene::GameObject::createFromDerived(textureObject));

    Vector2 elementSize = Vector2(150, 40);
    admirals::UI::Button testBtn("Test Button1", 0, "Click Me!", elementSize,
                                 Color::BLACK, Color::WHITE, OnButtonClick);
    engine.AddUIElement(UI::Element::createFromDerived(testBtn));

    admirals::UI::TextElement testText("Text Element1", 0,
                                       "This is a text element.", elementSize,
                                       Color::BLACK);
    testText.SetDisplayPosition(admirals::UI::DisplayPosition::LowerLeft);
    engine.AddUIElement(std::make_shared<admirals::UI::TextElement>(testText));

    engine.StartGameLoop();

    return EXIT_SUCCESS;
}