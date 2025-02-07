#pragma once

#include "engine/registry.hh"
#include "engine/entity.hh"

namespace engine {

class game_engine {
    friend game_engine& get_engine();
private:
    game_engine() = default;
    ~game_engine() = default;

    component_registry _registry;

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
        std::shared_ptr<Component> cmpt = std::make_shared<Component>(entity, std::forward<_Args>(args)...);
        _registry.add(cmpt);
        entity->register_component(cmpt);
        return cmpt;
    }

    void on_update() {
        _registry.update_all();
    }

    void on_render() {
        _registry.render_all();
    }

};

/*
 * Static access
*/
game_engine& get_engine();

} // namespace engine
