#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>

#include <spdlog/spdlog.h>
#include <SDL.h>

using namespace engine;

void game_engine::start() {
    if(_running) {
        spdlog::error("Cannot start game-engine : already running !");
        return;
    }
    if(!sdl::get_window().initialized()) {
        spdlog::error("Cannot start game-engine : initialize game-window first !");
        return;
    }
    _running = true;
    on_loop_initialize();

    SDL_Event event;
    spdlog::info("GameEngine started.");
    while(_running) {
        // Poll events from window
        while (SDL_PollEvent(&event))
            on_loop_event(event);

        // render + update
        on_loop_update();
        on_loop_render();
        
        // Update screen (buffer swapping)
        SDL_RenderPresent(sdl::get_renderer());

        // Chores
        handle_queue__entity_delete();
    }
    on_loop_shutdown();
    sdl::destroy_window();
    spdlog::info("GameEngine stopped.");
}
