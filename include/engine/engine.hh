#pragma once

#include "engine/registry.hh"
#include "engine/entity.hh"
#include "engine/world.hh"
#include "engine/game_camera.hh"
#include "engine/guaranteed_ptr.hh"
#include "engine/sdl/inputs.hh"

#include <SDL2/SDL_events.h>
#include <queue>
#include <functional>

namespace engine {

// FWD
namespace math {
    class Point;
}

using setup_operation = std::function<void(guaranteed_ptr<world>)>;

class game_engine {
    friend game_engine& get_engine();
private:
    game_engine();
    ~game_engine() = default;

    // State and elements
    bool _running = false;
    component_registry _registry;
    std::shared_ptr<world> _current_world;
    game_camera _camera;
    Inputs _inputs;

    // user-setup
    std::vector<setup_operation> _setup_operations;
    
    // Queues
    std::queue<entity*> _to_delete_entities;
    void handle_queue__entity_delete();

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

    void register_setup_operation(setup_operation operation) {
        _setup_operations.push_back(operation);
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

    // Get camera.
    game_camera& camera() { return _camera; }

    // ---- RO

    const Inputs& inputs() const { return _inputs; }
    math::Point world_to_camera(float x, float y) const { return world_to_camera(math::Point(x, y)); }
    math::Point world_to_camera(math::Point world_pos) const;
    math::Point camera_to_world(float x, float y) const { return camera_to_world(math::Point(x, y)); }
    math::Point camera_to_world(math::Point camera_pos) const;
    
    // ---- queues

    void request_entity_deletion(entity* entity_ptr) {
        _to_delete_entities.push(entity_ptr);
    }
};

/*
 * Static access
*/
game_engine& get_engine();

} // namespace engine
