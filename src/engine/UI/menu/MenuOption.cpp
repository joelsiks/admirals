#include "UI/menu/MenuOption.hpp"
#include "Renderer.hpp"

using namespace admirals::UI::menu;

// MenuOption
MenuOption::MenuOption(const std::string &name, float order,
                       const std::string &text)
    : Element(name, order, text, Vector2(0, 0)) {}

void MenuOption::Render(const EngineContext &ctx) const {
    if (m_shouldDrawBackground) {
        renderer::Renderer::DrawRectangle(m_boundingBox, Color::GREY);
    }

    renderer::Renderer::DrawText(*ctx.fontTexture, m_boundingBox.Position(),
                                 m_textColor, GetOptionText());
}

void MenuOption::OnClick(events::MouseClickEventArgs &args) {
    onClick.Invoke(this, args);
}

void MenuOption::OnMouseEnter(events::MouseMotionEventArgs &) {
    m_shouldDrawBackground = true;
    renderer::Renderer::SetCursor(renderer::Cursor::Hand);
}

void MenuOption::OnMouseLeave(events::MouseMotionEventArgs &) {
    m_shouldDrawBackground = false;
    renderer::Renderer::SetCursor(renderer::Cursor::Arrow);
}

void MenuOption::OnMouseMove(events::MouseMotionEventArgs &) {
    renderer::Renderer::SetCursor(renderer::Cursor::Hand);
}

void MenuOption::OnShown() {}

void MenuOption::OnHidden() {
    renderer::Renderer::SetCursor(renderer::Cursor::Arrow);
    m_shouldDrawBackground = false;
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

InputOption::InputOption(
    const std::string &name, float order,
    events::EventSystem<events::KeyPressEventArgs> &eventHandler,
    const std::string &initString, const std::string &placeholder)
    : MenuOption(name, order, initString), m_placeholderText(placeholder),
      m_keyPressEventHandler(eventHandler) {

    onClick.Subscribe([](void *sender, events::MouseClickEventArgs &args) {
        if (args.pressed) {
            auto *inputOption = static_cast<menu::InputOption *>(sender);
            inputOption->ToggleActive();

            if (inputOption->IsActive()) {
                inputOption->m_keyPressEventHandler.Subscribe(
                    BIND_EVENT_HANDLER_FROM(
                        menu::InputOption::HandleKeyPressEvent, inputOption));
            } else {
                inputOption->m_keyPressEventHandler.Unsubscribe(
                    BIND_EVENT_HANDLER_FROM(
                        menu::InputOption::HandleKeyPressEvent, inputOption));
            }
        }
    });
}

void InputOption::Render(const EngineContext &ctx) const {
    if (m_isActive || m_shouldDrawBackground) {
        renderer::Renderer::DrawRectangle(m_boundingBox, Color::GREY);
    }

    renderer::Renderer::DrawText(*ctx.fontTexture, m_boundingBox.Position(),
                                 m_textColor, GetOptionText());
}

void InputOption::OnMouseEnter(events::MouseMotionEventArgs &) {
    m_shouldDrawBackground = true;
    renderer::Renderer::SetCursor(renderer::Cursor::IBeam);
}

void InputOption::OnMouseLeave(events::MouseMotionEventArgs &) {
    m_shouldDrawBackground = false;
    renderer::Renderer::SetCursor(renderer::Cursor::Arrow);
}

void InputOption::OnMouseMove(events::MouseMotionEventArgs &) {
    m_shouldDrawBackground = true;
    renderer::Renderer::SetCursor(renderer::Cursor::IBeam);
}

void InputOption::OnHidden() {
    renderer::Renderer::SetCursor(renderer::Cursor::Arrow);

    if (m_isActive) {
        m_keyPressEventHandler.Unsubscribe(BIND_EVENT_HANDLER_FROM(
            menu::InputOption::HandleKeyPressEvent, this));
        m_isActive = false;
    }
}

std::string InputOption::GetOptionText() const {
    if (m_text.length() == 0) {
        return m_placeholderText;
    }

    return m_text;
}

void InputOption::HandleKeyPressEvent(void *, events::KeyPressEventArgs &args) {
    if (!args.isKeyUp) {
        return;
    }

    // TODO: This only handles ASCII-values...
    if (args.key == SDLK_BACKSPACE && m_text.length() > 0) {
        m_text.pop_back();
        args.handled = true;
    } else if (args.key >= 32 && args.key <= 126) {
        m_text.push_back(static_cast<char>(args.key));
        args.handled = true;
    }

    if (args.handled) {
        events::TextEventArgs args(m_text);
        onInput.Invoke(this, args);
    }
}