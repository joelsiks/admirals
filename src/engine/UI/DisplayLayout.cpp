#include "UI/DisplayLayout.hpp"
#include "Renderer.hpp"

using namespace admirals;
using namespace admirals::UI;

static const char *FONT_PATH = "assets/font.png";
static const float FONT_CHAR_WIDTH = 16.0;
static const float FONT_CHAR_HEIGHT = 36.0;

DisplayLayout::DisplayLayout()
    : m_font(Texture::LoadFromPath(FONT_PATH)), m_fontWidth(FONT_CHAR_WIDTH),
      m_fontHeight(FONT_CHAR_HEIGHT) {}

void DisplayLayout::AddElement(std::shared_ptr<Element> element) {
    this->m_elements.Insert(std::move(element));
}

Vector2 DisplayLayout::GetOriginFromDisplayPosition(DisplayPosition pos,
                                                    const Vector2 &displaySize,
                                                    const EngineContext &c) {
    Vector2 origin(0, 0);

    // Handle center.
    if (pos == DisplayPosition::Center) {
        origin[0] = (c.windowSize.x() - displaySize[0]) / 2.0f;
        return origin;
    }

    // Fix right offset.
    if (pos == DisplayPosition::UpperRight ||
        pos == DisplayPosition::LowerRight) {
        origin[0] = c.windowSize.x() - displaySize[0];
    }

    // Fix lower offset.
    if (pos == DisplayPosition::LowerLeft ||
        pos == DisplayPosition::LowerRight) {
        origin[1] = c.windowSize.y() - displaySize[1];
    }

    return origin;
}

void DisplayLayout::Render(const EngineContext &c) const {
    Vector4 positionOffsets = Vector4(0);

    for (const auto &element : m_elements) {
        const DisplayPosition pos = element->GetDisplayPosition();
        Vector2 displaySize = element->GetDisplaySize();

        // Calculate the origin with respect to the matching positionOffset.
        Vector2 origin = GetOriginFromDisplayPosition(pos, displaySize, c);
        origin[0] += positionOffsets[pos];

        element->SetDisplayOrigin(origin);

        element->Render(this->m_font);

        // If debugging, render an outline around the UI Element.
        if (c.renderDebugOutlines) {
            renderer::Renderer::DrawRectangleOutline(origin, displaySize, 2,
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

void DisplayLayout::HandleEvent(SDL_Event &e) {
    for (const auto &element : m_elements) {
        element->HandleEvent(e);
    }
}