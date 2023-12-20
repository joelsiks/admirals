#pragma once

#include "IDisplayLayer.hpp"
#include "UI/menu/MenuOption.hpp"

namespace admirals::UI::menu {

class Menu : public IDisplayLayer {
public:
    Menu(const std::string &menuTitle, const Color &foregroundColor,
         const Color &backgroundColor, float topPadding = 0);

    virtual void Render(const EngineContext &ctx) const override;
    virtual void Update(const EngineContext &ctx) override;

    inline void SetTextColor(const Color &color) { m_fgColor = color; }

    inline std::string GetTitle() const {
        return m_titleOption->GetOptionText();
    }

    inline void SetTitle(const std::string &title) {
        m_titleOption->SetText(title);
    }

    /// @brief Creates a new `MenuOption` in the menu and returns a
    /// shared pointer to it.
    /// @tparam T The type of the `MenuOption`, must extend the `MenuOption`
    /// class
    /// @tparam ..._Args The types of the constructor arguments to the
    /// `MenuOption` type
    /// @param ..._args The constructor arguments to the `MenuOption` type
    /// @return A shared pointer to the newly created `MenuOption`
    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeMenuOption(_Args &&..._args) {
        auto object = std::make_shared<T>(std::forward<_Args>(_args)...);
        AddDisplayable(object);
        return object;
    }

    // TODO: This should probably be handled some other way..
    static constexpr float commonDepthOrder = 10;

private:
    std::shared_ptr<TextOption> m_titleOption;
    Color m_fgColor, m_bgColor;
    float m_topPadding;
};

} // namespace admirals::UI::menu