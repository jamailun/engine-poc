#pragma once

#include "engine/utils/memory.hh"
#include "engine/utils/timer.hh"

namespace engine {

/**
 * Triggers a callback every once in a while.
 */
template<typename Clock>
class stopwatch {
    using duration_unit = float; // seconds
    using consumer_t = std::function<void(duration_unit)>;
private:
    timer<Clock> _timer;
    consumer_t _callback = [](duration_unit){};
    supplier<bool> _stop_condition = [](){return false;};
    float _duration;

    bool _running = false;

public:
    stopwatch(float duration) : _duration(duration) {};
    ~stopwatch() = default;

    void start_blocking() {
        if(_running) return;
        _running = true;

        _timer.start();
        while(_running && !_stop_condition()) {
            if(_timer.elapsed_secs() >= _duration) {
                _callback(_timer.elapsed_secs());
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

    void set_duration(duration_unit duration) {
        _duration = duration;
    }
    
    void set_duration_of_millis(uint64_t millis) {
        _duration = ((float)millis) / 1000.0;
    }

};

using system_stopwatch = stopwatch<std::chrono::system_clock>;

} // namespace engine
