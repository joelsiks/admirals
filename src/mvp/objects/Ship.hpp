#pragma once
#include "Sprite.hpp"
#include "commontypes.hpp"

namespace admirals::mvp::objects {

class Ship : public Sprite {
public:
    events::EventSystem<events::EventArgs> onChanged;

    Ship(const ShipData &data, const Vector2 &size, const Texture &source);
    ~Ship();

    void OnUpdate(const EngineContext &ctx) override;

    void OnClick(events::MouseClickEventArgs &args) override;
    void HandleClick(void *sender, events::MouseClickEventArgs &args);

    void OnMouseEnter(events::MouseMotionEventArgs &args) override;
    void OnMouseLeave(events::MouseMotionEventArgs &args) override;

    void Render(const EngineContext &ctx) const;

    void SetHealth(uint16_t health) { m_data.health = health; }
    uint16_t GetHealth() const { return m_data.health; }

    void SetID(uint16_t id) { m_data.id = id; }
    uint16_t GetID() const { return m_data.id; }

    void SetPlayerId(uint8_t playerId) { m_data.owner = playerId; }
    uint8_t GetPlayerId() const { return m_data.owner; }

    void SetType(uint8_t type) { m_data.type = type; }
    uint8_t GetType() const { return m_data.type; }

    void SetAction(uint8_t action) { m_data.action = action; }
    uint8_t GetAction() const { return m_data.action; }

    void SetActionX(uint8_t actionX) { m_data.moveData.actionX = actionX; }
    uint8_t GetActionX() const { return m_data.moveData.actionX; }

    void SetActionY(uint8_t actionY) { m_data.moveData.actionY = actionY; }
    uint8_t GetActionY() const { return m_data.moveData.actionY; }

    void SetAttackID(uint16_t attackID) { m_data.attackTargetID = attackID; }
    uint16_t GetAttackID() const { return m_data.attackTargetID; }

    void SetData(const ShipData &data) { m_data = data; }
    const ShipData &GetData() const { return m_data; }

    static Vector2 ShipTypeToTexOffset(uint16_t type);

private:
    ShipData m_data;
    bool m_drawOutline = false;
    bool m_selected = false;
    std::deque<admirals::Vector2> m_path;

    void HandleAction();
};

} // namespace admirals::mvp::objects