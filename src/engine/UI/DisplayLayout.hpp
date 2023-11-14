
#pragma once

#include <memory>
#include <vector>

#include <VK2D/VK2D.h>

#include "Element.hpp"
#include "IDrawable.hpp"

namespace admirals {
namespace UI {

class DisplayLayout : public renderer::IDrawable {
public:
    DisplayLayout(int windowWidth, int windowHeight);

    void AddElement(std::shared_ptr<Element> element);

    float GetHeightFromDisplayPosition(DisplayPosition pos,
                                       const vec2 &displaySize) const;

    void render() const;

private:
    VK2DTexture m_font;

    int m_windowWidth, m_windowHeight;
    std::vector<std::shared_ptr<Element>> m_elements;
};

} // namespace UI
} // namespace admirals