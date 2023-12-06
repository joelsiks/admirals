#include "objects/SelectionManager.hpp"
#include "GameData.hpp"
#include "Ship.hpp"

using namespace admirals;
using namespace admirals::mvp::objects;

const Color SelectionColor = Color(0.1, 0.1, 0.8, 0.5);

void SelectionManager::SetSelectionToPreviewArea() {
    Logger::LogMessage("SelectionManager::SetSelectionToPreviewArea = " +
                       m_selectionArea.ToString());
    m_selected.clear();

    const auto objects =
        GameData::engine->GetScene()->GetQuadTree().GetObjectsInArea(
            m_selectionArea);

    for (const auto &o : objects) {
        if (m_selectionArea.Overlaps(o->GetBoundingBox())) {
            const auto ship = std::dynamic_pointer_cast<Ship>(o);
            if (ship != nullptr && ship->IsOwned()) {
                ship->Select();
            }
        }
    }

    ClearSelectionPreview();
}

void SelectionManager::Render(const EngineContext &) const {
    if (!m_selectionArea.IsZero()) {
        renderer::Renderer::DrawRectangle(m_selectionArea, SelectionColor);
    }

    for (const auto &selected : m_selected) {
        const auto obj =
            GameData::engine->GetScene()->FindDisplayable(selected);
        if (obj != nullptr) {
            renderer::Renderer::DrawRectangleOutline(obj->GetBoundingBox(), 3.f,
                                                     Color::BLUE);
        }
    }
}