#pragma once
#include "Client.hpp"
#include "GameObject.hpp"
#include "UI/TextElement.hpp"
#include "shared.hpp"

namespace admirals::mvp::objects {

class GameManager;

class NetworkManager : public scene::GameObject, public net::Client {

public:
    NetworkManager(const std::string &name, GameManager &gameManager);
    ~NetworkManager();

    void OnStart() override;
    void OnUpdate() override;
    void Render(const renderer::RendererContext &r) const override {}

    void BuyShip(uint8_t type);
    void MoveShip(uint16_t id, uint8_t x, uint8_t y);
    void AttackShip(uint16_t id, uint16_t targetId);
    void ReadyUp();

private:
    void HandleMessages();
    void ReadyUpResponse(net::Message &msg);
    void GameStart();
    void GameStop();
    void UpdateBoard(net::Message &msg);

private:
    GameManager &m_gameManager;
    uint32_t m_playerId = 0;

    std::shared_ptr<admirals::UI::TextElement> m_debugText;
    bool m_debug = true;
};
} // namespace admirals::mvp::objects