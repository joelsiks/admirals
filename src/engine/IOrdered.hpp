#pragma once
#include <string>

namespace admirals {
class IOrdered {
public:
    IOrdered(const std::string &name, float order)
        : m_name(name), m_order(order) {}

    inline const std::string &name() const { return m_name; }
    inline float order() const { return m_order; };

protected:
    const std::string m_name;
    const float m_order;
};
} // namespace admirals