
#include "UI/DisplayLayout.hpp"
#include "DisplayLayout.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "UI/Data.hpp"

using namespace admirals;
using namespace admirals::UI;

void DisplayLayout::Render(const EngineContext &ctx) const {
    for (const auto &displayable : m_displayables) {
        displayable->Render(ctx);

        // If debugging, render an outline around the UI Element.
        if (Engine::HasDebugMask(ctx.debug,
                                 EngineDebugMode::DebugEnabled |
                                     EngineDebugMode::DebugRendering)) {
            renderer::Renderer::DrawRectangleOutline(displayable->GetPosition(),
                                                     displayable->GetSize(), 2,
                                                     Color::RED);
        }
    }

    if (Engine::HasDebugMask(ctx.debug, EngineDebugMode::DebugEnabled |
                                            EngineDebugMode::DebugQuadTree)) {
        m_quadtree.DrawTree();
    }
}

void DisplayLayout::Update(const EngineContext &ctx) {
    IDisplayLayer::Update(ctx);

    Vector4 positionOffsets = Vector4(0);

    for (const auto &displayable : m_displayables) {
        auto element = std::dynamic_pointer_cast<UI::Element>(displayable);
        if (element == nullptr) {
            continue;
        }

        const DisplayOrientation orientation = element->GetDisplayOrientation();
        const Vector2 elementSize = element->GetSize();

        // Calculate the position with respect to the matching positionOffset.
        Vector2 position =
            GetPositionFromOrientation(orientation, elementSize, ctx);
        position[0] += positionOffsets[static_cast<int>(orientation)];

        element->SetPosition(position);

        // Update the matching positionOffset.
        if (orientation == DisplayOrientation::UpperRight ||
            orientation == DisplayOrientation::LowerRight) {
            positionOffsets[static_cast<int>(orientation)] -= elementSize[0];
        } else {
            positionOffsets[static_cast<int>(orientation)] += elementSize[0];
        }
    }

    RebuildQuadTree(ctx.windowSize);
}
