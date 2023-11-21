#pragma once
#include "EventArgs.hpp"

#include <SDL_events.h>

namespace admirals::events {

class ButtonClickEventArgs : public EventArgs {
public:
    ButtonClickEventArgs(const SDL_Event &data) : m_data(data) {}
    const SDL_Event m_data;
};

} // namespace admirals::events