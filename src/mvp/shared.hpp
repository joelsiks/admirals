#pragma once
#include <memory>

#include "Engine.hpp"

namespace admirals::mvp {

class GameData {
private:
public:
    GameData(){};
    ~GameData(){};

    static std::unique_ptr<Engine> engine;
};

} // namespace admirals::mvp