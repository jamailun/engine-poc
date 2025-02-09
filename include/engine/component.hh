#pragma once

#include <concepts>
#include "engine/guaranteed_ptr.hh"

namespace engine {

// forward
class entity;

template <typename T>
concept base_component_concept = requires(T cmpt) {
    { cmpt.get_entity() } -> std::same_as<guaranteed_ptr<entity>>;
};

template <typename T>
concept updatable = requires(T cmpt) {
    { cmpt.update() } -> std::same_as<void>;
};

template <typename T>
concept renderable = requires(T cmpt) {
    { cmpt.render() } -> std::same_as<void>;
    { cmpt.z_index() } -> std::same_as<int>;
};

/**
 * Component concept.
 */
template <typename T>
concept component = base_component_concept<T> && (updatable<T> || renderable<T>);

/**
 * Abstract class of something able to being a component.
 */
class base_component {
protected:
    using entity_ptr = guaranteed_ptr<entity>;
private:
    entity_ptr _entity;
protected:
    base_component(entity* entity) : _entity(entity) {}
    base_component(entity_ptr entity) : _entity(entity) {}
    virtual ~base_component() {
        destroy_now();
    }
public:
    entity_ptr get_entity() { return _entity; }
    void destroy_now() {
        _entity = nullptr;
    }
    virtual int z_index() const {return 0;}
};

} // namespace engine
