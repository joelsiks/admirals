#include <chrono>
#include <cmath>
#include <memory>
#include <stdbool.h>
#include <stdio.h>
#include <utility>
#include <vector>

#include "Engine.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/TextElement.hpp"

using namespace admirals;

#define FPS_BUFFER_SIZE 100

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
const int CELL_SIZE = 100;
const float CELL_SPEED = 300.f;
const std::vector<Color> COLOR_LOOP = {Color::BLUE, Color::RED, Color::GREEN,
                                       Color::BLACK};

class CellObject : public scene::GameObject {
private:
    Color m_color;

public:
    CellObject(const std::string &name, const Vector3 &pos, const Color &color)
        : scene::GameObject(name, pos.x(), pos.xy(), Vector2(CELL_SIZE)),
          m_color(color) {}

    void OnStart(const EngineContext &c) override {}

    void OnClick(events::MouseClickEventArgs &args) override {
        if (args.button != events::MouseButton::Left || !args.pressed) {
            return;
        }

        int index = 1;
        for (const Color &color : COLOR_LOOP) {
            if (color == this->m_color) {
                break;
            }
            index++;
        }
        m_color = COLOR_LOOP[index % COLOR_LOOP.size()];
    }

    void OnMouseEnter(events::MouseMotionEventArgs &args) override {
        m_drawOutline = true;
    }
    void OnMouseLeave(events::MouseMotionEventArgs &args) override {
        m_drawOutline = false;
    }

    void OnUpdate(const EngineContext &c) override {
        Vector2 position = this->GetPosition();
        float x = position.x() + CELL_SPEED * static_cast<float>(c.deltaTime);
        while (x > WINDOW_WIDTH) {
            x = -CELL_SIZE + (x - WINDOW_WIDTH);
        }
        position.SetX(x);
        this->SetPosition(position);
    }

    void Render(const EngineContext &c) const override {
        // Calculate scaling
        const Vector2 scale =
            c.windowSize / Vector2(static_cast<float>(WINDOW_WIDTH),
                                   static_cast<float>(WINDOW_HEIGHT));
        const Vector2 pos = this->GetPosition() * scale;
        const Vector2 size = scale * CELL_SIZE;
        renderer::Renderer::DrawRectangle(pos, size, this->m_color);

        // Mouse is inside bounds
        if (m_drawOutline) {
            renderer::Renderer::DrawRectangleOutline(pos, size, 3,
                                                     Color::BLACK);
        }
    }

private:
    bool m_drawOutline = false;
};

int main(int, char *[]) {
    Engine engine("Renderer Test", WINDOW_WIDTH, WINDOW_HEIGHT, true);
    engine.AddGameObject(scene::GameObject::CreateFromDerived(
        CellObject("1", Vector3(0, 0, 2), Color::BLUE)));
    engine.AddGameObject(scene::GameObject::CreateFromDerived(
        CellObject("2", Vector3(50, 50, 3), Color::RED)));
    auto c1 = engine.MakeGameObject<CellObject>("3", Vector3(100, 100, 1),
                                                Color::BLACK);
    auto c2 = engine.MakeGameObject<CellObject>("4", Vector3(100, 200, 0),
                                                Color::BLUE);
    auto c3 = engine.MakeGameObject<CellObject>("5", Vector3(200, 100, 0),
                                                Color::RED);
    auto c4 = engine.MakeGameObject<CellObject>("6", Vector3(200, 200, 0),
                                                Color::GREEN);
    engine.StartGameLoop();

    return EXIT_SUCCESS;
}