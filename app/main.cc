#include <iostream>

#include <engine/entity.hh>
#include <engine/engine.hh>
#include <engine/guaranteed_ptr.hh>
#include <engine/sdl/game_window.hh>
#include <engine/sdl/drawer.hh>
#include <engine/resources/resources_manager.hh>
#include <engine/components/text_renderer.hh>
#include <engine/components/transform.hh>
#include <engine/components/image_renderer.hh>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include "game/game_state.hh"
#include "game/components/debug_components.hh"
#include "game/components/clock_component.hh"
#include "game/components/fps_displayer.hh"
#include "game/components/key_controller.hh"
#include "game/components/key_camera_controller.hh"
#include "game/components/army_controller.hh"
#include "game/components/paintable_region.hh"
#include "game/configuration/args_reader.hh"

#include <engine/utils/quadtree.hh>

    static size_t NEXT_ID = 0;
class Foo {
private:
    using Pt = engine::math::Point;
    Pt _pos;
    size_t _id;
public:
    Foo(float x, float y) : _pos(x, y), _id(NEXT_ID++) {}
    Foo() : Foo(0, 0) {}

    Pt get_pos() const {
        return _pos;
    }
    void set_pos(Pt pos) {
       _pos = pos;
    }
    bool operator==(const Foo& foo) {
        return _id == foo._id;
    }
    size_t id() const {
        return _id;
    }
};

void quad() {
    using QuadTree = engine::quad_tree<Foo>;

    QuadTree root ([](const Foo& foo) { return foo.get_pos(); }, engine::math::Rect(0, 0, 64, 64), 2, 4);
    Foo a = Foo(12, 12);
    Foo b = Foo(12, 12);
    Foo c = Foo(4, 4);
    Foo d = Foo(12, 12);
    root.insert(a);
    root.insert(b);
    root.insert(Foo(12, 11));
    root.insert(c);
    root.insert(d);
    spdlog::info("ROOT (size={}) =\n{}", root.size(), root.to_string());


    root.remove(a);
    spdlog::info("ROOT (size={}) =\n{}", root.size(), root.to_string());

    auto query = root.query(engine::math::Rect(1, 1, 12, 11));
    spdlog::warn("QUERY. Found = {} elem.", query.size());
    for(auto& foo : query) {
        spdlog::info("Query: {}", foo.id());
    }

    root.remove(b);
    root.remove(c);
    root.remove(d);
    spdlog::info("ROOT (size={}) =\n{}", root.size(), root.to_string());
}

void setup(guaranteed_ptr<engine::world> world);
void setup_test(guaranteed_ptr<engine::world> world);

game::argument_read_output program_config;

int main(int argc, char** argv) {
    // quad();
    // if(1 < 4) return 0;

    program_config = game::read_arguments(argc, argv);
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
    // engine::get_engine().register_setup_operation([](guaranteed_ptr<engine::world> w){setup_test(w);});

    // Start engine
    engine::get_engine().start();
}


void setup(guaranteed_ptr<engine::world> world) {
    auto world_dim = engine::get_engine().camera().get_dimensions();
    int width = program_config.config.screen_width;
    int height = program_config.config.screen_height;

    auto terrain = world->create_entity("terrain");
    terrain->create_component<game::PaintableRegion>(width, height);
    terrain->create_component<game::ArmyController>(game::get_state().get_player_army());
    terrain->create_component<game::KeyCameraController>();
    terrain->get_transform()->set_pos(-width/2, -height/2);

    // FPS
    auto fps_ui = world->create_entity("fps-display");
    auto fps_ui_txt = fps_ui->create_component<engine::text_renderer>("<>", 20);
    fps_ui->get_transform()->set_pos(-world_dim.w/2, -world_dim.h/2);
    fps_ui->create_component<game::ClockComponent>(
        [fps_ui_txt](){
            size_t fps = engine::get_engine().fps();
            fps_ui_txt->set_text(fmt::format("FPS: {}", fps));
        },
        0.5f
    );

    // Entity displayer
    auto ec_ui = world->create_entity("entity-count");
    auto ec_ui_txt = ec_ui->create_component<engine::text_renderer>("<>", 20);
    ec_ui->get_transform()->set_pos(-world_dim.w/2, -world_dim.h/2 + 32);
    ec_ui->create_component<game::ClockComponent>(
        [ec_ui_txt](){
            size_t size = game::get_state().get_player_army()->get_soldiers_size();
            ec_ui_txt->set_text(fmt::format("Soldiers: {}", size));
        },
        0.5f
    );
}

void setup_test(guaranteed_ptr<engine::world> world) {
    auto toto = world->create_entity("toto");
    toto->create_component<engine::image_renderer>("sprite1.png");
    toto->create_component<suicide_components>();
    toto->create_component<game::KeyController>(200);

    auto text = world->create_entity("text");
    text->create_component<engine::text_renderer>("coucou !", 18);
}
