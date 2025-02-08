#pragma once

#include "engine/registry.hh"
#include "engine/entity.hh"
#include "engine/world.hh"
#include "engine/game_camera.hh"

#include <SDL2/SDL_events.h>

namespace engine {

// FWD
namespace math {
    class Point;
}

class game_engine {
    friend game_engine& get_engine();
private:
    game_engine();
    ~game_engine() = default;

    bool _running = false;
    component_registry _registry;
    std::shared_ptr<world> _current_world;

    game_camera _camera;

    // Loop : pre/post
    void on_loop_initialize();
    void on_loop_shutdown();
    // Loop : each iteration/event
    void on_loop_event(SDL_Event& event);
    void on_loop_update();
    void on_loop_render();

public:
    inline component_registry& registry() { return _registry; }

    /**
     * Create a new component, push it on the registry, add it to an entity.
     * @param entity : entity to add the component to.
     * @param args : arguments to pass to the component construtor.
     * @returns a new shared_ptr of the component.
     */
    template <component Component, typename... _Args>
    std::shared_ptr<Component> create_component(std::shared_ptr<entity> entity, _Args&&... args) {
        std::shared_ptr<Component> cmpt = std::make_shared<Component>(entity.get(), std::forward<_Args>(args)...);
        _registry.add(cmpt);
        entity->register_component(cmpt);
        return cmpt;
    }

    // Start the game engine.
    void start();

    // Get the current loaded world.
    std::shared_ptr<world> get_current_world() {
        return _current_world;
    }

    // utils: create a new entity into the current world.
    std::shared_ptr<entity> create_entity(std::string entity_name) {
        if(_current_world) {
            return _current_world->create_entity(entity_name);
        }
        return {};
    }

    game_camera& camera() { return _camera; }
    math::Point world_to_camera(float x, float y) const { return world_to_camera(math::Point(x, y)); }
    math::Point world_to_camera(math::Point world_pos) const;
    math::Point camera_to_world(float x, float y) const { return camera_to_world(math::Point(x, y)); }
    math::Point camera_to_world(math::Point camera_pos) const;
};

/*
 * Static access
*/
game_engine& get_engine();

} // namespace engine
