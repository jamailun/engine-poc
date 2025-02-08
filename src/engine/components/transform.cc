#include <engine/components/transform.hh>
#include <engine/engine.hh>

using namespace engine;

void transform::move(float dx, float dy) {
    _x += dx;
    _y += dy;
}

engine::math::Point transform::to_camera_pos() const {
    return engine::get_engine().world_to_camera(_x, _y);
}

