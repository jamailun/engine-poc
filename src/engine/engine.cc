#include <engine/engine.hh>

using namespace engine;

game_engine& engine::get_engine() {
    static game_engine ENGINE;
    return ENGINE;
}

