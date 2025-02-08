
#include <engine/entity.hh>
#include <engine/world.hh>
#include <engine/components/transform.hh>

using namespace engine;

entity::entity(guaranteed_ptr<world> world, std::string name) : _world(world), _name(name) {
    std::cout << "Nouvelle entity: '" << name << "'." << std::endl;
    create_component<transform>();
}