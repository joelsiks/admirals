#include <chrono>
#include <cmath>
#include <memory>
#include <stdbool.h>
#include <stdio.h>

#include "Engine.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/TextElement.hpp"
using namespace admirals;

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
float deltaT = 0;

class CellObject : public scene::GameObject {
private:
    Color m_color;

public:
    CellObject(const std::string &name, const Vector3 &pos, const Color &color)
        : scene::GameObject(name, pos), m_color(color) {}

    void OnStart() override {}

    void OnUpdate() override {
        Vector2 position = this->GetPosition();
        float x = position.x() + 1000.f * deltaT;
        while (x > WINDOW_WIDTH) {
            x = -100 + (x - WINDOW_WIDTH);
        }
        position.SetX(x);
        this->SetPosition(position);
    }

    void Render(const renderer::RendererContext &r) const override {
        Vector2 pos = this->GetPosition();
        // Calculate scaling
        float x = r.windowWidth / static_cast<float>(WINDOW_WIDTH);
        float y = r.windowHeight / static_cast<float>(WINDOW_HEIGHT);

        Vector2 size(100.f * x, 100.f * y);
        pos[0] *= x;
        pos[1] *= y;
        renderer::Renderer::DrawRectangle(pos, size, this->m_color);
    }
};

class FpsTextElementController : public scene::GameObject {
public:
    FpsTextElementController(const std::string &name,
                             std::shared_ptr<UI::TextElement> textElement)
        : scene::GameObject(name, -1, Vector2(0)), m_textElement(textElement) {}

    void OnStart() override {
        m_time = std::chrono::high_resolution_clock::now();
    }

    void OnUpdate() override {
        auto time = std::chrono::high_resolution_clock::now();
        deltaT =
            std::chrono::duration_cast<std::chrono::microseconds>(time - m_time)
                .count() /
            1000000.f;
        char fpsString[100];
        sprintf(fpsString, "DT = %f, FPS: %f", deltaT, 1.f / deltaT);
        m_textElement->SetText(std::string(fpsString));
        m_time = time;
    }

    void Render(const renderer::RendererContext &r) const override {}

private:
    std::chrono::_V2::system_clock::time_point m_time;
    std::shared_ptr<UI::TextElement> m_textElement;
};

int main(int argc, char *argv[]) {
    Engine engine("Renderer Test", WINDOW_WIDTH, WINDOW_HEIGHT, true);
    engine.AddGameObject(scene::GameObject::CreateFromDerived(
        CellObject("1", Vector3(0, 0, 2), Color::BLUE)));
    engine.AddGameObject(scene::GameObject::CreateFromDerived(
        CellObject("2", Vector3(50, 50, 3), Color::RED)));
    engine.AddGameObject(scene::GameObject::CreateFromDerived(
        CellObject("3", Vector3(100, 100, 1), Color::BLACK)));
    engine.AddGameObject(scene::GameObject::CreateFromDerived(
        CellObject("4", Vector3(100, 200, 0), Color::BLUE)));
    engine.AddGameObject(scene::GameObject::CreateFromDerived(
        CellObject("5", Vector3(200, 100, 0), Color::RED)));
    engine.AddGameObject(scene::GameObject::CreateFromDerived(
        CellObject("6", Vector3(200, 200, 0), Color::GREEN)));
    auto fpsText = UI::Element::CreateFromDerived(UI::TextElement(
        "Fps TextElement", 0, "", Vector2(500, 40), Color::BLACK));
    auto textPtr = std::static_pointer_cast<UI::TextElement>(fpsText);
    textPtr->SetDisplayPosition(UI::DisplayPosition::LowerLeft);
    engine.AddGameObject(scene::GameObject::CreateFromDerived(
        FpsTextElementController("controller", textPtr)));
    engine.AddUIElement(fpsText);
    engine.StartGameLoop();

    return EXIT_SUCCESS;
}