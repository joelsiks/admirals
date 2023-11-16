#include "DisplayLayout.hpp"

using namespace admirals::UI;

DisplayLayout::DisplayLayout(int windowWidth, int windowHeight)
    : m_windowWidth(windowWidth), m_windowHeight(windowHeight),
      m_font(Texture::loadFromPath("assets/font.png")) {}

void DisplayLayout::AddElement(std::shared_ptr<Element> element) {
    m_elements.push_back(std::move(element));
}

float DisplayLayout::GetHeightFromDisplayPosition(
    DisplayPosition pos, const Vector2 &displaySize) const {
    float height = 0;

    switch (pos) {
    case DisplayPosition::UpperLeft:
    case DisplayPosition::UpperRight:
        height = 0;
        break;
    case DisplayPosition::LowerLeft:
    case DisplayPosition::LowerRight:
        height = (float)m_windowHeight - displaySize[1];
        break;
    }

    return height;
}

void DisplayLayout::render() const {

    float positionOffsets[4] = {0};

    for (const auto &element : m_elements) {
        DisplayPosition pos = element->GetDisplayPosition();
        Vector2 displaySize = element->GetDisplaySize();

        float startHeight = GetHeightFromDisplayPosition(pos, displaySize);
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