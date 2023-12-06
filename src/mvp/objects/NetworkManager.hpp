#pragma once
#include "Client.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "UI/TextElement.hpp"
#include "objects/GameManager.hpp"

namespace admirals::mvp::objects {

class NetworkManager : public GameObject, public net::Client {

public:
    NetworkManager(const std::string &name, GameManager &gameManager);
    ~NetworkManager();

    void OnStart(const EngineContext &ctx) override;
    void OnUpdate(const EngineContext &ctx) override;
    void Render(const EngineContext &ctx) const override {}

    bool StartAndConnectToServer(uint16_t port, const size_t maxTries);

    bool ConnectToServer(const std::string &ip, uint16_t port,
                         const size_t maxTries);

    void BuyShip(uint8_t type);
    void MoveShip(uint16_t id, uint8_t x, uint8_t y);
    void AttackShip(uint16_t id, uint16_t targetId);
    void ReadyUp();

private:
    void HandleMessages();
    void ReadyUpResponse(net::Message &msg);
    void GameStart();
    void GameStop(net::Message &msg);
    void GamePause();
    void GameResume();
    void UpdateBoard(net::Message &msg);

    GameManager &m_gameManager;
    uint32_t m_playerId = 0;

    bool m_isHost = false;
    std::thread m_serverThread;

    bool m_debug = true;
};
} // namespace admirals::mvp::objects