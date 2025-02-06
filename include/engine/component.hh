
#pragma once

#include <concepts>


namespace engine {

// forward
class entity;

template <typename T>
concept base_component = requires(T cmpt) {
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
concept component = base_component<T> && (updatable<T> || actionable<T>);

} // namespace engine
