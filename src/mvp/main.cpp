#include "Engine.hpp"
#include "objects/Background.hpp"
#include "objects/Grid.hpp"
#include "objects/Quad.hpp"
#include "objects/Ship.hpp"
#include "shared.hpp"
#include <SDL_vulkan.h>
#include <VK2D/Constants.h>
#include <VK2D/VK2D.h>

using namespace admirals;
using namespace admirals::mvp;
using namespace admirals::mvp::objects;

int main(int argc, char *argv[]) {
    GameData::engine = std::make_unique<Engine>(
        "Admirals", GameData::WindowWidth, GameData::WindowHeight, true);
    Color blue = Color::FromHEX("#3283cf");
    Color green = Color::FromHEX("#087311");
    Vector2 cellSize = Vector2(100, 100);
    Texture atlas = Texture::LoadFromPath("assets/admirals_texture_atlas.png");
    GameData::engine->AddGameObject(
        scene::GameObject::CreateFromDerived(Background("background", blue)));
    GameData::engine->AddGameObject(
        scene::GameObject::CreateFromDerived(Grid("grid", Color::BLACK)));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(Quad(
        "overlayTop", Vector3(0, 0, 1), Vector2(1000, 100), Color::BLACK)));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(
        Quad("overlayBottom", Vector3(0, GameData::WindowHeight - 100, 1),
             Vector2(GameData::WindowWidth, 100), Color::BLACK)));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(
        Quad("islandLeft0", Vector3(0, 100, 1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(
        Quad("islandLeft1", Vector3(0, 200, 1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(
        Quad("islandLeft2", Vector3(0, 300, 1), cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(Quad(
        "islandRight0",
        Vector3(GameData::WindowWidth - 100, GameData::WindowHeight - 200, 1),
        cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(Quad(
        "islandRight1",
        Vector3(GameData::WindowWidth - 100, GameData::WindowHeight - 300, 1),
        cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(Quad(
        "islandRight2",
        Vector3(GameData::WindowWidth - 100, GameData::WindowHeight - 400, 1),
        cellSize, green)));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(
        Sprite("base0", Vector3(0, 200, 2), cellSize, atlas, Vector2(64, 0))));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(Sprite(
        "base1",
        Vector3(GameData::WindowWidth - 100, GameData::WindowHeight - 300, 2),
        cellSize, atlas, Vector2(64, 0))));
    GameData::engine->AddGameObject(scene::GameObject::CreateFromDerived(Ship(
        "ship0",
        Vector3(100, 500, 2),
        cellSize, atlas)));
    GameData::engine->StartGameLoop();

    return EXIT_SUCCESS;
}