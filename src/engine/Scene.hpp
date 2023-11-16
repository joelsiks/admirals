#pragma once

#include <memory>
#include <set>
#include <vector>

#include "GameObject.hpp"
#include "IDrawable.hpp"
#include "OrderedCollection.hpp"

namespace admirals {
namespace scene {

class Scene : public renderer::IDrawable {
public:
    void render(const renderer::RendererContext &r) const;
    void addObject(const std::shared_ptr<GameObject> object);

private:
    OrderedCollection<GameObject> m_collection;
};

} // namespace scene
} // namespace admirals