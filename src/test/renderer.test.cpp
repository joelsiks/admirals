#include <memory>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <SDL_vulkan.h>
#include <VK2D/VK2D.h>
#include <VK2D/Validation.h>

#include "Engine.hpp"
#include "GameObject.hpp"
#include "UI/Button.hpp"
#include "UI/TextElement.hpp"

const int WINDOW_WIDTH = 550;
const int WINDOW_HEIGHT = 550;

using namespace admirals;

class TextureObject : public scene::GameObject {
public:
    TextureObject(const Vector3 &pos, const char *texturePath, float width,
                  float height)
        : scene::GameObject(pos), m_width(width), m_height(height) {
        m_texture = vk2dTextureLoad(texturePath);
    }

    ~TextureObject() { vk2dTextureFree(m_texture); }

    void onStart() {}

    void onUpdate() {}

    void render() const {
        vk2dRendererDrawTexture(m_texture, 0, 0, 0.4, 0.4, 0, 0, 0, 0, 0,
                                m_width, m_height);
    }

private:
    VK2DTexture m_texture;
    float m_width, m_height;
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
    Vector3 texturePosition = Vector3(0, 0, 0);
    TextureObject textureObject(texturePosition, "assets/admirals.png", 1024.0,
                                1024.0);
    engine.AddGameObject(scene::GameObject::createFromDerived(textureObject));

    Vector2 elementSize = Vector2(150, 40);
    admirals::UI::Button testBtn("Test Button1", "Click Me!", elementSize,
                                 Color::BLACK, Color::WHITE, OnButtonClick);
    engine.AddUIElement(UI::Element::createFromDerived(testBtn));

    admirals::UI::TextElement testText(
        "Text Element1", "This is a text element.", elementSize, Color::BLACK);
    testText.SetDisplayPosition(admirals::UI::DisplayPosition::LowerLeft);
    engine.AddUIElement(std::make_shared<admirals::UI::TextElement>(testText));

    engine.StartGameLoop();

    return EXIT_SUCCESS;
}