#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>
#include <engine/sdl/colors.hh>
#include <engine/components/ui/ui_text.hh>
#include <engine/resources/resources_manager.hh>

#include <spdlog/spdlog.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <iostream>

using namespace engine;

void ui_text::render_font() {
    guaranteed_ptr<TTF_Font> font = resources::get_manager().load_font("fonts/arial.ttf", _font_size);
    _valid = font.is_valid();
    if(!font.is_valid()) {
        spdlog::error("Could not render text: font is invalid.");
        return;
    }

    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(*font, _display_string.c_str(), _font_color.to_sdl());
    _text_texture = SDL_CreateTextureFromSurface(sdl::get_renderer(), textSurface);
    _display_rect.w = textSurface->w;
    _display_rect.h = textSurface->h;
    SDL_FreeSurface(textSurface);

    TTF_CloseFont(*font);
}

ui_text::ui_text(entity_ptr entity, std::string display_string, int font_size, Color color)
    : ui_component(entity), _display_string(display_string), _font_size(font_size), _font_color(color)
{
    render_font();
}

ui_text::~ui_text() {
    if(_text_texture != nullptr) {
        std::cout << "Delete texture " << _text_texture << "." << std::endl;
        SDL_DestroyTexture(_text_texture);
    }
    std::cout << "ui_text::deleter done." << std::endl;
}

void ui_text::render() {
    if(!is_valid()) return;

    _display_rect.x = (int) screen_position().x;
    _display_rect.y = (int) screen_position().y;

    SDL_RenderCopy(sdl::get_renderer(), _text_texture, nullptr, &_display_rect);
}

void ui_text::set_text(const std::string& text) {
    _display_string = text;
    render_font();
}


//
template<>
int engine::detail::z_index_of_component<ui_text>() {
    return 1000;
}
