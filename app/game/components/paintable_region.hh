#pragma once

#include <engine/component.hh>

#include <stdint.h>
#include <SDL_render.h>

namespace game {

/**
 * A region than can be painted on.
 */
class PaintableRegion : public engine::base_component {
private:
    uint32_t _width;
    uint32_t _height;

    SDL_Texture* _texture;
    SDL_Rect _texture_rect;

    void initialize();

public:
    PaintableRegion(entity_ptr entity);
    PaintableRegion(entity_ptr entity, uint32_t width, uint32_t height);
    ~PaintableRegion();
    
    virtual void render();

};

} // namespace game
