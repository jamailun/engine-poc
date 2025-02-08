#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/components/tilemap_renderer.hh>
#include <engine/components/transform.hh>
#include <engine/resources/resources_manager.hh>

#include <spdlog/spdlog.h>

using namespace engine;

void tilemap_renderer::render() {
    if(!_tilemap.is_valid()) return;
    
    math::Point pos = engine::get_engine().world_to_camera(get_entity()->get_world_pos());
    _tilemap->draw_call(pos.x, pos.y);
}

void tilemap_renderer::initialize(std::string tm_name) {
    _tilemap = engine::resources::get_manager().find_tile_map(tm_name);
    if(!_tilemap.is_valid()) spdlog::error("Invalid TileMap. TileMapRenderer could not locate TileMap '{}'.", tm_name);
}
