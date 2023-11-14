
#include "DisplayLayout.hpp"

using namespace admirals::UI;

DisplayLayout::DisplayLayout(int windowWidth, int windowHeight)
    : m_windowWidth(windowWidth), m_windowHeight(windowHeight) {

    // TODO: This path should probably be configured someplace else.
    m_font = vk2dTextureLoad("assets/font.png");

    resetPositionOffsets();
}

void DisplayLayout::AddElement(std::unique_ptr<Element> element) {
    m_elements.push_back(std::move(element));
}

float DisplayLayout::GetHeightFromDisplayPosition(DisplayPosition pos,
                                                  const vec2 &displaySize) {
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

void DisplayLayout::RenderUIElements() {
    for (const auto &element : m_elements) {
        DisplayPosition pos = element->GetDisplayPosition();
        const vec2 &displaySize = element->GetDisplaySize();

        float startHeight = GetHeightFromDisplayPosition(pos, displaySize);
        vec2 renderPosition = {m_positionOffsets[pos], startHeight};
        element->Render(this->m_font, renderPosition);

        m_positionOffsets[pos] += displaySize[0];
    }

    resetPositionOffsets();
}

void DisplayLayout::resetPositionOffsets() {
    m_positionOffsets = {0, 0, 0, 0};
}