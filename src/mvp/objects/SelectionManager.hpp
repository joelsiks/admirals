#pragma once

#include <unordered_set>
#include <vector>

#include "GameObject.hpp"
#include "Logger.hpp"

namespace admirals::mvp::objects {

class SelectionManager : public GameObject {
public:
    SelectionManager(const std::string &name) : GameObject(name, 1000.f) {}
    ~SelectionManager() {}

    inline void Select(const std::string &identifier) {
        Logger::LogMessage("SelectionManager::Select = " + identifier);
        m_selected.insert(identifier);
    }

    inline void Select(const std::vector<std::string> &identifiers) {
        Logger::LogMessage("SelectionManager::Select = <vector:" +
                           std::to_string(identifiers.size()) + ">");
        for (const auto &i : identifiers) {
            m_selected.insert(i);
        }
    }

    inline void SetSelection(const std::string &identifier) {
        Logger::LogMessage("SelectionManager::SetSetSelection = " + identifier);
        Clear();
        Select(identifier);
    }

    inline void SetSelection(const std::vector<std::string> &identifiers) {
        Logger::LogMessage("SelectionManager::SetSetSelection = <vector:" +
                           std::to_string(identifiers.size()) + ">");
        Clear();
        Select(identifiers);
    }

    void SetSelectionToPreviewArea();

    inline void SetSelectionPreview(const Rect &area) {
        m_selectionArea = area;
    }

    inline void ClearSelectionPreview() { m_selectionArea = Rect(); }

    inline Rect GetSelectionPreview() const { return m_selectionArea; }

    inline void DeSelect(const std::string &identifier) {
        Logger::LogMessage("SelectionManager::DeSelect = " + identifier);
        m_selected.erase(identifier);
    }

    inline void Clear() {
        Logger::LogMessage("SelectionManager::Clear");
        m_selected.clear();
        m_selectionArea = Rect();
    }

    inline bool IsSelected(const std::string &identifier) const {
        return m_selected.contains(identifier);
    }

    inline bool IsEmpty() const { return m_selected.empty(); }

    inline size_t NumSelected() const { return m_selected.size(); }

    virtual void Render(const EngineContext &ctx) const;

private:
    std::unordered_set<std::string> m_selected;
    Rect m_selectionArea;
};
} // namespace admirals::mvp::objects