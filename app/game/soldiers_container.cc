#include <engine/entity.hh>
#include <engine/math_utils.hh>

#include "game/soldiers_container.hh"
#include "game/components/soldiers/soldier.hh"

#include <algorithm>
#include <spdlog/spdlog.h>

using namespace game;

void SoldiersContainer::remove_soldier(soldier_ptr soldier) {
    spdlog::debug("SoldiersContainer: removing soldier {}.", soldier->get_entity()->name());

    size_t size_before = _soldiers.size();
    _soldiers.erase(std::remove(_soldiers.begin(), _soldiers.end(), soldier), _soldiers.end());
    
    // we can test if it worked !
    if(size_before != _soldiers.size())
        soldiers_changed();
}

void SoldiersContainer::register_soldier(soldier_ptr soldier) {
    if(soldier == nullptr) {
        spdlog::error("Tried to register an NULLPTR soldier onto soldier-container.");
        return;
    }
    _soldiers.push_back(soldier);
    soldier->observe_death([this, soldier]() { remove_soldier(soldier); });
    post_soldier_add(soldier);
    soldiers_changed();
}

void SoldiersContainer::register_soldiers(std::vector<soldier_ptr> soldiers) {
    for(auto soldier : soldiers)
        register_soldier(soldier);
}

bool SoldiersContainer::contains_soldier(const soldier_ptr soldier) const {
    return std::find(_soldiers.begin(), _soldiers.end(), soldier) != _soldiers.end();
}

void SoldiersContainer::clear_soldiers() {
    _soldiers.clear();
}

engine::math::Point SoldiersContainer::average_position() const {
    std::function<engine::math::Point(soldier_ptr)> function = [](soldier_ptr soldier){ return soldier->get_entity()->get_world_pos(); };
    return engine::math::average_position(_soldiers, function);
}
