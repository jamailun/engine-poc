#include <engine/entity.hh>
#include <engine/math_utils.hh>

#include "game/soldiers_container.hh"
#include "game/components/soldiers/soldier.hh"

#include <algorithm>
#include <spdlog/spdlog.h>

using namespace game;

void SoldiersContainer::remove_soldier(Soldier* soldier) {
    spdlog::debug("SoldiersContainer: removing soldier {}.", soldier->get_entity()->name());

    for(auto iter = _soldiers.begin(); iter != _soldiers.end(); ++iter) {
        if(iter->get() == soldier) {
            post_rem(*iter);
            _soldiers.erase(iter);
            return;
        }
    }
}

void SoldiersContainer::register_soldier(soldier_ptr soldier) {
    if(soldier == nullptr) {
        spdlog::error("Tried to register an NULLPTR soldier onto soldier-container.");
        return;
    }
    _soldiers.push_back(soldier);
    post_add(soldier);
}

void SoldiersContainer::register_soldiers(std::vector<soldier_ptr> soldiers) {
    for(auto soldier : soldiers)
        register_soldier(soldier);
}

bool SoldiersContainer::contains_soldier(const Soldier* soldier) const {
    for(auto iter = _soldiers.begin(); iter != _soldiers.end(); ++iter) {
        if(iter->get() == soldier) {
            return true;
        }
    }
    return false;
}

void SoldiersContainer::clear_soldiers() {
    for(auto& sol : _soldiers) {
        post_rem(sol);
    }
    _soldiers.clear();
}

engine::math::Point SoldiersContainer::average_position() const {
    std::function<engine::math::Point(soldier_ptr)> function = [](soldier_ptr soldier){ return soldier->get_entity()->get_world_pos(); };
    return engine::math::average_position(_soldiers, function);
}
