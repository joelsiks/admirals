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
    static const int WindowHeight;
    static const int WindowWidth;
};

} // namespace admirals::mvp