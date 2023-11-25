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
    menuOption->SetDisplayOrientation(DisplayOrientation::Center);

    this->AddElement(static_cast<std::shared_ptr<Element>>(menuOption));
}

void Menu::Render(const renderer::RendererContext &r) const {

    float centerPositionOffset = m_topPadding;

    // Draw background.
    renderer::Renderer::DrawRectangle(
        Vector2(0, 0),
        Vector2(static_cast<float>(r.windowWidth),
                static_cast<float>(r.windowHeight)),
        m_bgColor);

    for (auto it = m_elements.rbegin(); it != m_elements.rend(); ++it) {
        auto option = std::dynamic_pointer_cast<MenuOption>(*it);

        const DisplayOrientation orientation = option->GetDisplayOrientation();
        Vector2 displaySize = option->GetDisplaySize();

        // Calculate the position with respect to the matching positionOffset.
        Vector2 position = DisplayLayout::GetPositionFromOrientation(
            orientation, displaySize, r);
        position[1] += centerPositionOffset;

        // Update menu-dependent state of the options.
        option->SetDisplaySize(renderer::Renderer::TextFontSize(
            option->GetOptionText(), r.fontWidth, r.fontHeight));
        option->SetTextColor(m_fgColor);

        option->SetDisplayPosition(position);
        option->Render(r);

        // If we're rendering the menu title, add a line below it.
        if (option->name() == MENU_TITLE_NAME) {
            renderer::Renderer::DrawLine(
                Vector2(position[0], position[1] + displaySize[1]),
                Vector2(position[0] + displaySize[0],
                        position[1] + displaySize[1]),
                m_fgColor);
            centerPositionOffset += MENU_TITLE_LINE_OFFSET;
        }

        // If debugging, render an outline around the UI Element.
        if (r.renderDebugOutlines) {
            renderer::Renderer::DrawRectangleOutline(position, displaySize, 2,
                                                     Color::RED);
        }

        centerPositionOffset += displaySize[1];
    }
}