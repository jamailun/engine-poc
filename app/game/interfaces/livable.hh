#pragma once

namespace game {

class Livable {
protected:
    bool _dead;
    float _max_health;
    float _health;

    virtual void on_death() = 0;

public:
    Livable(float max_health) :
        _dead(false),
        _max_health(max_health),
        _health(max_health)
    {
        if(max_health <= 0) {
            _max_health = 1;
            _health = 1;
        }
    }
    virtual ~Livable() = default;

    inline void die() {
        if(!_dead) {
            _dead = true;
            on_death();
        }
    }

    /**
     * return true if the entity is now dead (or was already).
     */
    inline bool damage(float amount) {
        if(_dead) return true;
        _health -= amount;
        if(_health <= 0) {
            _health = 0;
            die();
            return true;
        }
        return false;
    }

    inline void heal(float amount) {
        if(_dead) return;
        _health += amount;
        if(_health > _max_health) {
            _health = _max_health;
        }
    }

    inline void set_health(float amount) {
        if(_dead) return;
        if(amount > _max_health) amount = _max_health;
        if(amount <= 0) {
            die();
            return;
        }
        _health = amount;
    }

    inline float health() const { return _health; }
    inline float max_health() const { return _max_health; }
    inline float health_ratio() const { return _health / _max_health; }

    inline bool is_alive() const { return !_dead; }
    inline bool is_dead() const { return _dead; }

};

} // namespace game
