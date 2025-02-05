
#pragma once

#include <concepts>


namespace engine {

template <typename T>
concept updatable = requires(T cmpt) {
    { cmpt.update() } -> std::same_as<void>;
};

template <typename T>
concept actionable = requires(T cmpt) {
    { cmpt.action() } -> std::same_as<void>;
};

template <typename T>
concept component = updatable<T> || actionable<T>;

} // namespace engine
