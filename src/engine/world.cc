#include <engine/world.hh>

using namespace engine;

//TODO optimize this using a quad tree / regions.
std::vector<std::shared_ptr<entity>> world::get_entities_around(math::Point position, float distance) {
    float sqr_dist = distance * distance;
    std::vector<std::shared_ptr<entity>> vec;
    for(auto entity : _entities) {
        if(position.squared_distance(entity->get_world_pos()) <= sqr_dist) {
            vec.push_back(entity);
        }
    }
    return vec;
}