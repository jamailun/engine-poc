#pragma once

#include "engine/utils/memory.hh"
#include <chrono>

namespace engine {

/**
 * A timer just counts time.
 */
template<typename Clock>
class timer {
    using time_unit = std::chrono::time_point<Clock>;
private:
    time_t _started_time;
    time_t _ended_time;
    bool _running = false;

public:
    timer() = default;
    ~timer() = default;

    void restart() {
        _started_time = Clock::now();
        _running = true;
    }
    void start() {
        if(!_running) {
            restart();
        }
    }
    void stop() {
        if(_running) {
            _ended_time = Clock::now();
            _running = false;
        }
    }
    bool running() const {
        return _running;
    }
    time_unit elapsed() {
        time_unit end = _running ? Clock::now() : _ended_time;
        return end - _started_time;
    }
    uint64_t elapsed_millis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed()).count();
    }

};

using system_timer = timer<std::chrono::system_clock>;

} // namespace engine
