#pragma once

#include <deque>
#include <unordered_set>

#include "GameObject.hpp"
#include "IDisplayLayer.hpp"

namespace admirals {

class Scene : public IDisplayLayer<GameObject> {
public:
    virtual void Render(const EngineContext &ctx) const override;

    virtual void OnStart(const EngineContext &ctx);
    virtual void OnUpdate(const EngineContext &ctx);

    bool IsInitialized() const { return m_isInitialized; }

    std::deque<Vector2> FindPath(const Vector2 &start, const Vector2 &dest,
                                 const Vector2 &pathSize,
                                 const std::unordered_set<float> &checkedOrders,
                                 float detailLevel) const;

private:
    bool m_isInitialized;
};

} // namespace admirals