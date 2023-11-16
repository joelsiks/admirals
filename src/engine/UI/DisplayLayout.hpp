
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

    void AddElement(const std::shared_ptr<Element> element);
    void render(const renderer::RendererContext &r) const;
    void handleEvent(SDL_Event &e);

private:
    Texture m_font;
    OrderedCollection<Element> m_collection;
};

} // namespace admirals::UI