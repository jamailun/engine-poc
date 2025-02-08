#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>
#include <engine/sdl/drawer.hh>

#include <spdlog/spdlog.h>

using namespace engine;

game_engine::game_engine() {
    _current_world = std::make_shared<world>("default");
}

void game_engine::on_loop_initialize() {
    spdlog::info("Game-engine is initializing.");
    //Set camera the same dimensions as the widnnow.
    auto& config = sdl::get_window().get_configuration();
    _camera.set_dimensions(
        config.get_screen_width(),
        config.get_screen_height()
    );
    _camera.move_to_center_screen();

    // Setup-operation
    for(auto& operation : _setup_operations) {
        operation(_current_world.get());
    }
}

void game_engine::on_loop_shutdown() {
    spdlog::info("Game-engine is stopping.");
}

// render + update
void game_engine::on_loop_update() {
    _registry.update_all();
}
void game_engine::on_loop_render() {
    // Clear screen
    drawer::enable_color(colors::black);
    SDL_Rect rect = {0, 0, 0, 0};
    SDL_GetWindowSize(sdl::get_window().sdl_window(), &(rect.w), &(rect.h));
    SDL_RenderFillRect(sdl::get_renderer(), &rect);

    // Render elements
    _registry.render_all();
}

// position
math::Point game_engine::world_to_camera(math::Point point) const {
    return math::Point {
        point.x - _camera.x(),
        point.y - _camera.y()
    };
}

math::Point game_engine::camera_to_world(math::Point point) const {
    return math::Point {
        point.x + _camera.x(),
        point.y + _camera.y()
    };
}

// STATIC

game_engine& engine::get_engine() {
    static game_engine ENGINE;
    return ENGINE;
}

component_registry& engine::get_registry() {
    return get_engine().registry();
}

