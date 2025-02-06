
#pragma once

#include <concepts>


namespace engine {

// forward
class entity;

template <typename T>
concept base_component_concept = requires(T cmpt) {
    { cmpt.get_entity() } -> std::same_as<std::shared_ptr<entity>>;
};

template <typename T>
concept updatable = requires(T cmpt) {
    { cmpt.update() } -> std::same_as<void>;
};

template <typename T>
concept actionable = requires(T cmpt) {
    { cmpt.action() } -> std::same_as<void>;
};

template <typename T>
concept component = base_component_concept<T> && (updatable<T> || actionable<T>);

/**
 * Abstract class of something able to being a component.
 */
class base_component {
protected:
    using entity_ptr = std::shared_ptr<entity>;
private:
    entity_ptr _entity;
protected:
    base_component(entity_ptr entity) : _entity(entity) {}
    virtual ~base_component() {
        destroy_now();
    }
public:
    entity_ptr get_entity() { return _entity; }
    void destroy_now() {
        _entity = nullptr;
    }
};

} // namespace engine
