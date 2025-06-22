#include <SDL2/SDL_timer.h>

namespace engine {

/**
 * Time barrier, to avoid spamming an action.
 */
class time_barrier {
private:
    time_t _last_hit = 0;
    time_t _min_delta; // ms
public:
    time_barrier(time_t min_delta) : _min_delta(min_delta) {}
    time_barrier() : _min_delta(100) {}

    /*
    * Test if the hit is allowed.
    * Calling this with a success will automatically updated stored value.
    */
    bool test() {
        time_t now = SDL_GetTicks64();
        if(now - _last_hit >= _min_delta) {
            _last_hit = now;
            return true;
        }
        return false;
    }

    bool test_ro() const {
        time_t now = SDL_GetTicks64();
        return now - _last_hit >= _min_delta;
    }

    time_t get_last_hit() const {
        return _last_hit;
    }

    time_t get_min_delta() const {
        return _min_delta;
    }

    void set_min_delta(time_t min_delta) {
        _min_delta = min_delta;
    }

};

}