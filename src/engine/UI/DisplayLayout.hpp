
#pragma once

#include <memory>
#include <vector>

#include <VK2D/VK2D.h>

#include "Element.hpp"

namespace admirals {
namespace UI {

class DisplayLayout {
public:
    DisplayLayout(int windowWidth, int windowHeight);

    void AddElement(std::unique_ptr<Element> element);

    float GetHeightFromDisplayPosition(DisplayPosition pos,
                                       const vec2 &displaySize);

    void RenderUIElements();

private:
    VK2DTexture m_font;

    int m_windowWidth, m_windowHeight;
    std::vector<std::unique_ptr<Element>> m_elements;

    std::vector<float> m_positionOffsets{4, 0};

    void resetPositionOffsets();
};

} // namespace UI
} // namespace admirals