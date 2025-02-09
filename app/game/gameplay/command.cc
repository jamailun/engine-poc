#include <engine/entity.hh>
#include <engine/components/transform.hh>

#include "./command.hh"
#include "game/components/soldiers/soldier.hh"

#include <spdlog/spdlog.h>

using namespace game;
using namespace engine::math;

Command::Command(Command::Type type, engine::math::Point target)
    : SoldiersContainer(), _type(type), _world_point(target) {}

Command::~Command() {
    //std::cout << "Command deleted..." << std::endl;
}

std::shared_ptr<Command> Command::create(Command::Type type, engine::math::Point target) {
    return std::make_shared<Command>(type, target);
}

std::shared_ptr<Command> Command::create_move(engine::math::Point target, std::vector<soldier_ptr> soldiers) {
    std::shared_ptr<Command> ptr = create(Type::move, target);
    for(auto soldier : soldiers) {
        ptr->_soldiers.push_back(soldier);
        soldier->set_command(ptr);
    }
    return ptr;
}

bool Command::is_valid() const {
    if( ! _valid || get_soldiers().empty())
        return false;
    
    // If type is movement : not valid anymore if soldiers close to destination
    if(_type == Type::move) {
        for(auto sol : _soldiers) {
            if(sol->get_entity()->get_world_pos().squared_distance(_world_point) >= 25) {
                return true;
            }
        }
        return false;
    }

    // By default : valid.
    return true;
}

float Command::distance(Soldier* soldier) const {
    Point pos = soldier->get_entity()->get_transform()->get_pos();
    return Vector::diff(pos, world_point()).length();
}

engine::math::Point Command::find_target(Soldier*) const {
    //TODO use formations
    return _world_point;
}
