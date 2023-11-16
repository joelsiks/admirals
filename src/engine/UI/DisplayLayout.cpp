#include "DisplayLayout.hpp"
#include "Renderer.hpp"

using namespace admirals;
using namespace admirals::UI;

DisplayLayout::DisplayLayout()
    : m_font(Texture::loadFromPath("assets/font.png")) {}

void DisplayLayout::AddElement(std::shared_ptr<Element> element) {
    m_elements.push_back(std::move(element));
}

static Vector2
GetOriginFromDisplayPosition(DisplayPosition pos, const Vector2 &displaySize,
                             const renderer::RendererContext &r) {
    Vector2 origin(0, 0);

    // Fix right offset.
    if (pos == DisplayPosition::UpperRight ||
        pos == DisplayPosition::LowerRight) {
        origin[0] = r.windowWidth - displaySize[0];
    }

    // Fix lower offset.
    if (pos == DisplayPosition::LowerLeft ||
        pos == DisplayPosition::LowerRight) {
        origin[1] = r.windowHeight - displaySize[1];
    }

    return origin;
}

void DisplayLayout::render(const renderer::RendererContext &r) const {
    Vector4 positionOffsets = Vector4(0);

    for (const auto &element : m_elements) {
        DisplayPosition pos = element->GetDisplayPosition();
        Vector2 displaySize = element->GetDisplaySize();

        // Calculate the origin with respect to the matching positionOffset.
        Vector2 origin = GetOriginFromDisplayPosition(pos, displaySize, r);
        origin[0] += positionOffsets[pos];

        element->SetDisplayOrigin(origin);

        element->Render(this->m_font);

        // If debugging, render an outline around the UI Element.
        if (r.renderDebugOutlines) {
            renderer::Renderer::drawRectangleOutline(origin, displaySize, 2,
                                                     Color::RED);
        }

        // Update the matching positionOffset.
        if (pos == DisplayPosition::UpperRight ||
            pos == DisplayPosition::LowerRight) {
            positionOffsets[pos] -= displaySize[0];
        } else {
            positionOffsets[pos] += displaySize[0];
        }
    }
}

void DisplayLayout::handleEvent(SDL_Event &e) {
    for (const auto &element : m_elements) {
        element->HandleEvent(e);
    }
}