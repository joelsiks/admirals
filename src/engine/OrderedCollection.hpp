#pragma once
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "IOrdered.hpp"

namespace admirals {

/// @brief Maps string keys (`IOrdered::name`) to `IOrdered` object pointers and
/// uses a separate collection to order the contained `IOrdered` object pointers
/// by their `IOrdered::order` value. The objects are ordered from low to hight.
/// Using these two collections, it can quickly search among objects in the
/// collection using their `IOrdered::name` value, while also keeping the
/// objects sorted by their `IOrdered::order` value`.
/// @tparam T The contained class implementing the `IOrdered` interface.
template <typename T = IOrdered> class OrderedCollection {
private:
    /// @brief Compares values of the multiset to order the values by their
    //         respective `IOrdered::order` value.
    struct Comparator {
        Comparator(const OrderedCollection &c) : m_collection(c) {}

        bool operator()(const std::string &l, const std::string &r) const {
            const std::shared_ptr<IOrdered> a = m_collection.m_objects.at(l);
            const std::shared_ptr<IOrdered> b = m_collection.m_objects.at(r);
            return a->order() < b->order();
        }

    private:
        const OrderedCollection &m_collection;
    };

    typedef std::map<std::string, std::shared_ptr<T>> KeyToPointerMap;
    typedef std::multiset<std::string, Comparator> OrderedKeySet;

public:
    OrderedCollection() : m_ordered(OrderedKeySet(Comparator(*this))) {}

    /// @brief Iterates over the `IOrdered` object points in the collection in
    /// the same order as the `IOrdered::name` values are sorted in the ordered
    /// multiset.
    struct Iterator {
        Iterator(OrderedKeySet::iterator i, const KeyToPointerMap &m_objects)
            : m_iter(i), m_objects(m_objects) {}

        const std::shared_ptr<T> operator*() const {
            auto v = (m_iter.operator*)();
            return m_objects.at(v);
        }

        // std::shared_ptr<T> operator->() {
        //     auto v = (m_iter.operator*)();
        //     return m_objects.at(v);
        // }

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
        OrderedKeySet::iterator m_iter;
        const KeyToPointerMap &m_objects;
    };

    /// @brief Iterator starting at the start of the ordered collection.
    Iterator begin() const { return Iterator(m_ordered.begin(), m_objects); }
    /// @brief Iterator at the end of the ordered collection.
    Iterator end() const { return Iterator(m_ordered.end(), m_objects); }

    /// @brief Inserts an `IOrdered` object pointer into the collection if no
    /// other contained object has the same`IOrdered::name` property.
    /// @param object The `IOrdered` object pointer to insert into the
    /// collection.
    inline void insert(const std::shared_ptr<T> object) {
        const std::string key = object->name();
        if (!m_objects.contains(key)) {
            m_objects[key] = std::move(object); // object stored in m_objects
            m_ordered.insert(key);              // object name is sorted
        }
    }

    /// @brief Erases an `IOrdered` object pointer from the collection it has
    /// the same `IOrdered::name` value as the given identifier `key`
    /// @param key The identifier (`IOrdered::name`) of the contained
    /// object to erase from the collection
    inline void erase(const std::string &key) {
        m_objects.erase(key);
        m_ordered.erase(key);
    }

    /// @brief Get the number of `IOrdered` objects contained in the collection.
    /// @return The number of `IOrdered` objects contained in the collection.
    inline size_t size() const { return m_objects.size(); }

    /// @brief Gets an `IOrdered` object pointer from the collection if some
    /// contained object has the same`IOrdered::name` property as the given
    /// identifier `key`. If no matching object is found, `nullptr` is returned.
    /// @param key The identifier (`IOrdered::name`) of the contained
    /// object to erase from the collection
    /// @return The pointer to the `IOrdered` object with the same
    /// `IOrdered::name` property as the given identifier `key`. If no matching
    /// object is found, `nullptr` is returned.
    inline Iterator find(const std::string &key) {
        if (auto res = m_objects.find(key) && res != m_objects.end()) {
            return res->second;
        }
        return nullptr;
    }

protected:
    OrderedKeySet m_ordered;
    KeyToPointerMap m_objects;
};

} // namespace admirals