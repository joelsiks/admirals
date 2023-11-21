#pragma once

#include <memory>

#include "events/EventSystem.hpp"
#include "UI/DisplayLayout.hpp"
#include "UI/MenuOption.hpp"

namespace admirals::UI {

class Menu : public DisplayLayout {
public:
    Menu(const std::string &menuTitle, const Color &foregroundColor);

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

    Color m_fgColor;
    std::string m_menuTitle;
};

} // namespace admirals::UI