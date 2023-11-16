
#pragma once

#include <memory>
#include <vector>

#include "Element.hpp"
#include "InteractiveDrawable.hpp"

namespace admirals {
namespace UI {

class DisplayLayout : public InteractiveDrawable {
public:
    DisplayLayout();

    void AddElement(std::shared_ptr<Element> element);
    void render(const renderer::RendererContext &r) const;
    void handleEvent(SDL_Event &e);

private:
    Texture m_font;
    std::vector<std::shared_ptr<Element>> m_elements;
};

} // namespace UI
} // namespace admirals