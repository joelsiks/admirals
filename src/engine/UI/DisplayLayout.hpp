#pragma once

#include <unordered_set>

#include "IDisplayLayer.hpp"
#include "UI/Element.hpp"

namespace admirals::UI {

class DisplayLayout : public IDisplayLayer {
public:
    virtual void Render(const EngineContext &ctx) const override;
    virtual void Update(const EngineContext &ctx) override;

    /// @brief Creates a new `Element` in the display layout and returns a
    /// shared pointer to it.
    /// @tparam T The type of the `Element`, must extend the `Element`
    /// class
    /// @tparam ..._Args The types of the constructor arguments to the
    /// `Element` type
    /// @param ..._args The constructor arguments to the `Element` type
    /// @return A shared pointer to the newly created `Element`
    template <typename T, typename... _Args>
    inline std::shared_ptr<T> MakeElement(_Args &&..._args) {
        auto object = std::make_shared<T>(std::forward<_Args>(_args)...);
        AddDisplayable(object);
        return object;
    }
};

} // namespace admirals::UI