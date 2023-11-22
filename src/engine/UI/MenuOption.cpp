#include "MenuOption.hpp"
#include "Renderer.hpp"

using namespace admirals::UI;

void MenuOptionClickBackground(void *sender, OptionClickEventArgs &event) {
    auto *option = static_cast<MenuOption *>(sender);

    if (event.m_data.type == SDL_MOUSEBUTTONUP) {
        option->SetDrawBackground(false);
    } else if (event.m_data.type == SDL_MOUSEBUTTONDOWN) {
        option->SetDrawBackground(true);
    }
}

// MenuOption
MenuOption::MenuOption(const std::string &name, float order,
                       const std::string &text)
    : Element(name, order, text, Vector2(0, 0)) {
    onClick.Subscribe(MenuOptionClickBackground);
}

void MenuOption::Render(const Texture &font) {
    if (m_clickedAndShouldDrawBackground) {
        renderer::Renderer::DrawRectangle(m_displayOrigin, m_displaySize,
                                          Color::GREY);
    }

    renderer::Renderer::DrawText(font, m_displayOrigin, m_textColor,
                                 GetOptionText());
}

bool MenuOption::HandleEvent(const SDL_Event &event) {
    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {

        const float mouseX = static_cast<float>(event.button.x);
        const float mouseY = static_cast<float>(event.button.y);

        if (mouseX >= m_displayOrigin[0] &&
            mouseX <= m_displayOrigin[0] + m_displaySize[0] &&
            mouseY >= m_displayOrigin[1] &&
            mouseY <= m_displayOrigin[1] + m_displaySize[1]) {

            // call click handler.
            OptionClickEventArgs e = OptionClickEventArgs(event);
            onClick.Invoke(this, e);

            return true;
        }

        if (m_clickedAndShouldDrawBackground &&
            event.type == SDL_MOUSEBUTTONUP) {
            SetDrawBackground(false);
        }
    }

    return false;
}

// TextOption
TextOption::TextOption(const std::string &name, float order,
                       const std::string &text)
    : MenuOption(name, order, text) {}

std::string TextOption::GetOptionText() { return m_text; }

// ClickOption
ClickOption::ClickOption(const std::string &name, float order,
                         const std::string &text)
    : MenuOption(name, order, text) {}

std::string ClickOption::GetOptionText() { return m_text; }

// ToggleOption
ToggleOption::ToggleOption(const std::string &name, float order,
                           const std::string &text, bool startToggled)
    : MenuOption(name, order, text), m_toggled(startToggled) {

    onClick.Subscribe([](void *sender, OptionClickEventArgs &e) {
        if (e.m_data.type != SDL_MOUSEBUTTONUP)
            return;

        auto *toggleOption = static_cast<ToggleOption *>(sender);
        toggleOption->Toggle();
    });
}

std::string ToggleOption::GetOptionText() {
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

std::string CycleOption::GetOptionText() {
    return m_text + " [" + std::string(m_cycleOptions[CurrentIndex()]) + "]";
}

void CycleOption::Cycle() {
    m_currentOption = (m_currentOption + 1) % m_cycleOptions.size();
}

size_t CycleOption::CurrentIndex() const { return m_currentOption; }