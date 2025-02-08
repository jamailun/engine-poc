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
    void update() {
        if(auto transform = get_entity()->get_component<engine::transform>()) {
            transform->move(2, -1);
        }
    }
};

class suicide_components : public engine::base_component {
public:
    suicide_components(entity_ptr entity) : base_component(entity) {}
    void update() {
        const auto& inputs = engine::get_inputs();
        if(inputs.is_key_pressed(SDL_Scancode::SDL_SCANCODE_DELETE)) {
            spdlog::debug("pressed delete.");
            get_entity()->request_destroy();
        }
        // spdlog::debug("just a chill input.");
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

    // Initialize resources (after window !)
    engine::resources::get_manager().load_path_from_disk("/opt/perso-cpp/engine-poc/assets");

    // Initialize world
    engine::get_engine().register_setup_operation([](guaranteed_ptr<engine::world> w){setup(w);});

    // Start engine
    engine::get_engine().start();
}


void setup(guaranteed_ptr<engine::world> world) {
    std::shared_ptr<engine::entity> toto = world->create_entity("toto");
    toto->create_component<component_wif>();

    std::shared_ptr<engine::entity> titi = world->create_entity("titi");
    titi->create_component<engine::image_renderer>("sprite1.png");
    titi->create_component<suicide_components>();
}
