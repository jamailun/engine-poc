#pragma once

#include <engine/math.hh>

#include "components/soldiers/soldier.hh"

#include <memory>
#include <vector>

namespace game {

/**
 * A container for soldiers.
 */
class SoldiersContainer {
protected:
    std::vector<soldier_ptr> _soldiers;

public:
    SoldiersContainer() = default;
    virtual ~SoldiersContainer() = default;

    void remove_soldier(Soldier* soldier);
    void remove_soldier(soldier_ptr soldier) { remove_soldier(soldier.get()); }
    void register_soldier(soldier_ptr soldier);
    void register_soldiers(std::vector<soldier_ptr> soldiers);
    
    bool contains_soldier(const Soldier* soldier) const;
    bool contains_soldier(const soldier_ptr soldier) const { return contains_soldier(soldier.get()); }

    void clear_soldiers();
    inline bool is_empty() const { return _soldiers.empty(); }

    inline size_t get_soldiers_size() const { return _soldiers.size(); }

    inline std::vector<soldier_ptr>& get_soldiers() { return _soldiers; }
    inline const std::vector<soldier_ptr>& get_soldiers() const { return _soldiers; }

    engine::math::Point average_position() const;
};

} // namespace game
