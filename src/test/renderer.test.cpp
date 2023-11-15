#include <SDL_vulkan.h>
#include <VK2D/VK2D.h>
#include <VK2D/Validation.h>
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

struct AssetStore {
    VK2DTexture texture;
};

void render_frame(const AssetStore assets) {}

class TextureObject : public scene::GameObject {
public:
    TextureObject(const vec2 &pos, const float &index, const char *texturePath)
        : scene::GameObject(pos, index) {
        m_texture = vk2dTextureLoad(texturePath);
    }

    ~TextureObject() { vk2dTextureFree(m_texture); }

    void onStart() {}

    void onUpdate() {}

    void render() {
        vk2dRendererDrawTexture(m_texture, 0, 0, 0.4, 0.4, 0, 0, 0, 0, 0, 1400,
                                1400);
    }

private:
    VK2DTexture m_texture;
};

void OnButtonClick(UI::Button *button, const SDL_Event &event) {
    // Your handling logic for button click event
    // For example:

    if (event.type == SDL_MOUSEBUTTONUP) {
        button->SetBackgroundColor(VK2D_BLACK);
        printf("Button was clicked! (mu)\n");
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        vec4 grey;
        vk2dColourRGBA(grey, 50, 50, 50, 255);
        button->SetBackgroundColor(grey);
        printf("Button was clicked! (md)\n");
    }
}

int main(int argc, char *argv[]) {

    admirals::Engine engine("Renderer Test", WINDOW_WIDTH, WINDOW_HEIGHT, true);

    // Create texture object.
    vec2 texturePosition = {0, 0};
    TextureObject textureObject(texturePosition, 0, "assets/test.jpg");
    engine.AddGameObject(scene::GameObject::createFromDerived(textureObject));

    vec2 elementSize = {150, 40};
    admirals::UI::Button testBtn("Test Button1", "Click Me!", elementSize,
                                 OnButtonClick);
    engine.AddUIElement(UI::Element::createFromDerived(testBtn));

    admirals::UI::TextElement testText("Text Element1",
                                       "This is a text element.", elementSize);
    testText.SetDisplayPosition(admirals::UI::DisplayPosition::LowerLeft);
    engine.AddUIElement(std::make_shared<admirals::UI::TextElement>(testText));

    engine.StartGameLoop();

    return EXIT_SUCCESS;
}