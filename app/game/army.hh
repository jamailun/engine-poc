#pragma once

#include <engine/sdl/colors.hh>
#include <engine/utils/quadtree.hh>
#include "./soldiers_container.hh"
#include "./components/soldiers/soldier.hh"
#include <engine/utils/spatial_grid.hh>
#include <engine/utils/dynamic_spatial_grid.hh>

#include <quadtree/QuadTree.h>

#include <vector>
#include <string>
#include <SDL2/SDL_rect.h>

namespace game {

// Spacial grid type, for soldiers.
using SpatialGrid = engine::dynamic_spatial_grid<size_t, soldier_ptr>;

/*
* An army of soldiers.
*/
class Army : public SoldiersContainer {
    std::string _name;
    engine::Color _color;
    bool _user_controlled = false;

    SoldiersContainer _selection;
    std::unique_ptr<SpatialGrid> _spatial_grid;

protected:
    void post_add(soldier_ptr soldier);
    void post_rem(soldier_ptr soldier);

public:
    Army(std::string name, engine::Color color, bool user_controlled);
    ~Army() = default;

    std::shared_ptr<Soldier> create_soldier_cac(engine::math::Point position);

    inline int soldiers_count() const {
        return get_soldiers().size();
    }

    inline const std::string& get_name() const { return _name; }
    inline const engine::Color& get_color() const { return _color; }

    void select_soldier(soldier_ptr soldier);
    void select_soldiers(std::vector<soldier_ptr> soldiers);
    void clear_selection();

    bool is_selected(Soldier* soldier) const;
    bool is_selected(soldier_ptr soldier) const { return is_selected(soldier.get()); }

    std::shared_ptr<Command> create_command_with_selection(engine::math::Point target);

    void draw_quadtree() const;
    void update_quad_tree();
    std::string to_string_tree() const {
        return "todo";
    }
    std::vector<soldier_ptr> query_soldiers(engine::math::Rect rect) const {
        return _spatial_grid->query(rect);
    }

};

} // namespace game
