#include <engine/entity.hh>
#include <engine/math.hh>
#include <engine/world.hh>
#include <engine/components/transform.hh>

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
