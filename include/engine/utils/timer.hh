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
    using duration_unit = std::chrono::duration<Clock>;
private:
    time_unit _started_time;
    time_unit _ended_time;
    bool _running = false;

public:
    timer() = default;
    ~timer() = default;

    void lap() {
        if(_running)
            _started_time = Clock::now();
    }
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
    uint64_t elapsed_millis() const {
        time_unit end = _running ? Clock::now() : _ended_time;
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - _started_time).count();
    }
    float elapsed_secs() const {
        float ms_value = elapsed_millis();
        return ms_value / 1000.0;
    }

};

using system_timer = timer<std::chrono::system_clock>;

} // namespace engine
