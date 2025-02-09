#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/components/transform.hh>
#include <engine/sdl/inputs.hh>

#include "./key_controller.hh"

using namespace game;

void KeyController::update() {
    engine::math::Vector dir;

    const engine::Inputs& inputs = engine::get_inputs();
    if(inputs.is_key_pressed(SDL_SCANCODE_RIGHT))
        dir.dx += 1;
    if(inputs.is_key_pressed(SDL_SCANCODE_LEFT))
        dir.dx -= 1;
    if(inputs.is_key_pressed(SDL_SCANCODE_UP))
        dir.dy -= 1;
    if(inputs.is_key_pressed(SDL_SCANCODE_DOWN))
        dir.dy += 1;
    
    get_entity()->get_transform()->move(dir.normalized() * _speed);
}
