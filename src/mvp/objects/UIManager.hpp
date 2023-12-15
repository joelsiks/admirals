#include "GameData.hpp"
#include "GameObject.hpp"
#include "UI/TextElement.hpp"
#include "UI/menu/Menu.hpp"
#include "objects/GridObject.hpp"

namespace admirals::mvp::objects {

class UIManager : public GameObject {
    using GameObject::GameObject; // Inherit constructors
public:
    inline void OnUpdate(const EngineContext &) override {
        const auto topLeft =
            GridObject::ConvertPositionGridToWorld(Vector2(0, -1));
        const auto bottomRight = GridObject::ConvertPositionGridToWorld(
            Vector2(GameData::GridCells, GameData::GridCells + 1));
        GameData::engine->SetDisplayPort({topLeft, bottomRight - topLeft});

        /*
        for(const auto index : GameData::engine->GetActiveLayersIndices()) {
            const auto layer = GameData::engine->GetLayer(index);
            for(const auto identifier : layer->GetDisplayableIdentifiers()){
                const auto displayable = layer->FindDisplayable(identifier);
                if (displayable == nullptr) {
                    continue;
                }
                // TODO: Rescale
                displayable->SetSize(displayable->GetSize());
            }
        }
        */
    }

    void Render(const EngineContext &ctx) const override {}
};

} // namespace admirals::mvp::objects