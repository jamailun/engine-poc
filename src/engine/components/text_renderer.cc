#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>
#include <engine/sdl/colors.hh>
#include <engine/components/text_renderer.hh>
#include <engine/components/transform.hh>
#include <engine/resources/resources_manager.hh>

#include <spdlog/spdlog.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <iostream>

using namespace engine;

text_renderer::text_renderer(entity_ptr entity, std::string display_string, int font_size)
    : base_component(entity), _display_string(display_string), _font_size(font_size)
{
    _font = resources::get_manager().load_font("fonts/arial.ttf", font_size);
    if(_font.is_valid()) {
        _text_surface = TTF_RenderText_Shaded(*_font, _display_string.c_str(), colors::red.to_sdl(), colors::blue.to_sdl());
    } else spdlog::error("Could NOT load font {}.", "fonts/arial.ttf");
}

text_renderer::~text_renderer() {
    if(is_valid()) {
        std::cout << "DELETE surface..." << std::endl;
        SDL_FreeSurface(_text_surface);
        std::cout << "CLOSE font..." << std::endl;
        TTF_CloseFont(*_font);
        _font = nullptr;
        std::cout << "deleter done." << std::endl;
    }
}

void text_renderer::render() {
    if(!is_valid()) return;

    math::Point pos = engine::get_engine().world_to_camera(get_entity()->get_world_pos());
    SDL_Rect pos_rect {(int)pos.x, (int)pos.y, 200, 200};
    spdlog::trace("draw at ({},{})", pos_rect.x, pos_rect.y);
    spdlog::trace("> {:p}", (void*)SDL_GetWindowSurface(sdl::get_window().sdl_window()));

    SDL_UpperBlit(_text_surface, nullptr, SDL_GetWindowSurface(sdl::get_window().sdl_window()), &pos_rect);
}

//
template<>
int engine::detail::z_index_of_component<text_renderer>() {
    return 1000;
}
