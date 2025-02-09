#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/components/transform.hh>
#include <engine/sdl/game_window.hh>

#include "./paintable_region.hh"

#include <spdlog/spdlog.h>

using namespace game;

PaintableRegion::PaintableRegion(entity_ptr entity) : PaintableRegion(entity, 256, 256) {}
PaintableRegion::PaintableRegion(entity_ptr entity, uint32_t width, uint32_t height)
    : engine::base_component(entity), _width(width), _height(height)
{
    initialize();
}

PaintableRegion::~PaintableRegion() {
    SDL_DestroyTexture(_texture);
}

void PaintableRegion::initialize() {
    // Create texture
    _texture = SDL_CreateTexture(
        engine::sdl::get_renderer(),
        SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888,
        SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET,
        _width, _height
    );
    _texture_rect = { 0, 0, (int) _width, (int) _height };

    // Fill with green
    SDL_SetRenderTarget(engine::sdl::get_renderer(), _texture);
    SDL_SetRenderDrawColor(engine::sdl::get_renderer(), 30, 220, 45, 255);
    SDL_RenderFillRect(engine::sdl::get_renderer(), &_texture_rect);

    // reset target
    SDL_SetRenderTarget(engine::sdl::get_renderer(), nullptr);
}

void PaintableRegion::render() {
    // Camera position to write texture
    engine::math::Point pos = engine::get_engine().world_to_camera(get_entity()->get_world_pos());
    SDL_Rect draw_rect = {
        (int) pos.x,
        (int) pos.y,
        (int) _width, (int) _height
    };

    // Write texture
    SDL_RenderCopy(
        engine::sdl::get_renderer(),
        _texture,
        &_texture_rect,
        &draw_rect
    );
}

//
template<>
int engine::detail::z_index_of_component<PaintableRegion>() {
    return 1;
}
