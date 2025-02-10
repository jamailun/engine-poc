#pragma once

#include <engine/component.hh>

namespace game {

/**
 *  Move the linked entity using directional arrows.
 */
class KeyController : public engine::base_component {
private:
    float _speed;

public:
    KeyController(entity_ptr entity) : engine::base_component(entity), _speed(1) {}
    KeyController(entity_ptr entity, float speed) : engine::base_component(entity), _speed(speed) {}
    ~KeyController() = default;

    inline float speed() const { return _speed; }
    inline void speed(float speed) { _speed = speed; }

    virtual void update(float elapsed);
};

} // namespace game
