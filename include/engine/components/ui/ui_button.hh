#pragma once

#include "engine/components/ui/ui_component.hh"
#include "engine/guaranteed_ptr.hh"
#include "engine/utils/memory.hh"
#include "engine/sdl/colors.hh"

#include <vector>

namespace engine {

/**
 * Display a text at screen position.
 */
class ui_button : public ui_component {
private:
    time_t _last_click_time = 0;
    Color _color = colors::alpha_white;
    Color _clicked_color = colors::alpha_gray;

    std::vector<callback> _actions;

public:
    ui_button(entity_ptr entity, math::Rect bounds);
    ~ui_button();
    
    void render() override;
    void update(float);

    inline time_t get_last_clicked_time() const { return _last_click_time;}
    inline void set_color(Color color) { _color = color; }
    inline void set_clicked_color(Color color) { _clicked_color = color; }
    inline Color get_color() const { return _color; }
    inline Color get_clicked_color() const { return _clicked_color; }

    inline void add_action(callback action) { _actions.push_back(action); }
};

} // namespace engine
