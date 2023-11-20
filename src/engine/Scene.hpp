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
    void AddObject(std::shared_ptr<GameObject> object);
    void RemoveObject(std::shared_ptr<GameObject> object);
    void RemoveObject(const std::string &key);
    bool ExistObject(std::shared_ptr<GameObject> object);
    bool ExistObject(const std::string &key);
    void Render(const renderer::RendererContext &r) const override;
    int NumObjectsInScene();

    std::vector<std::string> GetSceneObjectNames();
    void OnStart();
    void OnUpdate();

private:
    OrderedCollection<GameObject> m_objects;
};

} // namespace scene
} // namespace admirals