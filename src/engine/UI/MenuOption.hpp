#pragma once

#include <memory>
#include <vector>

#include "DataObjects.hpp"
#include "events/EventSystem.hpp"
#include "UI/Element.hpp"

namespace admirals::UI {

static const char *MENU_TITLE_NAME = "__OPTION_TITLE_NAME";
static const float MENU_TITLE_LINE_OFFSET = 10;

class OptionClickEventArgs : public events::EventArgs {
public:
    OptionClickEventArgs(const SDL_Event &data) : m_data(data) {}
    const SDL_Event m_data;
};

class MenuOption : public Element {
public:
    events::EventSystem<OptionClickEventArgs> onClick;

    MenuOption(const std::string &name, float order, const std::string &text);

    void Render(const Texture &font) override;

    bool HandleEvent(const SDL_Event &event) override;

    virtual std::string GetOptionText() = 0;
    inline void SetTextColor(const Color &color) { m_textColor = color; }

    inline void SetDrawBackground(bool value) {
        m_clickedAndShouldDrawBackground = value;
    }

    template <typename T>
    static std::shared_ptr<MenuOption>
    CreateFromDerived(const T &derivedObject) {
        // Assuming T is derived from Element
        std::shared_ptr<MenuOption> element =
            std::make_shared<T>(derivedObject);
        return element;
    }

private:
    Color m_textColor;
    bool m_clickedAndShouldDrawBackground = false;
};

// Text Option, is non-interactive.
class TextOption : public MenuOption {
public:
    TextOption(const std::string &name, float order, const std::string &text);

    std::string GetOptionText() override;
};

// Click Option, click and event occurs (Close, Exit)
class ClickOption : public MenuOption {
public:
    ClickOption(const std::string &name, float order, const std::string &text);

    std::string GetOptionText() override;
};

// Toggle Option, click and toggle between two states (ON/OFF)
class ToggleOption : public MenuOption {
public:
    ToggleOption(const std::string &name, float order, const std::string &text,
                 bool startToggled);

    std::string GetOptionText() override;

    void Toggle();

private:
    bool m_toggled;
};

// Cycle Option, click and cycle through some list of states (1/2/3/4...)
class CycleOption : public MenuOption {
public:
    CycleOption(const std::string &name, float order, const std::string &text,
                const std::vector<std::string> &cycleOptions,
                size_t startIndex);

    std::string GetOptionText() override;

    void Cycle();
    size_t CurrentIndex() const;

private:
    size_t m_currentOption;
    std::vector<std::string> m_cycleOptions;
};

} // namespace admirals::UI