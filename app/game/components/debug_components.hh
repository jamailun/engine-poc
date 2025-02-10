#pragma once

#include <iostream>

#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>
#include <engine/sdl/drawer.hh>
#include <engine/resources/resources_manager.hh>
#include <engine/components/transform.hh>
#include <engine/components/image_renderer.hh>

#include <spdlog/spdlog.h>

class component_wif : public engine::base_component {
public:
    component_wif(entity_ptr entity) : base_component(entity) {}
    void render() {
        if(auto transform = get_entity()->get_component<engine::transform>()) {
            engine::drawer::draw_rect_center(transform->get_pos(), 25, 25, engine::colors::yellow);
        }
    }
    void update(float elapsed) {
        if(auto transform = get_entity()->get_component<engine::transform>()) {
            transform->move(2 * elapsed, -1 * elapsed);
        }
    }
};

class suicide_components : public engine::base_component {
public:
    suicide_components(entity_ptr entity) : base_component(entity) {}
    void update(float) {
        const auto& inputs = engine::get_inputs();
        if(inputs.is_key_pressed(SDL_Scancode::SDL_SCANCODE_DELETE)) {
            spdlog::debug("pressed delete.");
            get_entity()->request_destroy();
        } else 
        if(inputs.is_key_pressed(SDL_Scancode::SDL_SCANCODE_X)) {
            spdlog::debug("pressed X.");
            get_entity()->destroy_component<suicide_components>();
        }
        // spdlog::debug("just a chill input.");
    }
};
