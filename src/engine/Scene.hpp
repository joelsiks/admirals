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
    void Render(const renderer::RendererContext &r) const override;

    void AddObject(std::shared_ptr<GameObject> object);

private:
    OrderedCollection<GameObject> m_objects;
};

} // namespace scene
} // namespace admirals