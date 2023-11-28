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

    void OnMouseEnter(events::MouseMotionEventArgs &) override {
        m_drawOutline = true;
    }
    void OnMouseLeave(events::MouseMotionEventArgs &) override {
        m_drawOutline = false;
    }

    Vector2 GetPosition() const override {
        return m_boundingBox.Position() * m_scale;
    }

    Vector2 GetSize() const override { return m_boundingBox.Size() * m_scale; }

    Rect GetBoundingBox() const override {
        return Rect(GetPosition(), GetSize());
    }

    void OnUpdate(const EngineContext &ctx) override {
        m_scale = ctx.windowSize / Vector2(static_cast<float>(WINDOW_WIDTH),
                                           static_cast<float>(WINDOW_HEIGHT));
        const float motion = CELL_SPEED * static_cast<float>(ctx.deltaTime);
        float x = m_boundingBox.PositionX() + motion;
        if (x > WINDOW_WIDTH) {
            x = x - WINDOW_WIDTH - CELL_SIZE;
        }
        m_boundingBox.SetPositionX(x);
    }

    void Render(const EngineContext &) const override {
        const Rect bounds = GetBoundingBox();
        renderer::Renderer::DrawRectangle(bounds, this->m_color);
        // Mouse is inside bounds
        if (m_drawOutline) {
            renderer::Renderer::DrawRectangleOutline(bounds, 3, Color::BLACK);
        }
    }

private:
    Vector2 m_scale = Vector2(1);
    bool m_drawOutline = false;
};

class PathFindingObject : public scene::GameObject {
public:
    PathFindingObject(const std::string &name, const Vector2 &position)
        : scene::GameObject(name, -1.f, position, Vector2()) {}

    void HandleMouseMove(void *sender, events::MouseMotionEventArgs args) {
        if (m_updated) {
            return;
        }

        Engine *engine = static_cast<Engine *>(sender);
        m_updated = true;
        const auto scene = engine->GetScene();
        const auto path =
            scene->FindPath(GetPosition(), args.Location(), {8}, CELL_SIZE);
        m_path = path;
    }

    void OnUpdate(const EngineContext &) override { m_updated = false; }

    void Render(const EngineContext &) const override {
        Vector2 prev = GetPosition();
        for (const Vector2 &part : m_path) {
            renderer::Renderer::DrawLine(prev, part, Color::BLUE);
            prev = part;
        }
    }

private:
    bool m_updated = false;
    std::deque<Vector2> m_path;
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
    auto pathFinding =
        engine.MakeGameObject<PathFindingObject>("path", Vector2(100));
    engine.onMouseMove += BIND_EVENT_HANDLER_FROM(
        PathFindingObject::HandleMouseMove, pathFinding);
    engine.StartGameLoop();

    return EXIT_SUCCESS;
}