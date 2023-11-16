#pragma once
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "IOrderedObject.hpp"

namespace admirals {

template <typename T = IOrderedObject> 
class DepthOrderedCollection {
private:
    // Should be sorted with respect to game object index, lower first.
    struct Comparator {
        Comparator(const DepthOrderedCollection &c) : m_collection(c) {}

        bool operator()(const std::string &l, const std::string &r) const {
            const std::shared_ptr<IOrderedObject> a = m_collection.m_objects.at(l);
            const std::shared_ptr<IOrderedObject> b = m_collection.m_objects.at(r);
            return a->m_order < b->m_order;
        }

    private:
        const DepthOrderedCollection &m_collection;
    };

    typedef std::map<std::string, std::shared_ptr<T>> MAP;
    typedef std::multiset<std::string, Comparator> SET;

public:
    DepthOrderedCollection() : m_order(SET(Comparator(*this))) {}

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = const std::shared_ptr<T>;
        using pointer = const std::shared_ptr<T> *;
        using reference = const std::shared_ptr<T> &;

        Iterator(SET::iterator i, const MAP &m_objects)
            : m_iter(i), m_objects(m_objects) {}
        reference operator*() const {
            auto v = (m_iter.operator*)();
            return m_objects.at(v);
        }
        pointer operator->() {
            auto v = (m_iter.operator*)();
            return &m_objects.at(v);
        }
        Iterator &operator++() {
            m_iter++;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend bool operator==(const Iterator &a, const Iterator &b) {
            return a.m_iter == b.m_iter;
        };
        friend bool operator!=(const Iterator &a, const Iterator &b) {
            return a.m_iter != b.m_iter;
        };

    private:
        SET::iterator m_iter;
        const MAP &m_objects;
    };

    Iterator begin() const { return Iterator(m_order.begin(), m_objects); }
    Iterator end() const { return Iterator(m_order.end(), m_objects); }

    inline void insert(const std::shared_ptr<T> &object) {
        const std::string key = object->m_name;
        if (!m_objects.contains(key)) {
            m_objects[key] = object; // object stored in m_objects
            m_order.insert(key);     // object name is sorted
        }
    }

    inline void erase(const std::string &key) {
        m_objects.erase(key);
        m_order.erase(key);
    }

    inline size_t size() const { return m_objects.size(); }

    inline std::shared_ptr<T> &find(const std::string &key) {
        return m_objects[key];
    }

protected:
    SET m_order;
    MAP m_objects;
};

} // namespace admirals