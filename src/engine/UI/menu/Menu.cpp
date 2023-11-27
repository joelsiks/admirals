#include "UI/menu/Menu.hpp"
#include "DataObjects.hpp"
#include "Renderer.hpp"

using namespace admirals::UI::menu;

Menu::Menu(const std::string &menuTitle, const Color &foregroundColor,
           const Color &backgroundColor, float topPadding)
    : m_menuTitle(menuTitle), m_fgColor(foregroundColor),
      m_bgColor(backgroundColor), m_topPadding(topPadding) {

    const TextOption titleOption(MENU_TITLE_NAME, Menu::commonDepthOrder,
                                 menuTitle);

    this->AddMenuOption(MenuOption::CreateFromDerived(titleOption));
}

void Menu::AddMenuOption(const std::shared_ptr<MenuOption> &menuOption) {
    menuOption->SetDisplayPosition(DisplayPosition::Center);

    this->AddElement(static_cast<std::shared_ptr<Element>>(menuOption));
}

void Menu::Render(const EngineContext &c) const {

    float centerPositionOffset = m_topPadding;

    // Draw background.
    renderer::Renderer::DrawRectangle(Vector2(0, 0), c.windowSize, m_bgColor);

    for (auto it = m_elements.rbegin(); it != m_elements.rend(); ++it) {
        auto option = std::dynamic_pointer_cast<MenuOption>(*it);

        const DisplayPosition pos = option->GetDisplayPosition();
        Vector2 displaySize = option->GetDisplaySize();

        // Calculate the origin with respect to the matching positionOffset.
        Vector2 origin =
            DisplayLayout::GetOriginFromDisplayPosition(pos, displaySize, c);
        origin[1] += centerPositionOffset;

        // Update menu-dependent state of the options.
        option->SetDisplaySize(TextFontSize(option->GetOptionText()));
        option->SetTextColor(m_fgColor);

        option->SetDisplayOrigin(origin);
        option->Render(this->m_font);

        // If we're rendering the menu title, add a line below it.
        if (option->name() == MENU_TITLE_NAME) {
            renderer::Renderer::DrawLine(
                Vector2(origin[0], origin[1] + displaySize[1]),
                Vector2(origin[0] + displaySize[0], origin[1] + displaySize[1]),
                m_fgColor);
            centerPositionOffset += MENU_TITLE_LINE_OFFSET;
        }

        // If debugging, render an outline around the UI Element.
        if (c.renderDebugOutlines) {
            renderer::Renderer::DrawRectangleOutline(origin, displaySize, 2,
                                                     Color::RED);
        }

        centerPositionOffset += displaySize[1];
    }
}