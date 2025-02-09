#include <engine/resources/resources_manager.hh>
#include <engine/resources/tile_map.hh>
#include <engine/resources/tile_set.hh>
#include <engine/sdl/game_window.hh>

#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include "nlohmann/json.hpp"

#include <SDL.h>

using namespace engine;
using namespace resources;

TileMap::TileMap(std::string name) : _name(name) {}

TileMap::~TileMap() {
    std::cout << "Destroying TileMap " << _name << "." << std::endl;
}

void TileMap::initialize(nlohmann::json& data) {
    if(_tileset_ref.is_valid()) {
        spdlog::warn("Should not initialize a TileMap twice !");
        return;
    }

    // Read JSON
    std::string ts_name = data["ressource"]["tileset"];
    _size_width  = data["params"]["size"]["width"];
    _size_height = data["params"]["size"]["height"];

    // Get image
    _tileset_ref = engine::resources::get_manager().find_tile_set(ts_name);
    if( ! _tileset_ref.is_valid()) {
        spdlog::error("TileMap could not find resource '{}'.", ts_name);
        return;
    }
    if( ! _tileset_ref->is_valid()) {
        spdlog::error("TileMap {} references TileSet '{}', which is NOT valid.", get_name(), _tileset_ref->get_name());
        return;
    }

    // Init array
    _matrix_ids = std::make_unique<uint16_t[]>(_size_width * _size_height);

    // Fill the matrixes
    try {
        auto& raw_json_data = data["data"];
        for (int idx_line = 0; idx_line < _size_height; idx_line++) {
            for (int idx_col = 0; idx_col < _size_width; idx_col++) {
                int texture_idx = raw_json_data[idx_line][idx_col];
                _matrix_ids[idx_col + _size_width * idx_line] = texture_idx;
            }
        }
    } catch(nlohmann::json::type_error& ex) {
        spdlog::critical("Could not read data from tilemap {} : {}.", get_name(), ex.what());
        return;
    }
}

void TileMap::draw_call(int x, int y) {
    if(!is_valid()) return;

    SDL_Rect target_rect { x, y, _tileset_ref->get_cell_width(), _tileset_ref->get_cell_height() };
    for (int idx_line = 0; idx_line < _size_height; idx_line++) {
        for (int idx_col = 0; idx_col < _size_width; idx_col++) {
            uint16_t data_id = _matrix_ids[idx_col + idx_line * _size_width];
            SDL_Rect& texture_rect = _tileset_ref->get(data_id);
            //TODO precompute all rect refs ? can pointer move meanwhile ?

            target_rect.x = x + (idx_col * _tileset_ref->get_cell_width());
            target_rect.y = y + (idx_line * _tileset_ref->get_cell_height());

            SDL_RenderCopy(
                sdl::get_renderer(),
                _tileset_ref->image()->sdl_texture(),
                & texture_rect, 
                & target_rect
            );
        }
    }
}

// -- static

std::unique_ptr<TileMap> engine::resources::load_tilemap_from_disk(std::string file) {
    std::ifstream ifs( file );
    nlohmann::json json_data = nlohmann::json::parse(ifs);
    
    std::unique_ptr<TileMap> tm = std::make_unique<TileMap>(file);
    tm->initialize(json_data);

    return tm;
}

