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
#include "game/configuration/args_reader.hh"

void setup(guaranteed_ptr<engine::world> world);

int main(int argc, char** argv) {
    auto program_config = game::read_arguments(argc, argv);
    spdlog::info("POC Started.");

    // Initialize window
    program_config.config.screen_title = "POC - Engine";
    engine::sdl::initialize_window(program_config.config);

    // Initialize resources (after window !)
    if(!engine::resources::get_manager().load_path_from_disk(program_config.assets_folder)) {
        spdlog::critical("Failed to load assets directory.");
        engine::sdl::destroy_window();
        return 1;
    }

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
