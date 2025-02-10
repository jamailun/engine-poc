#pragma once

#include <engine/component.hh>
#include <engine/entity.hh>
#include <engine/guaranteed_ptr.hh>
#include <engine/math.hh>

namespace game {

//fwd
class Army;

/**
 * Army controller.
 */
class ArmyController : public engine::base_component {
private:
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
};

} // namespace game
