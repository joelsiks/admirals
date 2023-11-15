#include "shared.hpp"
#include "Engine.hpp"
#include "objects/Background.hpp"
#include "objects/Grid.hpp"
#include "objects/Sprite.hpp"
#include <SDL_vulkan.h>
#include <VK2D/Constants.h>
#include <VK2D/VK2D.h>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1200;

using namespace admirals;
using namespace admirals::mvp;
using namespace admirals::mvp::objects;


int main(int argc, char *argv[]) {
    Color blue = Color::fromHEX("#3283cf");
    Color green = Color::fromHEX("#087311");
    Vector2 cellSize = Vector2(100,100);
    GameData::engine = std::make_unique<Engine>("Admirals", WINDOW_WIDTH, WINDOW_HEIGHT, true);
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Background(blue)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Grid(Color::BLACK)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(Vector3(0,0,1), Vector2(1000, 100), Color::BLACK)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(Vector3(0,1100,1), Vector2(1000, 100), Color::BLACK)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(Vector3(0,100,1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(Vector3(0,200,1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(Vector3(0,300,1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(Vector3(900,1000,1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(Vector3(900,900,1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(Vector3(900,800,1), cellSize, green)));
    GameData::engine->StartGameLoop();

    return EXIT_SUCCESS;
}