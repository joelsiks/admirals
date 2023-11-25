#include "UI/DisplayLayout.hpp"
#include "QuadTree.hpp"
#include "Renderer.hpp"

using namespace admirals;
using namespace admirals::UI;

void DisplayLayout::Render(const renderer::RendererContext &r) const {
    Vector4 positionOffsets = Vector4(0);

    for (const auto &element : m_elements) {
        const DisplayOrientation orientation = element->GetDisplayOrientation();
        const Vector2 displaySize = element->GetDisplaySize();

        // Calculate the position with respect to the matching positionOffset.
        Vector2 position =
            GetPositionFromOrientation(orientation, displaySize, r);
        position[0] += positionOffsets[orientation];

        element->SetDisplayPosition(position);

        element->Render(r);

        // If debugging, render an outline around the UI Element.
        if (r.renderDebugOutlines) {
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
}

void DisplayLayout::OnClick(events::MouseClickEventArgs &args) {
    // Create and build a QuadTree to find out what elements to handle click
    // events on.
    QuadTree tree = QuadTree();
    std::vector<std::shared_ptr<IDisplayable>> elements;
    for (const auto &element : m_elements) {
        elements.push_back(dynamic_pointer_cast<IDisplayable>(element));
    }
    tree.BuildTree(args.WindowSize(), elements);

    const Vector2 clickLocation = args.Location();
    for (const auto &element : tree.GetObjectsAtPosition(clickLocation)) {
        auto el = static_pointer_cast<Element>(element);
        if (el->GetBoundingBox().Contains(clickLocation)) {
            el->OnClick(args);
        }
    }
}

void DisplayLayout::AddElement(std::shared_ptr<Element> element) {
    this->m_elements.Insert(std::move(element));
}

Vector2
DisplayLayout::GetPositionFromOrientation(DisplayOrientation orientation,
                                          const Vector2 &displaySize,
                                          const renderer::RendererContext &r) {
    Vector2 position(0, 0);

    // Handle center.
    if (orientation == DisplayOrientation::Center) {
        position[0] =
            (static_cast<float>(r.windowWidth) - displaySize[0]) / 2.0f;
        return position;
    }

    // Fix right offset.
    if (orientation == DisplayOrientation::UpperRight ||
        orientation == DisplayOrientation::LowerRight) {
        position[0] = static_cast<float>(r.windowWidth) - displaySize[0];
    }

    // Fix lower offset.
    if (orientation == DisplayOrientation::LowerLeft ||
        orientation == DisplayOrientation::LowerRight) {
        position[1] = static_cast<float>(r.windowHeight) - displaySize[1];
    }

    return position;
}
