#pragma once

#include "engine/registry.hh"
#include "engine/entity.hh"
#include "engine/world.hh"

namespace engine {

class game_engine {
    friend game_engine& get_engine();
private:
    game_engine();
    ~game_engine() = default;

    component_registry _registry;

    std::shared_ptr<world> _current_world;

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

    // Update all components
    void on_update();

    // Render all components
    void on_render();

    std::shared_ptr<world> get_current_world() {
        return _current_world;
    }

    std::shared_ptr<entity> create_entity(std::string entity_name) {
        if(_current_world) {
            return _current_world->create_entity(entity_name);
        }
        return {};
    }
};

/*
 * Static access
*/
game_engine& get_engine();

} // namespace engine
