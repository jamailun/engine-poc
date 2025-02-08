#include <iostream>

#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>

#include <spdlog/spdlog.h>

class component_pos : public engine::base_component {
public:
    float x, y;
    explicit component_pos(entity_ptr entity) : base_component(entity), x(0), y(0) {}
    explicit component_pos(entity_ptr entity, float x, float y) : base_component(entity), x(x), y(y) {}
    void update() {}
};


class component_wif : public engine::base_component {
public:
    component_wif(entity_ptr entity) : base_component(entity) {}
    void render() {
        auto pos = get_entity()->get_component<component_pos>();
        if(pos) {
            spdlog::info("wif.render. Pos-component is at ({}, {}).", pos->x, pos->y);
            spdlog::info("-- entity/world = {}.", get_entity()->get_world()->name());
        }
    }
    void update() {
        auto pos = get_entity()->get_component<component_pos>();
        if(pos) {
            pos->x += 2;
            pos->y -= 1;
        }
    }
};

int main() {
    spdlog::set_level(spdlog::level::trace);
    
    spdlog::info("POC Started.");

    // Initialize window
    engine::sdl::configuration_builder builder {};
    builder.screen_title = "test";
    engine::sdl::initialize_window(builder);

    // Start engine
    engine::get_engine().start();


    spdlog::info("POC Terminated.");
}





// void before() {
//     std::shared_ptr<engine::entity> object = engine::get_engine().create_entity("toto");

//     object->create_component<component_wif>();
//     object->create_component<component_pos>(12, 12);

//     // "game-loop"
//     for(size_t i = 0; i < 4; i++) {
//         spdlog::info("------------({})------------", i);
//         engine::get_engine().on_update();
//         engine::get_engine().on_render();
//     }
//     spdlog::info("---------------------------");
// }
