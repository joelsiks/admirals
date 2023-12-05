#pragma once

#include <string>

namespace admirals {

class IOrdered {
public:
    IOrdered(const std::string &identifier, float order)
        : m_identifier(identifier), m_order(order) {}

    inline const std::string &identifier() const { return m_identifier; }
    inline float order() const { return m_order; };

protected:
    const std::string m_identifier;
    const float m_order;
};
} // namespace admirals