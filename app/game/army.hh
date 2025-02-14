#pragma once

#include <engine/sdl/colors.hh>
#include <engine/utils/quadtree.hh>
#include "./soldiers_container.hh"
#include "./components/soldiers/soldier.hh"

#include <vector>
#include <string>
#include <SDL2/SDL_rect.h>

namespace game {

using QuadTree = engine::quad_tree<soldier_ptr>;

class Army : public SoldiersContainer {
    std::string _name;
    engine::Color _color;
    bool _user_controlled = false;

    SoldiersContainer _selection;
    QuadTree _quad_tree;

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
        return _quad_tree.to_string();
    }
    std::vector<soldier_ptr> query_soldiers(engine::math::Rect rect) const {
        return _quad_tree.query(rect);
    }

};

} // namespace game
