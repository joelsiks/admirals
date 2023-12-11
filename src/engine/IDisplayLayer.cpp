#include "IDisplayLayer.hpp"

using namespace admirals;

void IDisplayLayer::Update(const EngineContext &) { HandleDeferredActions(); }

void IDisplayLayer::OnClick(events::MouseClickEventArgs &args) {
    const Vector2 clickLocation = args.Location();
    for (const auto &displayable :
         m_quadtree.GetObjectsAtPosition(clickLocation)) {
        if (args.handled) {
            break;
        }

        if (displayable->IsVisible() &&
            displayable->GetBoundingBox().Contains(clickLocation)) {
            displayable->OnClick(args);
        }
    }
}

void IDisplayLayer::OnMouseMove(events::MouseMotionEventArgs &args) {
    // TODO: This currently handles MouseEnter/MouseMove before any MouseLeave
    // events, which is reverse of what is happening chronologically. Instead,
    // one might want to handle MouseLeave events first.
    const Vector2 mouseLocation = args.Location();
    std::unordered_set<std::string> currentMouseOverElements;

    for (const auto &displayable :
         m_quadtree.GetObjectsAtPosition(mouseLocation)) {
        if (args.handled) {
            break;
        }

        const std::string identifier = displayable->identifier();
        if (displayable->IsVisible() &&
            displayable->GetBoundingBox().Contains(mouseLocation)) {
            currentMouseOverElements.insert(identifier);
            if (m_mouseOverSet.insert(identifier).second) {
                displayable->OnMouseEnter(args);
            } else {
                displayable->OnMouseMove(args);
            }
        }
    }

    // Handle elements that are no longer moused over
    for (auto it = m_mouseOverSet.begin(); it != m_mouseOverSet.end();) {
        if (args.handled) {
            break;
        }

        const std::string &identifier = *it;
        if (!currentMouseOverElements.contains(identifier)) {
            auto el = m_displayables.Find(identifier);
            if (el != nullptr) {
                el->OnMouseLeave(args);
            }
            it = m_mouseOverSet.erase(it);
        } else {
            it++;
        }
    }
}

void IDisplayLayer::OnShown() {
    for (const auto &displayable : m_displayables) {
        displayable->OnShown();
    }
}

void IDisplayLayer::OnHidden() {
    for (const auto &displayable : m_displayables) {
        displayable->OnHidden();
    }
}

void IDisplayLayer::AddDisplayable(
    std::shared_ptr<IInteractiveDisplayable> displayable) {
    m_deferredActions.emplace_back(DeferType::Add, displayable);
}

void IDisplayLayer::RemoveDisplayable(const std::string &identifier) {
    m_deferredActions.emplace_back(DeferType::Delete, identifier);
}

bool IDisplayLayer::ExistsDisplayable(const std::string &identifier) {
    return m_displayables.Find(identifier) != nullptr;
}

std::shared_ptr<IInteractiveDisplayable>
IDisplayLayer::FindDisplayable(const std::string &identifier) {
    return m_displayables.Find(identifier);
}

std::vector<std::string> IDisplayLayer::GetDisplayableNames() {
    std::vector<std::string> vec = {};
    for (const auto &value : this->m_displayables) {
        vec.emplace_back(value->identifier());
    }
    return vec;
}

void IDisplayLayer::RebuildQuadTree(const Vector2 &windowSize) {
    m_quadtree.BuildTree(windowSize, m_displayables.ToVector());
}

void IDisplayLayer::HandleDeferredActions() {
    for (const auto &[type, value] : m_deferredActions) {
        switch (type) {
        case DeferType::Add:
            m_displayables.Insert(
                std::get<std::shared_ptr<IInteractiveDisplayable>>(value));
            break;
        case DeferType::Delete:
            m_displayables.Erase(std::get<std::string>(value));
            break;
        default:
            break;
        }
    }
    m_deferredActions.clear();
}
