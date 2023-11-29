#pragma once
#include "Sprite.hpp"
#include "commontypes.hpp"

namespace admirals::mvp::objects {

class Ship : public Sprite {
public:
    events::EventSystem<events::EventArgs> onChanged;

    Ship(const ShipData &data, const Vector2 &size, const Texture &source);

    void OnUpdate(const EngineContext &ctx) override;

    void SetHealth(uint16_t health) { m_data.health = health; }
    uint16_t GetHealth() const { return m_data.health; }

    void SetID(uint16_t id) { m_data.id = id; }
    uint16_t GetID() const { return m_data.id; }

    void SetPlayerId(uint8_t playerId) { m_data.owner = playerId; }
    uint8_t GetPlayerId() const { return m_data.owner; }

    void SetType(uint8_t type) { m_data.type = type; }
    uint8_t GetType() const { return m_data.type; }

    void SetOwner(uint8_t owner) { m_data.owner = owner; }
    uint8_t GetOwner() const { return m_data.owner; }

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

protected:
    Vector2 CalcOrigin() const override;

private:
    ShipData m_data;
};

} // namespace admirals::mvp::objects