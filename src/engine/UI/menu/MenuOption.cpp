#include "UI/menu/MenuOption.hpp"
#include "Renderer.hpp"

using namespace admirals::UI::menu;

// MenuOption
MenuOption::MenuOption(const std::string &name, float order,
                       const std::string &text)
    : Element(name, order, text, Vector2(0, 0)) {}

void MenuOption::Render(const renderer::RendererContext &r) const {
    if (m_clickedAndShouldDrawBackground) {
        renderer::Renderer::DrawRectangle(m_boundingBox, Color::GREY);
    }

    renderer::Renderer::DrawText(*r.fontTexture, m_boundingBox.Position(),
                                 m_textColor, GetOptionText());
}

void MenuOption::OnClick(events::MouseClickEventArgs &args) {
    onClick.Invoke(this, args);
}

// TextOption
TextOption::TextOption(const std::string &name, float order,
                       const std::string &text)
    : MenuOption(name, order, text) {}

std::string TextOption::GetOptionText() const { return m_text; }

// ClickOption
ClickOption::ClickOption(const std::string &name, float order,
                         const std::string &text)
    : MenuOption(name, order, text) {}

std::string ClickOption::GetOptionText() const { return m_text; }

// ToggleOption
ToggleOption::ToggleOption(const std::string &name, float order,
                           const std::string &text, bool startToggled)
    : MenuOption(name, order, text), m_toggled(startToggled) {

    onClick.Subscribe([](void *sender, events::MouseClickEventArgs &args) {
        if (args.pressed)
            return;

        auto *toggleOption = static_cast<ToggleOption *>(sender);
        toggleOption->Toggle();
    });
}

std::string ToggleOption::GetOptionText() const {
    return m_text + " [" + std::string(m_toggled ? "ON" : "OFF") + "]";
}

void ToggleOption::Toggle() { m_toggled = !m_toggled; }

// CycleOption
CycleOption::CycleOption(const std::string &name, float order,
                         const std::string &text,
                         const std::vector<std::string> &cycleOptions,
                         size_t startIndex)
    : MenuOption(name, order, text), m_cycleOptions(cycleOptions),
      m_currentOption(startIndex) {}

std::string CycleOption::GetOptionText() const {
    return m_text + " [" + std::string(m_cycleOptions[CurrentIndex()]) + "]";
}

void CycleOption::Cycle() {
    m_currentOption = (m_currentOption + 1) % m_cycleOptions.size();
}

size_t CycleOption::CurrentIndex() const { return m_currentOption; }