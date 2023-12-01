#pragma once

#include "IDisplayLayer.hpp"
#include "UI/menu/MenuOption.hpp"

namespace admirals::UI::menu {

class Menu : public IDisplayLayer<MenuOption> {
public:
    Menu(const std::string &menuTitle, const Color &foregroundColor,
         const Color &backgroundColor, float topPadding = 0);

    void Render(const EngineContext &ctx) const override;

    inline void SetTextColor(const Color &color) { m_fgColor = color; }

    // TODO: This should probably be handled some other way..
    static constexpr float commonDepthOrder = 10;

private:
    std::string m_menuTitle;
    Color m_fgColor, m_bgColor;
    float m_topPadding;
};

} // namespace admirals::UI::menu