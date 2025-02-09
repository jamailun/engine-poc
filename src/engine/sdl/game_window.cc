#include <engine/sdl/game_window.hh>
#include <SDL.h>
#include <SDL_image.h>

#include <spdlog/spdlog.h>
#include <iostream>

using namespace engine::sdl;

game_window::~game_window() {
    sdl_stop_and_destroy();
}

void game_window::sdl_started(configuration config, SDL_Window* window_ptr, SDL_Renderer* renderer_ptr) {
    _config = config;
    _window_ptr = window_ptr;
    _renderer_ptr = renderer_ptr;
    _initialized = true;
    spdlog::debug("Game-window data initialized r={:p}", (void*)_renderer_ptr);
}

void game_window::sdl_stop_and_destroy() {
    if(_initialized) {
        SDL_DestroyRenderer(_renderer_ptr);
        SDL_DestroyWindow(_window_ptr);
        IMG_Quit();
        SDL_Quit();
        _initialized = false;
    }
}

// ---- STATIC

bool engine::sdl::initialize_window(configuration config) {
    if(get_window()._initialized) {
        spdlog::warn("SDL has already been initialized.");
        return true;
    }

    // Global init
    if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
        spdlog::critical("SDL could not be initialized.");
        spdlog::critical("SDL_Error: {}", SDL_GetError());
        return false;
    }

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
    // Disable compositor bypass
    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0")) {
        spdlog::error("SDL can not disable compositor bypass.");
        return false;
    }
#endif

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        config.get_screen_title().c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        config.get_screen_width(), config.get_screen_height(),
        SDL_WINDOW_SHOWN
    );
    if(!window) {
        spdlog::error("SDL Window could not be created.");
        spdlog::error("SDL_Error: {}", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        spdlog::error("SDL Renderer could not be created.");
        spdlog::error("SDL_Error: {}", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Alpha will work
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // register engine
    get_window().sdl_started(config, window, renderer);

    //TODO load ressources ?

    return true;
}

void engine::sdl::destroy_window() {
    get_window().sdl_stop_and_destroy();
}

game_window& engine::sdl::get_window() {
    static game_window window;
    return window;
}

SDL_Renderer* engine::sdl::get_renderer() {
    return get_window().renderer();
}
