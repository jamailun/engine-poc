#include <engine/sdl/inputs.hh>
#include <engine/engine.hh>

#include <algorithm>
#include <spdlog/spdlog.h>
#include <SDL.h>

using namespace engine;

// PRIVATE

Inputs::Inputs() {
    for(int i = 0; i < button::_MAX; i++) {
        _button_pressed[i].pressed = false;
        _button_pressed[i].timestamp = 0;
        _button_pressed[i].x = 0;
        _button_pressed[i].y = 0;
    }
}

void Inputs::clear() {
    std::fill(std::begin(_key_pressed), std::end(_key_pressed), false);
}

void Inputs::pressed(SDL_Scancode button) {
    _key_pressed[button] = true;
}

void Inputs::unpressed(SDL_Scancode button) {
    _key_pressed[button] = false;
}

void Inputs::mouse_up(uint8_t button) {
    _button_pressed[button].pressed = false;
}

void Inputs::mouse_down(SDL_MouseButtonEvent& mouseEvent) {
    uint8_t button = mouseEvent.button;
    // spdlog::trace("inputs::mouse_down({})", button);
    _button_pressed[button].pressed = true;
    _button_pressed[button].timestamp = mouseEvent.timestamp;
    _button_pressed[button].x = mouseEvent.x;
    _button_pressed[button].y = mouseEvent.y;
}

// PUBLIC

bool Inputs::is_key_pressed(SDL_Scancode button) const {
    return _key_pressed[button];
}

bool Inputs::is_mouse_button_down(uint8_t button) const {
    // spdlog::trace("Inputs::is_button_down: {}.", button);
    assert(button < button::_MAX);
    return _button_pressed[button].pressed;
}

bool Inputs::is_mouse_button_up(uint8_t button) const {
    return ! _button_pressed[button].pressed;
}

bool Inputs::is_clicking(uint8_t button) const {
    return is_mouse_button_down(button) && SDL_GetTicks() - _button_pressed[button].timestamp < 5;
}


int Inputs::get_mouse_x(uint8_t button) const {
    return _button_pressed[button].x;
}

int Inputs::get_mouse_y(uint8_t button) const {
    return _button_pressed[button].y;
}

math::Point Inputs::get_mouse_in_world(uint8_t button) const {
    return get_engine().camera_to_world(_button_pressed[button].x, _button_pressed[button].y);
}

math::Point Inputs::get_mouse_in_world() const {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return get_engine().camera_to_world(x, y);
}

// STATIC

const Inputs& engine::get_inputs() {
    return get_engine().inputs();
}
