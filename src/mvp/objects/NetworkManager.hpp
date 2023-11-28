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

    void OnStart(const EngineContext &ctx) override;
    void OnUpdate(const EngineContext &ctx) override;
    void Render(const EngineContext &ctx) const override {}

    bool ConnectToServer(std::string ip = "127.0.0.1",
                         std::string port = "60000",
                         const size_t maxTries = -1);

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

    GameManager &m_gameManager;
    uint32_t m_playerId = 0;

    bool m_debug = false;
};
} // namespace admirals::mvp::objects