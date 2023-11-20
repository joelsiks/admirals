#pragma once
#include "Sprite.hpp"

namespace admirals::mvp::objects {

class Ship : public Sprite {
public:
    Ship(const std::string &name, const Vector3 position, const Vector2 &size,
         const Texture &source, const Vector2 &texOffset = Vector2(0));

    void OnUpdate() override;
    void OnStart() override;

    void SetId(uint16_t id) { m_id = id; }
    uint16_t GetId() const { return m_id; }

    void SetHealth(uint8_t health) { m_health = health; }
    uint8_t GetHealth() const { return m_health; }

    void SetPlayerId(uint8_t playerId) { m_playerId = playerId; }
    uint8_t GetPlayerId() const { return m_playerId; }

    void SetType(uint8_t type) { m_type = type; }
    uint8_t GetType() const { return m_type; }

    void Move(int x, int y);

private:
    uint16_t m_id;
    uint8_t m_playerId;
    uint8_t m_type;
    uint8_t m_health;
};

} // namespace admirals::mvp::objects