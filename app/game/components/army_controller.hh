#pragma once

#include <engine/component.hh>
#include <engine/entity.hh>
#include <engine/guaranteed_ptr.hh>
#include <engine/math.hh>
#include <engine/utils/time_barrier.hh>

namespace game {

//fwd
class Army;

/**
 * Army controller.
 */
class ArmyController : public engine::base_component {
private:
    enum SelectMode {
        SELECT_AND_COMMAND,
        PAINT
    } _mode = SELECT_AND_COMMAND;
    engine::time_barrier _br_select_changed;

    guaranteed_ptr<Army> _army;

    // Selection management
    bool _selection_is_selecting = false;
    engine::math::Point _selection_first;
    engine::math::Rect _selection_outline_rect;
    void update_selection_rect();

public:
    ArmyController(entity_ptr entity, guaranteed_ptr<Army> army);
    ~ArmyController() = default;

    virtual void update(float);
    virtual void render();

    bool is_mode_paint() const {
        return _mode == PAINT;
    }
    bool is_mode_select_and_command() const {
        return _mode == SELECT_AND_COMMAND;
    }
};

} // namespace game
