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
protected:
    math::Rect _screen_bounds;
    virtual void position_changed() {/* Can be overwritten */}
public:
    ui_component(entity_ptr entity) : base_component(entity) {}
    ui_component(entity_ptr entity, math::Point screen_position) : base_component(entity), _screen_bounds(screen_position.to_rect(0,0)) {}
    virtual ~ui_component() = default;
    
    // render call
    virtual void render() = 0;

    int z_index() const { return _z_index; }
    void z_index(int z_index) { _z_index = z_index; }
    math::Rect screen_bounds() const { return _screen_bounds; }
    math::Point screen_position() const { return _screen_bounds.top_left(); }
    void screen_position(math::Point position) {
        _screen_bounds.x = position.x;
        _screen_bounds.y = position.y;
        position_changed();
    }
    void screen_bounds(math::Rect bounds) {
        _screen_bounds = bounds;
        position_changed();
    }
};

} // namespace engine
