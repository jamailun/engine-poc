#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/sdl/inputs.hh>

#include "./key_camera_controller.hh"

using namespace game;

void KeyCameraController::update() {
    float dx = 0;
    float dy = 0;

    const engine::Inputs& inputs = engine::get_inputs();
    if(inputs.is_key_pressed(SDL_SCANCODE_A))
        dx -= 1;
    if(inputs.is_key_pressed(SDL_SCANCODE_D))
        dx += 1;
    if(inputs.is_key_pressed(SDL_SCANCODE_W))
        dy -= 1;
    if(inputs.is_key_pressed(SDL_SCANCODE_S))
        dy += 1;
    
    engine::get_engine().camera().move(dx, dy);
}
