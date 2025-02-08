#pragma once

#include "engine/math.hh"

namespace engine {

class game_camera {
private:
    math::Rect _rect { 0, 0, 0, 0};
public:
    void set_dimensions(int width, int height);
    inline void move_to_center_screen() {
        set_position(-_rect.w * 0.5, -_rect.h * 0.5);
    }
    void set_position(float x, float y);

    void move(float dx, float dy);

    inline float x() const { return _rect.x; }
    inline float y() const { return _rect.y; }
};

} // namespace engine
