#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>
#include <engine/components/image_renderer.hh>
#include <engine/components/transform.hh>
#include <engine/resources/resources_manager.hh>

#include <spdlog/spdlog.h>
#include <SDL_render.h>
#include <sstream>

using namespace engine;

void image_renderer::render() {
    if(!_image.is_valid()) return;
    math::Point pos = engine::get_engine().world_to_camera(get_entity()->get_world_pos());

    SDL_FRect draw_rect = _image->create_rect(pos.x, pos.y);
    SDL_RenderCopyF(engine::sdl::get_renderer(), _image->sdl_texture(), nullptr, &draw_rect);
}

void image_renderer::initialize(std::string image_id) {
    _image = engine::resources::get_manager().find_image(image_id);
    if(!_image.is_valid()) spdlog::error("Invalid Image. ImageRenderer could not locate Image '{}'.", image_id);
}

//
template<>
int engine::detail::z_index_of_component<image_renderer>() {
    return 20;
}
