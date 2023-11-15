
#include <chrono>
#include <cmath>
#include <memory>
#include <thread>

#include <stdbool.h>
#include <stdio.h>

#include <SDL_vulkan.h>
#include <VK2D/Constants.h>
#include <VK2D/VK2D.h>

#include "Engine.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/TextElement.hpp"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
float deltaT = 0;

using namespace admirals;

class CellObject : public scene::GameObject {
private:
    vec4 color;

public:
    CellObject(const vec2 &pos, const float &index, const vec4 &color)
        : scene::GameObject(pos, index) {
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->color[3] = color[3];
    }

    void onStart() {}

    void onUpdate() {
        this->position[0] = (this->position[0] + 1000.f * deltaT);
        if (this->position[0] > WINDOW_WIDTH) {
            this->position[0] = -100 + this->position[0] - WINDOW_WIDTH;
        }
    }

    void render() {
        vec2 size = {100, 100};
        renderer::Renderer::drawRectangle(this->position, size, this->color);
    }
};

int check_quit() {
    bool quit = false;
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
    }

    SDL_PumpEvents();
    return quit;
}

int main(int argc, char *argv[]) {
    renderer::Renderer renderer =
        renderer::Renderer("Admirals", WINDOW_WIDTH, WINDOW_HEIGHT);
    renderer.init(true);

    scene::Scene *scene = new scene::Scene();
    vec2 pos1 = {0, 0};
    CellObject cell1 = CellObject(pos1, 2, VK2D_BLUE);
    scene->addObject(scene::GameObject::createFromDerived(cell1));

    vec2 pos2 = {50, 50};
    CellObject cell2 = CellObject(pos2, 3, VK2D_RED);
    scene->addObject(scene::GameObject::createFromDerived(cell2));

    vec2 pos3 = {100, 100};
    CellObject cell3 = CellObject(pos3, 1, VK2D_BLACK);
    scene->addObject(scene::GameObject::createFromDerived(cell3));

    vec2 pos4 = {100, 200};
    CellObject cell4 = CellObject(pos4, 0, VK2D_BLUE);
    scene->addObject(scene::GameObject::createFromDerived(cell4));

    vec2 pos5 = {200, 100};
    CellObject cell5 = CellObject(pos5, 0, VK2D_RED);
    scene->addObject(scene::GameObject::createFromDerived(cell5));

    vec2 pos6 = {200, 200};
    CellObject cell6 = CellObject(pos6, 0, VK2D_GREEN);
    scene->addObject(scene::GameObject::createFromDerived(cell6));

    UI::DisplayLayout *layout =
        new UI::DisplayLayout(WINDOW_WIDTH, WINDOW_HEIGHT);
    vec2 elementSize = {150, 40};

    UI::TextElement fpsText("Fps TextElement", "", elementSize, VK2D_BLACK);
    fpsText.SetDisplayPosition(UI::DisplayPosition::LowerLeft);
    auto sharedFpsText = UI::Element::createFromDerived(fpsText);

    layout->AddElement(sharedFpsText);

    std::vector<std::shared_ptr<renderer::IDrawable>> layers;
    layers.emplace_back(scene);
    layers.emplace_back(layout);

    // Start render loop
    bool quit = false;
    SDL_Event e;
    char fpsString[100];
    auto t1 = std::chrono::high_resolution_clock::now();
    while (!quit) {
        quit = check_quit();
        renderer.render(layers);

        auto t2 = std::chrono::high_resolution_clock::now();
        deltaT = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
                     .count() /
                 1000000.f;
        t1 = t2;

        sprintf(fpsString, "DT = %f, FPS: %f", deltaT, 1.f / deltaT);

        std::static_pointer_cast<UI::TextElement>(sharedFpsText)
            ->SetText(std::string(fpsString));
    }

    return EXIT_SUCCESS;
}