#pragma once

#include "engine/component.hh"
#include "engine/math.hh"

namespace engine {

/**
 * A transform is a must-be component on entities.
 */
class transform : public base_component {
private:
    float _x, _y;
    float _theta;
public:
    transform(entity_ptr entity) : base_component(entity) {}
    ~transform() = default;

    inline float x() const { return _x; }
    inline float y() const { return _y;}
    inline math::Point get_pos() const { return math::Point(_x, _y);}
    inline float theta() const { return _theta; }

    inline void set_x(float x) { _x = x; }
    inline void set_y(float y) { _y = y; }
    inline void set_pos(float x, float y) { _x = x; _y = y; }
    inline void set_pos(math::Point point) { _x = point.x; _y = point.y; }
    inline void set_theta(float theta) { _theta = theta; }

    void move(float dx, float dy);
    inline void move(math::Vector vector) { move(vector.dx, vector.dy); }
    inline void update() {}

    math::Point to_camera_pos() const;
};

} // namespace engine
