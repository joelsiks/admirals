#pragma once

#include <memory>
#include <vector>

#include "DataObjects.hpp"
#include "UI/Element.hpp"
#include "events/EventSystem.hpp"
#include "events/MouseClickEvent.hpp"

namespace admirals::UI::menu {

static const char *MENU_TITLE_NAME = "__OPTION_TITLE_NAME";
static const float MENU_TITLE_LINE_OFFSET = 10;

class MenuOption : public Element {
public:
    events::EventSystem<events::MouseClickEventArgs> onClick;

    MenuOption(const std::string &name, float order, const std::string &text);

    virtual void Render(const EngineContext &c) const override;

    void OnClick(events::MouseClickEventArgs &args) override;

    virtual std::string GetOptionText() const = 0;
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

    std::string GetOptionText() const override;
};

// Click Option, click and event occurs (Close, Exit)
class ClickOption : public MenuOption {
public:
    ClickOption(const std::string &name, float order, const std::string &text);

    std::string GetOptionText() const override;
};

// Toggle Option, click and toggle between two states (ON/OFF)
class ToggleOption : public MenuOption {
public:
    ToggleOption(const std::string &name, float order, const std::string &text,
                 bool startToggled);

    std::string GetOptionText() const override;

    void Toggle();

private:
    bool m_toggled;
};

// Cycle Option, click and cycle through some list of states (1/2/3/4...)
// The user is responsible for calling Cycle() when the option is pressed in
// order for it to be predictable with other actions.
class CycleOption : public MenuOption {
public:
    CycleOption(const std::string &name, float order, const std::string &text,
                const std::vector<std::string> &cycleOptions,
                size_t startIndex);

    std::string GetOptionText() const override;

    void Cycle();
    size_t CurrentIndex() const;

private:
    size_t m_currentOption;
    std::vector<std::string> m_cycleOptions;
};

} // namespace admirals::UI::menu