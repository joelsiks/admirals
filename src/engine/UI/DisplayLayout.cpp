
#include "UI/DisplayLayout.hpp"
#include "Renderer.hpp"
#include "UI/Data.hpp"

using namespace admirals;
using namespace admirals::UI;

void DisplayLayout::Render(const EngineContext &ctx) const {
    Vector4 positionOffsets = Vector4(0);

    for (const auto &displayable : m_displayables) {
        auto element = std::dynamic_pointer_cast<UI::Element>(displayable);
        const DisplayOrientation orientation = element->GetDisplayOrientation();
        const Vector2 displaySize = element->GetSize();

        // Calculate the position with respect to the matching positionOffset.
        Vector2 position =
            GetPositionFromOrientation(orientation, displaySize, ctx);
        position[0] += positionOffsets[static_cast<int>(orientation)];

        element->SetPosition(position);

        element->Render(ctx);

        // If debugging, render an outline around the UI Element.
        if (ctx.debug) {
            renderer::Renderer::DrawRectangleOutline(position, displaySize, 2,
                                                     Color::RED);
        }

        // Update the matching positionOffset.
        if (orientation == DisplayOrientation::UpperRight ||
            orientation == DisplayOrientation::LowerRight) {
            positionOffsets[static_cast<int>(orientation)] -= displaySize[0];
        } else {
            positionOffsets[static_cast<int>(orientation)] += displaySize[0];
        }
    }

    if (ctx.debug) {
        m_quadtree.DrawTree();
    }
}
