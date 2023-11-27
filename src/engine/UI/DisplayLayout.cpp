#include <vector>

#include "DisplayLayout.hpp"
#include "Renderer.hpp"
#include "UI/DisplayLayout.hpp"

using namespace admirals;
using namespace admirals::UI;

void DisplayLayout::Render(const EngineContext &ctx) const {
    Vector4 positionOffsets = Vector4(0);

    for (const auto &element : m_elements) {
        const DisplayOrientation orientation = element->GetDisplayOrientation();
        const Vector2 displaySize = element->GetSize();

        // Calculate the position with respect to the matching positionOffset.
        Vector2 position =
            GetPositionFromOrientation(orientation, displaySize, ctx);
        position[0] += positionOffsets[orientation];

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
            positionOffsets[orientation] -= displaySize[0];
        } else {
            positionOffsets[orientation] += displaySize[0];
        }
    }

    if (ctx.debug) {
        m_quadtree.DrawTree();
    }
}
void DisplayLayout::RebuildQuadTree(const Vector2 &windowSize) {
    // build a QuadTree to find out what elements to handle click events on.
    std::vector<std::shared_ptr<IDisplayable>> elements;
    for (const auto &element : m_elements) {
        elements.push_back(dynamic_pointer_cast<IDisplayable>(element));
    }
    m_quadtree.BuildTree(windowSize, elements);
}

void DisplayLayout::OnClick(events::MouseClickEventArgs &args) {
    const Vector2 clickLocation = args.Location();
    for (const auto &element : m_quadtree.GetObjectsAtPosition(clickLocation)) {
        if (args.handled)
            break;
        auto el = static_pointer_cast<Element>(element);
        if (el->IsVisible() && el->GetBoundingBox().Contains(clickLocation)) {
            el->OnClick(args);
        }
    }
}

void DisplayLayout::OnMouseMove(events::MouseMotionEventArgs &args) {
    const Vector2 mouseLocation = args.Location();
    for (const auto &element : m_quadtree.GetObjectsAtPosition(mouseLocation)) {
        if (args.handled)
            break;
        auto el = static_pointer_cast<Element>(element);
        const std::string name = el->name();
        if (el->IsVisible() && el->GetBoundingBox().Contains(mouseLocation)) {
            if (m_mouseOverSet.insert(name).second) {
                el->OnMouseEnter(args);
            } else {
                el->OnMouseMove(args);
            }
        } else if (m_mouseOverSet.erase(name) > 0) {
            el->OnMouseLeave(args);
        }
    }
}

void DisplayLayout::AddElement(std::shared_ptr<Element> element) {
    this->m_elements.Insert(std::move(element));
}

Vector2
DisplayLayout::GetPositionFromOrientation(DisplayOrientation orientation,
                                          const Vector2 &displaySize,
                                          const EngineContext &ctx) {
    Vector2 position(0, 0);

    // Handle center.
    if (orientation == DisplayOrientation::Center) {
        position[0] = (ctx.windowSize.x() - displaySize[0]) / 2.0f;
        return position;
    }

    // Fix right offset.
    if (orientation == DisplayOrientation::UpperRight ||
        orientation == DisplayOrientation::LowerRight) {
        position[0] = ctx.windowSize.x() - displaySize[0];
    }

    // Fix lower offset.
    if (orientation == DisplayOrientation::LowerLeft ||
        orientation == DisplayOrientation::LowerRight) {
        position[1] = ctx.windowSize.y() - displaySize[1];
    }

    return position;
}
