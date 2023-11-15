
#pragma once

#include <memory>
#include <vector>

#include <VK2D/VK2D.h>

#include "Element.hpp"
#include "InteractiveDrawable.hpp"

namespace admirals {
namespace UI {

class DisplayLayout : public InteractiveDrawable {
public:
    DisplayLayout(int windowWidth, int windowHeight);

    void AddElement(std::shared_ptr<Element> element);

    float GetHeightFromDisplayPosition(DisplayPosition pos,
                                       const Vector2 &displaySize) const;

    void render() const;
    void handleEvent(SDL_Event &e);

private:
    VK2DTexture m_font;

    int m_windowWidth, m_windowHeight;
    std::vector<std::shared_ptr<Element>> m_elements;
};

} // namespace UI
} // namespace admirals