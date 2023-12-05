#pragma once

#include "IDisplayLayer.hpp"
#include "UI/menu/MenuOption.hpp"

namespace admirals::UI::menu {

class Menu : public IDisplayLayer {
public:
    Menu(const std::string &menuTitle, const Color &foregroundColor,
         const Color &backgroundColor, float topPadding = 0);

    void Render(const EngineContext &ctx) const override;

    inline void SetTextColor(const Color &color) { m_fgColor = color; }

    inline std::string GetTitle() const {
        return m_titleOption->GetOptionText();
    }

    inline void SetTitle(const std::string &title) {
        m_titleOption->SetText(title);
    }

    // TODO: This should probably be handled some other way..
    static constexpr float commonDepthOrder = 10;

private:
    std::shared_ptr<TextOption> m_titleOption;
    Color m_fgColor, m_bgColor;
    float m_topPadding;
};

} // namespace admirals::UI::menu