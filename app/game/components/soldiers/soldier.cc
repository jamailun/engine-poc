#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/components/transform.hh>
#include <engine/sdl/inputs.hh>
#include <engine/sdl/drawer.hh>

#include "game/army.hh"
#include "game/components/soldiers/soldier.hh"
#include "game/gameplay/command.hh"

#include <math.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

using namespace game;

Soldier::Soldier(entity_ptr entity, guaranteed_ptr<Army> army, float speed, float radius)
    : base_component(entity), Livable(100), _army(army), _radius(radius), _speed(speed)
{}

void Soldier::update(float elapsed) {
    engine::math::Point pos = get_entity()->get_world_pos();
    if(has_command()) {
        engine::math::Vector dir = engine::math::Vector::diff(pos, _command->find_target(this));
        if(dir.squared_length() > 25) {
            engine::math::Vector movement = dir.normalized() * _speed * elapsed;
            get_entity()->get_transform()->move(movement);
            return;
        }
    }

    // remove command if expired
    if(_command && _command->type() == Command::Type::move) {
        clear_command();
    }

    // try to get out of others
    engine::math::Vector delta;
//    auto around = engine::get_engine().get_current_world()->get_entities_around(pos, 10);
    auto around = _army->query_soldiers(engine::math::Rect::from_center(pos, 20, 20));
    for(auto other_entity : around) {
        if(other_entity.get() == this) continue;
        engine::math::Vector dir = engine::math::Vector::diff(pos, other_entity->get_entity()->get_world_pos());
        delta = delta - dir.normalized_r();
       // spdlog::trace("{} >{}< {}", _owner_entity->get_name(), dir.length(), other->get_name());
    }
    engine::math::Vector movement = delta * (elapsed * _speed * 0.35);
    get_entity()->get_transform()->move(movement);
}

void Soldier::render() {
    // if selected ?
    if(is_selected()) {
        engine::drawer::fill_rect_center(get_entity()->get_transform()->get_pos(), _radius + 2, _radius + 2, engine::colors::yellow);
    }

    // Color
    engine::drawer::fill_rect_center(get_entity()->get_transform()->get_pos(), _radius, _radius, _army->get_color());
    
    // arrow
    if(has_command() && _command->distance(this) >= 5) {
        engine::drawer::draw_arrow(get_entity()->get_transform()->get_pos(), _command->world_point(), engine::colors::red);
    }
}

void Soldier::on_death() {
    spdlog::info("dead soldier : {}", get_entity()->name());
    if(_command) {
        spdlog::info("dead soldier will remove itself from command !");
        _command->remove_soldier(this);
    }
}

void Soldier::set_command(std::shared_ptr<Command> command) {
    clear_command();
    _command = command;
}

bool Soldier::has_command() const {
    return _command && _command->is_valid();
}
