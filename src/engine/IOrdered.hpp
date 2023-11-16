#pragma once
#include <string>

namespace admirals {
class IOrdered {
public:
    virtual std::string name() const = 0;
    virtual float order() const = 0;
};
} // namespace admirals