#pragma once

#include <string>

#include "events/EventArgs.hpp"

namespace admirals::events {

class TextEventArgs : public EventArgs {
public:
    TextEventArgs(const std::string &input) : text(input) {}

    const std::string text;
};

} // namespace admirals::events