#pragma once

#include <memory>
#include <functional>
#include <engine/utils/timer.hh>
#include <engine/utils/memory.hh>
#include <engine/component.hh>

namespace game {

/**
 *  A text-renderer with a supplier.
 */
class ClockComponent : public engine::base_component {
private:
    callback _callback;
    float _frequency;
    float _elapsed = 0;
public:
    ClockComponent(entity_ptr entity, const callback& cb, float frequency)
        : engine::base_component(entity), _callback(cb), _frequency(frequency)
    {}
    ~ClockComponent() = default;

    virtual void update(float);
};

} // namespace game
