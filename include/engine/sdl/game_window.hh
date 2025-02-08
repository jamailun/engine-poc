#pragma once

#include <SDL2/SDL_render.h>
#include "engine/sdl/configuration.hh"

namespace engine {
namespace sdl {

class game_window {
    friend bool initialize_window(configuration config);
    friend void destroy_window();
private:
    SDL_Window* _window_ptr;
    SDL_Renderer* _renderer_ptr;
    bool _initialized = false;
    configuration _config;

    void sdl_started(configuration config, SDL_Window* window_ptr, SDL_Renderer* renderer_ptr);
    void sdl_stop_and_destroy();

public:
    game_window() = default;
    ~game_window();

    SDL_Renderer* renderer() { return _renderer_ptr; }
    SDL_Window* sdl_window() { return _window_ptr; }
    bool initialized() const { return _initialized; }
    const configuration& get_configuration() const { return _config; }
};

// Initialize SDL
bool initialize_window(configuration config);
void destroy_window();

// Get the reference to the instance.
game_window& get_window();
SDL_Renderer* get_renderer();

} // namespace sdl
} // namespace engine
