#include <iostream>

#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>
#include <engine/sdl/drawer.hh>
#include <engine/resources/resources_manager.hh>
#include <engine/components/transform.hh>
#include <engine/components/image_renderer.hh>

#include <spdlog/spdlog.h>

#include "game/components/debug_components.hh"



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
