
#pragma once

#include <memory>
#include <vector>

#include <VK2D/Texture.h>

#include "Element.hpp"
#include "InteractiveDrawable.hpp"

namespace admirals {
namespace UI {

class DisplayLayout : public InteractiveDrawable {
public:
    DisplayLayout();

    void AddElement(std::shared_ptr<Element> element);
    void render(const renderer::Renderer *r) const;
    void handleEvent(SDL_Event &e);

private:
    VK2DTexture m_font;
    std::vector<std::shared_ptr<Element>> m_elements;
};

} // namespace UI
} // namespace admirals