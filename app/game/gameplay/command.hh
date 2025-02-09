#pragma once

#include <engine/math.hh>
#include "../soldiers_container.hh"

#include <stdint.h>
#include <vector>
#include <SDL_rect.h>

namespace game {

/**
 * A command is given to the soldiers of an army.
 */
class Command : public SoldiersContainer {
public:
    enum class Type : uint8_t {
        move
    };

private:
    Type _type;
    engine::math::Point _world_point;
    bool _valid = true;

public:
    explicit Command(Type type, engine::math::Point target);
    ~Command();

    static std::shared_ptr<Command> create_move(engine::math::Point target, std::vector<soldier_ptr> soldiers);
    static std::shared_ptr<Command> create(Type type, engine::math::Point target);

    inline const engine::math::Point& world_point() const { return _world_point; }
    inline Type type() const { return _type; }

    bool is_valid() const;
    inline void invalidate() { _valid = false; }

    float distance(soldier_ptr soldier) const;
    engine::math::Point find_target(soldier_ptr soldier) const;
};

} // namespace game
