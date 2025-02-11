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

Army::Army(std::string name, engine::Color color, bool user_controlled)
    : SoldiersContainer(), _name(name), _color(color), _user_controlled(user_controlled)
{}

static uint64_t sid = 0;
std::shared_ptr<Soldier> Army::create_soldier_cac(engine::math::Point position) {
    auto entity = engine::get_engine().create_entity(fmt::format("soldier-{}-{}", get_name(), sid++));
    entity->get_transform()->set_pos(position);

    auto soldierCmpt = entity->create_component<SoldierCac>(this);
    register_soldier(soldierCmpt);

    return soldierCmpt;
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
