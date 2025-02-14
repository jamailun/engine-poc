#pragma once

#include "engine/component.hh"
#include "engine/guaranteed_ptr.hh"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace engine {

/**
 * Display a text at transform position.
 */
class text_renderer : public base_component {
private:
    std::string _display_string;
    SDL_Rect _display_rect;
    int _font_size;
    guaranteed_ptr<TTF_Font> _font = nullptr;
    SDL_Surface* _text_surface = nullptr;

    int _z_index = 30;

public:
    text_renderer(entity_ptr entity, std::string display_string, int font_size);
    text_renderer(entity_ptr entity, std::string display_string) : text_renderer(entity, display_string, 12) {}
    text_renderer(entity_ptr entity) : text_renderer(entity, "") {}
    ~text_renderer();
    
    // render call
    void render();

    int z_index() const { return _z_index; }
    void z_index(int z_index) { _z_index = z_index; }

    bool is_valid() const { return _font.is_valid(); }
};

} // namespace engine
