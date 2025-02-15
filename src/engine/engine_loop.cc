#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>

#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>
#include <engine/utils/timer.hh>

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
    system_timer update_timer;
    system_timer fps_timer;
    update_timer.start();
    fps_timer.start();
    size_t frames = 0;
    while(_running) {
        // Poll events from window
        while (SDL_PollEvent(&event))
            on_loop_event(event);

        // render + update
        // done by thread on_loop_update();
        on_loop_render();
        float elapsed = update_timer.elapsed_secs();
        if(elapsed >= 0.016f) {
            // spdlog::trace("elapsed = {:4}s", elapsed);
            on_loop_update(elapsed);
            update_timer.lap();
        }
        
        // Update screen (buffer swapping)
        SDL_RenderPresent(sdl::get_renderer());
        frames++;
        if(fps_timer.elapsed_secs() > _fps_frequency) {
            _fps = frames / _fps_frequency;
            spdlog::trace("FPS = {}", _fps);
            frames = 0;
            fps_timer.lap();
        }

        // Chores
        handle_queue__entity_delete();
    }
    on_loop_shutdown();
    sdl::destroy_window();
    spdlog::info("GameEngine stopped.");
}
