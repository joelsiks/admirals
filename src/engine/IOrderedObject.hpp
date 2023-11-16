#pragma once
#include <string>

namespace admirals {
class IOrderedObject {
public:
    IOrderedObject(const std::string &name, float order)
        : m_name(name), m_order(order) {}
    const std::string m_name;
    const float m_order;
};
} // namespace admirals