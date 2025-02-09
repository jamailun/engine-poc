#pragma once

#include "./game_state.hh"

using namespace game;

game_state::game_state() {
    _player_army = std::make_unique<Army>("player", engine::colors::blue, true);
    _enemy_army = std::make_unique<Army>("IA", engine::colors::red, false);
}

game_state& game::get_state() {
    static game_state GAME_STATE;
    return GAME_STATE;
}