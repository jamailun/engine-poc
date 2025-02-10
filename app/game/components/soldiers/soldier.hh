#pragma once

#include <engine/guaranteed_ptr.hh>
#include <engine/component.hh>
#include <engine/sdl/colors.hh>

#include <memory>
#include <iostream>
#include "../../interfaces/livable.hh"

namespace game {

class Army; // fwd
class Command; // fwd

/**
 * A soldier is a war unit.
 */
class Soldier : public engine::base_component, public Livable {
    friend class Command;
private:
    guaranteed_ptr<Army> _army;
    float _radius;
    float _half_radius;
protected:
    float _speed;
    std::shared_ptr<Command> _command;

    inline void set_radius(float radius) { _radius = radius; _half_radius = radius / 2; }
    
    virtual void on_death();

public:
    Soldier(entity_ptr entity, guaranteed_ptr<Army> army, float speed, float radius);
    virtual ~Soldier() = default; // polymorphism

    inline float get_speed() const { return _speed; }
    inline void set_speed(float speed) { _speed = speed; }
    inline float get_radius() const { return _radius; }
    inline void clear_command() { if(_command) _command = nullptr; }
    void set_command(std::shared_ptr<Command> command);
    inline bool has_command() const;

    virtual bool can_update() const { return true; }
    virtual bool can_render() const { return true; }
    virtual void render();
    virtual void update();

};

using soldier_ptr = std::shared_ptr<Soldier>;

} // namespace game
