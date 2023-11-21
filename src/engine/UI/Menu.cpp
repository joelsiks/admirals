#include "DataObjects.hpp"
#include "Renderer.hpp"
#include "UI/Menu.hpp"

using namespace admirals::UI;

Menu::Menu(const std::string &menuTitle, const Color &foregroundColor)
    : m_menuTitle(menuTitle), m_fgColor(foregroundColor) {

    const TextOption titleOption(MENU_TITLE_NAME, Menu::commonDepthOrder,
                                 menuTitle);

    this->AddMenuOption(MenuOption::CreateFromDerived(titleOption));
}

void Menu::AddMenuOption(const std::shared_ptr<MenuOption> &menuOption) {
    menuOption->SetDisplayPosition(DisplayPosition::Center);

    this->AddElement(static_cast<std::shared_ptr<Element>>(menuOption));
}

void Menu::Render(const renderer::RendererContext &r) const {

    float centerPositionOffset = 0;

    // Draw background.
    renderer::Renderer::DrawRectangle(
        Vector2(0, 0), Vector2((float)r.windowWidth, (float)r.windowHeight),
        Color::LIGHT_GREY);

    for (auto it = m_elements.rbegin(); it != m_elements.rend(); ++it) {
        auto option = std::dynamic_pointer_cast<MenuOption>(*it);

        const DisplayPosition pos = option->GetDisplayPosition();
        Vector2 displaySize = option->GetDisplaySize();

        // Calculate the origin with respect to the matching positionOffset.
        Vector2 origin =
            DisplayLayout::GetOriginFromDisplayPosition(pos, displaySize, r);
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
        if (r.renderDebugOutlines) {
            renderer::Renderer::DrawRectangleOutline(origin, displaySize, 2,
                                                     Color::RED);
        }

        centerPositionOffset += displaySize[1];
    }
}