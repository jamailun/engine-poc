#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/components/transform.hh>
#include <engine/sdl/drawer.hh>
#include <engine/sdl/game_window.hh>

#include "game/army.hh"
#include "game/game_state.hh"
#include "game/components/army_controller.hh"
#include "game/components/soldiers/soldier_cac.hh"

#include <cmath>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

using namespace game;

ArmyController::ArmyController(entity_ptr entity, guaranteed_ptr<Army> army)
    : engine::base_component(entity), _army(army)
{}

static guaranteed_ptr<Army> get_player_army() {
    return game::get_state().get_player_army();
}

void select_region(engine::math::Rect selected_rect) {
    get_player_army()->clear_selection();
    for(auto soldier : get_player_army()->get_soldiers()) {
        if( selected_rect.contains(soldier->get_entity()->get_transform()->get_pos()) ) {
            //spdlog::trace("SELECT: {}", soldier->get_entity()->get_name());
            get_player_army()->select_soldier(soldier);
        }
    }
}


void ArmyController::update_selection_rect() {
    const engine::Inputs& inputs = engine::get_inputs();

    if(inputs.is_mouse_button_down(engine::button::left)) {
        // Start ?
        if( ! _selection_is_selecting) {
            _selection_is_selecting = true;
            _selection_first = inputs.get_mouse_in_world(engine::button::left);
        }
        // rect pos
        _selection_outline_rect = engine::math::Rect::between(_selection_first, inputs.get_mouse_in_world());
    } else {
        // End ?
        if(_selection_is_selecting) {
            select_region(_selection_outline_rect);
            _selection_is_selecting = false;
        }
    }
}

void ArmyController::render() {
    // Selection rect
    if(_selection_is_selecting) {
        engine::drawer::fill_rect(_selection_outline_rect, engine::colors::alpha_white);
    }
}

void ArmyController::update(float) {
    // selection
    update_selection_rect();
    
    // other
    const engine::Inputs& inputs = engine::get_inputs();
    
    // create a soldier
    if(inputs.is_clicking(engine::button::alt_1)) {
        engine::math::Point pos = inputs.get_mouse_in_world(engine::button::alt_1);
        get_player_army()->create_soldier_cac(pos);
    } else if(inputs.is_key_pressed(SDL_Scancode::SDL_SCANCODE_P)) {
        engine::math::Point pos = inputs.get_mouse_in_world();
        get_player_army()->create_soldier_cac(pos);
    }

    if(inputs.is_key_pressed(SDL_Scancode::SDL_SCANCODE_O)) {
        static int rem = 3;
        if(rem > 0) {
            rem--;
            engine::math::Point pos = inputs.get_mouse_in_world();
            get_player_army()->create_soldier_cac(pos);
        }
    }

    // create command
    if(inputs.is_clicking(engine::button::right)) {
        engine::math::Point pos = inputs.get_mouse_in_world(engine::button::right);
        get_player_army()->create_command_with_selection(pos);
    }
}

//
template<>
int engine::detail::z_index_of_component<ArmyController>() {
    return 1000;
}
