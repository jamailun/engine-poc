#pragma once

#include "engine/math.hh"

#include <vector>
#include <SDL2/SDL_events.h>

namespace engine {

// fwd
class game_engine;

// Mouse buttons.
namespace button {
    const uint8_t left = 1;
    const uint8_t right = 3;
    const uint8_t middle = 2;
    const uint8_t alt_1 = 4;
    const uint8_t alt_2 = 5;

    const uint8_t _MAX = 6;
};

/**
 * A class to get the inputs of the human being the screen.
 */
class Inputs {
private:
    struct Inputs_ButtonData {
        bool pressed = false;
        uint32_t timestamp = 0;
        int x, y;
    };

    bool _key_pressed [SDL_NUM_SCANCODES] {false};
    Inputs_ButtonData _button_pressed [button::_MAX];
public:
    Inputs();

    void clear();
    void pressed(SDL_Scancode button);
    void unpressed(SDL_Scancode button);

    void mouse_up(uint8_t button);
    void mouse_down(SDL_MouseButtonEvent& mouseEvent);

    /*
    * Test if a key is currently pressed.
    * @return true if the user is pressing the key.
    */
    bool is_key_pressed(SDL_Scancode button) const;

    // Is mouse button currently pressed ?
    bool is_mouse_button_down(uint8_t button) const;

    // Is mouse button currently up ?
    bool is_mouse_button_up(uint8_t button) const;

    // Button down only since a few instants
    bool is_clicking(uint8_t button) const;

    math::Point get_mouse(uint8_t button) const;
    int get_mouse_x(uint8_t button) const;
    int get_mouse_y(uint8_t button) const;
    math::Point get_mouse_in_world(uint8_t button) const;
    math::Point get_mouse_in_world() const;

};

/**
 * Get the input instances, fetched from the GameEngine instance.
 */
const Inputs& get_inputs();

} // namespace engine
