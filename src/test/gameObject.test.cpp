#include <chrono>
#include <cmath>
#include <memory>
#include <stdbool.h>
#include <stdio.h>
#include <utility>
#include <vector>

#include "Engine.hpp"
#include "Logger.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/TextElement.hpp"

using namespace admirals;

#define FPS_BUFFER_SIZE 100

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
const float CELL_SIZE = 100;
const float CELL_SPEED = 300.f;
const std::vector<Color> COLOR_LOOP = {Color::BLUE, Color::RED, Color::GREEN,
                                       Color::BLACK};

class CellObject : public GameObject {
private:
    Color m_color;

public:
    CellObject(const std::string &name, const Vector3 &pos, const Color &color)
        : GameObject(name, pos.z(), pos.xy(), Vector2(CELL_SIZE)),
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

class PathFindingObject : public GameObject {
public:
    PathFindingObject(const std::string &name, const Vector2 &position,
                      const std::shared_ptr<Scene> &scene)
        : GameObject(name, 10.f, position, Vector2(0)), m_scene(scene) {}

    void HandleMouseMove(void *, events::MouseMotionEventArgs args) {
        m_mousePosition = args.Location();
    }

    void Render(const EngineContext &ctx) const override {
        const Vector2 scale =
            ctx.windowSize / Vector2(static_cast<float>(WINDOW_WIDTH),
                                     static_cast<float>(WINDOW_HEIGHT));
        const Vector2 pathSize = Vector2(CELL_SIZE - __FLT_EPSILON__);
        const auto path =
            m_scene->FindPath(GetPosition(), m_mousePosition, pathSize,
                              {0, 1, 2, 3}, pathSize.x());

        Vector2 prev = GetPosition();
        for (const Vector2 &part : path) {
            renderer::Renderer::DrawRectangleOutline(part, pathSize, 1,
                                                     Color::BLUE);
        }

        for (const Vector2 &part : path) {
            const Vector2 newPosition = part + pathSize / 2;
            renderer::Renderer::DrawLine(prev, newPosition, Color::BLUE);
            prev = newPosition;
        }

        if (!path.empty()) {
            // Draw square at destination
            renderer::Renderer::DrawRectangle(prev - Vector2(10), Vector2(20),
                                              Color::BLUE);
        }
        // Draw square at origin
        renderer::Renderer::DrawRectangle(GetPosition() - Vector2(10),
                                          Vector2(20), Color::BLUE);
    }

private:
    Vector2 m_mousePosition;
    const std::shared_ptr<Scene> m_scene;
};

int main(int, char *[]) {
    Engine engine("GameObject Test", WINDOW_WIDTH, WINDOW_HEIGHT, false);
    engine.AddGameObject(
        std::make_shared<CellObject>("1", Vector3(0, 0, 2), Color::BLUE));
    engine.AddGameObject(
        std::make_shared<CellObject>("2", Vector3(50, 50, 3), Color::RED));

    auto c1 = engine.MakeGameObject<CellObject>("3", Vector3(100, 100, 1),
                                                Color::BLACK);
    auto c2 = engine.MakeGameObject<CellObject>("4", Vector3(100, 200, 0),
                                                Color::BLUE);
    auto c3 = engine.MakeGameObject<CellObject>("5", Vector3(200, 100, 0),
                                                Color::RED);
    auto c4 = engine.MakeGameObject<CellObject>("6", Vector3(200, 200, 0),
                                                Color::GREEN);
    auto pathFinding = engine.MakeGameObject<PathFindingObject>(
        "path", Vector2(150), engine.GetScene());
    engine.onMouseMove += BIND_EVENT_HANDLER_FROM(
        PathFindingObject::HandleMouseMove, pathFinding);
    engine.StartGameLoop();

    Logger::ClearLogFile();
    Logger::LogMessage("gete");

    return EXIT_SUCCESS;
}