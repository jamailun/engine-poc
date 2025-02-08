#include <iostream>

#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>
#include <engine/sdl/drawer.hh>
#include <engine/components/transform.hh>

#include <spdlog/spdlog.h>

class component_wif : public engine::base_component {
public:
    component_wif(entity_ptr entity) : base_component(entity) {}
    void render() {
        if(auto transform = get_entity()->get_component<engine::transform>()) {
            engine::drawer::draw_rect_center(transform->get_pos(), 25, 25, engine::colors::yellow);
        }
    }
    void update() {
        if(auto transform = get_entity()->get_component<engine::transform>()) {
            transform->move(2, -1);
        }
    }
};

void setup(guaranteed_ptr<engine::world> world);

int main() {
    spdlog::set_level(spdlog::level::trace);
    spdlog::info("POC Started.");

    // Initialize window
    engine::sdl::configuration_builder builder {};
    builder.screen_title = "test";
    engine::sdl::initialize_window(builder);

    // Initialize world
    engine::get_engine().register_setup_operation([](guaranteed_ptr<engine::world> w){setup(w);});

    // Start engine
    engine::get_engine().start();
}


void setup(guaranteed_ptr<engine::world> world) {
    std::shared_ptr<engine::entity> object = world->create_entity("toto");
    object->create_component<component_wif>();
}
