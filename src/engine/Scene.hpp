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
    void RemoveObject(std::shared_ptr<GameObject> object);
    void RemoveObject(const std::string &key);
    bool ExistObject(std::shared_ptr<GameObject> object);
    bool ExistObject(const std::string &key);
    void Render(const EngineContext &c) const override;
    int NumObjectsInScene();

    std::vector<std::string> GetSceneObjectNames();
    void OnStart(const EngineContext &c);
    void OnUpdate(const EngineContext &c);

    bool IsInitialized() const { return m_isInitialized; }

private:
    OrderedCollection<GameObject> m_objects;
    bool m_isInitialized;
};

} // namespace admirals::scene