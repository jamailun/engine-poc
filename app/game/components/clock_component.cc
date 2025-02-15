#include "./clock_component.hh"

using namespace game;

void ClockComponent::update(float elapsed) {
    _elapsed += elapsed;
    if(_elapsed >= _frequency) {
        _callback();
        _elapsed = 0;
    }
}
