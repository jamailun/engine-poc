#pragma once

#include "engine/components/ui/ui_component.hh"
#include "engine/guaranteed_ptr.hh"
#include "engine/sdl/colors.hh"

namespace engine {

/**
 * Display a text at screen position.
 */
class ui_button : public ui_component {
private:
    time_t _last_click_time = 0;
    Color _color = colors::alpha_white;
    Color _clicked_color = colors::alpha_gray;

public:
    ui_button(entity_ptr entity, math::Rect bounds);
    ~ui_button();
    
    void render() override;
    void update(float elapsed);

    inline time_t get_last_clicked_time() const {
        return _last_click_time;
    }
};

} // namespace engine
