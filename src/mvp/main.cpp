#include "Engine.hpp"
#include "objects/Background.hpp"
#include "objects/Grid.hpp"
#include "objects/Sprite.hpp"
#include "shared.hpp"
#include <SDL_vulkan.h>
#include <VK2D/Constants.h>
#include <VK2D/VK2D.h>

using namespace admirals;
using namespace admirals::mvp;
using namespace admirals::mvp::objects;

int main(int argc, char *argv[]) {
    Color blue = Color::fromHEX("#3283cf");
    Color green = Color::fromHEX("#087311");
    Vector2 cellSize = Vector2(100, 100);
    GameData::engine = std::make_unique<Engine>(
        "Admirals", GameData::WindowWidth, GameData::WindowHeight, true);
    GameData::engine->AddGameObject(
        scene::GameObject::createFromDerived(Background(blue)));
    GameData::engine->AddGameObject(
        scene::GameObject::createFromDerived(Grid(Color::BLACK)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(
        Sprite(Vector3(0, 0, 1), Vector2(1000, 100), Color::BLACK)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(
        Sprite(Vector3(0, GameData::WindowHeight - 100, 1),
               Vector2(GameData::WindowWidth, 100), Color::BLACK)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(
        Sprite(Vector3(0, 100, 1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(
        Sprite(Vector3(0, 200, 1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(
        Sprite(Vector3(0, 300, 1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(
        Vector3(GameData::WindowWidth - 100, GameData::WindowHeight - 200, 1),
        cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(
        Vector3(GameData::WindowWidth - 100, GameData::WindowHeight - 300, 1),
        cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::createFromDerived(Sprite(
        Vector3(GameData::WindowWidth - 100, GameData::WindowHeight - 400, 1),
        cellSize, green)));
    GameData::engine->StartGameLoop();

    return EXIT_SUCCESS;
}