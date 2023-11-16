#pragma once

#include <memory>
#include <set>
#include <vector>

#include "DepthOrderedCollection.hpp"
#include "GameObject.hpp"
#include "IDrawable.hpp"

namespace admirals {
namespace scene {

class Scene : public renderer::IDrawable {
public:
    void render(const renderer::RendererContext &r) const;
    void addObject(const std::shared_ptr<GameObject> &object);

private:
    DepthOrderedCollection<GameObject> m_collection;
};

} // namespace scene
} // namespace admirals