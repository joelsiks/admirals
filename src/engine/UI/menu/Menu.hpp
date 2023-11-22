#pragma once

#include <memory>

#include "UI/DisplayLayout.hpp"
#include "UI/menu/MenuOption.hpp"
#include "events/EventSystem.hpp"

namespace admirals::UI::menu {

class Menu : public DisplayLayout {
public:
    Menu(const std::string &menuTitle, const Color &foregroundColor,
         const Color &backgroundColor);

    void AddMenuOption(const std::shared_ptr<MenuOption> &menuOption);

    void Render(const renderer::RendererContext &r) const override;

    inline void SetTextColor(const Color &color) { m_fgColor = color; }

    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeMenuOption(_Args &&..._args) {
        auto object = std::make_shared<T>(_args...);
        AddMenuOption(object);
        return object;
    }

private:
    static constexpr float commonDepthOrder = 10;

    Color m_fgColor, m_bgColor;
    std::string m_menuTitle;
};

} // namespace admirals::UI::menu