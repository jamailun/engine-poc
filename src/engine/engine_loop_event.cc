#include <engine/engine.hh>
#include <spdlog/spdlog.h>

#include <SDL.h>

using namespace engine;

void game_engine::on_loop_event(SDL_Event& event) {
    switch(event.type) {
        // -- Keyboard
        case SDL_KEYDOWN:
            // Try to exit
            if(event.key.keysym.sym == SDLK_ESCAPE)
                _running = false;
            break;
        // Exit
        case SDL_QUIT:
            _running = false;
            break;
        case SDL_WINDOWEVENT:
            switch(event.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    _running = false;
                    break;
                }
            break;
        default: break;
    }
}
