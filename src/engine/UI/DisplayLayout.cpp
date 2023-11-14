
#include "DisplayLayout.hpp"

void admirals::UI::DisplayLayout::AddElement(std::unique_ptr<Element> element) {
    m_elements.push_back(std::move(element));
}

float admirals::UI::DisplayLayout::GetHeightFromDisplayPosition(
    DisplayPosition pos, const vec2 &displaySize) {
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

void admirals::UI::DisplayLayout::RenderUIElements() {
    for (const auto &element : m_elements) {
        DisplayPosition pos = element->GetDisplayPosition();
        const vec2 &displaySize = element->GetDisplaySize();

        float startHeight = GetHeightFromDisplayPosition(pos, displaySize);
        element->Render(this->m_font,
                        vec2{m_positionOffsets[pos], startHeight});

        m_positionOffsets[pos] += displaySize[0];
    }

    // Reset position offsets.
    m_positionOffsets = {0, 0, 0, 0};
}