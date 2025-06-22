#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>
#include <engine/sdl/colors.hh>
#include <engine/sdl/drawer.hh>
#include <engine/sdl/inputs.hh>
#include <engine/components/ui/ui_button.hh>

#include <spdlog/spdlog.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <iostream>

using namespace engine;

ui_button::ui_button(entity_ptr entity, math::Rect bounds)
    : ui_component(entity)
{
    _screen_bounds = bounds;
}

ui_button::~ui_button() {
    std::cout << "ui_button::deleter done." << std::endl;
}

void ui_button::render() {
    Color color = SDL_GetTicks() - _last_click_time < 100 ? _clicked_color : _color;
    engine::drawer::enable_color(color);

    // Draw rect
    SDL_FRect sdl_rect = _screen_bounds.to_sdl();
    SDL_RenderFillRectF(sdl::get_renderer(), &sdl_rect);
}

void ui_button::update(float) {
    // Only care about left click.
    if( ! engine::get_inputs().is_clicking(engine::button::left)) return;
    math::Point mouse_pos = engine::get_inputs().get_mouse(engine::button::left);
    if(!_screen_bounds.contains(mouse_pos)) return;
    _last_click_time = SDL_GetTicks();

    for(auto& c : _actions) c();
}


//
template<>
int engine::detail::z_index_of_component<ui_button>() {
    return 1100;
}
