#pragma once

#include "engine/utils/memory.hh"
#include "engine/utils/timer.hh"

namespace engine {

/**
 * Triggers a callback every once in a while.
 */
template<typename Clock>
class stopwatch {
    using time_unit = std::chrono::time_point<Clock>;
    using consumer_t = std::function<void(time_unit)>;
private:
    timer<Clock> _timer;
    consumer_t _callback = [](time_unit){};
    supplier<bool> _stop_condition = [](){return false;};
    time_unit _duration;

    bool _running = false;

public:
    stopwatch() = default;
    ~stopwatch() = default;

    void start_blocking() {
        if(_running) return;

        _timer.start();
        while(_running && !_stop_condition()) {
            if(_timer.elapsed() >= _duration) {
                _callback(_timer.elapsed());
                _timer.restart();
            }
        }
    }

    void stop() {
        _running = false;
        _timer.stop();
    }

    void set_callback(consumer_t callback) {
        _callback = callback;
    }

    void set_stop_condition(supplier<bool> stop_condition) {
        _stop_condition = stop_condition;
    }

};

using system_timer = timer<std::chrono::system_clock>;

} // namespace engine
