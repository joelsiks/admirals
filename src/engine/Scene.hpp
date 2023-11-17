#pragma once

#include <memory>
#include <set>
#include <vector>

#include "GameObject.hpp"
#include "IDrawable.hpp"
#include "OrderedCollection.hpp"

namespace admirals::scene {

class Scene : public renderer::IDrawable {
public:
    void AddObject(std::shared_ptr<GameObject> object);

    void Render(const renderer::RendererContext &r) const override;

    void OnStart();
    void OnUpdate();

private:
    OrderedCollection<GameObject> m_objects;
};

} // namespace admirals::scene