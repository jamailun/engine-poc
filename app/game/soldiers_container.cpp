#include "engine/entity.hpp"
#include "engine/math_utils.hpp"

#include "game/soldiers_container.hpp"
#include "game/components/soldiers/soldier.hpp"

#include <algorithm>
#include <spdlog/spdlog.h>

using namespace game;

void SoldiersContainer::remove_soldier(Soldier* soldier) {
    spdlog::debug("SoldiersContainer: removing soldier {}.", soldier->get_entity()->get_name());

    size_t size_before = _soldiers.size();
    _soldiers.erase(std::remove(_soldiers.begin(), _soldiers.end(), soldier), _soldiers.end());
    
    // we can test if it worked !
    if(size_before != _soldiers.size())
        soldiers_changed();
}

void SoldiersContainer::register_soldier(std::shared_ptr<Soldier> soldier) {
    if(soldier == nullptr) {
        spdlog::error("Tried to register an NULLPTR soldier onto soldier-container.");
        return;
    }
    _soldiers.push_back(soldier);
    soldier->observe_death([this, soldier]() { remove_soldier(soldier); });
    post_soldier_add(soldier);
    soldiers_changed();
}

void SoldiersContainer::register_soldiers(std::vector<Soldier*> soldiers) {
    for(auto* soldier : soldiers)
        register_soldier(soldier);
}

bool SoldiersContainer::contains_soldier(const Soldier* soldier) const {
    return std::find(_soldiers.begin(), _soldiers.end(), soldier) != _soldiers.end();
}

void SoldiersContainer::clear_soldiers() {
    _soldiers.clear();
}

engine::math::Point SoldiersContainer::average_position() const {
    std::function<engine::math::Point(Soldier*)> function = [](Soldier* soldier){ return soldier->get_pos(); };
    return engine::math::average_position(_soldiers, function);
}
