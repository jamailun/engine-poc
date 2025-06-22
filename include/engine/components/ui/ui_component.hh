#pragma once

#include "engine/guaranteed_ptr.hh"
#include "engine/component.hh"
#include "engine/math.hh"

namespace engine {

/**
 * A UI component will have a screen_position and a z-index. The entity position is thus not used.
 */
class ui_component : public base_component {
private:
    int _z_index = 1000;
    math::Point _screen_position;
protected:
    virtual void position_changed() {/* Can be overwritten */}
public:
    ui_component(entity_ptr entity) : base_component(entity) {}
    ui_component(entity_ptr entity, math::Point screen_position) : base_component(entity), _screen_position(screen_position) {}
    virtual ~ui_component() = default;
    
    // render call
    virtual void render() = 0;

    int z_index() const { return _z_index; }
    void z_index(int z_index) { _z_index = z_index; }
    math::Point screen_position() const { return _screen_position; }
    void screen_position(math::Point position) {
        _screen_position = position;
        position_changed();
    }
};

} // namespace engine
