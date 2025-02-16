#include <engine/engine.hh>
#include <engine/entity.hh>
#include <engine/components/transform.hh>

#include "game/army.hh"
#include "game/gameplay/command.hh"
#include "game/components/soldiers/soldier_cac.hh"

#include <algorithm>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

using namespace game;

// static engine::math::Point get_pos(const soldier_ptr soldier) {
//     if( ! soldier->get_entity().is_valid()) {
//         spdlog::warn("Invalid soldier ??");
//         return engine::math::Point(0);
//     }
//     return soldier->get_entity()->get_world_pos();
// }

quadtree::Box<float> game::get_box(const soldier_ptr& soldier) {
    auto pos = soldier->get_entity()->get_world_pos();
    return quadtree::Box<float>(pos.x, pos.y, soldier->get_radius(), soldier->get_radius());
}

bool game::soldierEquals(const soldier_ptr& a, const soldier_ptr& b) {
    return a == b;
}

Army::Army(std::string name, engine::Color color, bool user_controlled)
    : SoldiersContainer(), _name(name), _color(color), _user_controlled(user_controlled)//,
//    _quad_tree([](const soldier_ptr& soldier) { return get_pos(soldier); }, engine::math::Rect(-500, -400, 800, 800), 8, 5)
{
    quadtree::Box<float> box(-500, -400, 800, 800);
    _quad_tree = std::make_unique<QuadTree>(box, get_box, soldierEquals);
}

static uint64_t sid = 0;
std::shared_ptr<Soldier> Army::create_soldier_cac(engine::math::Point position) {
    auto entity = engine::get_engine().create_entity(fmt::format("soldier-{}-{}", get_name(), sid++));
    entity->get_transform()->set_pos(position);

    auto soldierCmpt = entity->create_component<SoldierCac>(this);
    register_soldier(soldierCmpt);

    return soldierCmpt;
}

void Army::post_add(soldier_ptr soldier) {
    _quad_tree->add(soldier);
}

void Army::post_rem(soldier_ptr soldier) {
    _quad_tree->remove(soldier);
}

void Army::select_soldier(soldier_ptr soldier) {
    _selection.register_soldier(soldier);
    soldier->set_selected(true);
}

void Army::select_soldiers(std::vector<soldier_ptr> soldiers) {
    for(auto& soldier : soldiers) {
        select_soldier(soldier);
    }
}

bool Army::is_selected(Soldier* soldier) const {
    return _selection.contains_soldier(soldier);
}

void Army::clear_selection() {
    for(auto& soldier : _selection.get_soldiers()) {
        soldier->set_selected(false);
    }
    _selection.clear_soldiers();
}

std::shared_ptr<Command> Army::create_command_with_selection(engine::math::Point target) {
    if(_selection.is_empty())
        return nullptr;

    std::shared_ptr<Command> command = Command::create_move(target, _selection.get_soldiers());

    spdlog::info("New command with {} soldier{}. Target = ({},{}).", command->get_soldiers_size(), command->get_soldiers_size()>1?"s":"", target.x, target.y);
    return command;
}

void Army::draw_quadtree() const {
   _quad_tree->debug_draw();
}

void Army::update_quad_tree() {
    _quad_tree->findAllIntersections();
    //_quad_tree.update_positions();
    //spdlog::info("updated pos. new size={}", _quad_tree.size());
}
