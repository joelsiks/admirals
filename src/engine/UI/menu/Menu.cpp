#include "UI/menu/Menu.hpp"

#include "Renderer.hpp"
#include "UI/Data.hpp"

using namespace admirals::UI;
using namespace admirals::UI::menu;

Menu::Menu(const std::string &menuTitle, const Color &foregroundColor,
           const Color &backgroundColor, float topPadding)
    : m_fgColor(foregroundColor), m_bgColor(backgroundColor),
      m_topPadding(topPadding) {

    m_titleOption = std::make_shared<TextOption>(
        TextOption(MENU_TITLE_NAME, Menu::commonDepthOrder, menuTitle));

    this->AddDisplayable(m_titleOption);
}

void Menu::Render(const EngineContext &ctx) const {

    float centerPositionOffset = m_topPadding;

    // Draw background.
    renderer::Renderer::DrawRectangle(Vector2(0, 0), ctx.windowSize, m_bgColor);

    for (auto it = m_displayables.rbegin(); it != m_displayables.rend(); ++it) {
        if (*it == nullptr) {
            continue;
        }

        auto element = std::dynamic_pointer_cast<UI::Element>(*it);
        auto option = std::dynamic_pointer_cast<menu::MenuOption>(element);

        // TODO: This should be fixed somewhere else in the future. This is
        // needed to make sure that all elements are displayed in a center
        // orientation, even if it is changed externally.
        option->SetDisplayOrientation(DisplayOrientation::Center);

        const DisplayOrientation orientation = option->GetDisplayOrientation();
        Vector2 displaySize = option->GetSize();

        // Calculate the position with respect to the matching positionOffset.
        Vector2 position =
            GetPositionFromOrientation(orientation, displaySize, ctx);
        position[1] += centerPositionOffset;

        // Update menu-dependent state of the options.
        option->SetSize(renderer::Renderer::TextFontSize(
            option->GetOptionText(), ctx.fontWidth, ctx.fontHeight));
        option->SetTextColor(m_fgColor);

        option->SetPosition(position);
        option->Render(ctx);

        // If we're rendering the menu title, add a line below it.
        if (option->identifier() == MENU_TITLE_NAME) {
            renderer::Renderer::DrawLine(
                Vector2(position[0], position[1] + displaySize[1]),
                Vector2(position[0] + displaySize[0],
                        position[1] + displaySize[1]),
                m_fgColor);
            centerPositionOffset += MENU_TITLE_LINE_OFFSET;
        }

        // If debugging, render an outline around the UI Element.
        if (ctx.debug) {
            renderer::Renderer::DrawRectangleOutline(position, displaySize, 2,
                                                     Color::RED);
        }

        centerPositionOffset += displaySize[1];
    }

    if (ctx.debug) {
        m_quadtree.DrawTree();
    }
}