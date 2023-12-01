#pragma once

#include <unordered_set>

#include "IDisplayable.hpp"
#include "OrderedCollection.hpp"
#include "QuadTree.hpp"
#include "UI/Element.hpp"
#include "events/MouseClickEvent.hpp"

namespace admirals {

class DisplayLayer {
public:
    virtual void Render(const EngineContext &ctx) const = 0;

    virtual void OnClick(events::MouseClickEventArgs &args);
    virtual void OnMouseMove(events::MouseMotionEventArgs &args);

    virtual void OnShown();
    virtual void OnHidden();

    // Collection
    void AddDisplayable(std::shared_ptr<IInteractiveDisplayable> displayble);
    void RemoveDisplayable(const std::string &identifier);
    bool ExistsDisplayable(const std::string &identifier);

    inline size_t NumDisplayables() const { return m_displayables.Size(); }

    std::vector<std::string> GetSceneObjectNames() const {
        std::vector<std::string> vec = {};
        for (const auto &value : this->m_displayables) {
            vec.emplace_back(value->identifier());
        }
        return vec;
    }

    void RebuildQuadTree(const Vector2 &windowSize);

protected:
    OrderedCollection<IInteractiveDisplayable> m_displayables;

    QuadTree m_quadtree;
    std::unordered_set<std::string> m_mouseOverSet;
};

} // namespace admirals
