#pragma once

#include "engine/components/ui/ui_component.hh"
#include "engine/sdl/colors.hh"
#include "engine/guaranteed_ptr.hh"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace engine {

/**
 * Display a text at screen position.
 */
class ui_text : public ui_component {
private:
    std::string _display_string;
    SDL_Rect _display_rect;
    int _font_size;
    Color _font_color;
    bool _valid = false;
    SDL_Texture* _text_texture = nullptr;

    void render_font();

public:
    ui_text(entity_ptr entity, std::string display_string, int font_size, Color color);
    ui_text(entity_ptr entity, std::string display_string, int font_size) : ui_text(entity, display_string, font_size, colors::white) {}
    ui_text(entity_ptr entity, std::string display_string) : ui_text(entity, display_string, 12) {}
    ui_text(entity_ptr entity) : ui_text(entity, "") {}
    ~ui_text();
    
    // render call
    void render();
    void set_text(const std::string& text);

    bool is_valid() const { return _valid; }
};

} // namespace engine
