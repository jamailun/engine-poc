#include <engine/game_camera.hh>

using namespace engine;

void game_camera::set_dimensions(int width, int height) {
    _rect.w = width;
    _rect.h = height;
}

void game_camera::set_position(float x, float y) {
    _rect.x = x;
    _rect.y = y;
}

void game_camera::move(float dx, float dy) {
    _rect.x += dx;
    _rect.y += dy;
}
