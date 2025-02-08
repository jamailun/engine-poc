#pragma once

#include "engine/component.hh"
#include "engine/resources/tile_map.hh"

namespace engine {

    
/**
 * Display an TileMap at the transform position
 */
class tilemap_renderer : public base_component {
private:
    guaranteed_ptr<resources::TileMap> _tilemap;
    SDL_Rect _display_rect;

public:
    tilemap_renderer(entity_ptr entity) : base_component(entity) {}
    tilemap_renderer(entity_ptr entity, std::string tilemap_id) : base_component(entity) {
        initialize(tilemap_id);
    }
    ~tilemap_renderer() = default;
    
    // render call
    void render();

    inline bool is_valid() const {
        return _tilemap.is_valid();
    }

    void initialize(std::string tilemap_id);

};

} // namespace engine
