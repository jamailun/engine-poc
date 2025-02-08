#include <engine/entity.hh>
#include <engine/math.hh>
#include <engine/world.hh>
#include <engine/engine.hh>
#include <engine/components/transform.hh>

#include <spdlog/spdlog.h>

using namespace engine;

entity::entity(guaranteed_ptr<world> world, std::string name) : _world(world), _name(name) {
    std::cout << "Nouvelle entity: '" << name << "'." << std::endl;
    create_component<transform>();
}

std::shared_ptr<transform> entity::get_transform() {
    return get_component<transform>();
}

math::Point entity::get_world_pos() {
    return get_transform()->get_pos();
}

void entity::request_destroy() {
    if(_destroy_has_been_requested)
        return;
    _destroy_has_been_requested = true;
    spdlog::debug("REQUEST: destroy for entity {}.", _name);
    engine::get_engine().request_entity_deletion(this);
}
