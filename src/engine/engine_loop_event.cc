#include <engine/engine.hh>
#include <spdlog/spdlog.h>

#include <SDL2/SDL.h>

using namespace engine;

void game_engine::on_loop_event(SDL_Event& event) {
    switch(event.type) {
        // -- Keyboard
        case SDL_KEYDOWN:
            // Register key press
            _inputs.pressed(event.key.keysym.scancode);
            
            // Try to exit
            if(event.key.keysym.sym == SDLK_ESCAPE)
                _running = false;
            break;
        case SDL_KEYUP:
            // Register key release
            _inputs.unpressed(event.key.keysym.scancode);
            break;

        // -- Button
        case SDL_MOUSEBUTTONDOWN:
            _inputs.mouse_down(event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            _inputs.mouse_up(event.button.button);
            break;

        // -- Exit
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
