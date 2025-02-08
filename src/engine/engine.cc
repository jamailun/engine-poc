#include <engine/engine.hh>

using namespace engine;

// class

game_engine::game_engine() {
    _current_world =  std::make_shared<world>("default");
}

void game_engine::on_update() {
    _registry.update_all();
}

void game_engine::on_render() {
    _registry.render_all();
}

// STATIC

game_engine& engine::get_engine() {
    static game_engine ENGINE;
    return ENGINE;
}

component_registry& engine::get_registry() {
    return get_engine().registry();
}

