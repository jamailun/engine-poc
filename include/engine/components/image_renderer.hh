#pragma once

#include "engine/component.hh"
#include "engine/resources/image.hh"

namespace engine {

/**
 * Display an Image at the transform position
 */
class image_renderer : public base_component {
private:
    guaranteed_ptr<resources::Image> _image;
    SDL_Rect _display_rect;

    int _z_index = 20;

public:
    image_renderer(entity_ptr entity) : base_component(entity) {}
    image_renderer(entity_ptr entity, std::string image_id) : base_component(entity) {
        initialize(image_id);
    }
    ~image_renderer() = default;
    
    // render call
    void render();

    int z_index() const { return _z_index; }
    void z_index(int z_index) { _z_index = z_index; }

    inline bool is_valid() const {
        return _image.is_valid();
    }

    void initialize(std::string image_id);

};

} // namespace engine
