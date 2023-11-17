
#pragma once

#include <memory>
#include <vector>

#include "Element.hpp"
#include "InteractiveDrawable.hpp"
#include "OrderedCollection.hpp"

namespace admirals::UI {

class DisplayLayout : public InteractiveDrawable {
public:
    DisplayLayout();

    void Render(const renderer::RendererContext &r) const override;
    void HandleEvent(SDL_Event &e) override;

    void AddElement(std::shared_ptr<Element> element);

private:
    Texture m_font;
    OrderedCollection<Element> m_elements;
};

} // namespace admirals::UI