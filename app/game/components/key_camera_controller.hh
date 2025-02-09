#pragma once

#include <engine/component.hh>

namespace game {

/**
 * Once affected to an entity, will move the camera.
 */
class KeyCameraController : public engine::base_component {
private:
    float _speed;

public:
    KeyCameraController(entity_ptr entity) : engine::base_component(entity), _speed(1) {}
    KeyCameraController(entity_ptr entity, float speed) : engine::base_component(entity), _speed(speed) {}
    ~KeyCameraController() = default;

    inline float speed() const { return _speed; }
    inline void speed(float speed) { _speed = speed; }

    virtual void update();
};

} // namespace game
