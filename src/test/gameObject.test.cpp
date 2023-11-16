#include <chrono>
#include <cmath>
#include <memory>
#include <stdbool.h>
#include <stdio.h>

#include "Engine.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/TextElement.hpp"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
float deltaT = 0;

using namespace admirals;

class CellObject : public scene::GameObject {
private:
    Color m_color;

public:
    CellObject(const std::string &name, const Vector3 &pos, const Color &color)
        : scene::GameObject(name, pos), m_color(color) {}

    void onStart() {}

    void onUpdate() {
        Vector2 position = this->position();
        float x = position.x() + 1000.f * deltaT;
        while (x > WINDOW_WIDTH) {
            x = -100 + (x - WINDOW_WIDTH);
        }
        position.setX(x);
        this->setPosition(position);
    }

    void render(const renderer::RendererContext &r) {
        Vector2 pos = this->position();
        // Calculate scaling
        float x = r.windowWidth / static_cast<float>(WINDOW_WIDTH);
        float y = r.windowHeight / static_cast<float>(WINDOW_HEIGHT);

        Vector2 size(100.f * x, 100.f * y);
        pos[0] *= x;
        pos[1] *= y;
        renderer::Renderer::drawRectangle(pos, size, this->m_color);
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

    SDL_PumpEvents(); // clears and gathers events for next loop
    return quit;
}

int main(int argc, char *argv[]) {
    renderer::Renderer renderer =
        renderer::Renderer("Admirals", WINDOW_WIDTH, WINDOW_HEIGHT);
    renderer.init(true);

    scene::Scene *scene = new scene::Scene();
    CellObject cell1 = CellObject("1", Vector3(0, 0, 2), Color::BLUE);
    scene->addObject(scene::GameObject::createFromDerived(cell1));

    CellObject cell2 = CellObject("2", Vector3(50, 50, 3), Color::RED);
    scene->addObject(scene::GameObject::createFromDerived(cell2));

    CellObject cell3 = CellObject("3", Vector3(100, 100, 1), Color::BLACK);
    scene->addObject(scene::GameObject::createFromDerived(cell3));

    CellObject cell4 = CellObject("4", Vector3(100, 200, 0), Color::BLUE);
    scene->addObject(scene::GameObject::createFromDerived(cell4));

    CellObject cell5 = CellObject("5", Vector3(200, 100, 0), Color::RED);
    scene->addObject(scene::GameObject::createFromDerived(cell5));

    CellObject cell6 = CellObject("6", Vector3(200, 200, 0), Color::GREEN);
    scene->addObject(scene::GameObject::createFromDerived(cell6));

    UI::DisplayLayout *layout = new UI::DisplayLayout();
    UI::TextElement fpsText("Fps TextElement", 0, "", Vector2(150, 40),
                            Color::BLACK);
    fpsText.SetDisplayPosition(UI::DisplayPosition::LowerLeft);
    auto sharedFpsText = UI::Element::createFromDerived(fpsText);

    layout->AddElement(sharedFpsText);

    renderer::DrawableCollection layers;
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