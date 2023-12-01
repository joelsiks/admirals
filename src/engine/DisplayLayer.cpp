
#include "DisplayLayer.hpp"

using namespace admirals;

void DisplayLayer::OnClick(events::MouseClickEventArgs &args) {
    const Vector2 clickLocation = args.Location();
    for (const auto &object : m_quadtree.GetObjectsAtPosition(clickLocation)) {
        if (args.handled) {
            break;
        }

        if (object->IsVisible() &&
            object->GetBoundingBox().Contains(clickLocation)) {
            object->OnClick(args);
        }
    }
}