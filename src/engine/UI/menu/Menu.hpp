#pragma once

#include <memory>

#include "UI/DisplayLayout.hpp"
#include "UI/menu/MenuOption.hpp"

namespace admirals::UI::menu {

class Menu : public DisplayLayout {
public:
    Menu(const std::string &menuTitle, const Color &foregroundColor,
         const Color &backgroundColor, float topPadding = 0);

    void AddMenuOption(const std::shared_ptr<MenuOption> &menuOption);
    void SetMenuTitle(const std::string &title);

    void Render(const EngineContext &ctx) const override;

    inline void SetTextColor(const Color &color) { m_fgColor = color; }

    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeMenuOption(_Args &&..._args) {
        auto object = std::make_shared<T>(_args...);
        AddMenuOption(object);
        return object;
    }

private:
    static constexpr float commonDepthOrder = 10;

    std::string m_menuTitle;
    std::shared_ptr<TextOption> m_titleOption;
    Color m_fgColor, m_bgColor;
    float m_topPadding;
};

} // namespace admirals::UI::menu