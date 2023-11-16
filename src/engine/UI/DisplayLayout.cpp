#include "DisplayLayout.hpp"
#include "Renderer.hpp"

using namespace admirals;
using namespace admirals::UI;

DisplayLayout::DisplayLayout() : m_font(Texture::loadFromPath("assets/font.png")) {}

void DisplayLayout::AddElement(std::shared_ptr<Element> element) {
    m_elements.push_back(std::move(element));
}

static float GetHeightFromDisplayPosition(DisplayPosition pos,
                                          const Vector2 &displaySize,
                                          const Vector2 &windowSize) {
    float height = 0;

    switch (pos) {
    case DisplayPosition::UpperLeft:
    case DisplayPosition::UpperRight:
        height = 0;
        break;
    case DisplayPosition::LowerLeft:
    case DisplayPosition::LowerRight:
        height = windowSize[1] - displaySize[1];
        break;
    }

    return height;
}

void DisplayLayout::render(const renderer::RendererContext &r) const {
    Vector4 positionOffsets = Vector4(0);

    for (const auto &element : m_elements) {
        DisplayPosition pos = element->GetDisplayPosition();
        Vector2 displaySize = element->GetDisplaySize();

        float startHeight = GetHeightFromDisplayPosition(
            pos, displaySize, Vector2(r.windowWidth, r.windowHeight));
        element->SetDisplayOrigin(Vector2(positionOffsets[pos], startHeight));
        element->Render(this->m_font);

        positionOffsets[pos] += displaySize[0];
    }
}

void DisplayLayout::handleEvent(SDL_Event &e) {
    for (const auto &element : m_elements) {
        element->HandleEvent(e);
    }
}