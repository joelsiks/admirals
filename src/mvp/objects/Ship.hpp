#pragma once
#include "CommonTypes.hpp"
#include "Sprite.hpp"

namespace admirals::mvp::objects {

class Ship : public Sprite {
public:
    events::EventSystem<events::EventArgs> onChanged;

    Ship(const ShipData &data, const Vector2 &size, const Texture &source);
    virtual ~Ship();

    void OnUpdate(const EngineContext &ctx) override;

    void OnClick(events::MouseClickEventArgs &args) override;
    void HandleClick(void *sender, events::MouseClickEventArgs &args);

    void OnMouseEnter(events::MouseMotionEventArgs &args) override;
    void OnMouseLeave(events::MouseMotionEventArgs &args) override;

    void Render(const EngineContext &ctx) const;

    inline bool IsOwned() const { return GameData::PlayerId == GetPlayerId(); }
    inline bool IsSelected() const {
        return GameData::selectedShip == m_identifier;
    }
    inline void Select() const { GameData::selectedShip = m_identifier; }
    inline static void DeSelect() { GameData::selectedShip = ""; }

    inline void SetOwner(uint8_t owner) { m_data.owner = owner; }
    inline uint8_t GetOwner() const { return m_data.owner; }

    inline void SetHealth(uint16_t health) { m_data.health = health; }
    inline uint16_t GetHealth() const { return m_data.health; }

    inline void SetID(uint16_t id) { m_data.id = id; }
    inline uint16_t GetID() const { return m_data.id; }

    inline void SetPlayerId(uint8_t playerId) { m_data.owner = playerId; }
    inline uint8_t GetPlayerId() const { return m_data.owner; }

    inline void SetType(uint8_t type) { m_data.type = type; }
    inline uint8_t GetType() const { return m_data.type; }

    inline void SetAction(uint8_t action) { m_data.action = action; }
    inline uint8_t GetAction() const { return m_data.action; }

    inline void SetActionX(uint8_t actionX) {
        m_data.moveData.actionX = actionX;
    }
    inline uint8_t GetActionX() const { return m_data.moveData.actionX; }

    inline void SetActionY(uint8_t actionY) {
        m_data.moveData.actionY = actionY;
    }
    inline uint8_t GetActionY() const { return m_data.moveData.actionY; }

    inline void SetAttackID(uint16_t attackID) {
        m_data.attackTargetID = attackID;
    }
    inline uint16_t GetAttackID() const { return m_data.attackTargetID; }

    inline void SetData(const ShipData &data) { m_data = data; }
    inline const ShipData &GetData() const { return m_data; }

    static Vector2 ShipTypeToTexOffset(uint16_t type, uint8_t owner);

private:
    ShipData m_data;
    bool m_drawOutline = false;
    bool m_selected = false;
    std::deque<admirals::Vector2> m_path;
    std::string m_target;
    std::shared_ptr<NavMesh> m_navMesh;

    void HandleAction();
    void HandlePathTarget();
};

} // namespace admirals::mvp::objects