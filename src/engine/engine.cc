#include <engine/engine.hh>
#include <engine/sdl/game_window.hh>
#include <engine/sdl/drawer.hh>
#include <engine/utils/stopwatch.hh>

#include <spdlog/spdlog.h>

#include <thread>

using namespace engine;

game_engine::game_engine() {
    _current_world = std::make_shared<world>("default");
}

void start_thread_method(std::function<void(float)> method) {
    system_stopwatch clock (0.1f);
    clock.set_callback([method](float elapsed) {
        method(elapsed);
    });
    clock.set_stop_condition([](){return engine::get_engine().running();});
    clock.start_blocking();
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

    // Start update thread
    spdlog::info("Starting update thread.");
    std::thread(start_thread_method, [this](float elapsed){ this->on_loop_update(elapsed); }).detach();
    spdlog::info("update thread started.");
}

void game_engine::on_loop_shutdown() {
    spdlog::info("Game-engine is stopping.");
}

// render + update
void game_engine::on_loop_update(float elapsed) {
    _registry.update_all(elapsed);
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

// Queues

void game_engine::handle_queue__entity_delete() {
    while(!_to_delete_entities.empty()) {
        entity* ptr = _to_delete_entities.front();
        _current_world->destroy_entity_now(ptr);
        _to_delete_entities.pop();
    }
}

// STATIC

game_engine& engine::get_engine() {
    static game_engine ENGINE;
    return ENGINE;
}

component_registry& engine::get_registry() {
    return get_engine().registry();
}

