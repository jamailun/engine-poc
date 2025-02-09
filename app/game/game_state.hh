#pragma once

#include <engine/guaranteed_ptr.hh>
#include "./army.hh"

namespace game {

/**
 *  Global state of the game.
 */
class game_state {
    friend game_state& get_state();
private:
    std::unique_ptr<Army> _player_army;
    std::unique_ptr<Army> _enemy_army;
    game_state();
public:
    guaranteed_ptr<Army> get_player_army() { return _player_army.get(); }
    guaranteed_ptr<Army> get_enemy_army() { return _enemy_army.get(); }
};

game_state& get_state();

} // namespace game
